#ifndef BEZIER_H
#define BEZIER_H

#include <irrlicht.h>

using namespace irr;
using namespace core;

class bezier4f
{
    public:
        bezier4f();
        bezier4f(const vector3df &p1, const vector3df &p2, const vector3df &p3, const vector3df &p4);

        ~bezier4f();

        void setPoints(const vector3df &p1, const vector3df &p2, const vector3df &p3, const vector3df &p4);

        vector3df getPositionAt(const f32 &t);
    private:
        vector3df p1, p2, p3, p4;
};

#endif
