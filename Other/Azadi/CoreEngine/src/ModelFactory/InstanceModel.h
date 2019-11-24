#ifndef INSTANCEMODEL_H
#define INSTANCEMODEL_H

#include <string>
#include <vector>

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <SDL/SDL.h>

#include <cal3d/cal3d.h>
#include "CoreModel.h"

#include "../Texture/TextureFactory.h"

class InstanceModel
{
    public:
        InstanceModel(CoreModel*);
        InstanceModel(InstanceModel*);

        void startAction(const std::string &animationName, const float &easeIn = 0.0f, const float &easeOut = 0.0f);
        void stopAction(const std::string &animationName);
        void startCycle(const std::string &animationName, const float &weight = 1.0f, const float &delay = 0.0f);
        void stopCycle(const std::string &animationName, const float &delay = 0.0f);

        void update();
        void draw();
        void setPosition(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f);
        void setRotation(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f);

        InstanceModel& operator=(CoreModel*);

        std::vector<std::vector<float> > BoundingBoxes;
        std::vector<std::string> AnimationNames;
    private:
        float x, y, z;
        float rx, ry, rz;

        CalModel *instModel;
        CoreModel *parentCore;
        CalRenderer *renderer;

        TextureFactory *textureFact;

        unsigned long int lastTick;
};

#endif
