#include "GeometryNode.h"

SceneGraphNodes::GeometryNode::GeometryNode(const GLfloat *vertices, const unsigned long int &count, const GLfloat *texCoord, const unsigned long int &texCoordsCount)
{
    verts = (GLfloat*)malloc(sizeof(verts) * count);
    memcpy(verts, vertices, count);
    vertCount = count;

    texCoords = (GLfloat*)malloc(sizeof(texCoords) * texCoordsCount);
    memcpy(texCoords, texCoord, texCoordsCount);
    texCoordCount = texCoordsCount;

    type = Geometry;
}

SceneGraphNodes::GeometryNode::~GeometryNode()
{
    free(verts);
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::GeometryNode::execute()
{
    cleanup();

    if (texCoordCount)
    {
        for (unsigned long int i = 0, j = 0; i < vertCount; i+=3, j+=2)
        {
            glTexCoord2f(texCoords[j], texCoords[j + 1]);
            glVertex3f(verts[i], verts[i+1], verts[i+2]);
        }
    }
    else
    {
        for (unsigned long int i = 0; i < vertCount; i+=3)
            glVertex3f(verts[i], verts[i+1], verts[i+2]);
    }

    for (std::vector<BaseNode*>::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->execute();
}
