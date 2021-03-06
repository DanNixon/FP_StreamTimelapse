#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <pthread.h>
#include <signal.h>

#include <cv.h>
#include <highgui.h>
#include <exiv2/exiv2.hpp>

#include "equi_gen_buf.h"
#include "gps_util.h"

#define DEG_2_RAD 0.0174532925

using namespace std;
using namespace cv;

const char *rs_tl_args = "-w 2000 -h 2000 -mm matrix --exif EXIF.MakerNote=BubbleScopeTimelapseFrame";
const char *rs_strm_args = "-th 0:0:0 -w 400 -h 400 -mm matrix";
const int stream_delay = 5000;
const int min_tl_delay = 5000; //>500
const int tl_cap_run_in = 1000; //>50, delay between starting camera in stills mode and taking image
const char *frame_location = "s_frame.jpg";
const char *img_count_filename = "last_frame_no.log";

float min_cap_dist;
int use_gps = 0;
char *save_path;
int run;
float last_lat = 0.0f;
float last_long = 0.0f;
char *post_tl_cmd;

//Safely terminate the application
void terminate(int arg)
{
    cout<<"Got SIGTERM, will now terminate"<<endl;
    run = 0;
}

//Thread to handle equi. image generation
void *process_image(void *arg)
{
    //Get the two filenames
    char img_fn[50];
    strcpy(img_fn, (char *)arg);
    char img_loc[50];
    sprintf(img_loc, "./%s/original/%s.jpg", save_path, img_fn);
    cout<<"Processing image: "<<img_loc<<endl;
    char out_fn[50];
    sprintf(out_fn, "./%s/equi/%s_e.jpg", save_path, img_fn);

    //Convert the original image
    createEquiRectangularUnwrap(img_loc, out_fn,
                                2000,
                                0.5f, 0.5f,
                                0.38f, 0.86f,
                                (180.0f * DEG_2_RAD), 1.0f);
    cout<<"Processed image saved: "<<out_fn<<endl;

    //Get EXIF data from the original image
    Exiv2::Image::AutoPtr original_image = Exiv2::ImageFactory::open(img_loc);
    original_image->readMetadata();
    Exiv2::ExifData &original_exif_data = original_image->exifData();

    //Clear the image width and height
    original_exif_data.erase(original_exif_data.findKey(Exiv2::ExifKey("Exif.Image.ImageLength")));
    original_exif_data.erase(original_exif_data.findKey(Exiv2::ExifKey("Exif.Image.ImageWidth")));

    //Save EXIF data to equi image
    Exiv2::Image::AutoPtr equi_image = Exiv2::ImageFactory::open(out_fn);
    equi_image->setExifData(original_exif_data);
    equi_image->writeMetadata();

    //Execute post capturew command
    if(strlen(post_tl_cmd) != 0)
    {
        char post_cmd[200];
        sprintf(post_cmd, post_tl_cmd, img_fn);
        int rVal = system(post_cmd);
        cout<<"Excuted command: "<<post_cmd<<", got return value "<<rVal<<endl;
    }

    return NULL;
}

