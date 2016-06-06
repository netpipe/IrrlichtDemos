/*-----------------------------------------------------------------------------*
| headerfile APath.h                                                           |
|                                                                              |
| version 1.10                                                                 |
| date: (11.05.2007)                                                           |
|                                                                              |
| author:  Michal Švantner                                                     |
|                                                                              |
| for Irrlicht engine                                                          |
| path finding based on A*                                                     |
*-----------------------------------------------------------------------------*/

#ifndef APATH_H
#define APATH_H

#include <irrlicht.h>
using namespace irr;



enum NODE_STATUS
{
    UNUSED = 0,
    OPEN,
    CLOSED,
};



class MapNode
{
    // position
    int x;
    int y;
    
    // traffic capacity, 0 = blocked, 100 = not restricting movement
    int cap;
    
    // G, H, F values for A* pathfinding calculations
    int g;
    int h;
    int f;
    
    // is node on open or closed list or unused
    NODE_STATUS stat;
    
    // parent node for A* pathfinding
    MapNode* par;
    
public:
    
    MapNode()
    {
        cap = 100;
        g = 0; h = 0; f = 0;
        stat = UNUSED;
        par = NULL;
    }
    
    ~MapNode(){}
    
    void setPosition(core::position2d<s32> i)
    {
        x = i.X;
        y = i.Y;
    }
    
    void setPosition(int nx, int ny)
    {
        x = nx;
        y = ny;
    }
    
    void setCapacity(int i) {cap = i;}
    
    void setG(int i){g = i;}
    
    void setH(int i){h = i;}
    
    void setF(int i){f = i;}
    
    void setStatus(NODE_STATUS s){stat = s;}
    
    void setParent(MapNode* i){par = i;}
    
    core::position2d<s32> position(){return core::position2d<s32>(x,y);}
    
    int X(){return x;}
    
    int Y(){return y;}
    
    int capacity(){return cap;}
    
    int G(){return g;}
    
    int H(){return h;}
    
    int F(){return f;}
    
    NODE_STATUS status(){return stat;}
    
    MapNode* parent(){return par;}
    
    void reset()
    {
        g = 0; h = 0; f = 0;
        stat = UNUSED;
        par = NULL;
    }
};




class PlayerNode
{
    // position
    MapNode *pos;
    
    // target
    MapNode *target;
    
    // waypoints
    core::list<MapNode*> waypoints;

public:
    
    PlayerNode(MapNode *position)
    {
        pos = position;
        target = NULL;
    }
    
    // move to next waypoint
    bool move()
    {
        core::list<MapNode*>::Iterator p = waypoints.begin();
        if(p == waypoints.end()) return false;
        pos = *p;
        core::list<MapNode*>::Iterator tmp = waypoints.begin();
        waypoints.erase(tmp);
        return true;
    }
    
    MapNode* getPosition() {return pos;}
    
    void setPosition(MapNode* node) {pos = node;}
    
    MapNode* getTarget() {return target;}
    
    void setTarget(MapNode* node) {target = node;}
    
    void addWaypointEnd(MapNode* node) {waypoints.push_back(node);}
    
    void addWaypointFront(MapNode* node) {waypoints.push_front(node);}
    
    u32 getWaipointsCount()
    {
        return waypoints.getSize();
    }
    
    MapNode* getWaipoint(u32 i)
    {
        core::list<MapNode*>::Iterator it = waypoints.begin();
        it+=i;
        return *it;
    }
    
    void clearWaipoints()
    {
        waypoints.clear();
    }
};




int calcG(MapNode* dest, MapNode* origin)
{
    // cost to move from one tile to another -10 verticaly and horizontaly, 14 diagonaly
    int cost;
    if(origin->X() == dest->X() || origin->Y() == dest->Y()) cost = 10;
    else cost = 14;
    cost = dest->capacity() * cost / 100;
    return origin->G() + cost;
}

int calcH(MapNode* node, MapNode* end)
{
    // calculated using Manhattan method
    int xcost = end->X() - node->X();
    if(xcost < 0) xcost = -xcost;
    int ycost = end->Y() - node->Y();
    if(ycost < 0) ycost = -ycost;
    
    return(xcost + ycost) * 10;
}

int calcF(MapNode* node)
{
    return node->G() + node->H();
}

bool findPath(MapNode **map, int mapsizeX, int mapsizeY, PlayerNode *player)
{
    MapNode *start = player->getPosition();
    MapNode *end = player->getTarget();
    
    // create open and closed lists
    core::list<MapNode*> openlist;
    core::list<MapNode*> closedlist;
    
    // start pathfinding by puting start location to open list
    openlist.push_back(start);
    start->setStatus(OPEN);
    start->setG(0);
    start->setH(calcH(start, end));
    start->setF(calcF(start));
    
    bool path_exist = true;
    
    MapNode* current = start;
    
    // look for path until target node is reached
    while(current != end)
	{
        // if there are no nodes left in open list no path exist
        if(openlist.empty())
        {
            path_exist = false;
            break;
        }
            
        // take tile with lovest F from open list
        core::list<MapNode*>::Iterator test = openlist.begin();
        core::list<MapNode*>::Iterator lowest = test;
        while(test != openlist.getLast())
        {
            test++;
            if( (*test)->F() < (*lowest)->F() ) lowest = test;
        }
        current = *lowest;
        
        // put tile with lowest F to closed list
        openlist.erase(lowest);
        closedlist.push_back(current);
        current->setStatus(CLOSED);
    
        // test adjacent tiles
        for(int i=0; i<8; i++)
        {
            int x, y;
            switch(i)
            {
                case 0: x = current->X() + 1; y = current->Y() + 0; break;
                case 1: x = current->X() + 1; y = current->Y() + 1; break;
                case 2: x = current->X() + 0; y = current->Y() + 1; break;
                case 3: x = current->X() - 1; y = current->Y() + 1; break;
                case 4: x = current->X() - 1; y = current->Y() + 0; break;
                case 5: x = current->X() - 1; y = current->Y() - 1; break;
                case 6: x = current->X() + 0; y = current->Y() - 1; break;
                case 7: x = current->X() + 1; y = current->Y() - 1; break;
            }
            
            // be sure not to reach out of bounds
            if(x>=0 && y>=0 && x<mapsizeX && y<mapsizeY)
            {
                MapNode* adjacent = &map[x][y];
            
                // if tile is in open list test if path from current tile is not better
                if(adjacent->status() == OPEN)
                {
                    int g = calcG(adjacent, current);
                    if(g < adjacent->G())
                    {
                        adjacent->setParent(current);
                        adjacent->setG(g);
                        adjacent->setF(calcF(adjacent));
                    }
                }
                else
                {
                    // if tile is free add it to open list
                    if(adjacent->capacity() != 0 && adjacent->status() != CLOSED)
                    {
                        adjacent->setG( calcG(adjacent, current) );
                        adjacent->setH( calcH(adjacent, end) );
                        adjacent->setF( calcF(adjacent) );
                        adjacent->setParent(current);
                        openlist.push_back(adjacent);
                        adjacent->setStatus(OPEN);
                    }
                }
            }
        }
    }
    
    // add waipoints in to list
    if(path_exist)
        while(current->parent())
        {
            player->addWaypointFront(current);
            current = current->parent();
        }
    
    // clear nodes in closed list
    core::list<MapNode*>::Iterator it = closedlist.begin();
    while(it != closedlist.end())
    {
        (*it)->reset();
        it++;
    }
    
    // clear nodes in open list
    it = openlist.begin();
    while(it != openlist.end())
    {
        (*it)->reset();
        it++;
    }
        
    return path_exist;
}
#endif
