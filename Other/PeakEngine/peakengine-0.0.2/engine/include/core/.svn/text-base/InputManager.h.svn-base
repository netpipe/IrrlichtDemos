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

#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include <string>
#include <map>

#include "core/Keycode.h"
#include "core/GameObject.h"

//tolua_begin
namespace peak
{
	class Event;
	
	/**
	 * \brief Class managing input events and translating keys to actions
	 *
	 * The events are sent by the event receiver of the graphics engine, the
	 * InputManager then looks whether keys are associated to an action and
	 * sends either mouse movement or actions to Game.
	 *
	 * The actions are defined using the SettingsManager using the category
	 * "actions".
	 *
	 * Example:
	 * \code
	 * [actions]
	 * forward = UP
	 * back = DOWN
	 * \endcode
	 */
	class InputManager : public GameObject
	{
		public:
			/**
			 * \brief Returns pointer to the InputManager class.
			 *
			 * \return Pointer to the input manager
			 */
			static InputManager *get(void);
			
			/**
			 * \brief Initializes the input manager
			 */
			bool init(void);
			/**
			 * \brief Closes the input manager
			 */
			bool shutdown(void);
			
			/**
			 * \brief Injects a keyboard event
			 * \param code Code of the key which caused the event
			 * \param pressed If set to true, the key was pressed, if set to
			 * false, it was released.
			 */
			void keyboardEvent(Keycode code, bool pressed);
			
			/**
			 * \brief Injects a mouse movement event
			 * \param x Relative movement on the X-axis
			 * \param y Relative movement on the Y-axis
			 */
			void mouseMoved(float x, float y);
			
			/**
			 * \brief Sets whether input is enabled
			 *
			 * This is mostly only used internally.
			 * \param enabled If set to true, input is enabled
			 */
			void setInputEnabled(bool enabled);
			/**
			 * \brief Returns whether input is enabled
			 */
			bool getInputEnabled(void);
		private:
			//tolua_end
			InputManager();
			~InputManager();
			
			std::map<Keycode, std::string> actions;
			bool enabled;
			
			Event *keychanged;
			Event *keypressed;
			Event *keyreleased;
			Event *mousemoved;
			//tolua_begin
	};

}
//tolua_end
#endif /*INPUTMANAGER_H_*/
