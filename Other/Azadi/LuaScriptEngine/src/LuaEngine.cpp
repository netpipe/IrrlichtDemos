#include "LuaEngine.h"

class azScriptEngine;

LuaEngine* LuaEngine::instance = NULL;

LuaEngine::LuaEngine()
{
    instance = this;
    buildingCount = 0;
    unitCount = 0;
}

LuaEngine::~LuaEngine()
{

}

bool LuaEngine::registerAzadiCore(azCore *az)
{
    if (az != NULL)
    {
        azadi = az;
        azadi->outputConsole("Registered the Azadi core");
        return true;
    }
    return false;
}

bool LuaEngine::registerModelFactory(azModelFactory *mfact)
{
    if (mfact != NULL)
    {
        model_factory = mfact;
        azadi->outputConsole("Registered the Model factory");
        return true;
    }
    return false;
}

bool LuaEngine::registerSoundFactory(azSoundFactory *sfact)
{
    if (sfact != NULL)
    {
        sound_factory = sfact;
        azadi->outputConsole("Registered the Sound factory");
        return true;
    }
    return false;
}

bool LuaEngine::registerTextureFactory(azTextureFactory *tfact)
{
    if (tfact != NULL)
    {
        texture_factory = tfact;
        azadi->outputConsole("Registered the Texture factory");
        return true;
    }
    return false;
}

bool LuaEngine::registerDrawingEngine(azDrawing *draw)
{
    if (draw != NULL)
    {
        drawing = draw;
        azadi->outputConsole("Registered the Drawing engine");
        return true;
    }
    return false;
}

bool LuaEngine::registerPhysicsEngine(azPhysics *physEng)
{
    if (physEng != NULL)
    {
        physics_engine = physEng;
        azadi->outputConsole("Registered the Physics engine");
        return true;
    }
    return false;
}

bool LuaEngine::registerInputEngine(azInput *inputEng)
{
    if (inputEng != NULL)
    {
        input_engine = inputEng;
        azadi->outputConsole("Registered the Input API");
        return true;
    }
    return false;
}

bool LuaEngine::isActive()
{
    return true;
}

void LuaEngine::setWorkingDirectory(const std::string &workingDir)
{
    workingDirectory = workingDir;
}

bool LuaEngine::initialize()
{
    luavm = lua_open();
    if (luavm == NULL)
    {
        azadi->outputConsole("Could not initialize the Lua state machine");
        return false;
    }

    // Register all the functions needed by this lib =)
    registerLuaFunctions();

    // Open up the standard library
    luaL_openlibs(luavm);

    return true;
}

bool LuaEngine::includeFile(const std::string &fileName)
{
    std::string filename = std::string("./") + workingDirectory + std::string("/Scripts/") + fileName;
    if (luaL_loadfile(luavm, filename.c_str()) || lua_pcall(luavm, 0, 0, 0))
    {
        azadi->outputConsole(std::string(lua_tostring(luavm, 1)));
        lua_pop(luavm, 1);
        return true;
    }
    return false;
}

bool LuaEngine::execute(const std::string &command)
{
    if (luaL_loadbuffer(luavm, command.c_str(), command.size(), command.c_str()) || lua_pcall(luavm, 0, 0, 0))
    {
        azadi->outputConsole(std::string(lua_tostring(luavm, 1)));
        lua_pop(luavm, 1);
        return true;
    }
    return false;
}


/*====================================================


    Return functions defined after this


=====================================================*/

short LuaEngine::returnedShort()
{
    if (shorts.size() > 0)
    {
        short temp = shorts.top();
        shorts.pop();
        return temp;
    }
    return 0;
}

int LuaEngine::returnedInt()
{
    if (shorts.size() > 0)
    {
        int temp = ints.top();
        ints.pop();
        return temp;
    }
    return 0;
}

long LuaEngine::returnedLong()
{
    if (shorts.size() > 0)
    {
        long temp = longs.top();
        longs.pop();
        return temp;
    }
    return 0;
}

float LuaEngine::returnedFloat()
{
    if (shorts.size() > 0)
    {
        float temp = floats.top();
        floats.pop();
        return temp;
    }
    return 0.0f;
}

double LuaEngine::returnedDouble()
{
    if (shorts.size() > 0)
    {
        double temp = doubles.top();
        doubles.pop();
        return temp;
    }
    return 0.0;
}

bool LuaEngine::returnedBool()
{
    if (shorts.size() > 0)
    {
        bool temp = bools.top();
        bools.pop();
        return temp;
    }
    return false;
}

std::string LuaEngine::returnedString()
{
    if (strings.size() > 0)
    {
        std::string temp = strings.top();
        strings.pop();
        return temp;
    }
    return "";
}


/*=====================================

    Return functions for the scripting itself

======================================*/

void LuaEngine::returnShort(const short &val)
{
    shorts.push(val);
}

void LuaEngine::returnInt(const int &val)
{
    ints.push(val);
}

void LuaEngine::returnLong(const long &val)
{
    longs.push(val);
}

void LuaEngine::returnFloat(const float &val)
{
    floats.push(val);
}

void LuaEngine::returnDouble(const double &val)
{
    doubles.push(val);
}

void LuaEngine::returnBool(const bool &val)
{
    bools.push(val);
}

void LuaEngine::returnString(const std::string &val)
{
    strings.push(val);
}

/*=====================================

    Building functions

 ====================================*/

