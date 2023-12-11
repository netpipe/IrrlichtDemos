#include "TextureNode.h"

SceneGraphNodes::TextureNode::TextureNode(const std::string &TextureName)
{
    textureName = textureName;
    TextureFactory::getInstance()->loadTexture(textureName);
    textureID = TextureFactory::getInstance()->getTextureID(textureName);

    type = Texture;
}

SceneGraphNodes::TextureNode::~TextureNode()
{
    TextureFactory::getInstance()->deleteTexture(textureName);
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::TextureNode::execute()
{
    cleanup();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
