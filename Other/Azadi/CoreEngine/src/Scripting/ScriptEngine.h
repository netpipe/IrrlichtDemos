#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <string>
#include <sstream>

#ifndef WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

#include "azScriptEngine.h"

class ScriptEngine
{
	public:
		ScriptEngine(const std::string &workingDirectory);
		~ScriptEngine();

        short returnedShort();
		int returnedInt();
		long returnedLong();
        float returnedFloat();
        double returnedDouble();
        bool returnedBool();
        std::string returnedString();

		void runfile(const std::string &fileName);
		void dostring(const std::string &command);

		void findSelectedEntity(const float &mousex, const float &mousey, const float &mousez, const float cam[3]);

		static ScriptEngine* getInstance() { return instance; }
	private:
        static ScriptEngine* instance;

        std::string workingDirectory;
		bool working;

        typedef azScriptEngine *script_inst();

        azScriptEngine *scEngine;
};

#endif
