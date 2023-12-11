#ifndef _A_IRR_PATHFIND_H_
#define _A_IRR_PATHFIND_H_

// Copyright (C) 2005 Cestmir "CZestmyr" Houska
// This file is using some classes from "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "irrlicht.h"
#include "fstream.h"

using namespace irr;

enum PATHNODE_IN_LIST{
    PIL_NONE = 0,
    PIL_OPEN,
    PIL_CLOSED
};

class PathNode{
    public:
    
        // constructor
        PathNode(const core::vector3df& pos);

        // returns the distance from another PathNode
        u32 distance(PathNode* other);
        
        // returns overestimated distance to a PathNode. It is used by the 
        // pathfinding algorithm to explore those PathNodes first, that are more
        // likely to lead to the goal.
        u32 Hdist(PathNode* other);
        
        // adds the other PathNode to the list of the neighbours
        void setNeighbour(PathNode* other);
        
        // removes the other PathNode from the list of the neighbours
        void removeNeighbour(PathNode* other);
        
        // checks, whether the other PathNode is a neighbour of this node
        bool isNeighbour(PathNode* other);
        
        // draws the node with the given driver, using the given color
        void draw(video::IVideoDriver* driver, const video::SColor& color);

        core::vector3df position;             // Position of this node
        u32 Gdist;                            // G - already walked distance
        u32 Fdist;                            // F - walked distance + overestimated distance to the goal
        core::list<PathNode*> neighbours;     // neighbour nodes of this node
        PathNode* parent;                     // parent node (used to determine path)
        PATHNODE_IN_LIST list;
};

class path: public core::list<PathNode*>{
    public:

        // constructor
        path();

        // Copy constructor
        path(const path& other);

        // = operator
        path& operator=(const path& other);

        // checks, whether the path contains the searched node
        bool contains(PathNode* searchedNode);
};

// IMPORTANT: The network is not intended for copying - it does not contain
// copy constructor.

class Network{
    public:

        // constructor
        Network(video::IVideoDriver* driv, ITimer* tim, bool createLogFile = false);

        // destructor
        ~Network();

        // draws the node, for testing and editiong purposes
        void draw();

        // returns pointer to the node, that is nearest to the given point
        PathNode* nearestNodePointer(const core::vector3df& pos);

        // adds a node to this network at the given point
        void addNode(const core::vector3df& pos);

        // adds two-way connection between two nodes
        void addConnection(PathNode* a, PathNode* b);

        // adds one-way connection between two nodes
        void addOneWayConnection(PathNode* from, PathNode* to);

        // removes node from net with all its connections
        void removeNodeFromNet(PathNode* a);

        // checks, whether there is any connection between two nodes
        bool connectionExists(PathNode* a, PathNode* b);

        // selects a node for purpose of making a two-way connection
        void selectNode(const core::vector3df& pos);

        // selects first and then a second node in order to find a path between
        // them. Also stores this path to the pathSegments member.
        // Returns true in case of success, else returns false
        bool findPath(const core::vector3df& fromOrTo);

        // Saves the pathfinding net to a file
        void saveNetToFile(const c8* filename);

        // Loads pathfinding network from a file
        void loadNetFromFile(const c8* filename);

        // Reinitializes the Network, deleting all pointers and lists
        void reinitializeNet();

        bool log;
        path pathSegments;

    private:

        video::IVideoDriver* driver;
        u16 state;
        PathNode* selectedNode;
        PathNode* startNode;
        PathNode* endNode;
        core::list<PathNode*> nodes;
        ITimer* timer;
};

#endif
