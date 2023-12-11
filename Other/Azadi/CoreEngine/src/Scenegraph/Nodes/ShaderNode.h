#ifndef SHADERNODE_H
#define SHADERNODE_H

#include "../BaseNode.h"
#include "../../Shaders/Shaders.h"

namespace SceneGraphNodes
{
    class ShaderNode : public BaseNode
    {
        public:
            ShaderNode(const std::string &shaderName);
            ~ShaderNode();

            void execute();
        private:
            GLuint shaderID;
    };
};

#endif
