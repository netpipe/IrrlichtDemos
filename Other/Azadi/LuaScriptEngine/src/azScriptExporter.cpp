#include "LuaEngine.h"

extern "C" azScriptEngine *getScriptEngineInstance()
{
    if (LuaEngine::getInstance() == NULL)
        return new LuaEngine();
    return LuaEngine::getInstance();
}
