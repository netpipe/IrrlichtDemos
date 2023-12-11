#include "ModelFactory.h"

using GUI::Console;

ModelFactory* ModelFactory::instance = NULL;

ModelFactory::ModelFactory(const std::string &workingDir)
{
	workingDirectory = workingDir;
    ModelCount = 0;
	instance = this;
}

ModelFactory::~ModelFactory()
{
    // Make sure to free up the models
    Flush();
}

int ModelFactory::LoadModel(const char * filename)
{
    std::string fileName = std::string("./") + workingDirectory + std::string("/Models/descriptors/") + filename;

    if (CoreModels.find(fileName) == CoreModels.end())
    {
        CoreModel *coreMod = new CoreModel(fileName);
        CoreModels[fileName] = coreMod;
    }

    InstanceModel *iModel = new InstanceModel(CoreModels[fileName]);

    while (ModelCount == 0 || InstancedModels[ModelCount] != NULL)
        ++ModelCount;

    SceneGraphNodes::ModelNode *tempMod = new SceneGraphNodes::ModelNode(ModelCount);
    Azadi::getInstance()->activeNode->addChild(tempMod);

    InstancedModels[ModelCount] = iModel;

    return ModelCount;
}

void ModelFactory::Update()
{
    for (std::map<unsigned long int, InstanceModel*>::iterator it = InstancedModels.begin(); it != InstancedModels.end(); ++it)
    {
        it->second->update();
    }
}

void ModelFactory::Draw(const unsigned int &ID)
{
    InstancedModels[ID]->draw();
}

void ModelFactory::Move( const unsigned int &ID, const float &x, const float &y, const float &z)
{
    InstancedModels[ID]->setPosition(x, y, z);
}

void ModelFactory::Rotate(const unsigned int &ID, const float &x, const float &y, const float &z)
{
    InstancedModels[ID]->setRotation(x, y, z);
}

void ModelFactory::StartAction(const unsigned int &ID, const std::string &animName, const float &easeIn, const float &easeOut)
{
    InstancedModels[ID]->startAction(animName, easeIn, easeOut);
}

void ModelFactory::StopAction(const unsigned int &ID, const std::string &animName)
{
    InstancedModels[ID]->stopAction(animName);
}

void ModelFactory::StartCycle(const unsigned int &ID, const std::string &animName, const float &weight, const float &delay)
{
    InstancedModels[ID]->startCycle(animName, weight, delay);
}

void ModelFactory::StopCycle(const unsigned int &ID, const std::string &animName, const float &delay)
{
    InstancedModels[ID]->stopCycle(animName, delay);
}

void ModelFactory::Flush()
{
    for (std::map<unsigned long int, InstanceModel*>::iterator it = InstancedModels.begin(); it != InstancedModels.end(); ++it)
    {
        if (it->second != NULL)
            delete it->second;
    }

    for (std::map<std::string, CoreModel*>::iterator it = CoreModels.begin(); it != CoreModels.end(); ++it)
        delete it->second;

    InstancedModels.clear();
    CoreModels.clear();
}

std::vector<std::vector<float> > ModelFactory::GetBoundingBoxes(const unsigned int &ID)
{
    return InstancedModels[ID]->BoundingBoxes;
}

std::vector<std::string> ModelFactory::GetAnimationNames(const unsigned int &ID)
{
    return InstancedModels[ID]->AnimationNames;
}

