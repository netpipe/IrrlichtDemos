#include "Building.h"
#include "../LuaEngine.h"

Building::Building(const std::string &name, const std::string &model, const unsigned long int &BuildingID)
{
    x = y = z = rx = ry = rz = 0;
    scriptName = "";

    buildingName = name;
    modelID = LuaEngine::getInstance()->getModelFactory()->LoadModel((char *)model.c_str());
    buildingID = BuildingID;

    // Give the building a local pointer to call the lua_engine, rather than having to use the static method all the time.
    lua_engine = LuaEngine::getInstance();

    // Instantiate a lua table to hold the building in
    lua_engine->execute(buildingName + " = {}");
    // Populate the table with the data required (position & rotation)
    syncToLuaStack();

    std::stringstream temp;
    temp << buildingID;

    // Register functions for the building's table to move and rotate
    lua_engine->execute("function " + buildingName + ".move(x, y, z)\n" + "setBuildingPos(" + buildingName + "[7], x, y, z);\nend");
    lua_engine->execute("function " + buildingName + ".rotate(x, y, z)\n" + "setBuildingRot(" + buildingName + "[7], x, y, z);\nend");
    lua_engine->execute("function " + buildingName + ".setAsInput()\n" + "setInputObject(" + temp.str() + ");\nend");
    lua_engine->execute("function " + buildingName + ".doAnimation(animName, delay)\n" + "setBuildingAnimation(" + buildingName + "[7], animName, delay, false);\nend");
    lua_engine->execute("function " + buildingName + ".stopAnimation(animName)\n" + "stopBuildingAnimation(" + buildingName + "[7], animName);\nend");
    lua_engine->execute("function " + buildingName + ".cycleAnimation(animName, weight)\n" + "setBuildingAnimation(" + buildingName + "[7], animName, weight, true);\nend");

    lua_engine->getAzadiInstance()->outputConsole(std::string("Instantiated a building named: ") + buildingName);
}

Building::~Building()
{

}

void Building::setScript(const std::string &script)
{
    scriptName = script;
}

void Building::setPosition(const float &X, const float &Y, const float &Z)
{
    x = X;
    y = Y;
    z = Z;

    lua_engine->getModelFactory()->Move(modelID, x, y, z);
    syncPosToLuaStack();
}

void Building::setRotation(const float &X, const float &Y, const float &Z)
{
    rx = X;
    ry = Y;
    rz = Z;

    lua_engine->getModelFactory()->Rotate(modelID, rx, ry, rz);
    syncRotToLuaStack();
}

void Building::doAnimation(const std::string &animName, const float &delay)
{
    lua_engine->getModelFactory()->StartAction(modelID, animName, delay);
}

void Building::doAnimCycle(const std::string &animName, const float &weight)
{
    lua_engine->getModelFactory()->StartCycle(modelID, animName, weight);
}

void Building::stopAnimation(const std::string &animName)
{
    lua_engine->getModelFactory()->StopAction(modelID, animName);
    lua_engine->getModelFactory()->StopCycle(modelID, animName);
}

void Building::executeScript()
{
    // Sets this building as the current building
    lua_engine->execute("currentBuilding = " + buildingName);

    // Executes the script associated with this building
    lua_engine->execute(scriptName);
}

// Syncs the current x, y, z, rx, ry, rz values to the lua instances
void Building::syncToLuaStack()
{
    std::stringstream temp;
    temp << x;
    lua_engine->execute(buildingName + "[1] = " + temp.str());
    temp.clear();
    temp << y;
    lua_engine->execute(buildingName + "[2] = " + temp.str());
    temp.clear();
    temp << z;
    lua_engine->execute(buildingName + "[3] = " + temp.str());
    temp.clear();
    temp << rx;
    lua_engine->execute(buildingName + "[4] = " + temp.str());
    temp.clear();
    temp << ry;
    lua_engine->execute(buildingName + "[5] = " + temp.str());
    temp.clear();
    temp << rz;
    lua_engine->execute(buildingName + "[6] = " + temp.str());
    temp.clear();
    temp << buildingID;
    lua_engine->execute(buildingName + "[7] = " + temp.str());
}

void Building::syncPosToLuaStack()
{
    std::stringstream temp;
    temp << x;
    lua_engine->execute(buildingName + "[1] = " + temp.str());
    temp.clear();
    temp << y;
    lua_engine->execute(buildingName + "[2] = " + temp.str());
    temp.clear();
    temp << z;
    lua_engine->execute(buildingName + "[3] = " + temp.str());
}

