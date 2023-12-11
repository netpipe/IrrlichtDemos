#ifndef TRANSLATENODE_H
#define TRANSLATENODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class TranslateNode : public BaseNode
    {
        public:
            TranslateNode(const GLfloat &x = 0.0f, const GLfloat &y = 0.0f, const GLfloat &z = 0.0f);
            ~TranslateNode();

            void execute();

            void updateTranslation(const GLfloat &x, const GLfloat &y, const GLfloat &z);
        private:
            GLfloat x, y, z;
    };
};

#endif
