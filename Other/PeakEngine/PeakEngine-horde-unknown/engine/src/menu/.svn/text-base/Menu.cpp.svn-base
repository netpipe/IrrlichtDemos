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

#include "menu/Menu.h"
#include "menu/Button.h"
#include "menu/CheckBox.h"
#include "menu/ListBox.h"
#include "menu/ComboBox.h"
#include "menu/Label.h"
#include "core/IniFile.h"
#include "core/Logger.h"
#include "core/Script.h"
#include "core/FileSystem.h"

namespace peak
{
	Menu::Menu()
	{
		script = 0;
	}
	Menu::~Menu()
	{
	}
	
	bool Menu::init(std::string name)
	{
		// Open ini file
		IniFile file;
		if (!file.load(std::string("menus/") + name + ".ini"))
		{
			return false;
		}
		// Load script if existing
		script = 0;
		std::string scriptname = FileSystem::get()->getFileExists(std::string("menus/") + name + ".lua");
		if (scriptname != "")
		{
			script = new Script;
			script->run(scriptname);
		}
		
		this->name = file.getItemValue("menu", "name");
		LDEBUG("Loading menu \"%s\".\n", name.c_str());
		int elementcount = file.getItemValueI("menu", "elements");
		
		// Load elements
		for (int i = 0; i < elementcount; i++)
		{
			std::string sectionname = std::string("element") + toString(i);
			if (file.getSectionIndex(sectionname) != -1)
			{
				std::string type = file.getItemValue(sectionname, "type");
				Vector2D position = file.getItemValue2D(sectionname, "position");
				Vector2D size = file.getItemValue2D(sectionname, "size");
				MenuElement *element = 0;
				std::string label = "";
				if (file.getItemIndex(sectionname, "label") != -1)
				{
					label = file.getItemValue(sectionname, "label");
				}
				if (type == "button")
				{
					// Create button
					Button *newbutton = new Button;
					if (!newbutton->init(this, 0))
					{
						delete newbutton;
						LERROR("Cannot create menu element (\"%s\": %d).\n", name.c_str(), i);
						continue;
					}
					newbutton->setLabel(label);
					
					// Read actions
					if (file.getItemIndex(sectionname, "action") != -1)
					{
						newbutton->setAction(file.getItemValue(sectionname, "action"));
					}
					if (file.getItemIndex(sectionname, "replacemenu") != -1)
					{
						newbutton->setMenu(file.getItemValue(sectionname, "replacemenu"));
						if (newbutton->getMenu() == "")
						{
							newbutton->setSpecialAction(ESA_CloseMenu);
						}
					}
					if (file.getItemIndex(sectionname, "quitgame") != -1)
					{
						if (file.getItemValueB(sectionname, "quitgame"))
						{
							newbutton->setSpecialAction(ESA_QuitGame);
						}
					}
					
					element = newbutton;
				}
				else if (type == "checkbox")
				{
					// Create button
					CheckBox *checkbox = new CheckBox;
					if (!checkbox->init(this, 0))
					{
						delete checkbox;
						LERROR("Cannot create menu element (\"%s\": %d).\n", name.c_str(), i);
						continue;
					}
					
					// Read setting
					if (file.getItemIndex(sectionname, "setting") != -1)
					{
						checkbox->setSetting(file.getItemValue(sectionname, "setting"));
					}
					// Read action
					if (file.getItemIndex(sectionname, "action") != -1)
					{
						checkbox->setAction(file.getItemValue(sectionname, "action"));
					}
					
					element = checkbox;
				}
				else if ((type == "listbox") || (type == "combobox"))
				{
					ListBox *listbox;
					if (type == "listbox")
					{
						listbox = new ListBox;
					}
					else
					{
						listbox = new ComboBox;
					}
					if (!listbox->init(this, 0))
					{
						delete listbox;
						LERROR("Cannot create menu element (\"%s\": %d).\n", name.c_str(), i);
						continue;
					}
					
					// Read content
					if (file.getItemIndex(sectionname, "itemcount") != -1)
					{
						int itemcount = file.getItemValueI(sectionname, "itemcount");
						for (int i = 0; i < itemcount; i++)
						{
							listbox->addItem(file.getItemValue(sectionname, std::string("item") + toString(i)));
						}
					}
					// Read action
					if (file.getItemIndex(sectionname, "action") != -1)
					{
						listbox->setAction(file.getItemValue(sectionname, "action"));
					}
					
					element = listbox;
				}
				else if (type == "label")
				{
					// Create button
					Label *label = new Label;
					if (!label->init(this, 0))
					{
						delete label;
						LERROR("Cannot create menu element (\"%s\": %d).\n", name.c_str(), i);
						continue;
					}
					element = label;
				}
				else
				{
					LERROR("Unknown menu element type \"%s\" (\"%s\": %d).\n", type.c_str(), name.c_str(), i);
					continue;
				}
				element->setPosition(position);
				element->setSize(size);
				
				// Actions?
				if (file.getItemIndex(sectionname, "action") != -1)
				{
					std::string action = file.getItemValue(sectionname, "action");
				}
				
				element->setID(i);
				elements.push_back(element);
			}
			else
			{
				LERROR("Cannot load menu element (\"%s\": %d).\n", name.c_str(), i);
			}
		}
		
		if (script && script->isFunction("create"))
		{
			script->callFunction("create", UserClass("peak::Menu", this));
		}
		
		return true;
	}
	bool Menu::destroy(void)
	{
		if (script && script->isFunction("destroy"))
		{
			script->callFunction("destroy");
		}
		while (elements.size() > 0)
		{
			(*elements.begin())->destroy();
			delete *elements.begin();
			elements.erase(elements.begin());
		}
		if (script)
		{
			delete script;
		}
		return false;
	}
	
