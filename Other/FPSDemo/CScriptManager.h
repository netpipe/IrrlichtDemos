#ifndef _CSCRIPTMANAGER_H_
#define _CSCRIPTMANAGER_H_

#include <irrlicht.h>

extern "C"
{
#ifdef WIN32
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#else
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif
}

#include "DScriptedFunctions.h"
#include "CEntityManager.h"

using namespace irr;

namespace agEngine
{
    namespace script
    {
        class CScriptManager : public virtual IReferenceCounted
        {
        public:
            CScriptManager(io::IFileSystem* fs, entities::CEntityManager* emgr);
            virtual ~CScriptManager();

            virtual core::stringc loadScript(const core::stringc& fileName);
            virtual void compileScript(const core::stringc& script);
            virtual void execute(const core::stringc& command, core::stringc& response);

            static CScriptManager* instance;

            // For lua functions
            entities::CEntityManager* emgr;
        private:
            lua_State* L;
            io::IFileSystem* fileSystem;
        };
    }
}

#endif
