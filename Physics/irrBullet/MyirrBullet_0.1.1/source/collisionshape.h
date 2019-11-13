#ifndef __COLLISION_SHAPE_H_INCLUDED__
#define __COLLISION_SHAPE_H_INCLUDED__

#define SPHERE_MULTIPLIER = 5.0
#define BOX_MULTIPLIER


#include "common.h"


enum ECollisionShapeType
{
    ECST_BOX,
    ECST_CONE,
    ECST_SPHERE,
    ECST_BVHTRIMESH,
    ECST_CONVEXHULL,
    ECST_GIMPACT
};

enum EScalingPair
{
    ESP_BOTH,
    ESP_COLLSHAPE,
    ESP_VISUAL
};

class ICollisionShape
{
public:
    ICollisionShape();

    virtual ~ICollisionShape();

    //////////////////////////
    // The 'dos' and 'sets' //
    //////////////////////////
    void setLocalScaling(const irr::core::vector3df &scaling, const EScalingPair esp);

    void setMargin(const irr::f64 margin);

    void calculateLocalInertia(const irr::core::vector3df &in);

    bool isPolyhedral() const;
    bool isConvex() const;
    bool isConcave() const;
    bool isCompound() const;
    bool isInfinite() const;


    ////////////////
    // The 'gets' //
    ////////////////
    irr::core::vector3df getLocalScaling() const;

    irr::core::stringc getName() const;

    ECollisionShapeType getShapeType() const;

    irr::f64 getMargin() const;

    irr::core::vector3df getLocalInertia() const;

    irr::f64 getMass() const {return mass;};

    btCollisionShape *getPointer() {return shape;};

    irr::scene::ISceneNode *getNode() const {return node;};

protected:
    ECollisionShapeType type;
    irr::scene::ISceneNode *node;
    btCollisionShape *shape;

    irr::f64 mass;
    irr::core::vector3df localInertia;

    // Functions
    virtual void createShape();
};


#endif // __COLLISION_SHAPE_H_INCLUDED__
