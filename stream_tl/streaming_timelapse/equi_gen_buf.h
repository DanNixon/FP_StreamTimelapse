#ifndef EQUIGEN_H
#define EQUIGEN_H

void createEquiRectangularUnwrap(char *img_in_loc, char *img_out_loc,
                                 int width,
                                 float uPerpendicularCentre, float vPerpendicularCentre,
                                 float minDiameter, float maxDiameter,
                                 float angleOffsetRadians, float aspect);

#endif
