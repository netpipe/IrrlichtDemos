#include "TranslateNode.h"

SceneGraphNodes::TranslateNode::TranslateNode(const GLfloat &X, const GLfloat &Y, const GLfloat &Z)
{
    x = X; y = Y; z = Z;

    type = Translate;
}

SceneGraphNodes::TranslateNode::~TranslateNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::TranslateNode::updateTranslation(const GLfloat &X, const GLfloat &Y, const GLfloat &Z)
{
    x = X; y = Y; z = Z;
}

void SceneGraphNodes::TranslateNode::execute()
{
    cleanup();

    glTranslatef(x, y, z);

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
