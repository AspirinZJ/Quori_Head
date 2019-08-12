#include "include/rotation_theta.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stddef.h>
// #include "sphere2Image.h"
#include <dlfcn.h>

#define PI 3.141592653589793

const int SCREENWIDTH = 1920, SCREENHEIGHT = 1080;
const float RADIUS = 4.0f;                   // the radius of the sphere screen
const float DX = -2.51f, DY = -1.14f;        // the parameter for calibrating the x and y-axis installation error
const float RM = 1.5f;                       // the radius of the mirror
const float RO = 2.0f;                       // the radius of the base circle at the bottom of the head
const float H = 1.0f;                        // the distance from the center of projection to bottom of the head
const float EPSILON = 0.059068559067049511f; // the angle between the axis of roation of the circle and the intersection of the projection axis and the sphere
const float L = 8.4476252817457738f;         // the distance between the projection center and the intersection of the projection axis and the sphere
static float angle = 0.0f;                   // the initial angle of the circle
static int Glut_Handle1_Window_FullScreen;   // Glut create handle

struct Circle
{
    float centerX, centerY;
    float r; // radius
};

struct Ellipse
{
    float centerX, centerY; // center position
    float semiMajor, semiMinor;
};

struct Rectangle
{
    float centerX, centerY;
    float width, height;
};

void displayFunc();
void idleFunc();
void kbdFunc(unsigned char key, int x, int y);
void stopProgram();
void drawCircle(struct Circle *circle);
void drawEllipse(struct Ellipse *ellipse);
void drawRectangle(struct Rectangle *rectangle);

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // double buffering
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
    Glut_Handle1_Window_FullScreen = glutCreateWindow("Quori Head Window");
    glutFullScreen(); // full screen mode
    glutKeyboardFunc(kbdFunc);
    glutDisplayFunc(displayFunc);
    glClearColor(0.0, 0.0, 0.0, 0.0); // black background, rgba
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 0, 0, 0);
    glShadeModel(GL_FLAT);
    glutIdleFunc(idleFunc);
    glFlush();
    glutMainLoop();

    return 0;
}

/* Display Function */
void displayFunc()
{
    float theta0 = 1.0 / 2.0 * PI; // initial polar angle, on the equator
    float psi0;                    // initial azimuthal angle
    float dtheta;
    float px, py;
    float semiMajor = 0.7, semiMinor = 0.7; // semi-major and semi-minor
    float phi = 0;                          // for rotating the figure locally
    int polyNum = 30;                       // use a 30-edge polygon to approximate a circle
    float CoordX[100], CoordY[100];
    float dPi = 2 * PI / polyNum; // P step size
    float x, y, xRot, yRot;       // x and y (and rotated xRot and yRot)position of a circle
    float theta, psi, Theta, Psi;
    float t; // parametric equation of a circle
    void *handle = dlopen("./libs2i.so", RTLD_LAZY);
    void (*sphere2Image)(float theta, float psi, float radius, float rm, float ro, float h, float epsilon, float L,
                         float dx, float dy, int sWidth, int sHeight, float *px, float *py);
    sphere2Image = dlsym(handle, "sphere2Image");
    glColor3f(1.0, 0.2, 0.2); // color of the circle

    glClear(GL_COLOR_BUFFER_BIT);
    //change the angle according to the x-direction movement of the mouse
    psi0 = angle / 280.0 * PI;
    dtheta = theta0 - 0.5 * PI;
    semiMinor = semiMajor = 0.7;
    //change color
    float BColor = sin(angle / 50.0);
    float GColor = cos(semiMajor * 5.0);
    glColor3f(1.0 - GColor, GColor, BColor);

    for (int i = 0; i < polyNum; i++)
    {
        t = i * dPi;
        // x and y save the position of an ellipse
        x = semiMajor * cos(t);
        y = semiMinor * sin(t);
        // x and y time a rotation matrix
        xRot = x * cos(phi) - y * sin(phi);
        yRot = x * sin(phi) + y * cos(phi);
        // approximate an circle from a flat plane to a sphere
        // ? theta = theta0 - yRot / RADIUS;
        // ? psi = xRot / (RADIUS * sin(theta0)) + psi0; // !sin(theta0) or sin(theta) ?
        theta = 0.5 * PI - yRot / RADIUS;
        psi = xRot / (RADIUS * sin(0.5 * PI)) + psi0;
        rotation_theta(theta, psi, psi0, dtheta, &Theta, &Psi);
        float coe1 = 0.15 * PI;
        float coe2 = -5;
        Theta += coe1 * exp(coe2 * Theta);
        sphere2Image(Theta, Psi, RADIUS, RM, RO, H, EPSILON, L,
                     DX, DY, SCREENWIDTH, SCREENHEIGHT, &px, &py);
        px = px - SCREENWIDTH / 2;
        py = py - SCREENHEIGHT / 2;
        CoordX[i] = 2 * px / SCREENHEIGHT;
        CoordY[i] = 2 * py / SCREENHEIGHT;
    }
    glBegin(GL_POLYGON); // glBegin(PARA); PARA can be GL_POLYGON GL_LINE_LOOP GL_POINTS GL_QUADS GL_LINE_STRIP
    for (int i = 0; i < polyNum; i++)
        glVertex2f(CoordX[i] * SCREENHEIGHT / SCREENWIDTH, CoordY[i]);
    glEnd();
    glFlush();
    // glutSwapBuffers(); // for double buffering
}

/* Idle Function */
void idleFunc()
{
    glutPostRedisplay(); // rerender(call displayFunc())
}

void kbdFunc(unsigned char key, int x, int y)
{
    if ('q' == key || 'Q' == key)
    {
        printf("Quitting the program.\n");
        glutDestroyWindow(Glut_Handle1_Window_FullScreen);
        exit(0);
    }
}

void stopProgram()
{
    glutDestroyWindow(Glut_Handle1_Window_FullScreen);
    exit(0);
}