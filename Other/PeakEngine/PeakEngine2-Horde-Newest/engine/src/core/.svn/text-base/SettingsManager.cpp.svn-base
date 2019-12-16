/*
Copyright (C) 2008  Mathias Gottschlag, Lukas Kropatschek

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

#include "core/SettingsManager.h"
#include "core/Util.h"
#include "core/GameEngine.h"
#include <cstdlib>

namespace peak
{
	SettingsManager *SettingsManager::get(void)
	{
		static SettingsManager settings;
		return &settings;
	}
	
	bool SettingsManager::load(void)
	{
		return file.load("settings.ini");;
	}
	bool SettingsManager::save(void)
	{
		return file.save("settings.ini");
	}
	
	std::string SettingsManager::getString(std::string name)
	{
		if (!doesExist(name)) return "";
		std::string section;
		std::string item;
		splitString(name, '.', &section, &item);
		return file.getItemValue(section, item);
	}
	bool SettingsManager::getBool(std::string name)
	{
		std::string setting = getString(name);
		if ((setting == "true") || (setting == "yes"))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	int SettingsManager::getInt(std::string name)
	{
		std::string setting = getString(name);
		return toInt(setting);
	}
	
	std::wstring SettingsManager::getWString(std::string name)
	{
		std::string string = getString(name);
		return toWString(string);
	}
	
	Vector2D SettingsManager::getVector2D(std::string name)
	{
		std::string setting = getString(name);
		return toVector2D(setting);
	}

	Vector2DI SettingsManager::getVector2DI(std::string name)
	{
		std::string setting = getString(name);
		return toVector2DI(setting);
	}
	
	Vector3D SettingsManager::getVector3D(std::string name)
	{
		std::string setting = getString(name);
		return toVector3D(setting);
	}
	
	void SettingsManager::setString(std::string name, std::string value)
	{
		std::string section;
		std::string item;
		splitString(name, '.', &section, &item);
		file.setItem(section, item, value);
	}
	void SettingsManager::setBool(std::string name, bool value)
	{
		if (value)
		{
			setString(name, "true");
		}
		else
		{
			setString(name, "false");
		}
	}
	void SettingsManager::setInt(std::string name, int value)
	{
		setString(name, toString(value));
	}
	void SettingsManager::setVector2D(std::string name, Vector2D value)
	{
		setString(name, toString(value));
	}
	void SettingsManager::setVector2DI(std::string name, Vector2DI value)
	{
		setString(name, toString(value));
	}
	void SettingsManager::setVector3D(std::string name, Vector3D value)
	{
		setString(name, toString(value));
	}
	
	void SettingsManager::deleteSetting(std::string name)
	{
		if (doesExist(name))
		{
			std::string section;
			std::string item;
			splitString(name, '.', &section, &item);
			file.deleteItem(section, item);
		}
	}
	bool SettingsManager::doesExist(std::string name)
	{
		std::string section;
		std::string item;
		splitString(name, '.', &section, &item);
		int sectionidx = file.getSectionIndex(section);
		if (sectionidx == -1)
		{
			return false;
		}
		int itemidx = file.getItemIndex(section, item);
		if (itemidx == -1)
		{
			return false;
		}
		return true;
	}
	
	std::vector<std::string> SettingsManager::getItemList(std::string section)
	{
		std::vector<std::string> list;
		int sectionidx = file.getSectionIndex(section);
		if (sectionidx == -1)
		{
			return list;
		}
		
		for (int i = 0; i < file.getItemCount(sectionidx); i++)
		{
			list.push_back(file.getItemName(sectionidx, i));
		}
		return list;
	}
	
	SettingsManager::SettingsManager()
	{
	}
}

