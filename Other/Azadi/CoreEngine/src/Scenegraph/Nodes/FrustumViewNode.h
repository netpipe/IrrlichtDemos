#ifndef FRUSTUMVIEWNODE_H
#define FRUSTUMVIEWNODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class FrustumViewNode : public BaseNode
    {
        public:
            FrustumViewNode(const float &left = 0.0f, const float &right = 0.0f, const float &bottom = 0.0f, const float &top = 0.0f, const float &near = 0.0f, const float &far = 0.0f);
            ~FrustumViewNode();

            void updateFrustum(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far);

            void execute();
        private:
            GLfloat left, right, bottom, top, nearPlane, farPlane;
    };
};

#endif
