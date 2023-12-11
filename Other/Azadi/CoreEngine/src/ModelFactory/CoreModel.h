#ifndef COREMODEL_H
#define COREMODEL_H

#include <string>
#include <fstream>
#include <map>
#include <vector>

#include "../XML/tinyxml.h"
#include <cal3d/cal3d.h>

#include "../Texture/TextureFactory.h"

class CoreModel
{
    public:
        CoreModel(const std::string &name);
        ~CoreModel();

        std::string name;
        CalCoreModel *coreMod;

        std::map<std::string, int> animations;
        std::vector<std::string> animNames;
        std::map<std::string, int> meshes;
        std::map<std::string, int> materials;
        std::vector<std::string> textures;
        std::map<std::string, int> materialThreads;
        std::map<std::string, int> materialSets;

        CoreModel* getInstance() { return this; }
};

#endif
