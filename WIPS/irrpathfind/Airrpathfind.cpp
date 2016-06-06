#include "irrlicht.h"
#include "Airrpathfind.h"

// Copyright (C) 2005 Cestmir "CZestmyr" Houska
// This file is using some classes from "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

using namespace irr;

/****************
/   PathNode    /
****************/

// constructor
PathNode::PathNode(const core::vector3df& pos)
: position(pos), list(PIL_NONE){}

// returns the distance from another PathNode
u32 PathNode::distance(PathNode* other)
{
    return (u32)(position.getDistanceFrom(other->position));
}

// returns overestimated distance to a PathNode. It is used by the 
// pathfinding algorithm to explore those PathNodes first, that are more
// likely to lead to the goal.
u32 PathNode::Hdist(PathNode* other)
{
    return (u32)(position.getDistanceFrom(other->position) * 1.2f);
}

// adds the other PathNode to the list of the neighbours
void PathNode::setNeighbour(PathNode* other)
{
    neighbours.push_back(other);
}

// removes the other PathNode from the list of the neighbours
void PathNode::removeNeighbour(PathNode* other)
{
    core::list<PathNode*>::Iterator iter = neighbours.begin();

    while (iter != neighbours.end()){
        if (*iter == other) iter = neighbours.erase(iter);
        else iter++;
    }
}

// checks, whether the other PathNode is a neighbour of this node
bool PathNode::isNeighbour(PathNode* other)
{
    core::list<PathNode*>::Iterator iter = neighbours.begin();

    while (iter != neighbours.end()){
        if (*iter == other) return true;
        iter++;
    }
    
    return false;
}

// draws the node with the given driver, using the given color
void PathNode::draw(video::IVideoDriver* driver, const video::SColor& color)
{
    video::S3DVertex NodeMeshVertices[4];

    NodeMeshVertices[0] = video::S3DVertex(0,2,0, 0,1,0,color,0,1);
    NodeMeshVertices[1] = video::S3DVertex(2,-2,-2, 1,0,-1,color,0,1);
    NodeMeshVertices[2] = video::S3DVertex(0,-2,2, 0,0,1,color,0,1);
    NodeMeshVertices[3] = video::S3DVertex(-2,-2,-2, -1,0,-1,color,0,1);

    u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1 };

    core::matrix4 translation;

    translation.makeIdentity();
    translation.setTranslation(position);

    driver->setTransform(video::ETS_WORLD, translation);
    driver->drawIndexedTriangleList(&NodeMeshVertices[0], 4, &indices[0], 4);

    core::list<PathNode*>::Iterator iter = neighbours.begin();
    while (iter != neighbours.end()){
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->draw3DLine(position, (*iter)->position, video::SColor(255,255,255,0));
        iter++;
    }
}

/*********
/  path  /
*********/

// constructor
path::path(): core::list<PathNode*>(){}

// Copy constructor
path::path(const path& other): core::list<PathNode*>()
{
    clear();
    core::list<PathNode*>::Iterator iter = other.begin();
    while (iter != other.end())
    {
        push_back(*iter);
        iter++;
    }
}

// = operator
path& path::operator=(const path& other)
{
    if (&other == this) return *this;
    clear();
    core::list<PathNode*>::Iterator iter = other.begin();
    while (iter != other.end())
    {
        push_back(*iter);
        iter++;
    }
    return *this;
}

// checks, whether the path contains the searched node
bool path::contains(PathNode* searchedNode)
{
    core::list<PathNode*>::Iterator iter = begin();

    while (iter != end()){
        if (*iter == searchedNode) return true;
        iter++;
    }

    return false;
}

/*****************
/     Network    /
*****************/

// constructor
Network::Network(video::IVideoDriver* driv, ITimer* tim, bool createLogFile)
: state(0), log(createLogFile), driver(driv), selectedNode(NULL), startNode(NULL),
endNode(NULL), timer(tim)
{}

// destructor
Network::~Network()
{
    core::list<PathNode*>::Iterator iter = nodes.begin();

    while (iter != nodes.end()){
        delete (*iter);
        iter++;
    }
}

