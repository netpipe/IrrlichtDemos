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

#ifndef _INIFILE_H_
#define _INIFILE_H_

#include <string>
#include <vector>

#include "core/Vector3D.h"
#include "core/Vector2D.h"

namespace peak
{
	/**
	 * \brief Structure holding name and value of an ini file item.
	 */
	struct Item
	{
		std::string name;
		std::string value;
	};
	/**
	 * \brief Structure holding an ini file section.
	 */
	struct Section
	{
		std::string name;
		std::vector<Item> items;
	};

	/**
	 * \brief Class for reading and writing ini files.
	 */
	class IniFile
	{
		public:
			IniFile();
			~IniFile();

			/**
			 * \brief Loads the data from a file.
			 */
			bool load(std::string filename);
			/**
			 * \brief Saves the data ro a file.
			 */
			bool save(std::string filename);

			/**
			 * \brief Clears the loaded data.
			 */
			void clear(void);

			/**
			 * \brief Returns the number of sections.
			 */
			int getSectionCount(void);
			/**
			 * \brief Returns the name of a section.
			 * \param index Index of the section
			 */
			std::string getSection(int index);
			/**
			 * \brief Returns the index of a section.
			 * \param name Name of the section
			 */
			int getSectionIndex(std::string name);

			/**
			 * \brief Creates a new section.
			 * \param name Name of the section
			 * \return Returns the index of the section
			 */
			int createSection(std::string name);

			/**
			 * \brief Deletes a section.
			 * \param index Index of the section
			 */
			void deleteSection(int index);
			/**
			 * \brief Deletes a section.
			 * \param name Name of the section
			 */
			void deleteSection(std::string name);

			/**
			 * \brief Returns the number of items in a section.
			 * \param section Index of the section
			 */
			int getItemCount(int section);
			/**
			 * \brief Returns the number of items in a section.
			 * \param section Name of the section
			 */
			int getItemCount(std::string section);

			/**
			 * \brief Returns the name of an item in a section.
			 */
			std::string getItemName(int section, int item);
			/**
			 * \brief Returns the name of an item in a section.
			 */
			std::string getItemName(std::string section, int item);

			/**
			 * \brief Returns the index of an item in a section.
			 */
			int getItemIndex(int section, std::string item);
			/**
			 * \brief Returns the index of an item in a section.
			 */
			int getItemIndex(std::string section, std::string item);

			/**
			 * \brief Returns the value of an item.
			 */
			std::string getItemValue(int section, int item);
			/**
			 * \brief Returns the value of an item.
			 */
			std::string getItemValue(std::string section, int item);
			/**
			 * \brief Returns the value of an item.
			 */
			std::string getItemValue(int section, std::string item);
			/**
			 * \brief Returns the value of an item.
			 */
			std::string getItemValue(std::string section, std::string item);
			/**
			 * \brief Returns the value of an item as bool.
			 */
			bool getItemValueB(std::string section, std::string item);
			/**
			 * \brief Returns the value of an item as float
			 */
			float getItemValueF(std::string section, std::string item);
			/**
			 * \brief Returns the value of an item as integer
			 */
			int getItemValueI(std::string section, std::string item);
			/**
			 * \brief Returns the value of an item as Vector3D
			 */
			Vector3D getItemValue3D(std::string section, std::string item);
			/**
			 * \brief Returns the value of an item as Vector2D
			 */
			Vector2D getItemValue2D(std::string section, std::string item);

			/**
			 * \brief Creates an item.
			 * \param section Section of the item
			 * \param item Name of the item
			 * \param value Initial value of the item
			 * \return Returns the index of the new item.
			 */
			int createItem(int section, std::string item, std::string value);
			/**
			 * \brief Creates an item.
			 * \param section Section of the item
			 * \param item Name of the item
			 * \param value Initial value of the item
			 * \return Returns the index of the new item.
			 */
			int createItem(std::string section, std::string item, std::string value);

			/**
			 * \brief Sets an item to a new value.
			 * \param section Section of the item
			 * \param item Item to be set
			 * \param value New value of the item
			 */
			void setItem(int section, int item, std::string value);
			/**
			 * \brief Sets an item to a new value.
			 * \param section Section of the item
			 * \param item Item to be set
			 * \param value New value of the item
			 */
			void setItem(std::string section, int item, std::string value);
			/**
			 * \brief Sets an item to a new value.
			 * \param section Section of the item
			 * \param item Item to be set
			 * \param value New value of the item
			 */
			void setItem(int section, std::string item, std::string value);
			/**
			 * \brief Sets an item to a new value.
			 * \param section Section of the item
			 * \param item Item to be set
			 * \param value New value of the item
			 */
			void setItem(std::string section, std::string item, std::string value);

			/**
			 * \brief Deletes an item.
			 * \param section Section of the item
			 * \param item Item to be deleted
			 */
			void deleteItem(int section, int item);
			/**
			 * \brief Deletes an item.
			 * \param section Section of the item
			 * \param item Item to be deleted
			 */
			void deleteItem(std::string section, int item);
			/**
			 * \brief Deletes an item.
			 * \param section Section of the item
			 * \param item Item to be deleted
			 */
			void deleteItem(int section, std::string item);
			/**
			 * \brief Deletes an item.
			 * \param section Section of the item
			 * \param item Item to be deleted
			 */
			void deleteItem(std::string section, std::string item);
		private:
			std::vector<Section> sections;
	};
}

#endif

