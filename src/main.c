#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stddef.h>
#include <stdbool.h>
#include <linux/input.h> // for getting the position of our mouse on GNU/LINUX
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "sphereToimage.h"
#include "rotation_theta.h"

#define PI 3.141592653589793

const int SCREENWIDTH = 1920, SCREENHEIGHT = 1080;
const float RADIUS = 4.0;                    // the radius of the sphere screen
const float DX = -2.51, DY = -1.14;          // the parameter for calibrating the x and y-axis installation error
const float RM = 1.5f;                       // the radius of the mirror
const float RO = 2.0f;                       // the radius of the base circle at the bottom of the head
const float H = 1.0f;                        // the distance from the center of projection to bottom of the head
const float EPSILON = 0.059068559067049511f; // the angle between the axis of roation of the circle and the intersection of the projection axis and the sphere
const float L = 8.4476252817457738f;         // the distance between the projection center and the intersection of the projection axis and the sphere
static float angle = 0;                      // the initial angle of the circle
static int Glut_Handle1_Window_FullScreen;   // Glut create handle
static int fd, retval;                       // get the mouse position
static char mousePosBuf[6];
static fd_set readfds;
static struct timeval tv;

static void displayFunc();
static void idleFunc();
static void kbdFunc(unsigned char key, int x, int y);
static void stopProgram();

int main(int argc, char *argv[])
{
    fd = open("/dev/input/mice", O_RDONLY); // open the mouse
    if (fd < 0)                             // failed to open
    {
        printf("Failed to open \"/dev/input/mice\".\n");
        exit(1);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // double buffering
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
    Glut_Handle1_Window_FullScreen = glutCreateWindow("Quori Head Window");
    glClearColor(0.0, 0.0, 0.0, 0.0); // black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    glutFullScreen(); // full screen mode
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idleFunc);
    glutKeyboardFunc(kbdFunc);
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
    int polyNum = 100;                      // use a 100-edge polygon to approximate a circle
    float CoordX[100], CoordY[100];
    float dPi = 2 * PI / polyNum; // P step size
    float x, y, xRot, yRot;       // x and y (and rotated xRot and yRot)position of a circle
    float theta, psi, Theta, Psi;
    float t; // parametric equation of a circle

    glColor3f(1.0, 0.2, 0.2); // color of the circle

    while (true)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        // set the maximum waiting time
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        retval = select(fd + 1, &readfds, NULL, NULL, &tv);
        /* if (retval == 0)
        {
            printf("Time out!\n");
        } */
        if (FD_ISSET(fd, &readfds))
        {
            // read the data in the mouse device
            if (read(fd, mousePosBuf, 6) <= 0)
            {
                continue;
            }
            // if (1 == (mousePosBuf[0] & 0x07))
            // {
            //     stopProgram();
            // }
            // print the data read from the mouse
            // printf("Button type = %d, X = %d, Y = %d, Z = %d\n", (mousePosBuf[0] & 0x07), mousePosBuf[1], mousePosBuf[2],   mousePosBuf[3]);
        }

        //change the angle according to the x-direction movement of the mouse
        angle += mousePosBuf[1] / 20.0;
        psi0 = angle / 280.0 * PI;
        theta0 -= mousePosBuf[2] / 1000.0;
        dtheta = theta0 - 0.5 * PI;
        //change the size of the circle according to the y-direction movement of the mouse
        // semiMajor += (float)mousePosBuf[2] / 500.0;
        // semiMinor += (float)mousePosBuf[2] / 500.0;
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
            sphereToimage(Theta, Psi, &px, &py);
            px = px - SCREENWIDTH / 2;
            py = py - SCREENHEIGHT / 2;
            CoordX[i] = 2 * px / SCREENHEIGHT;
            CoordY[i] = 2 * py / SCREENHEIGHT;
        }
        glBegin(GL_POLYGON);
        for (int i = 0; i < polyNum; i++)
            glVertex2f(CoordX[i] * SCREENHEIGHT / SCREENWIDTH, CoordY[i]);
        glEnd();
        glFlush();
        glutSwapBuffers(); // for double buffering
    }
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
    close(fd);
    exit(0);
}