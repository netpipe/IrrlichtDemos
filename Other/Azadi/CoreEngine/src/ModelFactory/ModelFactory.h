#ifndef MODEL_H_071106
#define MODEL_H_071106

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <SDL/SDL.h>

#include "../Azadi.h"

#include "../Texture/TextureFactory.h"
#include "../GUI/Console.h"
#include "../Input/ConsoleListener.h"

#include "../Scripting/Headers/ModelFactory.h"

#include "CoreModel.h"
#include "InstanceModel.h"

#include "../Scenegraph/Nodes.h"

#include <vector>

class Textures;

class ModelFactory : public azModelFactory
{
    public:
        ModelFactory(const std::string &workingDirectory);
        ~ModelFactory();

        int LoadModel(const char * filename);
        void Draw(const unsigned int &ID);
        void Update(); // Animation Update callback
        void Move(const unsigned int &iD, const float &x, const float &y, const float &z);
        void Rotate(const unsigned int &iD, const float &x, const float &y, const float &z);
        void Flush();
        void StartAction(const unsigned int &ID, const std::string &animName, const float &easeIn = 0.0f, const float &easeOut = 0.0f);
        void StopAction(const unsigned int &ID, const std::string &animName);
        void StartCycle(const unsigned int &ID, const std::string &animName, const float &weight = 1.0f, const float &delay = 0.0f);
        void StopCycle(const unsigned int &ID, const std::string &animName, const float &delay = 0.0f);

        std::vector<std::vector<float> > GetBoundingBoxes(const unsigned int &ID);
        std::vector<std::string> GetAnimationNames(const unsigned int &ID);

        static ModelFactory* getInstance() { return instance; }
private:
        static ModelFactory *instance;
        std::string workingDirectory;

        unsigned long int ModelCount;
        std::map<std::string, CoreModel*> CoreModels;
        std::map<unsigned long int, InstanceModel*> InstancedModels;
};

#endif
