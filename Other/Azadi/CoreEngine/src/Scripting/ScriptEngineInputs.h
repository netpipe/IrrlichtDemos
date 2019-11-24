#ifndef SCRIPTENGINEINPUTS_H
#define SCRIPTENGINEINPUTS_H

#include <cmath>

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include "../Input/Input.h"
#include "Headers/Input.h"
#include "ScriptEngine.h"

class ScriptEngineInputs : public azInput
{
    public:
        ScriptEngineInputs();
        ~ScriptEngineInputs();

        void parseInput(Input &input, const GLint viewport[4], const GLdouble projection[16], const GLdouble modelview[16], const float camPos[3]);
        void registerKey(const int &keysym);
        void registerMouse(const short &mbutton);
        void registerInput(InputObject *iobject);

        static ScriptEngineInputs* getInstance() { return instance; }
    private:
        static ScriptEngineInputs *instance;

        InputObject *iobject;
        std::vector<int> registeredKeys;
        std::vector<int> registeredMbuts;
};

#endif
