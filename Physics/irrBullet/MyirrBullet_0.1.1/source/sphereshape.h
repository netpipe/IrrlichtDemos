#ifndef __SPHERE_SHAPE_H_INCLUDED__
#define __SPHERE_SHAPE_H_INCLUDED__


#include "collisionshape.h"


class ISphereShape : public ICollisionShape
{
public:
    ISphereShape(irr::scene::ISceneNode *n, irr::f64 m);

    ~ISphereShape();

protected:
    // Functions
    virtual void createShape();
};


#endif // __SPHERE_SHAPE_H_INCLUDED__


