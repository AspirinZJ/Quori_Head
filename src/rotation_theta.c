#include "include/rotation_theta.h"
/* 
This function rotates a circle on the equtor to other places (different inclination angle)
 */
void rotation_theta(float thetaIn, float psiIn, float psiCenter, float dtheta,
                    float *theta, float *psi)
{
    float psi_prime = psiIn - psiCenter;
    float rotMat[9] = {cos(dtheta), 0, sin(dtheta),
                       0, 1, 0,
                       -sin(dtheta), 0, cos(dtheta)}; // rotation matrix
    float xyzPrime[3];
    float xyz[3] = {0};                          // xyz[3] = {x, y, z}
    xyzPrime[0] = sin(thetaIn) * cos(psi_prime); // x'
    xyzPrime[1] = sin(thetaIn) * sin(psi_prime); // y'
    xyzPrime[2] = cos(thetaIn);                  // z'

    // matrix multiplication
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            xyz[row] += rotMat[row * 3 + column] * xyzPrime[column];
        }
    }

    *theta = acos(xyz[2]);
    *psi = psiCenter + atan(xyz[1] / xyz[0]);
}