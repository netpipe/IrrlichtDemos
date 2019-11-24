#include "ScriptEngine.h"

#include "../Azadi.h"
#include "../GUI/Console.h"
#include "../ModelFactory/ModelFactory.h"
#include "../Sound/SoundFactory.h"
#include "../Texture/TextureFactory.h"
#include "../GUI/Drawing.h"
#include "../Physics/Physics.h"
#include "ScriptEngineInputs.h"

using GUI::Console;

ScriptEngine *ScriptEngine::instance = NULL;

//! Script engine constructor! Registers all required core lua functions
ScriptEngine::ScriptEngine(const std::string &workingDir)
{
    workingDirectory = workingDir;

	console << Console::normal << Console::high << "Initializing Scripting Engine" << Console::endl();

    working = true;
    scEngine = NULL;

    std::stringstream wordLength;

    wordLength << sizeof(int*) * 8;

	#ifdef WIN32
    HINSTANCE handle = LoadLibrary((std::string("./") + workingDir + std::string("/scriptEngine" + wordLength.str() + ".dll")).c_str());
    if (!handle)
    {
        console << Console::normal << Console::high << "Could not open the scripting engine library" << std::string("scriptEngine" + wordLength.str() + ".dll") << Console::endl();
        working = false;
    }
    if (working)
    {
        script_inst *gScriptEngineInstance = (script_inst*)GetProcAddress(handle, "getScriptEngineInstance");
        if (!gScriptEngineInstance)
        {
            console << Console::normal << Console::high << "Could not resolve all dependencies in the script lib!" << Console::endl();
            working = false;
        }
    #elif defined(__APPLE__)
    void *handle = dlopen((std::string("./") + workingDir + std::string("/scriptEngine" + wordLength.str() + ".dylib")).c_str(), RTLD_LAZY);
    if (!handle)
    {
        console << Console::normal << Console::high << "Could not open the scripting engine library " << std::string("scriptEngine" + wordLength.str() + ".dylib") << Console::endl();
        working = false;
    }
    if (working)
    {
        script_inst *gScriptEngineInstance = (script_inst*) dlsym(handle, "getScriptEngineInstance");
        if (!gScriptEngineInstance)
        {
            console << Console::normal << Console::high << "Could not resolve all dependencies in the script lib!" << Console::endl();
            dlclose(handle);
            working = false;
        }
    #else
	void *handle = dlopen((std::string("./") + workingDir + std::string("/scriptEngine" + wordLength.str() + ".so")).c_str(), RTLD_LAZY);
	if (!handle)
    {
        console << Console::normal << Console::high << "Could not open the scripting engine library " << std::string("scriptEngine" + wordLength.str() + ".so") << Console::endl();
        working = false;
    }
    if (working)
    {
        script_inst *gScriptEngineInstance = (script_inst*) dlsym(handle, "getScriptEngineInstance");
        if (!gScriptEngineInstance)
        {
            console << Console::normal << Console::high << "Could not resolve all dependencies in the script lib!" << Console::endl();
            dlclose(handle);
            working = false;
        }
    #endif
        if (working)
        {
            scEngine = gScriptEngineInstance();
        }
    }

    if (scEngine == NULL)
        working = false;
    else
    {
        if (scEngine->registerAzadiCore(Azadi::getInstance()))
        {
            scEngine->registerModelFactory(ModelFactory::getInstance());
            scEngine->registerSoundFactory(SoundFactory::getInstance());
            scEngine->registerTextureFactory(TextureFactory::getInstance());
            scEngine->registerDrawingEngine(Drawing::getInstance());
            scEngine->registerPhysicsEngine(Physics::getInstance());
            scEngine->registerInputEngine(ScriptEngineInputs::getInstance());

            scEngine->setWorkingDirectory(workingDirectory);

            scEngine->initialize();

            console << Console::normal << Console::high << "Scripting engine interface: " << scEngine->getScriptEngineIdentifier() << Console::endl();
        }
        else
            working = false;
    }

	instance = this;
}

//! Closes the lua virtual machine
ScriptEngine::~ScriptEngine()
{

}

//! Runs a script file
void ScriptEngine::runfile(const std::string &fileName)
{
    if (working)
        scEngine->includeFile(fileName);
}

void ScriptEngine::findSelectedEntity(const float &mousex, const float &mousey, const float &mousez, const float cam[3])
{
    if (working)
        scEngine->findSelectedEntity(mousex, mousey, mousez, cam);
}

//! Single line command interpreter
void ScriptEngine::dostring(const std::string &command)
{
    if (working)
        scEngine->execute(command);
}

short ScriptEngine::returnedShort()
{
    if (working)
        return scEngine->returnedShort();
    return 0;
}

int ScriptEngine::returnedInt()
{
    if (working)
        return scEngine->returnedInt();
    return 0;
}

long ScriptEngine::returnedLong()
{
    if (working)
        return scEngine->returnedLong();
    return 0;
}

float ScriptEngine::returnedFloat()
{
    if (working)
        return scEngine->returnedFloat();
    return 0.0f;
}

double ScriptEngine::returnedDouble()
{
    if (working)
        return scEngine->returnedDouble();
    return 0.0;
}

bool ScriptEngine::returnedBool()
{
    if (working)
        return scEngine->returnedBool();
    return true;
}

std::string ScriptEngine::returnedString()
{
    if (working)
        return scEngine->returnedString();
    return "";
}
