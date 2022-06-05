#ifndef PATHNODE_H
#define PATHNODE_H

#include <irrlicht.h>

class CTile;
struct PathNode
{
    PathNode(CTile* tile, CTile* target, PathNode* parent, int g);
    ~PathNode(void);
    bool isInPath(CTile* tile);
    bool addChildren(void);
    void sortNodes(irr::core::list<PathNode*>& Nodes);

    void getPath(irr::core::list<CTile*>& Path);

    bool isInClosedList(CTile* tile);
    int F(void);

    CTile* Tile;
    CTile* Target;
    PathNode* Parent;
    PathNode* Children[4];

    int H;
    int G;
    static irr::core::list<CTile*> ClosedList;
    static irr::core::array<PathNode*> Goals;
};

#endif // PATHNODE_H
