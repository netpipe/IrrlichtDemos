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

#include "core/IniFile.h"
#include "core/FileSystem.h"
#include "core/File.h"
#include "core/Util.h"
#include "core/Logger.h"

#include <cstring>

namespace peak
{
	IniFile::IniFile()
	{
	}
	IniFile::~IniFile()
	{
	}
	
	bool IniFile::load(std::string filename)
	{
		// Open file
		File *file = FileSystem::get()->openFile(filename, EFM_Read, false);
		if (!file)
		{
			LERROR("Cannot load INI file \"%s\".\n", filename.c_str());
			return false;
		}
		// Parse file
		char buffer[1024];
		std::vector<std::string> lines;
		// Split it into lines
		int read = 0;
		read = file->read(buffer, 1023);
		buffer[read] = 0;
		while (true)
		{
			// Parse buffer
			std::string line;
			char *newline;
			char *bufferpos = buffer;
			while ((newline = strchr(bufferpos, '\n')))
			{
				if (newline == buffer)
				{
					// Empty line
				}
				if (*(newline - 1) == '\\')
				{
					memmove(newline - 1, newline + 1, 1024 - (newline - buffer + 1));
					read -= 2;
					bufferpos = newline - 1;
				}
				else
				{
					char tmpline[1024];
					memcpy(tmpline, buffer, newline - buffer);
					tmpline[newline - buffer] = 0;
					line = tmpline;
					if (line.size() > 0)
					{
						lines.push_back(line);
					}
					
					memmove(buffer, newline + 1, 1024 - (newline - buffer + 1));
					read -= newline - buffer + 1;
				}
			}
			
			int oldread = read;
			read += file->read(buffer + read, 1023 - read);
			buffer[read] = 0;
			if (read == oldread)
			{
				break;
			}
		}
		// Go through the lines
		for (unsigned int i = 0; i < lines.size(); i++)
		{
			// Remove unnecessary whitespaces
			std::string line = lines[i];
			trimString(&line);
			//printf("Trimmed line: %s\n", line.c_str());
			if (line.size() < 3)
			{
				continue;
			}
			
			// Parse line
			if ((line[0] == '[') && (line[line.size() - 1] == ']'))
			{
				Section newsection;
				newsection.name = line.substr(1, line.size() - 2);
				//printf("Section: %s\n", newsection.name.c_str());
				sections.push_back(newsection);
			}
			else if (sections.size() > 0)
			{
				if (line[0] == '=')
				{
					continue;
				}
				// Look for equal sign
				int equal = 0;
				for (unsigned int i2 = 1; i2 < line.size(); i2++)
				{
					if ((line[i2] == '=') && (line[i2 - 1] != '\\'))
					{
						equal = i2;
						break;
					}
				}
				if (equal == 0)
				{
					continue;
				}
				// Get name/value
				std::string name = line.substr(0, equal);
				trimString(&name);
				if ((name[0] == '\"') && (name[name.size() - 1] == '\"'))
				{
					name = name.substr(1, name.size() - 2);
				}
				//printf("Name: \"%s\".\n", name.c_str());
				std::string value = line.substr(equal + 1, line.size() - equal - 1);
				trimString(&value);
				if ((value[0] == '\"') && (value[value.size() - 1] == '\"'))
				{
					value = value.substr(1, value.size() - 2);
				}
				//printf("Value: \"%s\".\n", value.c_str());
				// Add item to section
				Item newitem;
				newitem.name = name;
				newitem.value = value;
				sections[sections.size() - 1].items.push_back(newitem);
			}
		}
		delete file;
		return true;
	}
	bool IniFile::save(std::string filename)
	{
		// Open file
		File *file = FileSystem::get()->openFile(filename, EFM_Write, false);
		if (!file)
		{
			LERROR("Cannot save INI file \"%s\".\n", filename.c_str());
			return false;
		}
		// Write sections
		for (unsigned int i = 0; i < sections.size(); i++)
		{
			file->write("[", 1);
			file->write(sections[i].name.c_str(), sections[i].name.size());
			file->write("]\n", 2);
			for (unsigned int i2 = 0; i2 < sections[i].items.size(); i2++)
			{
				file->write("\"", 1);
				file->write(sections[i].items[i2].name.c_str(), sections[i].items[i2].name.size());
				file->write("\" = \"", 5);
				file->write(sections[i].items[i2].value.c_str(), sections[i].items[i2].value.size());
				file->write("\"\n", 2);
			}
		}
		
		delete file;
		return true;
	}
	
	void IniFile::clear(void)
	{
		sections.clear();
	}
	
	int IniFile::getSectionCount(void)
	{
		return sections.size();
	}
	std::string IniFile::getSection(int index)
	{
		return sections[index].name;
	}
	int IniFile::getSectionIndex(std::string name)
	{
		for (unsigned i = 0; i < sections.size(); i++)
		{
			if (sections[i].name == name)
			{
				return i;
			}
		}
		return -1;
	}
	