	std::string Menu::getName(void)
	{
		return name;
	}
	
	bool Menu::show(void)
	{
		bool success = true;
		for (std::list<MenuElement*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if (!(*it)->show())
			{
				success = false;
			}
		}
		if (script && script->isFunction("show"))
		{
			script->callFunction("show");
		}
		return success;
	}
	bool Menu::hide(void)
	{
		if (script && script->isFunction("hide"))
		{
			script->callFunction("hide");
		}
		bool success = true;
		for (std::list<MenuElement*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if (!(*it)->hide())
			{
				success = false;
			}
		}
		return success;
	}
	
	bool Menu::pressButton(int id)
	{
		for (std::list<MenuElement*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if ((*it)->getID() == id)
			{
				if ((*it)->getType() == EMET_Button)
				{
					((Button*)(*it))->press();
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}
	bool Menu::setCheckBox(int id, bool value)
	{
		for (std::list<MenuElement*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if ((*it)->getID() == id)
			{
				if ((*it)->getType() == EMET_CheckBox)
				{
					((CheckBox*)(*it))->set(value);
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}
	bool Menu::setListBox(int id, int selected)
	{
		for (std::list<MenuElement*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if ((*it)->getID() == id)
			{
				if (((*it)->getType() == EMET_ListBox) || ((*it)->getType() == EMET_ComboBox))
				{
					((ListBox*)(*it))->choose(selected);
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}
	
	ListBox *Menu::getListBox(int id)
	{
		return (ListBox*)getElement(id);
	}
	Button *Menu::getButton(int id)
	{
		return (Button*)getElement(id);
	}
	Label *Menu::getLabel(int id)
	{
		return (Label*)getElement(id);
	}
	
	void Menu::injectMouseButton(int button, int x, int y, int state)
	{
		if (script && script->isFunction("onMouseButton"))
		{
			script->callFunction("onMouseButton", x, y, button, state);
		}
	}
	void Menu::injectMouseMoved(int x, int y)
	{
		if (script && script->isFunction("onMouseMoved"))
		{
			script->callFunction("onMouseMoved", x, y);
		}
	}

	MenuElement *Menu::getElement(int id)
	{
		for (std::list<MenuElement*>::iterator it = elements.begin(); it != elements.end(); it++)
		{
			if ((*it)->getID() == id)
			{
				return *it;
			}
		}
		return 0;
	}
	
	Script *Menu::getScript(void)
	{
		return script;
	}
}

