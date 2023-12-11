#include "Arc.h"

Arc::Arc(Node* n1, Node* n2)
{
    Node1 = n1;
    Node2 = n2;
}

Node* Arc::getNode1()
{
    return Node1;
}
Node* Arc::getNode2()
{
    return Node2;
}

/*
void Arc::setNode1()
{
}
void Arc::setNode2()
{
}
**/
