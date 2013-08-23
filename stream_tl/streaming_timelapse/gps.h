#ifndef GPS_H
#define GPS_H

double haversine(double lat1, double long1, double lat2, double long2);
int wait_for_gps_lock();
int set_gps_exif(char *filename, double lat, double lon, double alt, double track, double speed, char *timestamp);

#endif
