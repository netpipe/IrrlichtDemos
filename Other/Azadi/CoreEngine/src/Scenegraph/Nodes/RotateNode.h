#ifndef ROTATENODE_H
#define ROTATENODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class RotateNode : public BaseNode
    {
        public:
            RotateNode(const GLfloat &w = 0.0f, const GLfloat &x = 0.0f, const GLfloat &y = 0.0f, const GLfloat &z = 0.0f);
            ~RotateNode();

            void updateRotation(const GLfloat &x, const GLfloat &y, const GLfloat &z);

            void execute();
        private:
            GLfloat x, y, z;
    };
};

#endif
