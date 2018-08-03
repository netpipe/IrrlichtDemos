#include "IOctreeNode.h"
#include "IOctree.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace std;

IOctreeNode::IOctreeNode(IOctreeNode* parent, ICameraSceneNode* octreeCam)
{
    this->octreeCam = octreeCam;
    frustum = octreeCam->getViewFrustum();

    for(int i=0;i<8;i++) childs[i] = NULL;
    this->parent = parent;

    MinEdge = vector3df(-0.5,-0.5,-0.5);
    MaxEdge = vector3df(0.5,0.5,0.5);
}

IOctreeNode::~IOctreeNode()
{
    this->irrSceneNodes.clear();

    for(int i=0;i<8;i++)
        if (childs[i] != NULL) delete childs[i];
}

void IOctreeNode::addNodeHere(ISceneNode* node)
{
    bool nodeAlreadyAdded = false;
    for(int i=0 ; i< irrSceneNodes.size(); i++)
        if(irrSceneNodes[i] == node)
        {
            nodeAlreadyAdded = true;
            break;
        }

    if(!nodeAlreadyAdded)
    {
        irrSceneNodes.push_back(node);
        #ifdef __IOCTREE_PRINT_DEBUG__
        printf("node added\n");
        #endif
    }
    else
    {
        #ifdef __IOCTREE_PRINT_DEBUG__
        printf("not added: node already exists in octree!\n");
        #endif
    }
}

