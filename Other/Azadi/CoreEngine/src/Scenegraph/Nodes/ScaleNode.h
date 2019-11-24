#ifndef SCALENODE_H
#define SCALENODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class ScaleNode : public BaseNode
    {
        public:
            ScaleNode(const GLfloat &x = 1.0f, const GLfloat &y = 1.0f, const GLfloat &z = 1.0f);
            ~ScaleNode();

            void updateScale(const GLfloat &x, const GLfloat &y, const GLfloat &z);

            void execute();
        private:
            GLfloat x, y, z;
    };
};

#endif
