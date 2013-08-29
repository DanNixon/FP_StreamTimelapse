#include <stdio.h>
#include <stdlib.h>

#include "equi_gen.h"

#define DEG_2_RAD 0.0174532925

int main(int argc, char **argv)
{
    createEquiRectangularUnwrap(argv[1], argv[2],
        1000,
        0.5f, 0.5f,
        0.45f, 0.85f,
        (float) 180 * DEG_2_RAD, 1.0f);
    return 0;
}
