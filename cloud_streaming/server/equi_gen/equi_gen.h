#ifndef EQUIGEN_H
#define EQUIGEN_H

void createEquiRectangularUnwrap(char *src_file,
        char *dest_file, int width,
        float uPerpendicularCentre, float vPerpendicularCentre,
        float minDiameter, float maxDiameter,
        float angleOffsetRadians, float aspect);

#endif
