#include "include/genFigure.h"

#define PI 3.141592653589793

const int SCREENWIDTH = 1920, SCREENHEIGHT = 1080;
const float RADIUS = 4.0f;                   // the radius of the sphere screen
const float DX = -2.51f, DY = -1.14f;        // the parameter for calibrating the x and y-axis installation error
const float RM = 1.5f;                       // the radius of the mirror
const float RO = 2.0f;                       // the radius of the base circle at the bottom of the head
const float H = 1.0f;                        // the distance from the center of projection to bottom of the head
const float EPSILON = 0.059068559067049511f; // the angle between the axis of roation of the circle and the intersection of the projection axis and the sphere
const float L = 8.4476252817457738f;         // the distance between the projection center and the intersection of the projection axis and the sphere
// static float angle = 0.0f;                   // the initial angle of the circle
static int Glut_Handle1_Window_FullScreen; // Glut create handle
float paramArr[15];                        // the parameters array passed from the main function

void genFigure(float param[], size_t size, int argc, char *argv[])
{
    memcpy(paramArr, param, size * sizeof(float));
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
}

/* Display Function */
void displayFunc()
{
    struct Circle leftEye = {*(paramArr), *(paramArr + 1), *(paramArr + 2)};
    struct Circle rightEye = {*(paramArr + 3), *(paramArr + 4), *(paramArr + 5)};
    struct Rectangle leftEB = {*(paramArr + 6), *(paramArr + 7), *(paramArr + 8), *(paramArr + 9)};
    struct Rectangle rightEB = {*(paramArr + 10), *(paramArr + 11), *(paramArr + 12), *(paramArr + 13)};
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle(&leftEye);
    drawCircle(&rightEye);
    drawRectangle(&leftEB);
    drawRectangle(&rightEB);
    glFlush();
    // glutSwapBuffers(); // for double buffering
}

void drawCircle(struct Circle *circle)
{
    float theta0 = circle->ctrTheta; // initial polar angle
    float psi0 = circle->ctrPsi;     // initial azimuthal angle
    float dtheta = theta0 - 0.5 * PI;
    float r = circle->r;
    float px, py;
    float phi = 0;    // for rotating the figure locally
    int polyNum = 30; // use a 30-edge polygon to approximate a circle
    float CoordX[polyNum], CoordY[polyNum];
    float dPi = 2 * PI / polyNum; // P step size
    float x, y, xRot, yRot;       // x and y (and rotated xRot and yRot)position of a circle
    float theta, psi, Theta, Psi;
    void *handle = dlopen("./libs2i.so", RTLD_LAZY);
    void (*sphere2Image)(float theta, float psi, float radius, float rm, float ro, float h, float epsilon, float L,
                         float dx, float dy, int sWidth, int sHeight, float *px, float *py);
    sphere2Image = dlsym(handle, "sphere2Image");
    //change color
    float BColor = sin(theta0);
    float GColor = cos(r * 5.0);
    glColor3f(1.0 - GColor, GColor, BColor);

    for (int i = 0; i < polyNum; i++)
    {
        // x and y save the position of an ellipse
        x = r * cos(i * dPi); // parametric equation of a circle
        y = r * sin(i * dPi);
        // x and y time a rotation matrix
        xRot = x * cos(phi) - y * sin(phi);
        yRot = x * sin(phi) + y * cos(phi);
        // approximate an circle from a flat plane to a sphere
        // ? theta = theta0 - yRot / RADIUS;
        // ? psi = xRot / (RADIUS * sin(theta0)) + psi0; // !sin(theta0) or sin(theta) ?
        theta = 0.5 * PI - yRot / RADIUS;
        psi = xRot / (RADIUS * sin(0.5 * PI)) + psi0;
        rotateTheta(theta, psi, psi0, dtheta, &Theta, &Psi);
        float coe1 = 0.15 * PI, coe2 = -5; // figure calibration coefficient
        Theta += coe1 * exp(coe2 * Theta);
        sphere2Image(Theta, Psi, RADIUS, RM, RO, H, EPSILON, L,
                     DX, DY, SCREENWIDTH, SCREENHEIGHT, &px, &py);
        CoordX[i] = 2 * (px - SCREENWIDTH / 2) / SCREENHEIGHT;
        CoordY[i] = 2 * (py - SCREENHEIGHT / 2) / SCREENHEIGHT;
    }
    glBegin(GL_POLYGON); // glBegin(PARA); PARA can be GL_POLYGON GL_LINE_LOOP GL_POINTS GL_QUADS GL_LINE_STRIP
    for (int i = 0; i < polyNum; i++)
        glVertex2f(CoordX[i] * SCREENHEIGHT / SCREENWIDTH, CoordY[i]);
    glEnd();
    glFlush();
}

void drawRectangle(struct Rectangle *rectangle)
{
    float dtheta = rectangle->ctrTheta - 0.5 * PI;
    float CoordX[4], CoordY[4];
    for (int i = 0; i < 4; i++)
    {
        float x = -0.5 * rectangle->width * (i < 2 ? 1 : -1);
        float y = 0.5 * rectangle->height * (fabs((float)i - 1.5) > 1 ? 1 : -1); // ----------------testing
        float theta = 0.5 * PI - y / RADIUS;
        float psi = x / (RADIUS * sin(0.5 * PI)) + rectangle->ctrPsi;
        float Theta, Psi;
        float px, py;
        rotateTheta(theta, psi, rectangle->ctrPsi, dtheta, &Theta, &Psi);
        void *handle = dlopen("./libs2i.so", RTLD_LAZY);
        void (*sphere2Image)(float theta, float psi, float radius, float rm, float ro, float h, float epsilon, float L,
                             float dx, float dy, int sWidth, int sHeight, float *px, float *py);
        sphere2Image = dlsym(handle, "sphere2Image");
        sphere2Image(Theta, Psi, RADIUS, RM, RO, H, EPSILON, L,
                     DX, DY, SCREENWIDTH, SCREENHEIGHT, &px, &py);
        px = px - SCREENWIDTH / 2;
        py = py - SCREENHEIGHT / 2;
        CoordX[i] = 2 * px / SCREENHEIGHT;
        CoordY[i] = 2 * py / SCREENHEIGHT;
    }
    //change color
    float BColor = sin(rectangle->width);
    float GColor = cos(rectangle->height);
    glColor3f(1.0 - GColor, GColor, BColor);
    glBegin(GL_POLYGON); // glBegin(PARA); PARA can be GL_POLYGON GL_LINE_LOOP GL_POINTS GL_QUADS GL_LINE_STRIP
    for (int i = 0; i < 4; i++)
        glVertex2f(CoordX[i] * SCREENHEIGHT / SCREENWIDTH, CoordY[i]);
    glEnd();
    glFlush();
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
        stopProgram();
    }
}

void stopProgram()
{
    glutDestroyWindow(Glut_Handle1_Window_FullScreen);
    exit(0);
}