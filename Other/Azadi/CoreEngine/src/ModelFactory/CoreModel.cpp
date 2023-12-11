#include "CoreModel.h"

CoreModel::CoreModel(const std::string &filename)
{
    TiXmlDocument xmldoc;
    if (!(xmldoc.LoadFile(filename)))
    {
        printf("Something broke!\n");
        return;
    }

    TiXmlHandle handle(&xmldoc);

    TiXmlElement *element = handle.FirstChild("Model").Element();

    coreMod = new CalCoreModel(element->Attribute("name"));

    element = handle.FirstChild("Model").FirstChild("Skeleton").Element();

    if (!coreMod->loadCoreSkeleton(element->Attribute("filename")))
    {
        printf("Could not load the core skeleton\n");
    }

    unsigned int ind = 0;
    while ((element = handle.FirstChild("Model").Child("Animation", ind++).Element()))
    {
        animations[element->Attribute("name")] = coreMod->loadCoreAnimation(element->Attribute("file"));
        animNames.push_back(element->Attribute("name"));
    }

    ind = 0;
    while ((element = handle.FirstChild("Model").Child("Mesh", ind++).Element()))
        meshes[element->Attribute("name")] = coreMod->loadCoreMesh(element->Attribute("file"));

    ind = 0;
    while ((element = handle.FirstChild("Model").Child("Material", ind++).Element()))
        materials[element->Attribute("name")] = coreMod->loadCoreMaterial(element->Attribute("file"));

    for (int i = 0; i < coreMod->getCoreMaterialCount(); ++i)
    {
        CalCoreMaterial *coreMat;
        coreMat = coreMod->getCoreMaterial(i);

        for (int j = 0; j < coreMat->getMapCount(); ++j)
        {
            textures.push_back(coreMat->getMapFilename(j));
            TextureFactory::getInstance()->loadTexture(coreMat->getMapFilename(j));
            Cal::UserData temp = (Cal::UserData)(textures.size() - 1);
            coreMat->setMapUserData(j, temp);
        }
    }

    ind = 0;
    while ((element = handle.FirstChild("Model").Child("MaterialSet", ind++).Element()))
    {
        std::string temp = element->Attribute("thread");
        std::string temp2 = element->Attribute("set");
        std::string temp3 = element->Attribute("mId");

        if (materialThreads.find(temp) == materialThreads.end())
        {
            coreMod->createCoreMaterialThread(materialThreads.size());
            materialThreads[temp] = materialThreads.size() - 1;
        }

        if (materialSets.find(temp2) == materialSets.end())
            materialSets[temp2] = materialSets.size() - 1;

        coreMod->setCoreMaterialId(materialThreads[temp], materialSets[temp2], materials[temp3]);
    }
}

CoreModel::~CoreModel()
{
    delete coreMod;
}
