#include <math.h>

#include "mathf.h"
#include "vectors.h"
#include "matrices.h"

float clampf(float v, float min, float max)
{
    return (v < min) ? min : (v > max) ? max : v;
}

float deg2rad(float deg)
{
    return deg * PI / 180.0f;
}

float rad2deg(float rad)
{
    return rad * 180.0f / PI;
}