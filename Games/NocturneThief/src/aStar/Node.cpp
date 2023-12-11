#include "Node.h"
#include <iostream>


Node::Node(int nid,vector3df newPos)
{
    pos=newPos;
    id = nid;
    parent = NULL;
    g = 0;
    h = 0;
    f = 0;
}

//getters
int Node::getId()
{
    return id;
}
int Node::getG()
{
    return g;
}
int Node::getH()
{
    return h;
}
int Node::getF()
{
    return f;
}
Node* Node::getParent()
{
    return parent;
}
//setters

void Node::setG(int ng)
{
    g = ng;
}
void Node::setH(int nh)
{
    h = nh;
}
void Node::setF(int nf)
{
    f = nf;
}
void Node::setParent(Node *nparent)
{
    parent = nparent;
}

vector3df Node::getPosition()
{
    return pos;
}

