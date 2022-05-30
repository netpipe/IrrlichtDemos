#include "CScriptManager.h"

namespace agEngine
{
    namespace script
    {
        CScriptManager* CScriptManager::instance = NULL;

        CScriptManager::CScriptManager(io::IFileSystem* fs, entities::CEntityManager* entMgr)
        : emgr(entMgr), fileSystem(fs)
        {
            L = lua_open();
            luaL_openlibs(L);

            if (fs)
                fs->grab();

            if (entMgr)
                entMgr->grab();

            // Register all the scripting functions
            s_registerScriptFunctions(L);

            instance = this;
        }

        CScriptManager::~CScriptManager()
        {
            fileSystem->drop();
            emgr->drop();
        }

        core::stringc CScriptManager::loadScript(const core::stringc& scriptName)
        {
            io::IReadFile* scriptFile = fileSystem->createAndOpenFile(scriptName.c_str());
            c8* buffer = new c8[scriptFile->getSize()];
            scriptFile->read(buffer, scriptFile->getSize());

            scriptFile->drop();

            return core::stringc(buffer);
        }

        void CScriptManager::execute(const core::stringc& command, core::stringc& response)
        {
            if (luaL_loadbuffer(L, command.c_str(), command.size(), "noDebug") || lua_pcall(L, 0, 0, 0))
            {
                // return result and pop it off the stack
                response = luaL_checkstring(L, 1);
                lua_pop(L, 1);
            }
        }

        void CScriptManager::compileScript(const core::stringc& script)
        {
            core::stringc result;
            execute(script, result);
        }
    }
}
