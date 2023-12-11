#ifndef LUAENGINE_H
#define LUAENGINE_H

#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <map>

#include "azScriptEngine.h"
#include "Buildings/Building.h"
#include "Units/Unit.h"

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#define lua_checkstring luaL_checkstring
#define lua_optstring luaL_optstring
#define lua_checknumber luaL_checknumber
#define lua_checkany luaL_checkany

class Building;
class Unit;

class LuaEngine : public azScriptEngine
{
    public:
        LuaEngine();
        ~LuaEngine();

        void setWorkingDirectory(const std::string &workingDirectory);

        bool registerAzadiCore(azCore *azadi);
        bool registerModelFactory(azModelFactory* model_factory);
        bool registerSoundFactory(azSoundFactory* sound_factory);
        bool registerDrawingEngine(azDrawing* drawing);
        bool registerTextureFactory(azTextureFactory* texture_factory);
        bool registerPhysicsEngine(azPhysics *phy);
        bool registerInputEngine(azInput *inp);

        bool initialize();
        bool isActive();
        bool includeFile(const std::string &fileName);
        bool execute(const std::string &command);

        short returnedShort();
        int returnedInt();
        long returnedLong();
        float returnedFloat();
        double returnedDouble();
        bool returnedBool();
        std::string returnedString();

        void returnShort(const short &val);
        void returnInt(const int &val);
        void returnLong(const long &val);
        void returnFloat(const float &val);
        void returnDouble(const double &val);
        void returnBool(const bool &val);
        void returnString(const std::string &val);

        std::string getScriptEngineIdentifier() { return "Lua"; }

        static LuaEngine* getInstance() { return instance; }

        // For Buildings
        void runBuildingScripts();
        unsigned long newBuilding(const std::string &name, const std::string &model);
        void setBuildingScript(const unsigned long &buildingID, const std::string &script);
        void moveBuilding(const unsigned long &buildingID, const float &x, const float &y, const float &z);
        void rotateBuilding(const unsigned long &buildingID, const float &x, const float &y, const float &z);
        void setBuildingAnim(const unsigned long &buildingID, const std::string &animName, const float &delay);
        void setBuildingAnimCycle(const unsigned long &buildingID, const std::string &animName, const float &weight);
        void stopBuildingAnim(const unsigned long &buildingID, const std::string &animName);
        void flushBuildings();

        // For Units
        void runUnitScripts();
        unsigned long newUnit(const std::string &name, const std::string &model);
        void setUnitScript(const unsigned long &unitID, const std::string &script);
        void moveUnit(const unsigned long &unitID, const float &x, const float &y, const float &z);
        void rotateUnit(const unsigned long &unitID, const float &x, const float &y, const float &z);
        void setUnitAnim(const unsigned long &unitID, const std::string &animName, const float &delay);
        void setUnitAnimCycle(const unsigned long &unitID, const std::string &animName, const float &weight);
        void stopUnitAnim(const unsigned long &unitID, const std::string &animName);
        void flushUnits();

        // Input related
        void findSelectedEntity(const float &mousex, const float &mousey, const float &mousez, const float cam[3]);
        void findSelectedBuilding(const float &mousex, const float &mousey, const float &mousez, const float cam[3]);
        void findSelectedUnit(const float &mousex, const float &mousey, const float &mousez, const float cam[3]);
        void setInputObjectBuilding(const unsigned long int &buildingID);
        void setInputObjectUnit(const unsigned long int &unitID);

        // Because lua is a C lib
        azCore *getAzadiInstance() { return azadi; }
        azModelFactory *getModelFactory() { return model_factory; }
        azSoundFactory *getSoundFactory() { return sound_factory; }
        azDrawing *getDrawingEngine() { return drawing; }
        azTextureFactory *getTextureFactory() { return texture_factory; }
        azInput *getInputEngine() { return input_engine; }

    private:
        static LuaEngine *instance;

        std::string workingDirectory;

        azCore *azadi;
        azModelFactory *model_factory;
        azSoundFactory *sound_factory;
        azDrawing *drawing;
        azTextureFactory *texture_factory;
        azPhysics *physics_engine;
        azInput *input_engine;

        // Register all the lua functions
        void registerLuaFunctions();

        // Buildings
        std::map<unsigned long int, Building*> buildings;
        unsigned long int buildingCount;

        // Units
        std::map<unsigned long int, Unit*> units;
        unsigned long int unitCount;

        // return value stacks
        std::stack<short> shorts;
        std::stack<int> ints;
        std::stack<long> longs;
        std::stack<float> floats;
        std::stack<double> doubles;
        std::stack<bool> bools;
        std::stack<std::string> strings;

        lua_State *luavm;
};

#endif
