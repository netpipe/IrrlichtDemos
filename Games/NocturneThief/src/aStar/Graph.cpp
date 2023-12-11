#include "Graph.h"

Graph::Graph()
{
}

Node* Graph::addNode(int id,vector3df pos)
{
    Node* node = new Node(id,pos);
    nodes.push_back(node);
    return node;
}


Arc* Graph::addArc(Node* n1, Node* n2)
{
    Arc* arc = new Arc(n1, n2);
    arcs.push_back(arc);
    return arc;
}

vector<Node*> Graph::getNodes()
{
    return nodes;
}

Node* Graph::getNodeByID(int id)
{
    for(int i=0;i<arcs.size();i++)
    {
        if( ((Node*)nodes[i])->getId() == id) return nodes[i];
    }

    //printf("Node not found:%d\n",id);
    return NULL;
}

vector<Arc*> Graph::getArcs()
{
    return arcs;
}
