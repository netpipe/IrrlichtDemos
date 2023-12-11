#include "AStar.h"



AStar::AStar(Graph* ng)
{
    g = ng;
}
bool AStar::Contains(vector<Node*> v, Node* n)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == n)
            return true;
    }
    return false;
}

vector<Node*> AStar::AStarAlgorithm(Node* start, Node* goal)
{
    vector<Node*> nodes=g->getNodes();

    path.clear();

    vector<Node*> closed;
    vector<Node*> open;

    //insert the start node into the open list
    open.push_back(start);



    while (!Contains(closed, goal) || open.size() == 0)
    {
        int minF = 999999;
        Node* minFNode = NULL;

        //select the node with the min F into open list
        for(int i = 0; i < open.size(); i++)
        {
            if (open[i]->getF() <= minF)
            {
                minFNode = open[i];
                minF = open[i]->getF();
            }
        }



        //add this node to closed list and remove it from open list

        closed.push_back(minFNode);

        for(int i = 0; i < open.size(); i++)
        {
            if (open[i] == minFNode)
            {
                open.erase(open.begin() + i);

            }
        }

        //for all the node neighbors:

        vector<Node*> nNeighbors = NodeNeighbors(minFNode, closed);

        for(int i = 0; i < nNeighbors.size(); i++)
        {
            Node* item = nNeighbors[i];
            if (!Contains(open, item))
            {
                open.push_back(item);
                item->setParent(minFNode);
                item->setG(10);

                //if (goal->getId() - item->getId() < 0)
                    //item->setH(item->getId() - goal->getId()); //distancia do atual até o final
                //else
                    item->setH( goal->getPosition().getDistanceFrom(item->getPosition()) ); //distancia do atual até o final

                item->setF( item->getG() + item->getH());
            }
            else
            {
                if (minFNode->getG() + 10 < item->getG())
                {
                    item->setParent( minFNode);
                    item->setG(minFNode->getG() + 10);
                    item->setF(item->getG() + item->getH());
                }
            }
        }

    }

    nodes.clear();

    Path(start, goal);
    return path;
}

vector<Node*> AStar::NodeNeighbors(Node* node, vector<Node*> closed)
{
    vector<Node*> neighbors;

    for (int i = 0; i < g->getArcs().size(); i++)
    {
        Arc* item = g->getArcs()[i];

        if (item->getNode1() == node &&
            !Contains(neighbors, item->getNode2())
            && !Contains(closed, item->getNode2()))
            neighbors.push_back(item->getNode2());
        else if (item->getNode2() == node &&
            !Contains(neighbors, item->getNode1())
            && !Contains(closed, item->getNode2()))
            neighbors.push_back(item->getNode1());
    }


    return neighbors;
}

void AStar::Path(Node* start, Node* node)
{
    path.push_back(node);
    if (node != start)
    {
        Path(start, node->getParent());
    }

}


