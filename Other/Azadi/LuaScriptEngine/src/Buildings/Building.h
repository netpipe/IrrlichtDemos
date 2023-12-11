#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <vector>
#include "../LuaEngine.h"
#include "../Headers/PhysicsObject.h"
#include "../Headers/InputObject.h"

class LuaEngine;

class Building : public PhysicsObject, public InputObject
{
    public:
        Building(const std::string &buildingName, const std::string &buildingModel, const unsigned long int &buildingID);
        ~Building();

        void setScript(const std::string &scriptName);
        void setPosition(const float &x, const float &y, const float &z);
        void setRotation(const float &x, const float &y, const float &z);
        void doAnimation(const std::string &animName, const float &delay);
        void doAnimCycle(const std::string &animName, const float &weight);
        void stopAnimation(const std::string &animName);

        void pushKey(const int &keysym, const char &ch);
        void pushClick(const char &button);
        bool checkClicked(const float &mousex, const float &mousey, const float &mousez, const float cam[3]);

        void executeScript();

        std::string getBuildingName() { return buildingName; }
    private:
        void syncToLuaStack();
        void syncPosToLuaStack();
        void syncRotToLuaStack();

        std::string buildingName;
        std::string scriptName;

        long modelID;
        unsigned long int buildingID;

        LuaEngine* lua_engine;

        std::vector<std::vector<float> > boundingBoxes;
};

#endif
