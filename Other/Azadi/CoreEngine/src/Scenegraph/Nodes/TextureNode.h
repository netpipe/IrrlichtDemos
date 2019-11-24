#ifndef TEXTURENODE_H
#define TEXTURENODE_H

#include "../BaseNode.h"
#include "../../Texture/TextureFactory.h"

namespace SceneGraphNodes
{
    class TextureNode : public BaseNode
    {
        public:
            TextureNode(const std::string &textureName);
            ~TextureNode();

            void execute();
        private:
            std::string textureName;
            GLuint textureID;
    };
};

#endif
