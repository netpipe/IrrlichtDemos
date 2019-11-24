#ifndef COLORNODE_H
#define COLORNODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class ColorNode : public BaseNode
    {
        public:
            ColorNode(const GLfloat &r = 1.0f, const GLfloat &g = 1.0f, const GLfloat &b = 1.0f, const GLfloat &a = 1.0f);
            ~ColorNode();

            void updateColor(const GLfloat &r, const GLfloat &g, const GLfloat &b, const GLfloat &a = 1.0f);

            void execute();
        private:
            GLfloat r, g, b, a;
    };
};

#endif
