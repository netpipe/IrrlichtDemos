#ifndef ORTHOVIEWNODE_H
#define ORTHOVIEWNODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class OrthoViewNode : public BaseNode
    {
        public:
            OrthoViewNode(const GLfloat &left = 0.0f, const GLfloat &right = 0.0f, const GLfloat &bottom = 0.0f, const GLfloat &top = 0.0f, const GLfloat &near = 0.0f, const GLfloat &far = 0.0f);
            ~OrthoViewNode();

            void updateOrtho(const GLfloat &left, const GLfloat &right, const GLfloat &bottom, const GLfloat &top, const GLfloat &near, const GLfloat &far);

            void execute();
        private:
            GLfloat left, right, bottom, top, nearPlane, farPlane;
    };
};

#endif
