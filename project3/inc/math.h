#ifndef MATH_H_
#define MATH_H_
#include <math.h>

double gaussian2d_kernel(int x, int y, double muX, double muY, double sigmaX,
                         double sigmaY);
double symm_gaussian2d_kernel(int x, int y, double sigmaX, double sigmaY);

#endif