// draws the node, for testing and editiong purposes
void Network::draw()
{
    video::SMaterial material;
    material.Lighting = false;
    video::SColor color;

    core::list<PathNode*>::Iterator iter = nodes.begin();

    while (iter != nodes.end()){
        if (*iter == selectedNode) color = video::SColor(255,0,64,128);
        else if (*iter == startNode || *iter == endNode || pathSegments.contains(*iter))
            color = video::SColor(255,0,128,64);
        else color = video::SColor(255,255,255,128);
        driver->setMaterial(material);
        (*iter)->draw(driver, color);
        iter++;
    }
}

// returns pointer to the node, that is nearest to the given point
PathNode* Network::nearestNodePointer(const core::vector3df& pos)
{
    u32 dist = 32000;

    core::list<PathNode*>::Iterator ret = nodes.begin();
    core::list<PathNode*>::Iterator iter = nodes.begin();

    while (iter != nodes.end()){
        u32 d = (u32)pos.getDistanceFrom((*iter)->position);
        if (d < dist) {dist = d; ret = iter;}
        iter++;
    }

    return *ret;
}

// adds a node to this network at the given point
void Network::addNode(const core::vector3df& pos)
{
    PathNode* node = new PathNode(pos);
    nodes.push_back(node);
}

// adds two-way connection between two nodes
void Network::addConnection(PathNode* a, PathNode* b)
{
    if (a == b) return;
    if (connectionExists(a,b)){       //This method not only adds, but also deletes connections
        a->removeNeighbour(b);
        b->removeNeighbour(a);
        return;
    }

    a->setNeighbour(b);
    b->setNeighbour(a);
    return;
}

// adds one-way connection between two nodes
void Network::addOneWayConnection(PathNode* from, PathNode* to)
{
    if (connectionExists(from,to)){       //This method not only adds, but also deletes connections
        from->removeNeighbour(to);
        to->removeNeighbour(from);
        return;
    }

    from->setNeighbour(to);
    return;
}

// removes node from net with all its connections
void Network::removeNodeFromNet(PathNode* a)
{
    core::list<PathNode*>::Iterator iter = nodes.begin();
    core::list<PathNode*>::Iterator toErase = 0;

    while (iter != nodes.end()){
        if (*iter == a) toErase = iter;
        else (*iter)->removeNeighbour(a);
        iter++;
    }

    nodes.erase(toErase);
    delete a;
}

// checks, whether there is any connection between two nodes
bool Network::connectionExists(PathNode* a, PathNode* b)
{
    if (a->isNeighbour(b) || b->isNeighbour(a)) return true;
    else return false;
}

// selects a node for purpose of making a two-way connection
void Network::selectNode(const core::vector3df& pos)
{
    if (selectedNode == NULL)
    selectedNode = nearestNodePointer(pos);
    else {
        PathNode* helper = nearestNodePointer(pos);
        addConnection(helper, selectedNode);
        selectedNode = NULL;
    }
}

