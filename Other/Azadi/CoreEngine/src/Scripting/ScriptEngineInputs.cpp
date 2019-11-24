#include "ScriptEngineInputs.h"

ScriptEngineInputs* ScriptEngineInputs::instance = NULL;

ScriptEngineInputs::ScriptEngineInputs()
{
    instance = this;
    iobject = NULL;
}

ScriptEngineInputs::~ScriptEngineInputs()
{

}

void ScriptEngineInputs::parseInput(Input &input, const GLint viewport[4], const GLdouble projection[16], const GLdouble modelview[16], const float camPos[3])
{
    // Get the OpenGL Y coordinate(since opengl's Y coordinate is inverse of SDL's)
    GLint realY = viewport[3] - input.getMouseY() -1;
    GLdouble coord1[3];
    GLdouble coord2[3];

    // Unproject at both z = 0.0f, and z = 1.0f
    gluUnProject(input.getMouseX(), realY, 0.0f, modelview, projection, viewport, &coord1[0], &coord1[1], &coord1[2]);
    gluUnProject(input.getMouseX(), realY, (1.0f / 2000), modelview, projection, viewport, &coord2[0], &coord2[1], &coord2[2]);

    // Get the resulting vector
    coord2[0] -= coord1[0];
    coord2[1] -= coord1[1];
    coord2[2] -= coord1[2];

    float total = sqrt((coord2[0] * coord2[0]) + (coord2[1] * coord2[1]) + (coord2[2] * coord2[2]));

    coord2[0] /= total;
    coord2[1] /= total;
    coord2[2] /= total;

    if (iobject != NULL)
    {
        for (unsigned int i = 0; i < registeredKeys.size(); ++i)
            if (input.getKeyPress(registeredKeys[i]))
                iobject->pushKey(registeredKeys[i], registeredKeys[i]);

        if ((!input.anyMousePressed()) && (!iobject->checkClicked(coord2[0], coord2[1], coord2[2], camPos)))
        {
            for (unsigned int i = 0; i < registeredMbuts.size(); ++i)
                if (input.getMouseClick(registeredMbuts[i]))
                    iobject->pushClick(registeredMbuts[i]);

            return;
        }
    }
    else if (input.anyMousePressed())
        ScriptEngine::getInstance()->findSelectedEntity(coord2[0], coord2[1], coord2[2], camPos);
}

void ScriptEngineInputs::registerKey(const int &keysym)
{
    registeredKeys.push_back(keysym);
}

void ScriptEngineInputs::registerInput(InputObject *inObject)
{
    iobject = inObject;
}
