#include "PathFinder.h"

PathNode::PathNode(int x, int y, int c) : x(x), y(y), cost(c), parent(0)
{}

PathNode::~PathNode()
{}

void PathNodeList::destroyNodes() {
    Iterator it = begin();
    while (it != end()) {
        PathNode *p = *it;
        delete p;
        it++;
    }
    clear();
}

PathFinder::PathFinder() : ready(false), found(false)
{}

PathFinder::~PathFinder()
{
    clear();
}

bool PathFinder::run()
{
    if (!nodePaths.size())
        return false;

    iterations++;

    PathNodeList::Iterator it = nodePaths.begin();
    PathNode *n = *it;
    nodePaths.erase(it);
    traversedNodePaths.push_back(n);

    // up
    if (map->isPath(n->x, n->y - 1)) {
        addNode(n->x, n->y - 1, n);
        if (isReady()) {
            return true;
        }
    }

    // down
    if (map->isPath(n->x, n->y + 1)) {
        addNode(n->x, n->y + 1, n);
        if (isFound()) {
            return true;
        }
    }

    // left
    if (map->isPath(n->x - 1, n->y)) {
        addNode(n->x - 1, n->y, n);
        if (isFound()) {
            return true;
        }
    }

    // right
    if (map->isPath(n->x + 1, n->y)) {
        addNode(n->x + 1, n->y, n);
        if (isFound()) {
            return true;
        }
    }

    if (!nodePaths.size()) {
        ready = true;
    }

    return isReady();
}

void PathFinder::clear()
{
    iterations = 0;
    found = false;
    ready = false;
    nodePaths.destroyNodes();
    lastPathNode = 0;
    nearestPathNode = 0;
    traversedNodePaths.destroyNodes();
}

void PathFinder::findPath(PathFinderMap *map, int sx, int sy, int dx, int dy)
{
    this->map = map;

    clear();

    targetX = dx;
    targetY = dy;

    addNode(sx, sy);
}

PathNode* PathFinder::addNode(int x, int y, PathNode* parent)
{
    if (isOpenPath(x, y))
        return 0;

    int dx = targetX - x;
    int dy = targetY - y;
    int distanceSq = (dx * dx) + (dy * dy);

    PathNode *n = new PathNode(x, y, distanceSq);
    n->parent = parent;
    nodePaths.push_back(n);
    
    if (distanceSq == 1) {
        if (!nearestPathNode) {
            nearestPathNode = n;
            savePath(map);
        }
    }

    if (distanceSq == 0) {
        ready = true;
        found = true;
        lastPathNode = n;
        nearestPathNode = n;
        savePath(map);
    }

    return n;
}

bool PathFinder::isOpenPath(int x, int y)
{
    // queued
    PathNodeList::Iterator it = nodePaths.begin();
    while (it != nodePaths.end()) {
        PathNode *n = *it;
        if (n->x == x && n->y == y)
            return true;
        it++;
    }

    // traversed
    it = traversedNodePaths.begin();
    while (it != traversedNodePaths.end()) {
        PathNode *n = *it;
        if (n->x == x && n->y == y)
            return true;
        it++;
    }
    return false;
}

bool PathFinder::isReady()
{
    return ready;
}

bool PathFinder::isFound()
{
    return found;
}

void PathFinder::savePath(PathFinderMap *map)
{
    map->clearPathMarkers();
    if (nearestPathNode) {
        PathNode *n = nearestPathNode;
        while (n) {
            if (!map->addPathMarker(n->x, n->y))
                return;
            n = n->parent;
        }
    }
}
