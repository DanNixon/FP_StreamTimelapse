#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <time.h>

#include <exiv2/exiv2.hpp>

#include "gps_util.h"
#include "gpio.h"

#ifndef DEG_2_RAD
#define DEG_2_RAD 0.0174532925
#endif

#define MS1_2_KPH 3.6
#define MS1_2_MPH 2.23693629
#define MS1_2_KNOTS 1.94384449

const char gps_speed_units = 'M'; //K: K/h, M: MPH, N: Knots

using namespace std;

//Uses a Haversine algorithm to calculate distance between two GPS points
double haversine(double lat1, double long1, double lat2, double long2)
{
    double d_lat = (lat2 - lat1) * DEG_2_RAD;
    double d_long = (long2 - long1) * DEG_2_RAD;
    double a = pow(sin(d_lat / 2), 2) + cos(lat1 * DEG_2_RAD) * cos(lat2 * DEG_2_RAD) * pow(sin(d_long / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    if(gps_speed_units == 'M') return 3956 * c; //Use miles if speed unit is MPH
    return 6367 * c; //KM otherwise
}

//Waits for GPS lock, detrmined by output of 3D-FIX pin of GPS reciever
int wait_for_gps_lock()
{
    GPIOClass *fix_pin = new GPIOClass("23"); //Set to GPIO# of 3D-FIX pin
    fix_pin->export_gpio();
    fix_pin->setdir_gpio("in");
    const long SAMPLE_TIMEOUT = 5000; //Time GPIO must be low to assume a lock (1000<SAMPLE_TIMEOUT<15000)
    string fix_state;
    clock_t start_clock = clock();
    while(1)
    {
        clock_t dt = clock() - start_clock;
        long dt_ms = (((float)dt) / CLOCKS_PER_SEC) * 1000;
        if(dt_ms >= SAMPLE_TIMEOUT) return 1;
        fix_pin->getval_gpio(fix_state);
        if(fix_state == "1") start_clock = clock();
    }
    return 1;
}

//Adds GPS EXIF data to saved image
int set_gps_exif(char *filename, double lat, double lon, double alt, double track, double speed, char *timestamp)
{
    //Open image and get current metadata
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
    image->readMetadata();
    Exiv2::ExifData &exif_data = image->exifData();

    //Set EXIF lat/lon refs
    if(lat > 0) exif_data["Exif.GPSInfo.GPSLatitudeRef"] = "N";
    else exif_data["Exif.GPSInfo.GPSLatitudeRef"] = "S";
    if(lon > 0) exif_data["Exif.GPSInfo.GPSLongitudeRef"] = "E";
    else exif_data["Exif.GPSInfo.GPSLongitudeRef"] = "W";

    //Set EXIF latitude
    int lat_d = abs(lat);
    double lat_m_f = 60 * (abs(lat) - lat_d);
    int lat_m = lat_m_f;
    double lat_s_f = 60 * (lat_m_f - lat_m);
    int lat_s = lat_s_f * 100;
    Exiv2::URationalValue::AutoPtr lat_rv(new Exiv2::URationalValue);
    lat_rv->value_.push_back(std::make_pair(lat_d, 1));
    lat_rv->value_.push_back(std::make_pair(lat_m, 1));
    lat_rv->value_.push_back(std::make_pair(lat_s, 100));
    exif_data.add(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude"), lat_rv.get());

    //Set EXIF longitude
    int long_d = abs(lon);
    double long_m_f = 60 * (abs(lon) - long_d);
    int long_m = long_m_f;
    double long_s_f = 60 * (long_m_f - long_m);
    int long_s = long_s_f * 100;
    Exiv2::URationalValue::AutoPtr lon_rv(new Exiv2::URationalValue);
    lon_rv->value_.push_back(std::make_pair(long_d, 1));
    lon_rv->value_.push_back(std::make_pair(long_m, 1));
    lon_rv->value_.push_back(std::make_pair(long_s, 100));
    exif_data.add(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude"), lon_rv.get());

    //Set EXIF GPS speed
    int speed_i = speed * 100;
    exif_data["Exif.GPSInfo.GPSSpeed"] = Exiv2::Rational(speed_i, 100);

    //Set EXIF speed ref/unit
    switch(gps_speed_units)
    {
    case 'N':
        speed_i *= MS1_2_KNOTS;
        exif_data["Exif.GPSInfo.GPSSpeedRef"] = "N";
        break;
    case 'M':
        speed_i *= MS1_2_MPH;
        exif_data["Exif.GPSInfo.GPSSpeedRef"] = "M";
        break;
    case 'K':
        speed_i *= MS1_2_KPH;
        exif_data["Exif.GPSInfo.GPSSpeedRef"] = "K";
        break;
    }

    //Set EXIF altitude
    int alt_i = alt * 100;
    exif_data["Exif.GPSInfo.GPSAltitude"] = Exiv2::Rational(alt_i, 100);

    //Set EXIF track/heading and ref
    int track_i = track * 100;
    exif_data["Exif.GPSInfo.GPSTrack"] = Exiv2::Rational(track_i, 100);
    exif_data["Exif.GPSInfo.GPSTrackRef"] = "T";

    //Parse the GPS timestamp
    tm *gps_time;
    strptime(timestamp, "%Y-%m-%dT%H:%M:%SZ", gps_time);

    //Set EXIF timestamp
    //Exiv2::TimeValue *exif_timestamp = new Exiv2::TimeValue(gps_time->tm_hour, gps_time->tm_min, gps_time->tm_sec);

    Exiv2::URationalValue::AutoPtr exif_timestamp(new Exiv2::URationalValue);
    exif_timestamp->value_.push_back(std::make_pair(gps_time->tm_hour, 1));
    exif_timestamp->value_.push_back(std::make_pair(gps_time->tm_min, 1));
    exif_timestamp->value_.push_back(std::make_pair(gps_time->tm_sec, 1));

    exif_data.add(Exiv2::ExifKey("Exif.GPSInfo.GPSTimeStamp"), exif_timestamp.get());

    //Set EXIF datestamp
    int year = 1900 + gps_time->tm_year;
    int month = gps_time->tm_mon + 1;
    Exiv2::DateValue *exif_datestamp = new Exiv2::DateValue(year, month, gps_time->tm_mday);
    exif_data.add(Exiv2::ExifKey("Exif.GPSInfo.GPSDateStamp"), exif_datestamp); //If this produces an error in the console/logfiles it can be ignored, EXIF viewers parse the date format correcty

    //Write EXIF metadata to image
    image->writeMetadata();
    return 0;
}
