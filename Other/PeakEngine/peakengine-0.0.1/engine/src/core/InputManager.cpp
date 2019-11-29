/*
Copyright (C) 2008  Lukas Kropatschek

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

#include "core/InputManager.h"
#include "core/SettingsManager.h"
#include "core/Game.h"
#include "core/Logger.h"
#include "graphic/GraphicsEngine.h"
#ifdef _USE_IRRLICHT_
#include <irrlicht.h>
#endif

namespace peak
{

	InputManager::InputManager()
	{
		enabled = true;
	}

	InputManager *InputManager::get()
	{
		static InputManager mgr;
		return &mgr;
	}

	bool InputManager::init(void)
	{
		std::vector<std::string> actionnames = SettingsManager::get()->getItemList("actions");
		for (unsigned int i = 0; i < actionnames.size(); i++)
		{
			std::string keyname = SettingsManager::get()->getString(std::string("actions.") + actionnames[i]);
			LDEBUG("Action: %s: 0x%02X.\n", actionnames[i].c_str(), getKey(keyname));
			if (getKey(keyname) != 0)
			{
				actions.insert(std::pair<Keycode, std::string>(getKey(keyname), actionnames[i]));
			}
			else
			{
				LWARNING("Invalid key: %s: %s.\n", actionnames[i].c_str(), keyname.c_str());
			}
		}
		return true;
	}
	bool InputManager::shutdown(void)
	{
		actions.clear();
		return true;
	}

	void InputManager::keyboardEvent(Keycode code, bool pressed)
	{
		if (!enabled) return;
		if (Game::get()->isClient())
		{
			std::map<Keycode, std::string>::iterator it = actions.find(code);
			if (it != actions.end())
			{
				Game::get()->injectAction((*it).second, pressed);
			}
		}
	}

	void InputManager::mouseMoved(float x, float y)
	{
		if (!enabled) return;
		if (Game::get()->isClient())
		{
			if ((x == 200) && (y == 200)) return;
			Game::get()->injectMouseMovement(x - 200, y - 200, 0);
			#ifdef _USE_IRRLICHT_
			GraphicsEngine::get()->getCursorControl()->setPosition(200, 200);
			#endif
		}
	}

	void InputManager::setInputEnabled(bool enabled)
	{
		this->enabled = enabled;
	}
	bool InputManager::getInputEnabled(void)
	{
		return enabled;
	}
}
