#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <signal.h>

#include <cv.h>
#include <highgui.h>

#include "equi_gen_buf.h"

#define DEG_2_RAD 0.0174532925

using namespace std;
using namespace cv;

const char *rs_tl_args = "-n -w 2000 -h 2000 -awb off -ex off -ev -2 -mm average";
const char *rs_strm_args = "-tl 200 -n -th 0:0:0 -w 1000 -h 1000 -awb off -ex off -ev -4 -mm average";
const int min_tl_delay = 2000; //>500
const int tl_cap_run_in = 80; //>50, delay between starting camera in stills mode and taking image
const char *frame_location = "s_frame.jpg";

char *save_path;
int run;

void terminate(int arg)
{
    cout<<"Got SIGTERM, will now terminate"<<endl;
    run = 0;
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

    cout<<"Timelapse capture delay: "<<delay<<"ms"<<endl;
    cout<<"Requested frames (0=infinate): "<<f_count<<endl;
    cout<<"Timelaspe raspistill args: "<<rs_tl_args<<endl;
    cout<<"Streaming raspistill args: "<<rs_strm_args<<endl;
    cout<<"Timelapse capture run-in delay: "<<tl_cap_run_in<<"ms"<<endl;

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
    	
    	#ifdef USE_GPS
    	//Do GPS position schecking here
    	//add GPS EXIFs to rs_tl_args
    	#endif
    	
    	//If conditions for capture are met
    	if(capture_now)
    	{
	    //Generate filename and capture timelapse image in stills mode
            char frame_fn[50];
            sprintf(frame_fn, filename, frame);
            char tl_captrue_cmd[100];
            sprintf(tl_captrue_cmd, "raspistill -o %s/original/%s.jpg -t %d %s", save_path, frame_fn, tl_cap_run_in, rs_tl_args);

            cout<<"Starting timelapse capture"<<endl;
            system(tl_captrue_cmd);
            cout<<"Timelapse capture end"<<endl;

	    //Start thread to convert image (~900ms for conversion)
            pthread_t proc_thread_id;
            int thread_state = pthread_create(&proc_thread_id, NULL, process_image, frame_fn);
            if(thread_state) cout<<"Post-processing failure:"<<thread_state<<endl<<"Manual post-processing needed.\n"<<endl;
    	}

	//Start camera in timelapse mode to generate stream images
        char stream_capture_cmd[100];
        sprintf(stream_capture_cmd, "raspistill -o %s -t %d %s", frame_location,  (delay - tl_cap_run_in), rs_strm_args);

        cout<<"Starting streaming capture"<<endl;
        system(stream_capture_cmd);
        cout<<"Streaming capture end"<<endl;

	//Check if frame has reached capture limit
        frame++;
        if(run)
        {
            if(f_count) run = !(frame == f_count);
            else run = 1;
        }

	//Stop if needed
        if(!run)
        {
	    //If stopping, wait for images to process
            pthread_join(proc_thread_id, NULL);
            break;
        }
	//Deallocate resources for terminated processing threads
        pthread_detach(proc_thread_id);
    }

    //Delete the frame image used for streaming
    char rm_frame_cmd[50];
    sprintf(rm_frame_cmd, "rm %s", frame_location);
    int rm_frame_cmd_state = system(rm_frame_cmd);
    cout<<"Delete frame image: "<<rm_frame_cmd_state<<endl;

    return 0;
}
