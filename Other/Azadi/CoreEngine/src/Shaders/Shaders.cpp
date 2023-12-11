#include "Shaders.h"

#include "../Azadi.h"
#include "../GUI/Console.h"

using GUI::Console;

Shaders* Shaders::instance = NULL;

Shaders::Shaders(const std::string &workingDir)
{
    instance = this;

    workingDirectory = workingDir;

    if (Azadi::getInstance()->usingExtensions() && Azadi::getInstance()->usingShaders())
    {
        if (GLEW_ARB_shading_language_100)
        {
            if ((shaderDescriptions.LoadFile((std::string("./") + workingDirectory + std::string("/Shaders.xml")).c_str())) == false)
            {
                console << Console::medium << Console::log << "Could not load the Shader description file" << Console::endl();
                useGLSL = false;
                return;
            }
            useGLSL = true;

            TiXmlHandle handle(&shaderDescriptions);

            bool done = false;
            unsigned int iterator = 0;
            std::string shaderName;
            std::vector<std::string> shaderFileNames;
            while (!done)
            {
                TiXmlElement *shader = handle.FirstChild("Shaders").Child("Program", iterator).Element();
                while (shader)
                {
                    if (compiledPrograms.find(shader->Attribute("name")) != compiledPrograms.end())
                    {
                        console << Console::log << Console::medium << "Duplicate shaders defined! Aborting" << Console::endl();
                        useGLSL = false;
                        return;
                    }

                    shaderName = shader->Attribute("name");
                    unsigned int fileIterator = 0;
                    TiXmlElement *shaderFiles = handle.FirstChild("Shaders").Child("Program", iterator).Child("ShaderFile", fileIterator).Element();
                    while (shaderFiles)
                    {
                        shaderFileNames.push_back(shaderFiles->Attribute("filename"));

                        fileIterator++;
                        shaderFiles = handle.FirstChild("Shaders").Child("Program", iterator).Child("ShaderFile", fileIterator).Element();
                    }

                    GLuint cProgram = compileProgram(shaderFileNames);
                    if (cProgram != 0)
                        compiledPrograms[shaderName] = cProgram;

                    shaderFileNames.clear();

                    iterator++;
                    shader = handle.FirstChild("Shaders").Child("Program", iterator).Element();
                }
                done = true;
            }

            console << Console::log << Console::medium << "Compiled " << compiledPrograms.size() << " shaders" << Console::endl();
            if (compiledPrograms.size() == 0)
            {
                useGLSL = false;
                console << Console::log << Console::medium << "Disabling the use of shaders as no shaders could be compiled" << Console::endl();
            }
        }
    }
	else
		console << Console::log << Console::medium << "Shaders have been disabled" << Console::endl();
}

Shaders::~Shaders()
{

}

GLuint Shaders::compileProgram(const std::vector<std::string> &shaderFiles)
{
    std::string fileExtension;
    GLuint compiledShaderProgram = glCreateProgram();
    if (compiledShaderProgram == 0)
    {
        console << Console::log << Console::medium << "Could not create a shader program" << Console::endl();
        return 0;
    }
    GLuint compiledShaderFile;
    for (unsigned int i = 0; i < shaderFiles.size(); ++i)
    {
        if (compiledShaders.find(shaderFiles[i]) != compiledShaders.end())
        {
            glAttachShader(compiledShaderProgram, compiledShaders[shaderFiles[i]]);
            continue;
        }

        fileExtension = shaderFiles[i].substr(shaderFiles[i].size() - 3);
        if (fileExtension == "avs")
        {
            compiledShaderFile = glCreateShader(GL_VERTEX_SHADER);
        }
        else if (fileExtension == "afs")
        {
            compiledShaderFile = glCreateShader(GL_FRAGMENT_SHADER);
        }
        else
        {
            console << Console::medium << Console::log << "Unknown shader file type: " << fileExtension << " when trying to compile " << shaderFiles[i] << Console::endl();
            glDeleteProgram(compiledShaderProgram);
            return 0;
        }

        if (compiledShaderFile != 0)
        {
            std::string shaderSource = "";
            std::ifstream file((std::string("./") + workingDirectory + std::string("/Shaders/") + shaderFiles[i]).c_str(), std::ios::in);
            if (file.is_open())
            {
                console << Console::log << Console::silent << "Compiling shader: " << shaderFiles[i] << Console::endl();
                std::string shaderLine;
                while (!file.eof())
                {
                    getline(file, shaderLine);
                    if (shaderLine.size() == 0)
                        continue;
                    shaderSource += shaderLine;
                    shaderSource.erase(shaderSource.size());
                }
                file.close();
                const GLchar *source[] = {shaderSource.c_str()};

                glShaderSource(compiledShaderFile, 1, source, NULL);
                glCompileShader(compiledShaderFile);
                GLint returnValue;
                glGetShaderiv(compiledShaderFile, GL_COMPILE_STATUS, &returnValue);
                if (returnValue == GL_FALSE)
                {
                    console << Console::medium << Console::log << "Error compiling shader: " << shaderFiles[i] << Console::endl();
                    char *log = new char[256];
                    glGetShaderInfoLog(compiledShaderFile, 256, NULL, log);
                    console << Console::medium << Console::log << std::string(log) << Console::endl();
                    delete [] log;
                    glDeleteShader(compiledShaderFile);
                    glDeleteProgram(compiledShaderProgram);
                    return 0;
                }
                console << Console::silent << Console::log << "Shader file " << shaderFiles[i] << " compiled successfully" << Console::endl();
                compiledShaders[shaderFiles[i]] = compiledShaderFile;
                glAttachShader(compiledShaderProgram, compiledShaderFile);
            }
            else
            {
                console << Console::medium << Console::log << "Could not open the shader file: " << shaderFiles[i] << Console::endl();
                glDeleteShader(compiledShaderFile);
                glDeleteProgram(compiledShaderProgram);
                return 0;
            }
        }
        else
        {
            console << Console::medium << Console::log << "There was an error trying to allocate a shader program" << Console::endl();
            glDeleteProgram(compiledShaderProgram);
            return 0;
        }
    }

    glLinkProgram(compiledShaderProgram);
    GLint returnedValue;
    glGetProgramiv(compiledShaderProgram, GL_LINK_STATUS, &returnedValue);
    if (returnedValue == GL_FALSE)
    {
        console << Console::medium << Console::log << "An error occured while linking the shader program: " << Console::endl();
        char *log = new char[256];
        glGetProgramInfoLog(compiledShaderProgram, 256, NULL, log);
        console << Console::medium << Console::log << std::string(log) << Console::endl();
        delete [] log;
        glDeleteProgram(compiledShaderProgram);
        return 0;
    }

    GLint validation;
    glValidateProgram(compiledShaderProgram);
    glGetProgramiv(compiledShaderProgram, GL_VALIDATE_STATUS, &validation);
    if (validation == GL_FALSE)
    {
        console << Console::medium << Console::log << "Could not validate the shader program: " << Console::endl();
        char *log = new char[256];
        glGetProgramInfoLog(compiledShaderProgram, 256, NULL, log);
        console << Console::medium << Console::log << std::string(log) << Console::endl();
        delete [] log;
        glDeleteProgram(compiledShaderProgram);
        return 0;
    }

    return compiledShaderProgram;
}

