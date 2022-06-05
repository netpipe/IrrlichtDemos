#include "PathNode.h"
#include "CTile.h"

irr::core::list<CTile*> PathNode::ClosedList;
irr::core::array<PathNode*> PathNode::Goals;

PathNode::PathNode(CTile* tile, CTile* target, PathNode* parent, int g)
    {
        Children[0] = NULL;
        Children[1] = NULL;
        Children[2] = NULL;
        Children[3] = NULL;
        Parent = parent;
        Tile = tile;
        Target = target;
        G = g+1;
        H = abs(target->getGridPos().X-tile->getGridPos().X) + abs(target->getGridPos().Y-tile->getGridPos().Y);

        ClosedList.push_back(Tile);
    }
    PathNode::~PathNode(void)
    {
        if (Children[0])
            delete Children[0];
        if (Children[1])
            delete Children[1];
        if (Children[2])
            delete Children[2];
        if (Children[3])
            delete Children[3];
    }
    bool PathNode::isInPath(CTile* tile)
    {
        if (Tile == tile)
            return true;
        if (Parent)
            return Parent->isInPath(tile);
        return false;
    }
    bool PathNode::addChildren(void)
    {
        if (Tile == Target)
        {
            Goals.push_back(this);
            return true;
        }

        //printf("Add Children\n");

        if (Tile->getUp() && !isInPath(Tile->getUp()) && (Tile->getUp()->getType() == ETT_STREET || Tile->getUp()->getType() == ETT_GOAL))
        {
            //printf("Up is walkable\n");
            Children[0] = new PathNode(Tile->getUp(), Target, this, G);
        }

        if (Tile->getRight() && !isInPath(Tile->getRight()) && (Tile->getRight()->getType() == ETT_STREET || Tile->getRight()->getType() == ETT_GOAL))
        {
            //printf("Right is walkable\n");
            Children[1] = new PathNode(Tile->getRight(), Target, this, G);
        }

        if (Tile->getDown() && !isInPath(Tile->getDown()) && (Tile->getDown()->getType() == ETT_STREET || Tile->getDown()->getType() == ETT_GOAL))
        {
            //printf("Down is walkable\n");
            Children[2] = new PathNode(Tile->getDown(), Target, this, G);
        }

        if (Tile->getLeft() && !isInPath(Tile->getLeft()) && (Tile->getLeft()->getType() == ETT_STREET || Tile->getLeft()->getType() == ETT_GOAL))
        {
            //printf("Left is walkable\n");
            Children[3] = new PathNode(Tile->getLeft(), Target, this, G);
        }

        irr::core::list<PathNode*> Nodes;
        sortNodes(Nodes);
        irr::core::list<PathNode*>::Iterator it = Nodes.begin();
        while (it != Nodes.end())
        {
            if ((*it)->addChildren())
            {
                //return true;
            }
            ++it;
        }
        return false;
    }
    void PathNode::sortNodes(irr::core::list<PathNode*>& Nodes)
    {
        for (int i=0;i<4;++i)
        {
            irr::core::list<PathNode*>::Iterator it = Nodes.begin();
            while (it != Nodes.end())
            {
                if (Children[i] && Children[i]->F() < (*it)->F())
                {
                    Nodes.insert_before(it, Children[i]);
                    break;
                }
                ++it;
            }
            if (Children[i])
            Nodes.push_back(Children[i]);
        }
    }

    void PathNode::getPath(irr::core::list<CTile*>& Path)
    {
        Path.push_front(Tile);
        if (Parent)
            Parent->getPath(Path);
    }

    bool PathNode::isInClosedList(CTile* tile)
    {
        irr::core::list<CTile*>::Iterator it = ClosedList.begin();
        while (it != ClosedList.end())
        {
            if (tile == *it)
                return true;
            ++it;
        }
        return false;
    }

    int PathNode::F(void)
    {
        return G+H;
    }
