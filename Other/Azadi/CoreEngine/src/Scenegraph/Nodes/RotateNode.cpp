#include "RotateNode.h"

SceneGraphNodes::RotateNode::RotateNode(const GLfloat &W, const GLfloat &X, const GLfloat &Y, const GLfloat &Z)
{
    x = X;
    y = Y; z = Z;

    type = Rotate;
}

SceneGraphNodes::RotateNode::~RotateNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::RotateNode::updateRotation(const GLfloat &X, const GLfloat &Y, const GLfloat &Z)
{
    x = X;
    y = Y; z = Z;
}

void SceneGraphNodes::RotateNode::execute()
{
    cleanup();

    glRotatef(x, 1.0f, 0.0f, 0.0f);
    glRotatef(y, 0.0f, 1.0f, 0.0f);
    glRotatef(z, 0.0f, 0.0f, 1.0f);

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
