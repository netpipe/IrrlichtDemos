#include "BoundingBoxNode.h"

SceneGraphNodes::BoundingBoxNode::BoundingBoxNode(const float &BX, const float &BY, const float &BZ,
                            const float &TX, const float &TY, const float &TZ)
{
    bx = BX; by = BY; bz = BZ;
    tx = TX; ty = TY; tz = TZ;

    bounding = this;
    type = BoundingBox;
}

SceneGraphNodes::BoundingBoxNode::~BoundingBoxNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::BoundingBoxNode::update(const float &cbx, const float &cby, const float &cbz, const float &Ctx, const float &Cty, const float &Ctz)
{
    bx = cbx; by = cby; bz = cbz;
    tx = Ctx; ty = Cty; tz = Ctz;
}

void SceneGraphNodes::BoundingBoxNode::execute()
{
    cleanup();

    if (Geometry::vFrustum.cubeInFrustum(bx, by, bz, tx, ty, tz))
        for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
            (*it)->execute();
}
