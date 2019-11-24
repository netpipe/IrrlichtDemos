#include "ScaleNode.h"

SceneGraphNodes::ScaleNode::ScaleNode(const GLfloat &X, const GLfloat &Y, const GLfloat &Z)
{
    x = X; y = Y; z = Z;

    type = Scale;
}

SceneGraphNodes::ScaleNode::~ScaleNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::ScaleNode::updateScale(const GLfloat &X, const GLfloat &Y, const GLfloat &Z)
{
    x = X; y = Y; z = Z;
}

void SceneGraphNodes::ScaleNode::execute()
{
    cleanup();

    glScalef(x, y, z);

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
