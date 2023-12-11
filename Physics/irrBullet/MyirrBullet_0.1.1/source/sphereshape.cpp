#include "sphereshape.h"

using namespace irr;
using namespace core;
using namespace scene;

ISphereShape::ISphereShape(ISceneNode *n, f64 m)
{
    node = n;
    mass = m;

    type = ECST_SPHERE;

    createShape();
}

void ISphereShape::createShape()
{
	shape = new btSphereShape(node->getScale().X * 5.0);
	calculateLocalInertia(vector3df(0.0f,0.0f,0.0f));
}

ISphereShape::~ISphereShape()
{
}

