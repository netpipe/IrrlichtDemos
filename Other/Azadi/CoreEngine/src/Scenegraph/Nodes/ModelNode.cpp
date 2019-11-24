#include "ModelNode.h"

SceneGraphNodes::ModelNode::ModelNode(const unsigned long int &ID)
{
    modelID = ID;

    type = Model;
}

SceneGraphNodes::ModelNode::~ModelNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::ModelNode::execute()
{
    cleanup();

    ModelFactory::getInstance()->Draw(modelID);

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}

