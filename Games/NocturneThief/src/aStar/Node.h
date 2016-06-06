#ifndef __NODE__
#define __NODE__

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class Node
{

private:
    int id;
    int g;
    int h;
    int f;
    Node* parent;

    vector3df pos;

public:
    Node(int id,vector3df newPos);

    vector3df getPosition();

    int getId();
    int getG();
    int getH();
    int getF();
    Node* getParent();

    void setG(int ng);
    void setH(int nh);
    void setF(int nf);
    void setParent(Node* nparent);
};

#endif
