#include "FrustumViewNode.h"

SceneGraphNodes::FrustumViewNode::FrustumViewNode(const float &Left, const float &Right, const float &Bottom, const float &Top, const float &Near, const float &Far)
{
     left = Left; right = Right;
     bottom = Bottom; top = Top;
     nearPlane = Near; farPlane = Far;

     type = Frustum;
}

SceneGraphNodes::FrustumViewNode::~FrustumViewNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::FrustumViewNode::updateFrustum(const float &Left, const float &Right, const float &Bottom, const float &Top, const float &Near, const float &Far)
{
     left = Left; right = Right;
     bottom = Bottom; top = Top;
     nearPlane = Near; farPlane = Far;
}

void SceneGraphNodes::FrustumViewNode::execute()
{
    cleanup();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
