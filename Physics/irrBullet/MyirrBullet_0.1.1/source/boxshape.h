#ifndef __BOX_SHAPE_H_INCLUDED__
#define __BOX_SHAPE_H_INCLUDED__

#include "collisionshape.h"


class IBoxShape : public ICollisionShape
{
public:
    IBoxShape(irr::scene::ISceneNode *n, irr::f64 m);

    ~IBoxShape();

protected:
    // Functions
    virtual void createShape();
};


#endif // __BOX_SHAPE_H_INCLUDED__

