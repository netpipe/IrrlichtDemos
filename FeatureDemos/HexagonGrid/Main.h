#ifndef MAIN_H_
#define MAIN_H_

#define _USE_MATH_DEFINES
#include <math.h>

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace io;

static double DegreesToRadians(double degrees)
{
    return degrees * M_PI / 180;
}

static float CalculateH(int side)
{
    return (float)sin(DegreesToRadians(30)) * side;
}

static float CalculateR(int side)
{
    return (float)cos(DegreesToRadians(30)) * side;
} 

#endif