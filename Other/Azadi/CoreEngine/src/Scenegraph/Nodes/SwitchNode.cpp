#include "SwitchNode.h"

SceneGraphNodes::SwitchNode::SwitchNode(const bool &Flag)
{
    flag = Flag;

    type = Switch;
}

SceneGraphNodes::SwitchNode::~SwitchNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::SwitchNode::execute()
{
    cleanup();

    if (flag)
        for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
            (*it)->execute();
}
