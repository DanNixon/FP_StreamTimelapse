#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

#include <cv.h>
#include <highgui.h>

#include "equi_gen_buf.h"

using namespace cv;

void createEquiRectangularUnwrap(char *img_in_loc, char *img_out_loc,
                                 int width,
                                 float uPerpendicularCentre, float vPerpendicularCentre,
                                 float minDiameter, float maxDiameter,
                                 float angleOffsetRadians, float aspect)
{
    Mat image_in = imread(img_in_loc, CV_LOAD_IMAGE_COLOR);

    float minRadius = minDiameter * 0.5f;
    float maxRadius = maxDiameter * 0.5f;
    int fullHeight = (int) (width / (2 * M_PI));

    int data_size = (fullHeight * width) * 3;
    unsigned char *pixels = new unsigned char[data_size];
    unsigned char *oldPixels = image_in.data;

    int index = 0;
    int i, j;
    for (i = fullHeight - 1; i > 0; i--)
    {
        float amplitutde = ((maxRadius - minRadius) * (i / (float) fullHeight)) + minRadius;
        for (j = 0; j < width; ++j)
        {
            float longitudeAngle = (float) (2 * M_PI * (j / (float) width)) + angleOffsetRadians;

            float sinLongAngle = sin(longitudeAngle);
            float cosLongAngle = cos(longitudeAngle);

            float u = (aspect) * sinLongAngle;
            float v = cosLongAngle;

            u *= amplitutde;
            v *= amplitutde;

            u += uPerpendicularCentre;
            v += (1.0f - vPerpendicularCentre);

            int xPixel = (int) ((1 - v) * image_in.cols);
            int yPixel = (int) ((1 - u) * image_in.rows);

            int oldPixelIndex = ((yPixel * (image_in.cols)) + xPixel) * 3;

            pixels[index] = oldPixels[oldPixelIndex];
            pixels[index + 1] = oldPixels[oldPixelIndex + 1];
            pixels[index + 2] = oldPixels[oldPixelIndex + 2];

            index += 3;
        }
    }

    Mat image_out = Mat(fullHeight, width, CV_8UC3, pixels);
    imwrite(img_out_loc, image_out);

    image_out.release();
    delete pixels;
}
