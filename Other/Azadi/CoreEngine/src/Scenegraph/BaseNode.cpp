#include "BaseNode.h"
#include <vector>
#include <iostream>

void SceneGraphNodes::BaseNode::prepRemove()
{
    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++ it)
    {
        (*it)->prepRemove();
        delete (*it);
    }

    children.clear();
}

void SceneGraphNodes::BaseNode::init()
{
    children.clear();
}

void SceneGraphNodes::BaseNode::addChild(BaseNode *child)
{
    if (childExists(child))
        return;
    child->parent = this;
    child->root = root;
    child->bounding = bounding;
    children.push_back(child);
    child->childNumber = children.size();
}

bool SceneGraphNodes::BaseNode::childExists(BaseNode *child)
{
    return ((std::find(children.begin(), children.end(), child)) != children.end());
}

void SceneGraphNodes::BaseNode::removeChild(BaseNode *child)
{
    std::vector<BaseNode*>::iterator it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        (*it)->prepRemove();
        children.erase(it);
    }
}

void SceneGraphNodes::BaseNode::delayRemoveChild(BaseNode *child)
{
    toDelete.push_back(child);
}

void SceneGraphNodes::BaseNode::cleanup()
{
    for (std::vector<BaseNode*>::iterator ti = toDelete.begin(); ti != toDelete.end(); ++ti)
    {
        std::vector<BaseNode*>::iterator it = find(children.begin(), children.end(), *ti);
        if (it != children.end())
        {
            (*it)->prepRemove();
            children.erase(it);
        }
    }
    toDelete.clear();
}
