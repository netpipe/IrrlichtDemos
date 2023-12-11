#ifndef LINE_H
#define LINE_H

#include "vector.h"

namespace Geometry
{
    struct line
    {
        Math::Vector3<float> point1;
        Math::Vector3<float> point2;

        Math::Vector3<float> at(const float &pos)
        {
            return Math::Vector3<float> ((point1 * (1 - pos)) + (point2 * pos));
        }

        float distanceFromPoint(const Math::Vector3<float> &rhs)
        {
            return sqrt(Math::vSquared(rhs - point1) - (Math::vSquared((rhs - point1) * (point2 - point1)) / Math::vSquared(point2 - point1)));
        }
    };
};

#endif
