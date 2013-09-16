#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <ctime>

#include "opencv2/opencv.hpp"

#include "equi_gen_buf.h"

#define DEG_2_RAD 0.0174532925

const float ratio_coeff = 1.0f;

int main (int argc, char **argv)
{
    cv::Mat original_image;
    cv::Mat unwrap_image;
    int req_resolution;

    sscanf(argv[3], "%i", &req_resolution);

    original_image = cv::imread(argv[1]);

    float ratio_i = (float) original_image.rows / (float) original_image.cols;

    if(original_image.rows != req_resolution) return 0;
    if(original_image.cols != req_resolution) return 0;

    unwrap_image = createEquiRectangularUnwrap(original_image,
                   1000,
                   0.5f, 0.5f,
                   0.38f, 0.86f,
                   ((float) 180 * DEG_2_RAD),
                   (ratio_i * ratio_coeff));

    cv::imwrite(argv[2], unwrap_image);

    return 0;
}
