#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <irrlicht.h>

class PathFinderMap
{
public:

    virtual bool isPath(int x, int y) = 0;
    virtual bool addPathMarker(int x, int y) = 0;
    virtual void clearPathMarkers() = 0;
};

typedef struct PathMarkerType {
    int x;
    int y;
} PathMarker;

typedef irr::core::list<PathMarker> PathMarkerList;

class PathNode;

class PathNodeList : public irr::core::list<PathNode*>
{
public:

    void destroyNodes();
};

class PathNode {
public:

    PathNode(int x, int y, int c);
    ~PathNode();

    int x;
    int y;
    int cost;

    PathNode *parent;
};

class PathFinder
{
public:

    PathFinder();
    ~PathFinder();

    bool run();
    void findPath(PathFinderMap *map, int sx, int sy, int dx, int dy);
    bool isReady();
    bool isFound();
    void clear();

private:

    PathNode* addNode(int x, int y, PathNode* parent = 0);
    bool isOpenPath(int x, int y);
    void savePath(PathFinderMap *map);

    bool ready;
    bool found;
    PathFinderMap *map;

    PathNodeList nodePaths;
    PathNodeList traversedNodePaths;
    PathNode *lastPathNode;
    PathNode *nearestPathNode;

    int targetX;
    int targetY;
    int iterations;
};

#endif // PATHFINDER_H
