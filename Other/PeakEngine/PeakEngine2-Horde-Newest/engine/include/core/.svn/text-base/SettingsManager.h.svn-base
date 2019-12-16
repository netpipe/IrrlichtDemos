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

#ifndef _SETTINGSMANAGER_H_
#define _SETTINGSMANAGER_H_

#include <string>
#include <vector>
#include "core/IniFile.h"
#include "core/Vector2DI.h"
#include "core/Vector2D.h"
#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	/**
	 * \brief Class managing the core game settings.
	 */
	class SettingsManager
	{
		public:
			/**
			 * \brief Returns pointer to the SettingsManager class.
			 *
			 * \return Pointer to the settings manager
			 */
			static SettingsManager *get(void);
			
			/**
			 * \brief Loads all settings.
			 */
			bool load(void);
			/**
			 * \brief Saves all settings.
			 */
			bool save(void);
			
			/**
			 * \brief Returns a setting.
			 *
			 * The name of the setting is formed like "section.item" where section
			 * and item are the section and item name from the ini file.
			 * \param name Name of the setting
			 */
			std::string getString(std::string name);
			/**
			 * \brief Returns a setting as a bool.
			 *
			 * This just compares the output of getSetting() with "true" and "false".
			 * \param name Name of the setting
			 */
			bool getBool(std::string name);
			/**
			 * \brief Returns a setting as an int.
			 * \param name Name of the setting
			 */
			int getInt(std::string name);
			/**
			 * \brief Returns a setting as an std::wstring
			 * \param name Name of the setting
			 */
			std::wstring getWString(std::string name);
			
			/**
			 * \brief Returns a setting as an Vector2D
			 * \param name Name of the setting
			 */
			Vector2D getVector2D(std::string name);
			
			/**
			 * \brief Returns a setting as an Vector2DI
			 * \param name Name of the setting
			 */
			Vector2DI getVector2DI(std::string name);
			
			/**
			 * \brief Returns a setting as an Vector3D
			 * \param name Name of the setting
			 */
			Vector3D getVector3D(std::string name);
			
			/**
			 * \brief Sets a setting to a new value.
			 * \param name Name of the setting
			 * \param value New value
			 */
			void setString(std::string name, std::string value);
			/**
			 * \brief Sets a setting to a new value.
			 * \param name Name of the setting
			 * \param value New value
			 */
			void setBool(std::string name, bool value);
			/**
			 * \brief Sets a setting to a new value.
			 * \param name Name of the setting
			 * \param value New value
			 */
			void setInt(std::string name, int value);
			/**
			 * \brief Sets a setting to new value
			 * \param name Name of the setting
			 * \param value New value
			 */
			void setVector2D(std::string name, Vector2D value);
			/**
			 * \brief Sets a setting to a new value
			 * \param name Name of the setting
			 * \param value New value
			 */
			void setVector2DI(std::string name, Vector2DI value);
			/**
			 * \brief Sets a setting to a new value
			 * \param name Name of the setting
			 * \param value New value
			 */
			void setVector3D(std::string name, Vector3D value);
			/**
			 * \brief Returns a list with all items in a section.
			 * \param section Name of the section
			 * \return Returns a vector with the names of the items.
			 */
			std::vector<std::string> getItemList(std::string section);
			
			/**
			 * \brief Deletes a setting.
			 * \param name Name of the setting
			 */
			void deleteSetting(std::string name);
			/**
			 * \brief Returns whether a setting exists.
			 * \param name Name of the setting
			 */
			bool doesExist(std::string name);
			//tolua_end
		private:
			SettingsManager();
			
			IniFile file;
			//tolua_begin
	};
}
//tolua_end

#endif

