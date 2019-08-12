#ifndef GENFIGURE
#define GENFIGURE

#include "rotateTheta.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stddef.h>
#include <string.h>
// #include "sphere2Image.h"
#include <dlfcn.h>
#include <unistd.h>

struct Circle
{
    float ctrTheta, ctrPsi;
    float r; // radius
};

struct Ellipse
{
    float centerX, centerY; // center position
    float semiMajor, semiMinor;
};

struct Rectangle
{
    float ctrTheta;
    float ctrPsi;
    float width;
    float height;
};

extern void displayFunc();
extern void idleFunc();
extern void kbdFunc(unsigned char key, int x, int y);
extern void stopProgram();
extern void drawCircle(struct Circle *circle);
extern void drawEllipse(struct Ellipse *ellipse);
extern void drawRectangle(struct Rectangle *rectangle);
extern void genFigure(float param[], size_t size, int argc, char *argv[]);

#endif