void IOctreeNode::addSceneNode(ISceneNode* node, bool g)
{
    vector3df nodeMinEdge = node->getAbsolutePosition() + node->getBoundingBox().MinEdge;
    vector3df nodeMaxEdge = node->getAbsolutePosition() + node->getBoundingBox().MaxEdge;

    if(aabbox3df(nodeMinEdge, nodeMaxEdge).isFullInside(aabbox3df(MinEdge,MaxEdge)))
    {
        f32 biggerEdge = max_(nodeMaxEdge.X - nodeMinEdge.X,
                              nodeMaxEdge.Y - nodeMinEdge.Y,
                              nodeMaxEdge.Z - nodeMinEdge.Z);

        if(biggerEdge <= (MaxEdge.X - MinEdge.X)/2)
        {
            #ifdef __IOCTREE_PRINT_DEBUG__
            printf("split into\n");
            #endif

            OCTREE_CHILD_INDEX oci;
            vector3df newChildMinEdge;
            vector3df newChildMaxEdge;
            f32 hSize = (MaxEdge.X - MinEdge.X)/2;

            if(node->getAbsolutePosition().X > this->getCenter().X)
                if(node->getAbsolutePosition().Y > this->getCenter().Y)
                    if(node->getAbsolutePosition().Z > this->getCenter().Z)
                        {
                            oci = OCI_G_G_G;
                            newChildMinEdge = this->getCenter();
                            newChildMaxEdge = MaxEdge;
                        }
                    else
                        {
                            oci = OCI_G_G_L;
                            newChildMinEdge = this->getCenter()+vector3df(0,0,-hSize);
                            newChildMaxEdge = MaxEdge+vector3df(0,0,-hSize);
                        }
                else
                    if(node->getAbsolutePosition().Z > this->getCenter().Z)
                        {
                            oci = OCI_G_L_G;
                            newChildMinEdge = this->getCenter()+vector3df(0,-hSize,0);
                            newChildMaxEdge = MaxEdge+vector3df(0,-hSize,0);
                        }
                    else
                        {
                            oci = OCI_G_L_L;
                            newChildMinEdge = MinEdge + vector3df(hSize,0,0);
                            newChildMaxEdge = this->getCenter() + vector3df(hSize,0,0);
                        }
            else
                if(node->getAbsolutePosition().Y > this->getCenter().Y)
                    if(node->getAbsolutePosition().Z > this->getCenter().Z)
                        {
                            oci = OCI_L_G_G;
                            newChildMinEdge = this->getCenter() + vector3df(-hSize,0,0);
                            newChildMaxEdge = MaxEdge + vector3df(-hSize,0,0);
                        }
                    else
                        {
                            oci = OCI_L_G_L;
                            newChildMinEdge = MinEdge + vector3df(0,hSize,0);
                            newChildMaxEdge = this->getCenter() + vector3df(0,hSize,0);
                        }
                else
                    if(node->getAbsolutePosition().Z > this->getCenter().Z)
                    {
                        oci = OCI_L_L_G;
                        newChildMinEdge = MinEdge + vector3df(0,0,hSize);
                        newChildMaxEdge = this->getCenter() + vector3df(0,0,hSize);
                    }
                    else
                    {
                        oci = OCI_L_L_L;
                        newChildMinEdge = MinEdge;
                        newChildMaxEdge = this->getCenter();
                    }

            //check if the node is smaller than a half but intercepts the child, is this case the node
            //cannot be added in the child, so add it to the current box.
            if(aabbox3df(nodeMinEdge,nodeMaxEdge).isFullInside(aabbox3df(newChildMinEdge,newChildMaxEdge)))
            {
                if(!childs[oci])
                {
                    #ifdef __IOCTREE_PRINT_DEBUG__
                    printf("create child - split into\n");
                    #endif
                    childs[oci] = new IOctreeNode(this, octreeCam);
                    childs[oci]->MinEdge = newChildMinEdge;
                    childs[oci]->MaxEdge = newChildMaxEdge;
                    childs[oci]->addSceneNode(node);
                }
                else
                {
                    #ifdef __IOCTREE_PRINT_DEBUG__
                    printf("use existent child - split into\n");
                    #endif
                    childs[oci]->addSceneNode(node);
                }
            }
            else//intercepts the child
            {
                #ifdef __IOCTREE_PRINT_DEBUG__
                printf("node add: non minimal size - intercepts smaller childs)\n");
                #endif
                this->addNodeHere(node);
            }
        }
        else
        {
            this->addNodeHere(node);
        }
    }
    else
    {
        #ifdef __IOCTREE_PRINT_DEBUG__
        printf("split up!\n");
        #endif

        if(parent == NULL)
        {
            #ifdef __IOCTREE_PRINT_DEBUG__
            printf("create new parent\n");
            #endif

            IOctreeNode* newParent = new IOctreeNode(NULL, octreeCam);
            newParent->setMainStructure(this->mainStructure);
            parent = newParent;
            mainStructure->setRoot(newParent);

            f32 axisOffset = (MaxEdge-MinEdge).X;

            //parent size = double of this box
            vector3df parentMinEdge;
            vector3df parentMaxEdge;
            OCTREE_CHILD_INDEX oci;

            if(g)
            {
                parentMinEdge = MinEdge;
                parentMaxEdge = MaxEdge + (MaxEdge - MinEdge);
                oci = OCI_L_L_L;
            }
            else
            {
                parentMinEdge = MinEdge - (MaxEdge - MinEdge);
                parentMaxEdge = MaxEdge;
                oci = OCI_G_G_G;
            }

            newParent->setChild(oci, this);
            newParent->MinEdge=parentMinEdge;
            newParent->MaxEdge=parentMaxEdge;

            #ifdef __IOCTREE_PRINT_DEBUG__
            printf("%f %f %f - %f %f %f\n",
                   newParent->MinEdge.X,newParent->MinEdge.Y,newParent->MinEdge.Z,
                   newParent->MaxEdge.X,newParent->MaxEdge.Y,newParent->MaxEdge.Z);
            #endif

            newParent->addSceneNode(node, 1-g);
        }
        else
        {
            #ifdef __IOCTREE_PRINT_DEBUG__
            printf("add to existent parent\n");
            #endif
            parent->addSceneNode(node);
        }
    }
}

void IOctreeNode::cleanEmptyBoxes()
{
    for(int i=0;i<8;i++)
        if(childs[i] != NULL)
        {
            childs[i]->cleanEmptyBoxes();
            if(childs[i]->isEmpty())
            {
                delete childs[i];
                childs[i] = NULL;
            }
        }
}

void IOctreeNode::draw(IVideoDriver* driver)
{
    #ifdef __IOCTREE_PRINT_DEBUG__
    if(visible)
    {
        driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
        driver->draw3DBox(aabbox3df(MinEdge,MaxEdge));

        for(int i=0;i<8;i++) if(childs[i]) childs[i]->draw(driver);
    }
    #endif
}


