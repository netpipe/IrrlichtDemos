#include "Bezier.h"

bezier4f::bezier4f()
{

}

bezier4f::bezier4f(const vector3df &point1, const vector3df &point2, const vector3df &point3, const vector3df &point4)
{
    p1 = point1;
    p2 = point2;
    p3 = point3;
    p4 = point4;
}

bezier4f::~bezier4f()
{

}

void bezier4f::setPoints(const vector3df &point1, const vector3df &point2, const vector3df &point3, const vector3df &point4)
{
    p1 = point1;
    p2 = point2;
    p3 = point3;
    p4 = point4;
}

vector3df bezier4f::getPositionAt(const f32 &t)
{
    vector3df temp;
    f32 b = 1.0f-t;

    temp.X = (p1.X*(t*t*t)) + (p2.X*(3*(t*t*b))) + (p3.X*(3*(t*(b*b)))) + (p4.X*(b*b*b));
    temp.Y = (p1.Y*(t*t*t)) + (p2.Y*(3*(t*t*b))) + (p3.Y*(3*(t*(b*b)))) + (p4.Y*(b*b*b));
    temp.Z = (p1.Z*(t*t*t)) + (p2.Z*(3*(t*t*b))) + (p3.Z*(3*(t*(b*b)))) + (p4.Z*(b*b*b));

    return temp;
}
