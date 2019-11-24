#ifndef TERRAINNODE_H
#define TERRAINNODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class TerrainNode : public BaseNode
    {
        public:
            TerrainNode(const GLuint &VBO, const GLuint &EBO, const unsigned int &size);
            ~TerrainNode();

            void execute();
        private:
            GLuint vboID, eboID;
            unsigned long int size;
    };
};

#endif
