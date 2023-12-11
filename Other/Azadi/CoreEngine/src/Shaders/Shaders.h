#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "../XML/tinyxml.h"

class Shaders
{
    public:
        Shaders(const std::string &workingDirectory);
        ~Shaders();

        inline bool usingGLSL() { return useGLSL; }
        inline void applyShader(const std::string &shaderName)
        {
            if (useGLSL)
                glUseProgram(compiledPrograms[shaderName]);
        }

        inline GLuint getShaderID(const std::string &shaderName)
        {
            if (useGLSL)
                return compiledPrograms[shaderName];
            return 0;
        }

        static Shaders* getInstance() { return instance; }
    private:
        static Shaders* instance;

        GLuint compileProgram(const std::vector<std::string> &shaders);

        std::string workingDirectory;
        bool useGLSL;
        std::map<std::string, GLuint> compiledShaders;
        std::map<std::string, GLuint> compiledPrograms;

        TiXmlDocument shaderDescriptions;
};

#endif
