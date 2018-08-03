#ifndef IOCTREE_H
#define IOCTREE_H

#include <irrlicht.h>

#include "IOctreeNode.h"

//discomment this to print debug information on the console
//#define __IOCTREE_PRINT_DEBUG__

using namespace irr;
using namespace core;
using namespace scene;

const int BBOX_OUTSIDE = 2;
const int BBOX_INSIDE = 1;
const int BBOX_INTERSECT = 0;

class IOctree
{
    public:
        IOctree(ICameraSceneNode* octreeCam);
        virtual ~IOctree();

        //adds a new node to the structure
        void addSceneNode(ISceneNode* node);

        void draw(IVideoDriver* driver);//debug only

        //enables/disables octree culling. When disable all nodes are rendered as the original irrlicht does
        void setEnabled();

        //set the active octree camera
        void setOctreeCamera(ICameraSceneNode* octreeCam);

        //Update the octree culling. You should call this method every frame before smgr->drawAll()
        void update();

    protected:
        void setRoot(IOctreeNode* newRoot);
    private:
        //Scene culling will be based on this camera frustum
        ICameraSceneNode* octreeCam;

        //six camera frustum planes
        plane3df planes[6];

        //octree root
        IOctreeNode* root;

        friend class IOctreeNode;
};

#endif // IOCTREE_H
