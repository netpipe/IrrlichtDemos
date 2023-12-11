#include "Obj.h"
#include <string.h>

void ModelTypes::Obj::load(const std::string &filename)
{
    std::ifstream mFile(filename.c_str());
    if (mFile.is_open())
    {
        char buffer[1024];
        while (mFile.getline(buffer, 1024))
        {
            std::string tempo = buffer;
            if (buffer[0] == 'v')
            {
                if (buffer[1] == 't')
                {
                    float x, y;
                    std::vector<float> temp;
                    sscanf(buffer, "vt %f %f", &x, &y);
                    temp.push_back(x);
                    temp.push_back(y);
                    texCoords.push_back(temp);
                }
                else
                {
                    float x, y, z;
                    std::vector<float> temp;
                    sscanf(buffer, "v %f %f %f", &x, &y, &z);
                    temp.push_back(x);
                    temp.push_back(y);
                    temp.push_back(z);
                    verts.push_back(temp);
                }
                continue;
            }
            if (buffer[0] == 'f')
            {
                int count = 0;
                char *pch;
                pch = strtok(buffer, " ");
                while (pch != NULL)
                {
                    ++count;
                    pch = strtok(NULL, " ");
                }

                if (count == 3) // It's triangles
                {
                    unsigned int x, y, z;
                    std::vector<unsigned int> temp;
                    sscanf(buffer, "f %u %u %u", &x, &y, &z);
                    temp.push_back(x);
                    temp.push_back(y);
                    temp.push_back(z);
                    faces.push_back(temp);
                }
                else if (count == 4) // It's quads
                {
                    unsigned int w, x, y, z;
                    std::vector<unsigned int> temp;
                    sscanf(buffer, "f %u %u %u %u", &w, &x, &y, &z);
                    temp.push_back(w);
                    temp.push_back(x);
                    temp.push_back(y);
                    temp.push_back(z);
                    faces.push_back(temp);
                }
                continue;
            }
            if (strncmp(buffer, "usemap", 6) == 0)
            {

            }

            memset(buffer, 0, 1024);
        }
    }
}

void ModelTypes::Obj::update()
{

}

void ModelTypes::Obj::draw()
{

}
