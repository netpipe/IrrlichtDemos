#include "IOctree.h"

using namespace irr;
using namespace core;
using namespace scene;

IOctree::IOctree(ICameraSceneNode* octreeCam)
{
    this->octreeCam = octreeCam;

    const SViewFrustum* frustum = octreeCam->getViewFrustum();

    planes[0] = frustum->planes[scene::SViewFrustum::VF_LEFT_PLANE];
    planes[1] = frustum->planes[scene::SViewFrustum::VF_TOP_PLANE];
    planes[2] = frustum->planes[scene::SViewFrustum::VF_FAR_PLANE];
    planes[3] = frustum->planes[scene::SViewFrustum::VF_NEAR_PLANE];
    planes[4] = frustum->planes[scene::SViewFrustum::VF_BOTTOM_PLANE];
    planes[5] = frustum->planes[scene::SViewFrustum::VF_RIGHT_PLANE];

    root = new IOctreeNode(NULL, octreeCam);

    root->setMainStructure(this);
}

IOctree::~IOctree()
{
    //delete recursively the octree structure
    if(root != NULL) delete root;
}

void IOctree::addSceneNode(ISceneNode* node)
{
    node->updateAbsolutePosition();
    root->addSceneNode(node);
    root->cleanEmptyBoxes();
}

void IOctree::draw(IVideoDriver* driver)
{
    root->draw(driver);
}

void IOctree::setOctreeCamera(ICameraSceneNode* octreeCam)
{
    this->octreeCam = octreeCam;
}

void IOctree::setRoot(IOctreeNode* newRoot)
{
    this->root = newRoot;
}

void IOctree::update()
{
    root->update();
}