int main(int argc, char **argv)
{
    //Handle SIGTERM for safe shutdown
    cout<<"Send SIGTERM to terminate, SIGINT (Ctrl+C) may kill MMAL"<<endl;
    signal(SIGTERM, terminate);

    unsigned int delay, f_count, frame, curr_img_count;
    save_path = argv[1];
    char *filename = argv[2];
    sscanf(argv[3], "%d", &delay);
    sscanf(argv[4], "%d", &f_count);
    sscanf(argv[5], "%f", &min_cap_dist);
    sscanf(argv[6], "%d", &use_gps);
    post_tl_cmd = "";
    if(argc > 7) { post_tl_cmd = argv[7]; }

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

    if(use_gps)
    {
        //Wait for a GPS lock
        cout<<"Using GPS, waiting for lock..."<<endl;
        int lock = wait_for_gps_lock();
        cout<<"Got GPS lock: "<<lock<<endl;
    }

    char image_count_path[50];
    sprintf(image_count_path, "%s/%s", save_path, img_count_filename);

    //Get number of existing images, so as not to overwrite images in loss of power
    frame = 0;
    FILE *img_count_file;
    img_count_file = fopen(image_count_path, "r");
    if(img_count_file != NULL)
    {
        char ic_str[10];
        fgets(ic_str, 10, img_count_file);
        sscanf(ic_str, "%ld", &frame);
        fclose(img_count_file);
        frame++;
    }

    //Start capturing after last image in folder
    if(f_count) f_count += frame;
    cout<<"Will start capture at image "<<frame<<" and capture up to image "<<f_count<<endl;

    //Ensure delay is not too small, ensures reasonable quality stream and time for switching camera modes
    if(delay < min_tl_delay) delay = min_tl_delay;
    cout<<"Using timelapse delay time: "<<delay<<"ms."<<endl;

    run = 1;

    while(run)
    {
        //Assume we are good to capture
        int capture_now = 1;
        FILE *gps_get_reader;

        double current_lat;
        double current_long;
        double alt;
        double track;
        double speed;
        double delta_dist;
        char timestamp[100];

        if(use_gps)
        {
            //Get current GPS position
            cout<<"Getting GPS location"<<endl;
            gps_get_reader = fopen("gps.temp" , "r");
            fscanf(gps_get_reader, "%lf %lf %lf %lf %lf %s", &current_lat, &current_long, &alt, &track, &speed, timestamp);
            fclose(gps_get_reader);
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
                //Set current position as last position
                last_lat = current_lat;
                last_long = current_long;
            }
        }

        pthread_t proc_thread_id;
        //If conditions for capture are met
        if(capture_now)
        {
            //Generate filename and capture timelapse image in stills mode
            char frame_fn[50];
            sprintf(frame_fn, filename, frame);
            char tl_capture_cmd[500];
            sprintf(tl_capture_cmd, "raspistill -o %s/original/%s.jpg -t %d %s", save_path, frame_fn, tl_cap_run_in, rs_tl_args);

            cout<<"Starting timelapse capture";
            if(use_gps)  cout<<"(GPS_Time: "<<timestamp<<")"<<endl;
            else cout<<endl;
            system(tl_capture_cmd);
            cout<<"Timelapse capture end"<<endl;

            if(use_gps)
            {
                char file_path[100];
                sprintf(file_path, "./%s/original/%s.jpg", save_path, frame_fn);
                set_gps_exif(file_path, current_lat, current_long, alt, track, speed, timestamp);
            }

            FILE *num_file;
            num_file = fopen(image_count_path, "w");
            char num_str[10];
            sprintf(num_str, "%ld", frame);
            fputs(num_str, num_file);
            fclose(num_file);

            //Start thread to convert image (~900ms for conversion)
            int thread_state = pthread_create(&proc_thread_id, NULL, process_image, frame_fn);
            if(thread_state) cout<<"Post-processing failure:"<<thread_state<<endl<<"Manual post-processing needed.\n"<<endl;

            frame++;
        }

        //Use multiple RaspStill commnds to capture streaming frames
        int f_delay = stream_delay;
        if(f_delay < tl_cap_run_in) f_delay = tl_cap_run_in;
        char stream_capture_cmd[100];
        sprintf(stream_capture_cmd, "raspistill -o %s -t %d %s", frame_location, f_delay, rs_strm_args);

        cout<<"Starting streaming capture"<<endl;
        time_t timelapse_start;
        time(&timelapse_start);
        long elapsed_ms = 0;

        do
        {
            //Stop if needed
            if(!run)
            {
                //If stopping, wait for images to process
                if(capture_now) pthread_join(proc_thread_id, NULL);
                break;
            }

            system(stream_capture_cmd);

            time_t c_time;
            time(&c_time);
            double elapsed_sec = difftime(c_time, timelapse_start);
            elapsed_ms = (long) (elapsed_sec * 1000);
        }
        while (elapsed_ms <= delay);
        cout<<"Streaming capture end"<<endl;

        //Check if frame has reached capture limit
        if(run)
        {
            if(f_count) run = !(frame == f_count);
            else run = 1;
        }


        //Deallocate resources for terminated processing threads
        if(capture_now) pthread_detach(proc_thread_id);
    }

    //Delete the frame image used for streaming
    char rm_frame_cmd[50];
    sprintf(rm_frame_cmd, "rm %s", frame_location);
    int rm_frame_cmd_state = system(rm_frame_cmd);
    cout<<"Delete frame image: "<<rm_frame_cmd_state<<endl;

    return 0;
}
