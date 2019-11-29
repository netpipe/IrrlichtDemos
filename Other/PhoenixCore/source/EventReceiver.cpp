/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "EventReceiver.h"

using namespace phoenix;

//! Singleton Instance.
boost::shared_ptr<EventReceiver> EventReceiver::instance = boost::shared_ptr<EventReceiver>();

////////////////////////////////////////////////////////////////////////////////
// Resets arrays
////////////////////////////////////////////////////////////////////////////////

void EventReceiver::resetArrays( const bool all ){
	 for (int i=0;i<512;i++)
    {
        if( all == true ) keys[i]=0;
        keysdown[i]=0;
    }

    for (int i=0;i<16;i++)
    {
        if( all == true ) mousebutton[i]=0;
        mousebuttondown[i]=0;
    }
}


////////////////////////////////////////////////////////////////////////////////
// On event
////////////////////////////////////////////////////////////////////////////////
void EventReceiver::onWindowEvent( const WindowEvent& e )
{
	switch( e.type ){

		case WET_UPDATE:
			resetArrays();
			break;

		case WET_KEY:
			{
				if( e.int_data > 15 ) //Keyboard
				{
					if (e.bool_data == true) {
						keysdown[e.int_data]=true;
						keys[e.int_data]=true;

						//backspace for keyboard strings.
						if( e.int_data == PHK_BACKSPACE ){
							keyboardstring = keyboardstring.substr(0, keyboardstring.length() - 1);
						}

					} else {
						keys[e.int_data] = false;
						keysdown[e.int_data] = true;
					}
				}else{ //Mouse
					if ( e.bool_data == true ) {
						mousebutton[ e.int_data ]=true;
						mousebuttondown[ e.int_data ]=true;
					} else {
						mousebutton[ e.int_data ] = false;
						mousebuttondown[ e.int_data ]=true;
					}
				}
			} break;

		case WET_CHAR:
			{
				keyboardstring += e.int_data;
			}break;

		case WET_MOUSE_POSITION:
			{
				mousepos = e.vector_data;
			}break;

		case WET_MOUSE_WHEEL:
			{
				mousewheelpos = e.int_data;
			}

		default:
			break;
	}
}