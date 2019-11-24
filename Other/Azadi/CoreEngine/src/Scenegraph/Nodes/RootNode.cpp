#include "RootNode.h"

SceneGraphNodes::RootNode::RootNode()
{
    root = this;
    parent = this;
    bounding = NULL;

    type = Root;
}

SceneGraphNodes::RootNode::~RootNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::RootNode::execute()
{
    cleanup();

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
