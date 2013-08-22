#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include <cv.h>
#include <highgui.h>

#include "equi_gen_buf.h"
#include "gpio.h"

#define DEG_2_RAD 0.0174532925
#define MS1_2_KPH 3.6
#define MS1_2_MPH 2.23693629
#define MS1_2_KNOTS 1.94384449

using namespace std;
using namespace cv;

const char *rs_tl_args = "-n -w 2000 -h 2000 -awb off -ex off -ev -2 -mm average";
const char *rs_strm_args = "-tl 200 -n -th 0:0:0 -w 1000 -h 1000 -awb off -ex off -ev -4 -mm average";
const int min_tl_delay = 500; //>500
const int tl_cap_run_in = 80; //>50, delay between starting camera in stills mode and taking image
const char *frame_location = "s_frame.jpg";
const char *gps_temp_location = "gpsdata.log";
float min_cap_dist = 0.0f; //Minimum distance that must be traveled before another timelapse image will be captured, units defined in haversine.py
const char gps_speed_units = 'M'; //K: K/h, M: MPH, N: Knots

char *save_path;
int run;
float last_lat = 0.0f;
float last_long = 0.0f;

void terminate(int arg)
{
    cout<<"Got SIGTERM, will now terminate"<<endl;
    run = 0;
}

double haversine(double lat1, double long1, double lat2, double long2)
{
	double d_lat = (lat2 - lat1) * DEG_2_RAD;
	double d_long = (long2 - long1) * DEG_2_RAD;
	double a = pow(sin(d_lat / 2), 2) + cos(lat1 * DEG_2_RAD) * cos(lat2 * DEG_2_RAD) * pow(sin(d_long / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	#ifdef DIST_MI
	return 3956 * c;
	#endif
	return 6367 * c;
}

//Thread to handle equi. image generation
void *process_image(void *arg)
{
    char img_fn[50];
    strcpy(img_fn, (char *)arg);
    char img_loc[50];
    sprintf(img_loc, "%s/original/%s.jpg", save_path, img_fn);
    cout<<"Processing image: "<<img_loc<<endl;
    char out_fn[50];
    sprintf(out_fn, "%s/equi/%s_e.jpg", save_path, img_fn);
    createEquiRectangularUnwrap(img_loc, out_fn,
                                2000,
                                0.5f, 0.5f,
                                0.38f, 0.86f,
                                (180.0f * DEG_2_RAD), 1.0f);
    cout<<"Processed image saved: "<<out_fn<<endl;
    return NULL;
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

int main(int argc, char **argv)
{
	#ifdef USE_GPS
	cout<<"Using GPS, waiting for lock..."<<endl;
	int lock = wait_for_gps_lock();
	cout<<"Got GPS lock: "<<lock<<endl;
	#endif

    //Handle SIGTERM for safe shutdown
    cout<<"Send SIGTERM to terminate, SIGINT (Ctrl+C) may kill MMAL"<<endl;
    signal(SIGTERM, terminate);

    unsigned int delay, f_count, frame, curr_img_count;
    save_path = argv[1];
    char *filename = argv[2];
    sscanf(argv[3], "%d", &delay);
    sscanf(argv[4], "%d", &f_count);
    sscanf(argv[5], "%f", &min_cap_dist);

    cout<<"Timelapse capture delay: "<<delay<<"ms"<<endl;
    cout<<"Requested frames (0=infinate): "<<f_count<<endl;
    cout<<"Timelaspe raspistill args: "<<rs_tl_args<<endl;
    cout<<"Streaming raspistill args: "<<rs_strm_args<<endl;
    cout<<"Timelapse capture run-in delay: "<<tl_cap_run_in<<"ms"<<endl;
    cout<<"Minimun capture distance: "<<min_cap_dist<<endl;

    //Create folder for raw captures
    char o_folder_cmd[50];
    sprintf(o_folder_cmd, "mkdir %s/original", save_path);
    int o_folder_cmd_state = system(o_folder_cmd);
    cout<<"Create original folder: "<<o_folder_cmd_state<<endl;

    //Create folder for equi. images
    char e_folder_cmd[50];
    sprintf(e_folder_cmd, "mkdir %s/equi", save_path);
    int e_folder_cmd_state = system(e_folder_cmd);
    cout<<"Create equi folder: "<<e_folder_cmd_state<<endl;

    #ifdef USE_GPS
    //Create folder for GPS data
    char g_folder_cmd[50];
    sprintf(g_folder_cmd, "mkdir %s/gps", save_path);
    int g_folder_cmd_state = system(g_folder_cmd);
    cout<<"Create GPS folder: "<<g_folder_cmd_state<<endl;
    #endif

    //Get number of existing images, so as not to overwrite images in loss of power
    char file_count_cmd[50];
    sprintf(file_count_cmd, "ls %s/original | wc -l", save_path);
    FILE *file_count_reader = popen(file_count_cmd, "r");
    fscanf(file_count_reader, "%d", &curr_img_count);
    pclose(file_count_reader);
    cout<<"Detected "<<curr_img_count<<" images in original folder"<<endl;

    //Start capturing after last image in folder
    frame = curr_img_count;
    if(f_count) f_count += curr_img_count;
    cout<<"Will start capture at image "<<frame<<" and capture up to image "<<f_count<<endl;

    //Ensure delay is not too small, ensures reasonable quality stream and time for switching camera modes
    if(delay < min_tl_delay) delay = min_tl_delay;
    cout<<"Using timelapse delay time: "<<delay<<"ms."<<endl;

    run = 1;

    while(run)
    {
    	//Assume we are good to capture
    	int capture_now = 1;
    	char *gps_exif = "";

    	#ifdef USE_GPS
    	double current_lat;
    	double current_long;
    	double delta_dist;

	double alt;
	double track;
	double speed;

		//Get current GPS position
		cout<<"Getting GPS location"<<endl;
    	char gps_get_cmd[50];
		sprintf(gps_get_cmd, "python get_gps.py %s", gps_temp_location);
		FILE *gps_get_reader = popen(gps_get_cmd, "r");
		fscanf(gps_get_reader, "%lf %lf %lf %lf %lf", &current_lat, &current_long, &alt, &track, &speed);
		pclose(gps_get_reader);
		cout<<"Current position: "<<current_lat<<", "<<current_long<<endl;

    	//Calculate distance traveled since last capture
		delta_dist = haversine(last_lat, last_long, current_lat, current_long);
		cout<<"Delta dist: "<<delta_dist<<endl;

		//Check if have moved far enough
		if(delta_dist < min_cap_dist)
		{
			capture_now = 0;
			cout<<"Will not capture a frame here, haven't moved enough since last capture"<<endl;
		}
		else
		{
			//Add GPS EXIFs to raw timelapse frame capture
			gps_exif = (char *) malloc(600);

			char lat_ref;
                        char lon_ref;
                        if(current_lat > 0) lat_ref = 'N';
                        else lat_ref = 'S';
                        if(current_long > 0) lon_ref = 'E';
                        else lon_ref = 'W';

			int lat_d = abs(current_lat);
			double lat_m_f = 60 * (abs(current_lat) - lat_d);
			int lat_m = lat_m_f;
			double lat_s_f = 60 * (lat_m_f - lat_m);
			int lat_s = lat_s_f * 100;

			int long_d = abs(current_long);
                        double long_m_f = 60 * (abs(current_long) - long_d);
                        int long_m = long_m_f;
                        double long_s_f = 60 * (long_m_f - long_m);
			int long_s = long_s_f * 100;

			int speed_i = speed * 100;
			switch(gps_speed_units)
			{
				case 'N':
					speed_i *= MS1_2_KNOTS;
					break;
				case 'M':
					speed_i *= MS1_2_MPH;
					break;
				case 'K':
					speed_i *= MS1_2_KPH;
					break;
			}
			int alt_i = alt * 100;
			int track_i = track * 100;

			sprintf(gps_exif, "--exif GPS.GPSLatitude=%d/1,%d/1,%d/100 --exif GPS.GPSLongitude=%d/1,%d/1,%d/100 --exif GPS.GPSAltitude=%d/100 --exif GPS.GPSTrack=%d/100 --exif GPS.GPSSpeed=%d/100 --exif GPS.GPSTrackRef=T --exif GPS.GPSLatitudeRef=%c --exif GPS.GPSLongitudeRef=%c --exif GPS.GPSSpeedRef=%c", lat_d, lat_m, lat_s, long_d, long_m, long_s, alt_i, track_i, speed_i, lat_ref, lon_ref, gps_speed_units);

			//Set current position as last position
			last_lat = current_lat;
			last_long = current_long;

			//Move GPS log file to timelapse GPS folder
			char gps_fn[50];
            sprintf(gps_fn, filename, frame);
			char gps_mv_cmd[50];
			sprintf(gps_mv_cmd, "mv %s %s/gps/%s_g.log", gps_temp_location, save_path, gps_fn);
			int gps_mv_cmd_state = system(gps_mv_cmd);
			cout<<"Move GPS data file: "<<gps_mv_cmd_state<<endl;
		}
    	#endif

    	pthread_t proc_thread_id;
    	//If conditions for capture are met
    	if(capture_now)
    	{
			//Generate filename and capture timelapse image in stills mode
            char frame_fn[50];
            sprintf(frame_fn, filename, frame);
            char tl_capture_cmd[1000];
            sprintf(tl_capture_cmd, "raspistill -o %s/original/%s.jpg -t %d %s %s", save_path, frame_fn, tl_cap_run_in, rs_tl_args, gps_exif);

            cout<<"Starting timelapse capture"<<endl;
		cout<<tl_capture_cmd<<endl;
            system(tl_capture_cmd);
            cout<<"Timelapse capture end"<<endl;

			//Start thread to convert image (~900ms for conversion)
            int thread_state = pthread_create(&proc_thread_id, NULL, process_image, frame_fn);
            if(thread_state) cout<<"Post-processing failure:"<<thread_state<<endl<<"Manual post-processing needed.\n"<<endl;

            frame++;
    	}

		//Start camera in timelapse mode to generate stream images
        char stream_capture_cmd[100];
        sprintf(stream_capture_cmd, "raspistill -o %s -t %d %s", frame_location,  (delay - tl_cap_run_in), rs_strm_args);

        cout<<"Starting streaming capture"<<endl;
        system(stream_capture_cmd);
        cout<<"Streaming capture end"<<endl;

		//Check if frame has reached capture limit
        if(run)
        {
            if(f_count) run = !(frame == f_count);
            else run = 1;
        }

		//Stop if needed
        if(!run)
        {
			//If stopping, wait for images to process
            if(capture_now) pthread_join(proc_thread_id, NULL);
            break;
        }
		//Deallocate resources for terminated processing threads
        if(capture_now) pthread_detach(proc_thread_id);
    }

    //Delete the frame image used for streaming
    char rm_frame_cmd[50];
    sprintf(rm_frame_cmd, "rm %s", frame_location);
    int rm_frame_cmd_state = system(rm_frame_cmd);
    cout<<"Delete frame image: "<<rm_frame_cmd_state<<endl;

    //Delete temporary GPS data
    char rm_gps_cmd[50];
    sprintf(rm_gps_cmd, "rm %s", gps_temp_location);
    int rm_gps_cmd_state = system(rm_gps_cmd);
    cout<<"Delete temp GPS data: "<<rm_gps_cmd_state<<endl;

    return 0;
}
