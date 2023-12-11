#ifndef __ASTAR__
#define __ASTAR__

#include <vector>
#include <irrlicht.h>
#include "Graph.h"
#include <iostream>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

class AStar
{
private:
    Graph* g;
    vector<Node*> path;

    vector<ISceneNode*> realWaypoints;

public:

    AStar(Graph* ng);

    vector<Node*> AStarAlgorithm(Node* start, Node* goal);

    vector<Node*> NodeNeighbors(Node* node, vector<Node*> closed);

    void Path(Node* start, Node* node);

    bool Contains(vector<Node*> v, Node* n);
};

#endif
