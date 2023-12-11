#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include "../LuaEngine.h"
#include "../Headers/PhysicsObject.h"
#include "../Headers/InputObject.h"

class LuaEngine;

class Unit : public PhysicsObject, public InputObject
{
    public:
        Unit(const std::string &name, const std::string &model, const unsigned long int &UnitID);
        ~Unit();

        void pushKey(const int &keysym, const char &ch);
        void pushClick(const char &button);
        bool checkClicked(const float &mousex, const float &mousey, const float &mousez, const float cam[3]);

        void setScript(const std::string &scriptName);
        void setPosition(const float &x, const float &y, const float &z);
        void setRotation(const float &x, const float &y, const float &z);
        void doAnimation(const std::string &animName, const float &delay);
        void doAnimCycle(const std::string &animName, const float &weight);
        void stopAnimation(const std::string &animName);

        void executeScript();

        std::string getUnitName() { return unitName; }
    private:
        void syncToLuaStack();
        void syncPosToLuaStack();
        void syncRotToLuaStack();

        std::string unitName;
        std::string scriptName;

        long modelID;
        unsigned long int unitID;

        LuaEngine *lua_engine;

        std::vector<std::vector<float> > boundingBoxes;
};

#endif
