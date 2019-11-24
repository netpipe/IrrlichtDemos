#include "ColorNode.h"

SceneGraphNodes::ColorNode::ColorNode(const GLfloat &R, const GLfloat &G, const GLfloat &B, const GLfloat &A)
{
    r = R; g = G;
    b = B; a = A;

    type = Color;
}

SceneGraphNodes::ColorNode::~ColorNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::ColorNode::updateColor(const GLfloat &R, const GLfloat &G, const GLfloat &B, const GLfloat &A)
{
    r = R; g = G;
    b = B; a = A;
}

void SceneGraphNodes::ColorNode::execute()
{
    cleanup();
    glColor4f(r, g, b, a);

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
