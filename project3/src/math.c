#include "math.h"

double gaussian2d_kernel(int x, int y, double muX, double muY, double sigmaX,
                         double sigmaY) {
  return (1 / (2 * M_PI * sigmaX * sigmaY)) *
         exp(-0.5 * ((x - muX) * ((x - muX)) / (sigmaX * sigmaX) +
                     (y - muY) * (y - muY) / (sigmaY * sigmaY)));
}

double symm_gaussian2d_kernel(int x, int y, double sigmaX, double sigmaY) {
  return (1 / (2 * M_PI * sigmaX * sigmaY)) *
         exp(-0.5 * (x * x / (sigmaX * sigmaX) +
                     y * y / (sigmaY * sigmaY)));
}