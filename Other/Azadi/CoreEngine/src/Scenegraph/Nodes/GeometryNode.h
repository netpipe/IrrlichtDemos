#ifndef GEOMETRYNODE_H
#define GEOMETRYNODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class GeometryNode : public BaseNode
    {
        public:
            GeometryNode(const GLfloat *verts, const unsigned long int &vertCount, const GLfloat *texCoords, const unsigned long int &texCoordCount);
            ~GeometryNode();

            void execute();
        private:
            GLfloat *verts;
            GLfloat *texCoords;
            unsigned long int vertCount;
            unsigned long int texCoordCount;
    };
};

#endif
