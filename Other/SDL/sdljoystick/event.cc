#include <stdlib.h>
#include "event.h"

#include <iostream>
using std::cout;
using std::endl;

/* Input handling init/destroy
 *
 *   Init's the input handler using SDL
 *   Destroys the input handler
 */
SDL_InputEvent::SDL_InputEvent()
{
   int error = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK );
   
   if( error != 0 )
   {
      SDL_Quit();
      isInitialized = false;
      cout<<"SDL failed to initialize"<<endl;
      return;
   }

   SDL_JoystickEventState(SDL_ENABLE);
   joystick = SDL_JoystickOpen(0);
   
   for( int i=0; i<MAX_KEY_COUNT; i++ )
      KeyState[i] = KeyNull;
   
   PovState = JoyPovNull;
   
   for( int i=0; i<MAX_JOY_AXIS; i++)
      JoyAxisState[i] = JoyAxisNull;
   
   for( int i=0; i<MAX_JOY_BUTTON; i++ )
      JoyButtonState[i] = JoyButtonNull;
   
   for( int i=0; i<MAX_MOUSE_AXIS; i++ )
      MouseAxisState[i] = MouseNull;

   for( int i=0; i<MAX_MOUSE_WHEEL; i++ )
      MouseWheelState[i] = MouseWheelNull;

   for( int i=0; i<MAX_MOUSE_BUTTON; i++ )
      MouseButtonState[i] = MouseButtonNull;
   
   MouseButtonCount = 2;
   MouseAxisCount = 2;
   MouseWheelCount = 1;
   JoyButtonCount = SDL_JoystickNumButtons( joystick );
   JoyAxisCount = SDL_JoystickNumAxes( joystick );

   for( int i=0; i<MAX_JOY_AXIS; i++ )
      JoyAxis[i] = 0.0f;
   for( int i=0; i<MAX_MOUSE_AXIS; i++ )
      MouseAxis[i] = 0;
   
   MousePresent = true;
   KeyboardPresent = true;
   JoyPresent = (SDL_NumJoysticks()>0)?true:false;
   if( joystick != NULL )
      JoyPovPresent = (SDL_JoystickNumHats(joystick)>0)?true:false;
   else
      JoyPovPresent = false;
   quitMessage = false;
   
   cout<<"JoyPresent: "<< (char*)((JoyPresent)?"true":"false") <<endl;
   cout<<"JoyPovPresent: "<< (char*)((JoyPovPresent)?"true":"false") <<endl;
   
   isHandled = false;
   isInitialized = true;
   
   cout<<"SDL Initialized"<<endl;
}
SDL_InputEvent::~SDL_InputEvent()
{
   cout<<"SDL De-Initialized"<<endl;
   SDL_Quit();
}

/* Key input handleing
 *
 *   Handles key input
 */
KeyEventType SDL_InputEvent::getKeyState( unsigned char a ) const
{
   if( a < MAX_KEY_COUNT )
      return KeyState[a];
   else
      return KeyNull;
}

/* Joystick input handleing
 *
 *   Handles all Joystick input
 */
JoyPovAxisEventType SDL_InputEvent::getPovState() const
{
   if( JoyPovPresent )
      return PovState;
   else
      return JoyPovNull;
}
JoyAxisEventType SDL_InputEvent::getJoyAxisState( int a ) const
{
   if( JoyPresent && a < JoyAxisCount && a < MAX_JOY_AXIS )
      return JoyAxisState[a];
   else
      return JoyAxisNull;
}
JoyButtonEventType SDL_InputEvent::getJoyButtonState( int a ) const
{
   if( JoyPresent && a < JoyButtonCount && a < MAX_JOY_BUTTON )
      return JoyButtonState[a];
   else
      return JoyButtonNull;
}

/* Mouse input handling
 *
 *   Handles all Mouse input
 */
MouseMoveAxisEventType SDL_InputEvent::getMouseAxisState( int a ) const
{
   if( a < MouseAxisCount && a < MAX_MOUSE_AXIS )
      return MouseAxisState[a];
   else
      return MouseNull;
}
MouseButtonEventType SDL_InputEvent::getMouseButtonState( int a ) const
{
   if( a < MouseButtonCount && a < MAX_MOUSE_BUTTON )
      return MouseButtonState[a];
   else
      return MouseButtonNull;
}
MouseWheelEventType SDL_InputEvent::getMouseWheelState( int a ) const
{
   if( a < MouseWheelCount && a < MAX_MOUSE_WHEEL )
      return MouseWheelState[a];
   else
      return MouseWheelNull;
}

/* Input hardware verifier
 *
 *   Verifies existence of connected hardware
 */
