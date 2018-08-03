#ifndef IOCTREENODE_H
#define IOCTREENODE_H

#include <irrlicht.h>
#include <vector>

class IOctree;

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

using namespace std;

enum OCTREE_CHILD_INDEX
{
    OCI_L_L_L = 0,
    OCI_G_L_L = 1,
    OCI_L_G_L = 2,
    OCI_L_L_G = 3,
    OCI_G_G_L = 4,
    OCI_L_G_G = 5,
    OCI_G_L_G = 6,
    OCI_G_G_G = 7,
};

class IOctreeNode
{
    public:

        IOctreeNode(IOctreeNode* parent, ICameraSceneNode* octreeCam);
        virtual ~IOctreeNode();

        //adds an irrlicht scene node to this octree node
        void addSceneNode(ISceneNode* node, bool g = false);

        void cleanEmptyBoxes();

        void draw(IVideoDriver* driver);//debug only

        vector3df getCenter();

        bool isEmpty();

        void setChild(OCTREE_CHILD_INDEX index, IOctreeNode* child);

        void setMainStructure(IOctree* oct);

        void setVisible(bool visible);

        //u = 0 (box intersects frustum - verify if the box needs to be drawed)
        //u = 1 (box is fully inside frustum and needs to be drawed - avoid verification)
        //u = 2 (box is fully outside frustum and does not needs to be drawed - avoid verification)
        void update(int u = 0);
    protected:
        vector3df MinEdge;
        vector3df MaxEdge;
    private:
        IOctree* mainStructure;
        IOctreeNode* parent;

        IOctreeNode* childs[8];

        vector<ISceneNode*> irrSceneNodes;

        f32 getBoxArea();

        ICameraSceneNode* octreeCam;
        const SViewFrustum* frustum;
        plane3df planes[6];

        bool visible;

        void addNodeHere(ISceneNode* node);
};

#endif // IOCTREENODE_H
