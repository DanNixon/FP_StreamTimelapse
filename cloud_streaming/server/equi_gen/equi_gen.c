#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

#include <cv.h>
#include <highgui.h>

#include "equi_gen.h"

void createEquiRectangularUnwrap(char *src_file,
        char *dest_file, int width,
        float uPerpendicularCentre, float vPerpendicularCentre,
        float minDiameter, float maxDiameter,
        float angleOffsetRadians, float aspect)
{
    IplImage *image_in = cvLoadImage(src_file, CV_LOAD_IMAGE_COLOR);
    assert((image_in->nChannels) = 3);

    float minRadius = minDiameter * 0.5f;
    float maxRadius = maxDiameter * 0.5f;
    int fullHeight = (int) (width / (2 * M_PI));

    unsigned char* pixels = malloc((fullHeight * width) * 3);
    unsigned char* oldPixels = image_in->imageData;

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

            int xPixel = (int) ((1 - v) * image_in->width);
            int yPixel = (int) ((1 - u) * image_in->height);

            int oldPixelIndex = ((yPixel * (image_in->width)) + xPixel) * 3;

            pixels[index] = oldPixels[oldPixelIndex];
            pixels[index + 1] = oldPixels[oldPixelIndex + 1];
            pixels[index + 2] = oldPixels[oldPixelIndex + 2];

            index += 3;
        }
    }

    CvSize new_size;
    new_size.width = width;
    new_size.height = fullHeight;
    IplImage *new_image = cvCreateImageHeader(new_size, image_in->depth, 3);
    cvSetData(new_image, pixels, CV_AUTOSTEP);

    cvSaveImage(dest_file, new_image, 0);
}
