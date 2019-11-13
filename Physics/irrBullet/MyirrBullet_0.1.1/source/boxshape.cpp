#include "boxshape.h"

using namespace irr;
using namespace core;
using namespace scene;

IBoxShape::IBoxShape(ISceneNode *n, f64 m)
{
    node = n;
    mass = m;

    type = ECST_BOX;

    createShape();
}

void IBoxShape::createShape()
{
    btVector3 halfExtents(node->getScale().X * 0.5, node->getScale().Y * 0.5, node->getScale().Z * 0.5);

	shape = new btBoxShape(halfExtents);
	calculateLocalInertia(vector3df(0.0f,0.0f,0.0f));
}

IBoxShape::~IBoxShape()
{
}
