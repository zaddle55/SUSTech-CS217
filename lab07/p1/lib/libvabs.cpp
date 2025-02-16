#include "libvabs.hpp"

bool vabs(int *p, size_t n)
{
    if (p == nullptr)
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        p[i] = std::abs(p[i]);
    }
    return true;
}

bool vabs(float *p, size_t n)
{
    if (p == nullptr)
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        p[i] = std::abs(p[i]);
    }
    return true;
}

bool vabs(double *p, size_t n)
{
    if (p == nullptr)
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        p[i] = std::abs(p[i]);
    }
    return true;
}