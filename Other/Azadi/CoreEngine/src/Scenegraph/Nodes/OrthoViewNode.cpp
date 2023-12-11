#include "OrthoViewNode.h"

SceneGraphNodes::OrthoViewNode::OrthoViewNode(const GLfloat &Left, const GLfloat &Right, const GLfloat &Bottom, const GLfloat &Top, const GLfloat &Near, const GLfloat &Far)
{
    left = Left; right = Right;
    bottom = Bottom; top = Top;
    nearPlane = Near; farPlane = Far;

    type = Ortho;
}

SceneGraphNodes::OrthoViewNode::~OrthoViewNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::OrthoViewNode::updateOrtho(const GLfloat &Left, const GLfloat &Right, const GLfloat &Bottom, const GLfloat &Top, const GLfloat &Near, const GLfloat &Far)
{
    left = Left; right = Right;
    bottom = Bottom; top = Top;
    nearPlane = Near; farPlane = Far;
}

void SceneGraphNodes::OrthoViewNode::execute()
{
    cleanup();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
