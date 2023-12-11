// Copyright NVIDIA Corporation 2002-2005
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once

#include "nvsgcommon.h"

namespace nvui
{
  /*! \brief Keyboard input helper class to store keyboard press and release events.
   *  \par Namespace: nvui
   *  \remarks This class can be used to help managing simple keyboard press and release events. 
   *  It a simple container that can be used inside of a Manipulator class, for example, that uses keyboard  
   *  events to control its functionality.\n
   *  Whenever a key is pressed, the Manipulator stores the code for 
   *  this key in this container and removes the code when the key is released. So the key state 
    * represents the concatenation of the key codes of the currently pressed keys on the keyboard.\n
   *  A simple enum defines the key codes, so it makes sense to choose the codes to allow 
   *  combinations of different keys at the same time.
   *  \code
   *  enum
   *  { _NO_KEY       = 0x0000 
   *  , _CONTROL_KEY  = 0x0001 
   *  , _W_KEY        = 0x0002 
   *  , _S_KEY        = 0x0004 
   *  };
   *  \endcode
   *  \n\n
   *  Refer to the ManipSampleViewer tutorial on how to use this class.
   */
  class KeyInput
  {
  public:
    /*! \brief Default constructor.
     *  \remarks The constructor simply resets the key state.
     *  \sa resetKeyState */
    NVSG_API KeyInput(void);
    /*! \brief Default destructor. */
    NVSG_API virtual ~KeyInput(void);

    /*! \brief  Determine the current keyboard state.
     *  \return The concatenation of the current key codes. 
     *  \remarks This function can be used to get information on the currently pressed keys.
     *  \par Example
     *  Extract the key codes: 
     *  \code
     *  UINT WALK_FORWARD = _CONTROL_KEY | _W_KEY; 
     *  [...] 
     *  switch (m_keyInput.getKeyState())
     *  {
     *    case WALK_FORWARD:
     *      [...]
     *      break;
     *    [...]
     *  }
     *  \endcode
     *  \sa keyDown, keyUp */
    UINT getKeyState( void ) const;

    /*! \brief Reset the current keyboard state.
     *  \remarks This function resets all the information on currently pressed keys.*/
     void resetKeyState( void );

    /*! \brief Handle key press events. 
     *  \param key The key code of the pressed key.
     *  \remarks The application or manipulator can use this function to inform the KeyInput object 
     *  about pressed keys by providing the key code of the newly pressed key.\n
     *  The key code is defined by the application 
     *  or Manipulator. See the description of KeyInput for key codes.
     *  \sa keyUp, getKeyState, KeyInput */
    NVSG_API void keyDown( UINT key );

    /*! \brief Handle key release events. 
     *  \param key The key value of the released key.
     *  \remarks The application or manipulator can use this function to inform the KeyInput object 
     *  about released keys by providing the key code of the newly released key.\n
     *  The key code is defined by the application 
     *  or Manipulator. See the description of KeyInput for key codes.
     *  \sa keyDown, getKeyState, KeyInput */
    NVSG_API void keyUp( UINT key );
    
    /*! \brief Assignment operator.
     *  \param rhs Reference to an KeyInput object.
     *  \return Reference to an KeyInput object.
     *  \remarks Use the assignment operator to easily assign one KeyInput object to 
     *  another one.*/
    NVSG_API KeyInput & operator= ( const KeyInput &rhs );
    
    /*! \brief Equality operator.
     *  \param rhs Reference to an KeyInput object - object to compare with.
     *  \return When the data of both objects are equal, the return value is true.
     *  \remarks Compare the two objects for equality. Internally only, the key state mask
     *  containing the concatenated key codes is compared.
     *  \sa operator!=() */
    NVSG_API bool operator==( const KeyInput &rhs ) const;

    /*! \brief Inequality operator.
     *  \param rhs Reference to an KeyInput object - object to compare with.
     *  \return When the data of both objects are not equal, the return value is true.
     *  \remarks Compare the two objects on inequality. Internally only, the key state mask
     *  containing the concatenated key codes is compared. */
    NVSG_API bool operator!=( const KeyInput &rhs ) const;

  protected:
    UINT m_keyState; //!< Current key state mask containing the concatenation of key codes.
  };

  inline UINT KeyInput::getKeyState( void ) const
  {
    NVSG_TRACE();
    return( m_keyState );
  }

  inline void KeyInput::resetKeyState( void )
  {
    NVSG_TRACE();
    m_keyState = 0;
  }
} //namespace nvui
