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

#include "entity/ClientScriptComponent.h"
#include "core/Script.h"

namespace peak
{
	ClientScriptComponent::ClientScriptComponent()
	{
	}
	ClientScriptComponent::~ClientScriptComponent()
	{
	}
	
	bool ClientScriptComponent::init(Entity *entity)
	{
		script = new Script;
		return true;
	}
	bool ClientScriptComponent::destroy(void)
	{
		if (script->isFunction("destroy"))
		{
			script->callFunction("destroy");
		}
		delete script;
		return true;
	}
	
	bool ClientScriptComponent::needsUpdate(void)
	{
		return false;
	}
	int ClientScriptComponent::getUpdateSize(void)
	{
		return 0;
	}
	int ClientScriptComponent::writeUpdateData(Buffer *buffer)
	{
		return 0;
	}
	
	bool ClientScriptComponent::updateFromData(Buffer *buffer)
	{
		return false;
	}
	
	bool ClientScriptComponent::writeCompleteData(Buffer *buffer)
	{
		return false;
	}
	bool ClientScriptComponent::readCompleteData(Buffer *buffer)
	{
		return false;
	}
	
	bool ClientScriptComponent::runScript(std::string file)
	{
		return script->run(file);
	}
	
	ComponentType ClientScriptComponent::getType(void)
	{
		return ECT_ClientScript;
	}
	
	bool ClientScriptComponent::doWork(float msecs)
	{
		if (script->isFunction("update"))
		{
			script->callFunction("update", msecs);
		}
		return true;
	}
	
	Script *ClientScriptComponent::getScript(void)
	{
		return script;
	}
}