	int IniFile::createSection(std::string name)
	{
		Section section;
		section.name = name;
		sections.push_back(section);
		return sections.size() - 1;
	}
	
	void IniFile::deleteSection(int index)
	{
	}
	void IniFile::deleteSection(std::string name)
	{
	}
	
	int IniFile::getItemCount(int section)
	{
		return sections[section].items.size();
	}
	int IniFile::getItemCount(std::string section)
	{
		return sections[getSectionIndex(section)].items.size();
	}
	
	std::string IniFile::getItemName(int section, int item)
	{
		return sections[section].items[item].name;
	}
	std::string IniFile::getItemName(std::string section, int item)
	{
		return sections[getSectionIndex(section)].items[item].name;
	}
	
	int IniFile::getItemIndex(int section, std::string item)
	{
		for (unsigned i = 0; i < sections[section].items.size(); i++)
		{
			if (sections[section].items[i].name == item)
			{
				return i;
			}
		}
		return -1;
	}
	int IniFile::getItemIndex(std::string section, std::string item)
	{
		if (getSectionIndex(section) == -1) return -1;
		for (unsigned i = 0; i < sections[getSectionIndex(section)].items.size(); i++)
		{
			if (sections[getSectionIndex(section)].items[i].name == item)
			{
				return i;
			}
		}
		return -1;
	}
	
	std::string IniFile::getItemValue(int section, int item)
	{
		return sections[section].items[item].value;
	}
	std::string IniFile::getItemValue(std::string section, int item)
	{
		return sections[getSectionIndex(section)].items[item].value;
	}
	std::string IniFile::getItemValue(int section, std::string item)
	{
		return sections[section].items[getItemIndex(section, item)].value;
	}
	std::string IniFile::getItemValue(std::string section, std::string item)
	{
		if ((getSectionIndex(section) != -1) && (getItemIndex(section, item) != -1))
		{
			return sections[getSectionIndex(section)].items[getItemIndex(section, item)].value;
		}
		else
		{
			return "";
		}
	}
	
	bool IniFile::getItemValueB(std::string section, std::string item)
	{
		std::string val = sections[getSectionIndex(section)].items[getItemIndex(section, item)].value;	
		return toBool(val);
	}
	
	int IniFile::getItemValueI(std::string section, std::string item)
	{
		std::string val = sections[getSectionIndex(section)].items[getItemIndex(section, item)].value;
		int ret = toInt(val);
		return ret;
	}

	float IniFile::getItemValueF(std::string section, std::string item)
	{
		std::string val = sections[getSectionIndex(section)].items[getItemIndex(section, item)].value;
		float ret = toFloat(val);
		return ret;
	}
	
	Vector3D IniFile::getItemValue3D(std::string section, std::string item)
	{
		std::string val = sections[getSectionIndex(section)].items[getItemIndex(section, item)].value;
		Vector3D ret = toVector3D(val);
		return ret;
	}
	
	Vector2D IniFile::getItemValue2D(std::string section, std::string item)
	{
		std::string val = sections[getSectionIndex(section)].items[getItemIndex(section, item)].value;
		Vector2D ret = toVector2D(val);
		return ret;
	}
	
	
	int IniFile::createItem(int section, std::string item, std::string value)
	{
		Item newitem;
		newitem.name = item;
		newitem.value = value;
		sections[section].items.push_back(newitem);
		return sections[section].items.size() - 1;
	}
	int IniFile::createItem(std::string section, std::string item, std::string value)
	{
		return createItem(getSectionIndex(section), item, value);
	}
	
	void IniFile::setItem(int section, int item, std::string value)
	{
		sections[section].items[item].value = value;
	}
	void IniFile::setItem(std::string section, int item, std::string value)
	{
		sections[getSectionIndex(section)].items[item].value = value;
	}
	void IniFile::setItem(int section, std::string item, std::string value)
	{
		sections[section].items[getItemIndex(section, item)].value = value;
	}
	void IniFile::setItem(std::string section, std::string item, std::string value)
	{
		sections[getSectionIndex(section)].items[getItemIndex(section, item)].value = value;
	}
	
	void IniFile::deleteItem(int section, int item)
	{
		sections[section].items.erase(sections[section].items.begin() + item);
	}
	void IniFile::deleteItem(std::string section, int item)
	{
		sections[getSectionIndex(section)].items.erase(sections[getSectionIndex(section)].items.begin() + item);
	}
	void IniFile::deleteItem(int section, std::string item)
	{
		sections[section].items.erase(sections[section].items.begin() + getItemIndex(section, item));
	}
	void IniFile::deleteItem(std::string section, std::string item)
	{
		sections[getSectionIndex(section)].items.erase(sections[getSectionIndex(section)].items.begin() + getItemIndex(section, item));
	}
}

