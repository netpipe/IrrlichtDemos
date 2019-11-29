/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHOENIXERC__
#define __PHOENIXERC__

#include <string>
#include <boost/signals2/signal.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "config.h"
#include "WindowManager.h"
#include "Keys.h"
#include "Vector2d.h"

namespace phoenix
{

    //! Event receiver.
    /*!
		This is a helper class for Client code. This function listens for and tracks Keyboard
		and Mouse events. It provides a simple way to get the states of those events. It is not
		required by the engine and can be excluded. It is a Singleton and must be accessed 
		with Instance().
	*/
    class EventReceiver
    {

    public:

		//! Instance
		/*!
			Gets the singleton instance of an Event Receiver.
			\note A valid instance of WindowManager must exist.
		*/
		inline static boost::shared_ptr<EventReceiver> Instance(){
			if( ! instance ) instance = boost::shared_ptr<EventReceiver>(new EventReceiver());
			return instance;
		}

		//! Destructor
		~EventReceiver(){
			event_connection.disconnect();
		}

		//! On event.
		/*!
			Listening function for events from the WindowManager.
		*/
		void onWindowEvent( const WindowEvent& e );

        //! Gets the state of the given key, true if it is down.
        inline bool getKey(Key _k) { return keys[_k]; }

        //! Checks if a key was just pressed.
        inline bool getKeyPressed(Key _k) { return (keysdown[_k]&&keys[_k]); }

        //! Checks if a key was just released.
        inline bool getKeyReleased(Key _k) { return (keysdown[_k]&&(!keys[_k])); }

		//! Get the current keyboard string.
		/*!
			The event handler keeps track of the characters typed by the user
			via the keyboard. It also tracks the backspace key.
		*/
		inline const std::string& getKeyboardString(){ return keyboardstring; }

		//! Set the current keyboard string.
		inline void setKeyboardString( const std::string& _s ){ keyboardstring = _s; }

        //! Get mouse position.
        /*!
            \returns A vector that represents the mouse's position relative to the top left corner of the window.
        */
        inline const Vector2d& getMousePosition() { return mousepos; }

        //! Get a mouse button state, true if it is down.
        inline bool getMouseButton(MouseButton _b) { return mousebutton[_b]; }

        //! Check if a mouse button was just pressed.
        inline bool getMouseButtonPressed(MouseButton _b) { return mousebutton[_b]&&mousebuttondown[_b]; }

        //! Check if a mouse button was just released.
        inline bool getMouseButtonReleased(MouseButton _b) { return (!mousebutton[_b])&&mousebuttondown[_b]; }

        //! Get mouse wheel position, the amount the wheel was turned on its axis.
        inline int getMouseWheelPosition() { return mousewheelpos; }

	private:

		//! Constructor
        EventReceiver()
			: event_connection(), keyboardstring(), mousepos( Vector2d(0,0) ), mousewheelpos( 0 )
		{
			event_connection = WindowManager::Instance()->listen( boost::bind( &EventReceiver::onWindowEvent, this, _1 ) );
			resetArrays( true );
		};

		//! Singleton instance.
		static boost::shared_ptr<EventReceiver> instance;

		//! Connection with WindowManager
		boost::signals2::connection event_connection;

		//! Resets internal arrays.
		void resetArrays( const bool all = false );

        //! Array to store key state info.
        bool keys[512];

		//! Array to store mouse state info.
        bool mousebutton[16];

        //!array to store info on if a key was just pressed or released.
        bool keysdown[512];

		//!string to store keyboard input
		std::string keyboardstring;

        //!array to store info on if a mouse button was just pressed or released.
        bool mousebuttondown[16];

        //!stores the mouse's position.
        Vector2d mousepos;

        //!stores the mouse's wheel position
        int mousewheelpos;

    };

	typedef boost::shared_ptr<EventReceiver> EventReceiverPtr;

} //namespace phoenix

#endif //__PHOENIXERC__
