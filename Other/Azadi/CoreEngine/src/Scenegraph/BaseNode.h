#ifndef BASENODE_H
#define BASENODE_H

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

const unsigned int preallocCount = 5;

namespace SceneGraphNodes
{
    enum NodeType
    {
        Root,
        BoundingBox,
        Color,
        Frustum,
        Geometry,
        Model,
        Ortho,
        Rotate,
        Scale,
        Shader,
        Switch,
        Texture,
        Translate,
    };

    class BaseNode
    {
        public:
            virtual ~BaseNode() {};

            virtual void execute() {};

            void prepRemove();
            void init();
            void cleanup();

            void addChild(BaseNode *child);
            bool childExists(BaseNode *child);
            void removeChild(BaseNode *child);
            void delayRemoveChild(BaseNode *child);

            BaseNode *parent;
            BaseNode *root;
            BaseNode *bounding;
            std::vector<BaseNode*> children;
            std::vector<BaseNode*> toDelete;

            unsigned long int childNumber;

            enum NodeType type;
    };
};

#endif