// selects first and then a second node in order to find a path between
// them. Also stores this path to the pathSegments member.
// Returns true in case of success, else returns false
bool Network::findPath(const core::vector3df& fromOrTo)
{
    if (state == 0) {startNode = nearestNodePointer(fromOrTo); state = 1; return false;}

    // The state flag can now only be 1
    endNode = nearestNodePointer(fromOrTo);

    ofstream soubor;
    u32 timeStats;
    if (log)
    {
        soubor.open("log.txt");
        timeStats = timer->getTime();
        soubor << "Node Number: " << nodes.getSize() << std::endl;
        soubor << "Starting pathfinding..." << std::endl;
        soubor << "Reseting open and closed lists..." << std::endl;
    }

    // Reseting open and closed lists
    core::list<PathNode*>::Iterator iter = nodes.begin();
    while (iter != nodes.end())
    {
        (*iter)->list = PIL_NONE;
        iter++;
    }

    core::list<PathNode*> openList;  // Must be kept ordered
    openList.clear();

    //Adding first node into open list
    startNode->Gdist = 0;
    startNode->Fdist = startNode->Hdist(endNode);
    startNode->parent = NULL;
    openList.push_back(startNode);
    if (log) soubor << "First node added" << std::endl;

    u32 passCount = 1;
    bool pathFound = false;

    if (startNode == endNode) pathFound = true;

    while(openList.getSize() != 0)
    {
        if (log) soubor << std::endl << "Iteration number " << passCount << std::endl;

        //Remove the node with the lowest F score from the open list
        //The list is sorted, therefore removing the first node will do...
        PathNode* lowestScoreNode = *openList.begin();
        core::list<PathNode*>::Iterator toErase = openList.begin();
        openList.erase(toErase);
        lowestScoreNode->list = PIL_CLOSED;

        if (log)
        {
            soubor << "Lowest F score node in open list is: " <<
            lowestScoreNode->position.X << "," << lowestScoreNode->position.Y <<
            "," << lowestScoreNode->position.Z << ", F score: " <<
            lowestScoreNode->Fdist << ", G distance: " << lowestScoreNode->Gdist <<
            std::endl << "Adding neighbours to open list" << std::endl;
        }

        //Iterate through the neighbours of the PathNode
        core::list<PathNode*>::Iterator iter2 = lowestScoreNode->neighbours.begin();
        u32 i = 1;
        for (;iter2 != lowestScoreNode->neighbours.end(); ++i)
        {
            if (log) soubor << "    Neighbour " << i << ": " <<
                (*iter2)->position.X << "," << (*iter2)->position.Y << "," <<
                (*iter2)->position.Z;

            //If the neighbour is the end node, set its parent and don't care
            //about anything anymore
            if (*iter2 == endNode){
                endNode->parent = lowestScoreNode;
                pathFound = true;
                if (log) soubor << ", is the end node, leaving..." << std::endl;
                break;
            }

            //If the neighbour is on the closed list, leave
            if ((*iter2)->list == PIL_CLOSED)
            {
                if (log) soubor << ", is on the closed list, DISCARDING..." <<
                std::endl;
                ++iter2;
                continue;
            }

            //If the neighbour isn't on any of the lists, calculate its F and G,
            //set its parent to the current node, add it to open list and leave
            if ((*iter2)->list == PIL_NONE)
            {
                if (log) soubor << ", isn't on any of the lists, adding to open list..." <<
                std::endl;
                (*iter2)->Gdist = lowestScoreNode->distance(*iter2) + lowestScoreNode->Gdist;
                (*iter2)->Fdist = (*iter2)->Gdist + (*iter2)->Hdist(endNode);
                (*iter2)->parent = lowestScoreNode;
                (*iter2)->list = PIL_OPEN;
                //Add node to open list
                if (openList.getSize() == 0) {openList.push_back(*iter2);}
                else {
                    core::list<PathNode*>::Iterator addBefore = openList.begin();
                    while (addBefore != openList.end() && (*addBefore)->Fdist < (*iter2)->Fdist) addBefore++;
                    if (addBefore == openList.end()) openList.push_back(*iter2);
                    else openList.insert_before(addBefore, *iter2);
                }
                ++iter2;
                continue;
            }

            //Now the node can only be in the open list, so we just check if our
            //current path isn't better than the older path of the node. If it
            //is, we just change the parent of the node and its position in the
            //open list
            if (log) soubor << ", is already on the open list";
            u32 Gtemp = lowestScoreNode->distance(*iter2) + lowestScoreNode->Gdist;

            if (Gtemp < (*iter2)->Gdist)    //If the current path is better...
            {
                if (log) soubor << ", and is better, changing parent..." << std::endl;
                PathNode* currentNode = *iter2;
                currentNode->Gdist = Gtemp;
                currentNode->Fdist = Gtemp + currentNode->Hdist(endNode);
                currentNode->parent = lowestScoreNode;
                core::list<PathNode*>::Iterator toErase = openList.begin();
                while (*toErase != currentNode) toErase++;
                openList.erase(toErase);

                //Add node to open list
                if (openList.getSize() == 0) {openList.push_back(currentNode);}
                else {
                    core::list<PathNode*>::Iterator addBefore = openList.begin();
                    while (addBefore != openList.end() && (*addBefore)->Fdist < currentNode->Fdist) addBefore++;
                    if (addBefore == openList.end()) openList.push_back(currentNode);
                    else openList.insert_before(addBefore, currentNode);
                }
            }
            else {
                if (log) soubor << ", and is not better, going on..." << std::endl;
            }

            ++iter2;
        }
        if (pathFound) break;

        passCount++;
    }

    pathSegments.clear();
    if (pathFound)
    {
        if (log) soubor << std::endl << "Path exists and FOUND." << std::endl;
        PathNode* prevPathNode = endNode;
        while (prevPathNode != NULL)
        {
            pathSegments.push_front(prevPathNode);
            prevPathNode = prevPathNode->parent;
        }
    } else if (log) soubor << std::endl << "Path NOT FOUND." << std::endl;

    if (log)
    {
        core::list<PathNode*>::Iterator iter = pathSegments.begin();
        u32 nn = 1;
        while (iter != pathSegments.end())
        {
            soubor << "PathNode " << nn << ": " << (*iter)->position.X << "," <<
            (*iter)->position.Y << "," << (*iter)->position.Z << std::endl;
            iter++;
            nn++;
        }
        timeStats = timer->getTime() - timeStats;
        soubor << "Pathfinding took " << timeStats << " timer ticks (milliseconds).";
    }

    state = 0;
    soubor.close();
    return pathFound;
}

