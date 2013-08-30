#ifndef EQUIGEN_H
#define EQUIGEN_H

cv::Mat createEquiRectangularUnwrap(cv::Mat image_in,
                                 int width,
                                 float uPerpendicularCentre, float vPerpendicularCentre,
                                 float minDiameter, float maxDiameter,
                                 float angleOffsetRadians, float aspect);

#endif
