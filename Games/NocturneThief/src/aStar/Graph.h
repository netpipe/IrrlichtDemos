#ifndef __ASTAR_GRAPH__
#define __ASTAR_GRAPH__

#include <vector>
#include "Arc.h"

using namespace std;

class Graph
{

private:
    vector<Node*> nodes;
    vector<Arc*> arcs;

public:

    Graph();
    Node* addNode(int id,vector3df pos);
    Node* addNode(Node* n,vector3df pos);
    Arc* addArc(Node* n1, Node* n2);

    Node* getNodeByID(int id);

    vector<Node*> getNodes();
    vector<Arc*> getArcs();
};

#endif
