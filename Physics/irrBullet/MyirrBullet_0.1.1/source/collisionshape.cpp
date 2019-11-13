#include "collisionshape.h"

using namespace irr;
using namespace core;
using namespace scene;

ICollisionShape::ICollisionShape()
{
}


//////////////////////////
// The 'dos' and 'sets' //
//////////////////////////
void ICollisionShape::setLocalScaling(const vector3df &scaling, const EScalingPair esp)
{
    // TODO: Fix scaling for IGImpactMeshShapes. The bounding box does not update.
    // A call to updateBound() will make it work for what is inside its bounding
    // box, but it does not actually change to fit the real scale.
    f64 compensator = 1.0;

    ECollisionShapeType type = getShapeType();

    if(type == ECST_SPHERE)
        compensator = 5.0;

    else
    if(type == ECST_BOX)
        compensator = 0.5;


    shape->setLocalScaling(btVector3(scaling.X * compensator,scaling.Y * compensator,scaling.Z * compensator));
    node->setScale(scaling);

    calculateLocalInertia(vector3df(0.0f,0.0f,0.0f));

    /*else
    {
        if(esp == ESP_COLLSHAPE)
        {
            shape->setLocalScaling(btVector3(scaling.X,scaling.Y,scaling.Z));
        }

        else
        if(esp == ESP_VISUAL)
        {
            node->setScale(scaling);
        }
    }*/
}


void ICollisionShape::setMargin(const f64 margin)
{
    if(margin < 0.01)
    {
        shape->setMargin(0.01);
    }

    else
    {
        shape->setMargin(margin);
    }
}


void ICollisionShape::calculateLocalInertia(const vector3df &in)
{
    // Keeping this order is important. Strange things happen otherwise.
    btVector3 inertia(in.X,in.Y,in.Z);
    shape->calculateLocalInertia(getMass(), inertia);

    localInertia = vector3df(inertia.getX(),inertia.getY(),inertia.getZ());
}


void ICollisionShape::createShape()
{
}


////////////////
// The 'gets' //
////////////////
bool ICollisionShape::isPolyhedral() const
{
    return shape->isPolyhedral();
}

bool ICollisionShape::isConvex() const
{
    return shape->isConvex();
}

bool ICollisionShape::isConcave() const
{
    return shape->isConcave();
}

bool ICollisionShape::isCompound() const
{
    return shape->isCompound();
}

bool ICollisionShape::isInfinite() const
{
    return shape->isInfinite();
}


vector3df ICollisionShape::getLocalScaling() const
{
    const btVector3 &scale = shape->getLocalScaling();
    return vector3df(scale.getX(),scale.getY(),scale.getZ());
}

stringc ICollisionShape::getName() const
{
    return (stringc)shape->getName();
}

ECollisionShapeType ICollisionShape::getShapeType() const
{
    return type;
}


f64 ICollisionShape::getMargin() const
{
    return shape->getMargin();
}


vector3df ICollisionShape::getLocalInertia() const
{
    return localInertia;
}


ICollisionShape::~ICollisionShape()
{
    delete shape;
}
