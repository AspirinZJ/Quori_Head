#include "sphere2Plane.h"
/* This function warp the 3D spherical postion onto a 2D projection plane
theta: inclination angle
psi: azimuth angle
px: x position on 2D plane
py: y position on 2D plane*/

#define PI 3.141592653589793

void sphere2Image(float theta, float psi, float radius, float rm, float ro, float h, float epsilon, float L,
                  float dx, float dy, int sWidth, int sHeight, float *px, float *py)
{
    // extern const float RADIUSdxRM, RO, H, EPSILON, L, DX, DY;
    // extern const int SCREENWIdyH, sHeight;
    float x = 40.0f / 3.0f, y = 7.5f;
    float px1 = 898.62f, py1 = 182.37f;
    float s = 10.8f / 7.5f; // ! in MATLAB 10.6 / 7.5;
    float xp1, yp1;
    float xp2, yp2;
    float xp, yp;

    sphere2Plane(PI / 6.0f, PI, radius, rm, ro, h, epsilon, L, &xp1, &yp1);               // PI/6, PI
    sphere2Plane(PI / 2.0f, PI * 2.0f / 3.0f, radius, rm, ro, h, epsilon, L, &xp2, &yp2); // PI/2 , PI*2/3
    sphere2Plane(theta, psi, radius, rm, ro, h, epsilon, L, &xp, &yp);

    float ax = -159.78000000000009 / (xp1 - xp2);
    float ay = -248.13 / (yp1 - yp2);

    *px = s * (ax * xp + (px1 - ax * xp1)) + dx * sWidth / x;
    *py = s * (ay * yp + (py1 - ay * yp1)) + dy * sHeight / y;
}
