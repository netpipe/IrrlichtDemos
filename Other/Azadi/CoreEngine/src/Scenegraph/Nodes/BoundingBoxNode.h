#ifndef BOUNDINGBOXNODE_H
#define BOUNDINGBOXNODE_H

#include "../BaseNode.h"
#include "../../frustum.h"

namespace SceneGraphNodes
{
    class BoundingBoxNode : public BaseNode
    {
        public:
            BoundingBoxNode(const float &bx = 0.0f, const float &by = 0.0f, const float &bz = 0.0f,
                            const float &tx = 0.0f, const float &ty = 0.0f, const float &tz = 0.0f);

            ~BoundingBoxNode();

            void execute();

            void update(const float &cbx, const float &cby, const float &cbz,
                            const float &ctx, const float &cty, const float &ctz);
        private:
            float bx, by, bz, tx, ty, tz;
    };
};

#endif
