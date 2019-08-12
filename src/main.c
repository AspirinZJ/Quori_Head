#include <stdlib.h>
#include <stdio.h>
#include "include/genFigure.h"

#define PI 3.141592653

int main(int argc, char *argv[])
{
    // float param[15] = {0.4 * PI, 0.1 * PI, 1.0,
    //                    0.6 * PI, 0.2 * PI, 1.0,
    //                    0.5 * PI, 0.6 * PI, 0.6, 0.8,
    //                    0.6 * PI, 0.2 * PI, 0.6, 0.5};
    float param[15] = {0.5 * PI, 0.5 * PI, 1.0f, 1.0f, 0.2 * PI, 1.0, 0.5 * PI, 0.6 * PI, 0.6, 0.8, 0.6 * PI, 0.2 * PI, 0.6, 0.5};
    genFigure(param, 15, argc, argv);
    return 0;
}