bool SDL_InputEvent::isMouesPresent() const
{
   return MousePresent;
}
bool SDL_InputEvent::isKeyboardPresent() const
{
   return KeyboardPresent;
}
bool SDL_InputEvent::isJoyPresent() const
{
   return JoyPresent;
}
bool SDL_InputEvent::isJoyPovPresent() const
{
   return JoyPovPresent;
}
bool SDL_InputEvent::initialized() const
{
   return isInitialized;
}
bool SDL_InputEvent::quitMessageReceived() const
{
   return quitMessage;
}

/* Button/Axis Verifiers
 *
 *   Verifies existence of button and axis amounts
 */
int SDL_InputEvent::getMouseButtonCount() const
{
   return MouseButtonCount;
}
int SDL_InputEvent::getMouseAxisCount() const
{
   return MouseAxisCount;
}
int SDL_InputEvent::getMouseWheelCount() const
{
   return MouseWheelCount;
}
int SDL_InputEvent::getJoyButtonCount() const
{
   return JoyButtonCount;
}
int SDL_InputEvent::getJoyAxisCount() const
{
   return JoyAxisCount;
}

/* getAxis
 *
 *   returns axis values between -1.0f and 1.0f
 */
float SDL_InputEvent::getJoyAxis( int Axis ) const
{
   if( Axis < JoyAxisCount && Axis < MAX_JOY_AXIS )
      return JoyAxis[Axis];
   else
      return 0.0f;
}
unsigned int SDL_InputEvent::getMouseAxis( int Axis ) const
{
   if( Axis < MouseAxisCount && Axis < MAX_MOUSE_AXIS )
      return MouseAxis[Axis];
   else
      return 0;
}

/* beginEventHandle
 *
 *   Begins input capture sequence
 */
void SDL_InputEvent::beginEventHandle()
{
   for( int i=0; i<MAX_KEY_COUNT; i++ )
      if( KeyState[i] == KeyPressed )
         KeyState[i] = KeyUp;
   for( int i=0; i<MAX_JOY_BUTTON; i++ )
      if( JoyButtonState[i] == JoyButtonPress )
         JoyButtonState[i] = JoyButtonUp;
   
   while( SDL_PollEvent( &event) )
   {
      switch( event.type )
      {
         case SDL_QUIT:
            quitMessage = true;
            continue;
         case SDL_KEYDOWN:
            if( KeyState[ event.key.keysym.sym ] == KeyUp )
               KeyState[ event.key.keysym.sym ] = KeyPressed;
            else
               KeyState[ event.key.keysym.sym ] = KeyDown;
            continue;
         case SDL_KEYUP:
            if( KeyState[ event.key.keysym.sym ] == KeyDown )
               KeyState[ event.key.keysym.sym ] = KeyReleased;
            else
               KeyState[ event.key.keysym.sym ] = KeyUp;
            continue;
         case SDL_JOYBUTTONDOWN:
            if( JoyButtonState[ event.jbutton.button ] == JoyButtonUp )
               JoyButtonState[ event.jbutton.button ] = JoyButtonPress;
            else
               JoyButtonState[ event.jbutton.button ] = JoyButtonDown;
            continue;
         case SDL_JOYBUTTONUP:
            if( JoyButtonState[ event.jbutton.button ] == JoyButtonDown )
               JoyButtonState[ event.jbutton.button ] = JoyButtonRelease;
            else
               JoyButtonState[ event.jbutton.button ] = JoyButtonDown;
            continue;
         case SDL_JOYAXISMOTION:
            /* TODO:
             *   make the cushion value dynamic
             */
            if( event.jaxis.value > 3200 || event.jaxis.value < -3200 )
            {
               if( event.jaxis.value > 3200 )
                  JoyAxisState[ event.jaxis.axis ] = (JoyAxisEventType)(JoyAxisMove | JoyAxisMovePlus);
               if( event.jaxis.value < -3200 )
                  JoyAxisState[ event.jaxis.axis ] = (JoyAxisEventType)(JoyAxisMove | JoyAxisMoveMinus);
               JoyAxis[ event.jaxis.axis ] = (float)event.jaxis.value/(float)32768;
            }
            else
            {
               JoyAxisState[ event.jaxis.axis ] = JoyAxisNull;
               JoyAxis[ event.jaxis.axis ] = 0.0f;
            }
            continue;
         case SDL_JOYHATMOTION:
            PovState = JoyPovNull;
            if( event.jhat.type == SDL_HAT_UP )
               PovState = (JoyPovAxisEventType) (PovState | JoyPovUp);
            if( event.jhat.type == SDL_HAT_RIGHT )
               PovState = (JoyPovAxisEventType) (PovState | JoyPovRight);
            if( event.jhat.type == SDL_HAT_DOWN )
               PovState = (JoyPovAxisEventType) (PovState | JoyPovDown);
            if( event.jhat.type == SDL_HAT_LEFT )
               PovState = (JoyPovAxisEventType) (PovState | JoyPovLeft);
      }
   }
}
