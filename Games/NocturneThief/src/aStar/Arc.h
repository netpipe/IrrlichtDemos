#ifndef __ARC__
#define __ARC__

#include "Node.h"

class Arc
{
private:
    Node* Node1;
    Node* Node2;

public:
    Arc(Node* n1, Node* n2);

    Node* getNode1();
    Node* getNode2();

    //void setNode1();
    //void setNode2();

};

#endif
