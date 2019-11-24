#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>
#include <fstream>

namespace ModelTypes
{
    class Obj
    {
        public:
            void load(const std::string &modelName);
            void update();
            void draw();

            std::string name;
            std::vector<std::vector<float> > verts;
            std::vector<std::vector<unsigned int> > faces;
            std::vector<std::vector<float> > texCoords;
            std::vector<std::string> textureNames;

    };
};

#endif