void Building::syncRotToLuaStack()
{
    std::stringstream temp;
    temp << rx;
    lua_engine->execute(buildingName + "[4] = " + temp.str());
    temp.clear();
    temp << ry;
    lua_engine->execute(buildingName + "[5] = " + temp.str());
    temp.clear();
    temp << rz;
    lua_engine->execute(buildingName + "[6] = " + temp.str());
}

void Building::pushKey(const int &keysym, const char &ch)
{

}

void Building::pushClick(const char &button)
{

}

bool Building::checkClicked(const float &mousex, const float &mousey, const float &mousez, const float cam[3])
{
    boundingBoxes = lua_engine->getModelFactory()->GetBoundingBoxes(modelID);

    bool hit = false;

    float tx, ty, tz;
    float resultX, resultY, resultZ;

    for (unsigned int i = 0; i < boundingBoxes.size(); ++i)
    {
        if (mousex != boundingBoxes[i][0])
        {
            if (mousex >= 0)
            {
                tx = ((x + boundingBoxes[i][3]) - cam[0]) / mousex;
                resultX = cam[0] + (tx * mousex);
                resultY = cam[1] + (tx * mousey);
                resultZ = cam[2] + (tx * mousez);

                if (((y + boundingBoxes[i][4]) <= resultY) && (resultY <= (y + boundingBoxes[i][1])))
                    if (((z + boundingBoxes[i][2]) <= resultZ) && (resultZ <= (z + boundingBoxes[i][5])))
                    {
                        hit = true;
                        break;
                    }
            }
            else
            {
                tx = ((x + boundingBoxes[i][0]) - cam[0]) / mousex;
                resultX = cam[0] + (tx * mousex);
                resultY = cam[1] + (tx * mousey);
                resultZ = cam[2] + (tx * mousez);

                if (((y + boundingBoxes[i][4]) <= resultY) && (resultY <= (y + boundingBoxes[i][1])))
                    if (((z + boundingBoxes[i][2]) <= resultZ) && (resultZ <= (z + boundingBoxes[i][5])))
                    {
                        hit = true;
                        break;
                    }
            }
        }

        if (mousey != boundingBoxes[i][1])
        {
            if (mousey >= 0)
            {
                ty = ((y + boundingBoxes[i][1]) - cam[1]) / mousey;
                resultX = cam[0] + (ty * mousex);
                resultY = cam[1] + (ty * mousey);
                resultZ = cam[2] + (ty * mousez);

                if (((x + boundingBoxes[i][0]) <= resultX) && (resultX <= (x + boundingBoxes[i][3])))
                    if (((z + boundingBoxes[i][2]) <= resultZ) && (resultZ <= (z + boundingBoxes[i][5])))
                    {
                        hit = true;
                        break;
                    }
            }
            else
            {
                ty = ((y + boundingBoxes[i][4]) - cam[1]) / mousey;
                resultX = cam[0] + (ty * mousex);
                resultY = cam[1] + (ty * mousey);
                resultZ = cam[2] + (ty * mousez);

                if (((x + boundingBoxes[i][0]) <= resultX) && (resultX <= (x + boundingBoxes[i][3])))
                    if (((z + boundingBoxes[i][2]) <= resultZ) && (resultZ <= (z + boundingBoxes[i][5])))
                    {
                        hit = true;
                        break;
                    }
            }
        }

        if (mousez != boundingBoxes[i][2])
        {
            if (mousez >= 0)
            {
                tz = ((z + boundingBoxes[i][5]) - cam[2]) / mousez;
                resultX = cam[0] + (tz * mousex);
                resultY = cam[1] + (tz * mousey);
                resultZ = cam[2] + (tz * mousez);

                if (((x + boundingBoxes[i][0] <= resultX) && (resultX <= (x + boundingBoxes[i][3]))))
                    if (((y + boundingBoxes[i][4] <= resultY) && (resultY <= (y + boundingBoxes[i][1]))))
                    {
                        hit = true;
                        break;
                    }
            }
            else
            {
                tz = ((z + boundingBoxes[i][2]) - cam[2]) / mousez;
                resultX = cam[0] + (tz * mousex);
                resultY = cam[1] + (tz * mousey);
                resultZ = cam[2] + (tz * mousez);

                if (((x + boundingBoxes[i][0] <= resultX) && (resultX <= (x + boundingBoxes[i][3]))))
                    if (((y + boundingBoxes[i][4] <= resultY) && (resultY <= (y + boundingBoxes[i][1]))))
                    {
                        hit = true;
                        break;
                    }
            }
        }
    }
    /*
    if (hit)
        printf("Hit at %f %f %f\n", resultX, resultY, resultZ);
    */

    return hit;
}