// Saves the pathfinding net to a file
void Network::saveNetToFile(const c8* filename)
{
    ofstream soubor(filename);

    PathNode* helper;
    core::list<PathNode*>::Iterator iter = nodes.begin();

    soubor << nodes.getSize() << std::endl;

    while (iter != nodes.end())
    {
        helper = *iter;
        soubor << (helper->position.X) << std::endl;
        soubor << (helper->position.Y) << std::endl;
        soubor << (helper->position.Z) << std::endl;
        soubor << (helper->neighbours.getSize()) << std::endl;

        core::list<PathNode*>::Iterator iterNeigh = helper->neighbours.begin();

        while (iterNeigh != helper->neighbours.end())
        {
            soubor << ((*iterNeigh)->position.X) << std::endl;
            soubor << ((*iterNeigh)->position.Y) << std::endl;
            soubor << ((*iterNeigh)->position.Z) << std::endl;
            iterNeigh++;
        }

        iter++;
    }

    soubor.close();
}

// Loads pathfinding network from a file
void Network::loadNetFromFile(const c8* filename)
{
    reinitializeNet();

    ifstream soubor;
    soubor.open(filename);
    if (!soubor) return;

    u32 ns;
    soubor >> ns;

    for (u32 i = 0; i != ns; i++)
    {
        core::vector3df newpos;
        soubor >> newpos.X;
        soubor >> newpos.Y;
        soubor >> newpos.Z;
        addNode(newpos);

        u32 n;
        soubor >> n;
        float unused;

        for (u32 j = 0; j != n; j++)
        {
            soubor >> unused; soubor >> unused; soubor >> unused;
        }
    }

    soubor.close();

    soubor.open(filename);

    soubor >> ns;

    for (u32 i = 0; i != ns; i++)
    {
        core::vector3df thispos;
        soubor >> thispos.X;
        soubor >> thispos.Y;
        soubor >> thispos.Z;
        PathNode* thispoint = nearestNodePointer(thispos);

        u32 n;
        soubor >> n;

        for (u32 j = 0; j != n; j++)
        {
            core::vector3df otherpos;
            soubor >> otherpos.X;
            soubor >> otherpos.Y;
            soubor >> otherpos.Z;

            thispoint->setNeighbour(nearestNodePointer(otherpos));
        }
    }

    soubor.close();
}

// Reinitializes the Network, deleting all pointers and lists
void Network::reinitializeNet()
{
    core::list<PathNode*>::Iterator iter = nodes.begin();
    while (iter != nodes.end())
    {
        delete *iter;
        iter++;
    }

    nodes.clear();
    pathSegments.clear();

    startNode = NULL;
    endNode = NULL;
    selectedNode = NULL;

    return;
}
