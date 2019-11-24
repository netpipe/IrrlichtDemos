#include "ShaderNode.h"

SceneGraphNodes::ShaderNode::ShaderNode(const std::string &shaderName)
{
    shaderID = Shaders::getInstance()->getShaderID(shaderName);

    type = Shader;
}

SceneGraphNodes::ShaderNode::~ShaderNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::ShaderNode::execute()
{
    cleanup();

    glUseProgram(shaderID);

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