void LuaEngine::runBuildingScripts()
{
    for (std::map<unsigned long int, Building*>::iterator it = buildings.begin(); it!= buildings.end(); ++it)
    {
        if ((*it).second != NULL)
            (*it).second->executeScript();
    }
}

unsigned long LuaEngine::newBuilding(const std::string &name, const std::string &model)
{
    std::stringstream temp;
    // Set's the building's name to "name""count""inst", i.e, test0inst
    temp << name << buildingCount << "inst";
    Building *newBuilding = new Building(temp.str(), model, buildingCount);
    buildings[buildingCount++] = newBuilding;
    return (buildingCount - 1);
}

void LuaEngine::setBuildingScript(const unsigned long &buildingID, const std::string &scriptName)
{
    if (buildings.size())
        buildings[buildingID]->setScript(scriptName);
}

void LuaEngine::moveBuilding(const unsigned long &buildingID, const float &x, const float &y, const float &z)
{
    if (buildings.size())
        buildings[buildingID]->setPosition(x, y, z);
}

void LuaEngine::rotateBuilding(const unsigned long &buildingID, const float &x, const float &y, const float &z)
{
    if (buildings.size())
        buildings[buildingID]->setRotation(x, y, z);
}

void LuaEngine::setBuildingAnim(const unsigned long &buildingID, const std::string &animName, const float &delay)
{
    if (buildings.size())
        buildings[buildingID]->doAnimation(animName, delay);
}

void LuaEngine::setBuildingAnimCycle(const unsigned long &buildingID, const std::string &animName, const float &weight)
{
    if (buildings.size())
        buildings[buildingID]->doAnimCycle(animName, weight);
}

void LuaEngine::stopBuildingAnim(const unsigned long &buildingID, const std::string &animName)
{
    if (buildings.size())
        buildings[buildingID]->stopAnimation(animName);
}

void LuaEngine::flushBuildings()
{
    for (std::map<unsigned long int,Building*>::iterator it = buildings.begin(); it!= buildings.end(); ++it)
    {
        if ((*it).second != NULL)
            delete (*it).second;

        it++;
    }
}

/*============================================

    Unit functions

=============================================*/

void LuaEngine::runUnitScripts()
{
    for (std::map<unsigned long int, Building*>::iterator it = buildings.begin(); it!= buildings.end(); ++it)
    {
        if ((*it).second != NULL)
            (*it).second->executeScript();
    }
}

unsigned long LuaEngine::newUnit(const std::string &name, const std::string &model)
{
    std::stringstream temp;
    // Set's the building's name to "name""count""inst", i.e, test0inst
    temp << name << unitCount << "inst";
    Unit *newUnit = new Unit(temp.str(), model, unitCount);
    units[unitCount++] = newUnit;
    return (unitCount - 1);
}

void LuaEngine::setUnitScript(const unsigned long &unitID, const std::string &scriptName)
{
    if (units.size())
        units[unitID]->setScript(scriptName);
}

void LuaEngine::moveUnit(const unsigned long &unitID, const float &x, const float &y, const float &z)
{
    if (units.size())
        units[unitID]->setPosition(x, y, z);
}

void LuaEngine::rotateUnit(const unsigned long &unitID, const float &x, const float &y, const float &z)
{
    if (units.size())
        units[unitID]->setRotation(x, y, z);
}

void LuaEngine::setUnitAnim(const unsigned long &unitID, const std::string &animName, const float &delay)
{
    if (units.size())
        units[unitID]->doAnimation(animName, delay);
}

void LuaEngine::setUnitAnimCycle(const unsigned long &unitID, const std::string &animName, const float &weight)
{
    if (units.size())
        units[unitID]->doAnimCycle(animName, weight);
}

void LuaEngine::stopUnitAnim(const unsigned long &unitID, const std::string &animName)
{
    if (units.size())
        units[unitID]->stopAnimation(animName);
}

void LuaEngine::flushUnits()
{
    for (std::map<unsigned long int,Unit*>::iterator it = units.begin(); it!= units.end(); ++it)
    {
        if ((*it).second != NULL)
            delete (*it).second;

        it++;
    }
}

/*===============================================

    Input functions

================================================*/

void LuaEngine::setInputObjectBuilding(const unsigned long int &buildingID)
{
    input_engine->registerInput(buildings[buildingID]);
}

void LuaEngine::setInputObjectUnit(const unsigned long int &unitID)
{
    input_engine->registerInput(units[unitID]);
}

void LuaEngine::findSelectedEntity(const float &mousex, const float &mousey, const float &mousez, const float cam[3])
{
    findSelectedBuilding(mousex, mousey, mousez, cam);
    findSelectedUnit(mousex, mousey, mousez, cam);
}

void LuaEngine::findSelectedBuilding(const float &mousex, const float &mousey, const float &mousez, const float cam[3])
{
    for (std::map<unsigned long int, Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
    {
        // somehow check for visibility
        if ((*it).second->checkClicked(mousex, mousey, mousez, cam))
        {
            printf("Setting %s to take inputs\n", (*it).second->getBuildingName().c_str());
            setInputObjectBuilding(it->first);
        }
    }
}

void LuaEngine::findSelectedUnit(const float &mousex, const float &mousey, const float &mousez, const float cam[3])
{
    for (std::map<unsigned long int, Unit*>::iterator it = units.begin(); it != units.end(); ++it)
    {
        // check for visibility
        if ((*it).second->checkClicked(mousex, mousey, mousez, cam))
        {
            printf("Setting %s to take inputs\n", (*it).second->getUnitName().c_str());
            setInputObjectUnit(it->first);
        }
    }
}
