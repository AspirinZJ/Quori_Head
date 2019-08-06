#include "sphereToplane.h"
/* How this function works and the name of parameters in this function all
follow the paper*/

#define PI 3.141592653589793

void sphereToplane(float theta, float psi, float *xp, float *yp)
{
    extern const float RADIUS, RM, RO, H, EPSILON, L;

    float d = H + sqrt(RADIUS * RADIUS - RO * RO);
    float del = RADIUS - sqrt(RADIUS * RADIUS - RM * RM);
    float x = mTot(tan(theta / 2.0), d, RM, RADIUS, del);
    float phi = 2.0 * atan(x);
    phi = atan(sin(phi) / (((RADIUS - del) + d) / RM - cos(phi)));
    float l1 = L * tan(EPSILON - phi);
    float l2 = L * tan(EPSILON + phi);

    del = (-l1 + l2) / 2.0;
    phi = del * (sin(EPSILON) / cos(phi));
    phi = sqrt(del * del - phi * phi);
    x = cos(psi - PI / 2.0f);
    d = sin(psi - PI / 2.0f);
    phi = sqrt(1.0 / (x * x / (del * del) + d * d / (phi * phi)));
    *xp = (-l1 - l2) / 2.0 + phi * cos(psi - PI / 2.0f);
    *yp = phi * sin(psi - PI / 2.0f);
}