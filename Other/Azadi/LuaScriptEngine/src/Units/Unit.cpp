#include "Unit.h"

Unit::Unit(const std::string &name, const std::string &model, const unsigned long int &UnitID)
{
    x = y = z = rx = ry = rz = 0;
    scriptName = "";

    unitName = name;
    modelID = LuaEngine::getInstance()->getModelFactory()->LoadModel((char *)model.c_str());
    unitID = UnitID;

    // Give the unit a local pointer to call the lua_engine, rather than having to use the static method all the time.
    lua_engine = LuaEngine::getInstance();

    // Instantiate a lua table to hold the unit in
    lua_engine->execute(unitName + " = {}");
    // Populate the table with the data required (position & rotation)
    syncToLuaStack();

    std::stringstream temp;
    temp << unitID;

    // Register functions for the unit's table to move and rotate
    lua_engine->execute("function " + unitName + ".move(x, y, z)\n" + "setUnitPos(" + unitName + "[7], x, y, z);\nend");
    lua_engine->execute("function " + unitName + ".rotate(x, y, z)\n" + "setUnitRot(" + unitName + "[7], x, y, z);\nend");
    lua_engine->execute("function " + unitName + ".setAsInput()\n" + "setInputObject(" + temp.str() + ");\nend");
    lua_engine->execute("function " + unitName + ".doAnimation(animName, delay)\n" + "setUnitAnimation(" + unitName + "[7], animName, delay, false);\nend");
    lua_engine->execute("function " + unitName + ".stopAnimation(animName)\n" + "stopUnitAnimation(" + unitName + "[7], animName);\nend");
    lua_engine->execute("function " + unitName + ".cycleAnimation(animName, weight)\n" + "setUnitAnimation(" + unitName + "[7], animName, weight, true);\nend");

    lua_engine->getAzadiInstance()->outputConsole(std::string("Instantiated a unit named: ") + unitName);
}

Unit::~Unit()
{

}

void Unit::doAnimation(const std::string &animName, const float &delay)
{
    lua_engine->getModelFactory()->StartAction(modelID, animName, delay);
}

void Unit::doAnimCycle(const std::string &animName, const float &weight)
{
    lua_engine->getModelFactory()->StartCycle(modelID, animName, weight);
}

void Unit::stopAnimation(const std::string &animName)
{
    lua_engine->getModelFactory()->StopAction(modelID, animName);
    lua_engine->getModelFactory()->StopCycle(modelID, animName);
}

void Unit::syncToLuaStack()
{
    std::stringstream temp;
    temp << x;
    lua_engine->execute(unitName + "[1] = " + temp.str());
    temp.clear();
    temp << y;
    lua_engine->execute(unitName + "[2] = " + temp.str());
    temp.clear();
    temp << z;
    lua_engine->execute(unitName + "[3] = " + temp.str());
    temp.clear();
    temp << rx;
    lua_engine->execute(unitName + "[4] = " + temp.str());
    temp.clear();
    temp << ry;
    lua_engine->execute(unitName + "[5] = " + temp.str());
    temp.clear();
    temp << rz;
    lua_engine->execute(unitName + "[6] = " + temp.str());
    temp.clear();
    temp << unitID;
    lua_engine->execute(unitName + "[7] = " + temp.str());
}

void Unit::syncPosToLuaStack()
{
    std::stringstream temp;
    temp << x;
    lua_engine->execute(unitName + "[1] = " + temp.str());
    temp.clear();
    temp << y;
    lua_engine->execute(unitName + "[2] = " + temp.str());
    temp.clear();
    temp << z;
    lua_engine->execute(unitName + "[3] = " + temp.str());
}

void Unit::syncRotToLuaStack()
{
    std::stringstream temp;
    temp << rx;
    lua_engine->execute(unitName + "[4] = " + temp.str());
    temp.clear();
    temp << ry;
    lua_engine->execute(unitName + "[5] = " + temp.str());
    temp.clear();
    temp << rz;
    lua_engine->execute(unitName + "[6] = " + temp.str());
}

void Unit::setScript(const std::string &ScriptName)
{
    scriptName = ScriptName;
}

void Unit::setRotation(const float &X, const float &Y, const float &Z)
{
    rx = X;
    ry = Y;
    rz = Z;

    lua_engine->getModelFactory()->Rotate(modelID, rx, ry, rz);
    syncRotToLuaStack();
}

void Unit::setPosition(const float &X, const float &Y, const float &Z)
{
    x = X;
    y = Y;
    z = Z;

    lua_engine->getModelFactory()->Move(modelID, x, y, z);
    syncPosToLuaStack();
}

void Unit::pushKey(const int &keysym, const char &ch)
{

}

void Unit::pushClick(const char &button)
{

}

void Unit::executeScript()
{
    // Sets the current unit table in the lua state machine to this unit
    lua_engine->execute("currentUnit = " + unitName);

    // Executes the script
    lua_engine->execute(scriptName);
}

bool Unit::checkClicked(const float &mousex, const float &mousey, const float &mousez, const float cam[3])
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