vector3df IOctreeNode::getCenter()
{
    return MaxEdge - (MaxEdge - MinEdge)/2;
}

f32 IOctreeNode::getBoxArea()
{
    vector3df e = MaxEdge - MinEdge;
    return 2*(e.X*e.Y + e.X*e.Z + e.Y*e.Z);
}

bool IOctreeNode::isEmpty()
{
    bool hasChilds = false;

    for(int i=0;i<8;i++) if(childs[i]) hasChilds = true;

    bool hasIrrNodes = irrSceneNodes.size() != 0;

    return !(hasChilds || hasIrrNodes);
}

void IOctreeNode::setChild(OCTREE_CHILD_INDEX index, IOctreeNode* child)
{
    childs[index] = child;
}

void IOctreeNode::setMainStructure(IOctree* oct)
{
    this->mainStructure = oct;
}

void IOctreeNode::setVisible(bool visible)
{
    for(int i=0;i<8;i++)
        if (childs[i] != NULL) childs[i]->setVisible(visible);
}

void IOctreeNode::update(int u)
{
    planes[0] = frustum->planes[scene::SViewFrustum::VF_LEFT_PLANE];
    planes[1] = frustum->planes[scene::SViewFrustum::VF_TOP_PLANE];
    planes[2] = frustum->planes[scene::SViewFrustum::VF_FAR_PLANE];
    planes[3] = frustum->planes[scene::SViewFrustum::VF_NEAR_PLANE];
    planes[4] = frustum->planes[scene::SViewFrustum::VF_BOTTOM_PLANE];
    planes[5] = frustum->planes[scene::SViewFrustum::VF_RIGHT_PLANE];

    if(u == BBOX_INTERSECT)
    {
        aabbox3df bBox = aabbox3df(this->MinEdge,this->MaxEdge);

        u = BBOX_INSIDE;

        for(int pCount=0; pCount < 6; pCount++)
        {
            vector3df positiveVertex = bBox.MinEdge;

            if (planes[pCount].Normal.X >= 0)
                positiveVertex.X = bBox.MaxEdge.X;
            if (planes[pCount].Normal.Y >= 0)
                positiveVertex.Y = bBox.MaxEdge.Y;
            if (planes[pCount].Normal.Z >= 0)
                positiveVertex.Z = bBox.MaxEdge.Z;

            vector3df negativeVertex = bBox.MaxEdge;

            if (planes[pCount].Normal.X >= 0)
                negativeVertex.X = bBox.MinEdge.X;
            if (planes[pCount].Normal.Y >= 0)
                negativeVertex.Y = bBox.MinEdge.Y;
            if (planes[pCount].Normal.Z >= 0)
                negativeVertex.Z = bBox.MinEdge.Z;

            if (planes[pCount].getDistanceTo(negativeVertex) > 0)
            {
                u = BBOX_OUTSIDE;
                break;
            }
            else if (planes[pCount].getDistanceTo(positiveVertex) > 0)
                u =  BBOX_INTERSECT;
        }
    }

    bool noChilds = true;

    for(int i=0;i<8;i++) if(childs[i])
    {
        childs[i]->update(u);
        noChilds = false;
    }

    #ifdef __IOCTREE_PRINT_DEBUG__
    if(u == BBOX_INTERSECT) visible = true;
    #endif

    if(u == BBOX_INSIDE || u == BBOX_INTERSECT )
    {
        #ifdef __IOCTREE_PRINT_DEBUG__
        visible = true;
        #endif
        //for(int i=0; i<irrSceneNodes.size(); i++) irrSceneNodes[i]->setMaterialFlag(EMF_WIREFRAME,false);
        for(int i=0; i<irrSceneNodes.size(); i++) irrSceneNodes[i]->setVisible(true);
    }
    else if( u == BBOX_OUTSIDE)
    {
        #ifdef __IOCTREE_PRINT_DEBUG__
        visible = false;
        #endif
        //for(int i=0; i<irrSceneNodes.size(); i++) irrSceneNodes[i]->setMaterialFlag(EMF_WIREFRAME,true);
        for(int i=0; i<irrSceneNodes.size(); i++) irrSceneNodes[i]->setVisible(false);
    }
}
