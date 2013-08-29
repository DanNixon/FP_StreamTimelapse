#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <pthread.h>

#include "opencv2/opencv.hpp"

#include "image.h"
#include "equi_gen_buf.h"

#define DEG_2_RAD 0.0174532925

using namespace std;

static const int WIN_WIDTH = 800;
static const int WIN_HEIGHT = 600;
static const int NUM_ROWS = 128;
static const int NUM_COLUMNS = 128;

float angle = 0.0f;
pthread_t gl_thread;
unsigned char *tex_px;
int tex_width;
int tex_height;
int new_tex = 1;
int frame_counter = 0;

int main (int argc, char **argv)
{
    cout<<"Running OpenCV version "<<CV_MAJOR_VERSION<<"."<<CV_MINOR_VERSION<<endl;

    int unwrap = 0;

    int arg;
    for(arg = 1; arg < argc; arg++)
    {
        if(!strcmp(argv[arg], "unwrap")) unwrap = 1;
    }

    string url = argv[1];
    string videoStreamAddress = "http://" + url + "/?action=stream&amp;type=file.mjpg";
    float ratio_coeff = 1.0f;

    cv::VideoCapture vcap;
    cv::Mat raw_image;

    cout<<"Opening stream location: "<<videoStreamAddress<<endl;
    if(!vcap.open(videoStreamAddress))
    {
        cout<<"Error opening video stream"<<endl;
        return -1;
    }
    cout<<"Stream opened"<<endl;

    cv::Mat unwrap_image;

    while(1)
    {
        if(!vcap.read(raw_image))
        {
            cout<<"No frame"<<endl;
            cv::waitKey();
        }
        else
        {
            frame_counter++;
            //cv::imshow("Raw Image", raw_image);

            if(unwrap)
            {
                float ratio_i = (float) raw_image.rows / (float) raw_image.cols;
                unwrap_image = createEquiRectangularUnwrap(raw_image,
                                1000,
                                0.5f, 0.5f,
                                0.45f, 0.85f,
                                ((float) 180 * DEG_2_RAD),
                                (ratio_i * ratio_coeff));

                cv::imshow("Equirect. Image", unwrap_image);
            }

            cout<<"Rendered frame: "<<frame_counter<<" (width: "<<raw_image.cols<<", height: "<<raw_image.cols<<")"<<endl;
            if(cv::waitKey(20) >= 0) break;
        }
    }

    vcap.release();
    return 0;
}
