/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "entity/ServerScriptComponent.h"
#include "core/Script.h"

namespace peak
{
	ServerScriptComponent::ServerScriptComponent()
	{
	}
	ServerScriptComponent::~ServerScriptComponent()
	{
	}
	
	bool ServerScriptComponent::init(Entity *entity)
	{
		script = new Script;
		return true;
	}
	bool ServerScriptComponent::destroy(void)
	{
		if (script->isFunction("destroy"))
		{
			script->callFunction("destroy");
		}
		delete script;
		return true;
	}
	
	bool ServerScriptComponent::needsUpdate(void)
	{
		return false;
	}
	int ServerScriptComponent::getUpdateSize(void)
	{
		return 0;
	}
	int ServerScriptComponent::writeUpdateData(Buffer *buffer)
	{
		return 0;
	}
	
	bool ServerScriptComponent::updateFromData(Buffer *buffer)
	{
		return false;
	}
	
	bool ServerScriptComponent::writeCompleteData(Buffer *buffer)
	{
		return false;
	}
	bool ServerScriptComponent::readCompleteData(Buffer *buffer)
	{
		return false;
	}
	
	bool ServerScriptComponent::runScript(std::string file)
	{
		return script->run(file);
	}
	
	ComponentType ServerScriptComponent::getType(void)
	{
		return ECT_ServerScript;
	}
	
	bool ServerScriptComponent::doWork(float msecs)
	{
		script->callFunction("update", msecs);
		return true;
	}
	
	Script *ServerScriptComponent::getScript(void)
	{
		return script;
	}
}

