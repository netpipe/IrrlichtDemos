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
  /*! \brief Mouse input helper class to store mouse events.
   *  \par Namespace: nvui
   *  \remarks This class can be used to help managing simple mouse button press, release, and 
   *  motion events. 
   *  It a simple container that can be used inside of, for example, a Manipulator class that uses mouse  
   *  button and motion events to control its functionality.\n
   *  Whenever a mouse button is pressed the Manipulator stores the code and mouse position for 
   *  this button in this container and removes the code and position when the 
   *  button is released. The button state represents the concatenation of the button codes 
   *  of the currently pressed buttons on the mouse.\n
   *  The MouseInput class also helps also to keep track of the mouse motion.\n   
   *  A simple enum defines the button codes - It makes sense to choose the codes to allow 
   *  combinations of different buttons at the same time.
   *  \code
   *  enum
   *  { _NO_KEY           = 0x0000 
   *  , _LEFT_MOUSE_BTN   = 0x0001 
   *  , _MIDDLE_MOUSE_BTN = 0x0002 
   *  , _RIGHT_MOUSE_BTN  = 0x0004 
   *  };
   *  \endcode
   *  \n\n
   *  Have a look at the ManipSampleViewer tutorial on how to use this class.
   */
  class MouseInput
  {
  public:
    /*! \brief Default constructor.
     *  \remarks The constructor simply resets the button state and mouse positions.
     *  \sa resetButtonState, resetPositions */
    NVSG_API MouseInput();
    
    /*! \brief Default destructor. */
    NVSG_API virtual ~MouseInput();
 
    /*! \brief Reset the current mouse button state.
     *  \remarks This function resets all the information on currently pressed mouse buttons.
     *  Default: NULL */
    void resetButtonState();

    /*! \brief Reset the current mouse position information.
     *  \remarks This function resets all the information about mouse positions.
     *  Default value: (0,0)*/
    void resetPositions();

    /*! \brief  Determine the current mouse button state.
     *  \return The concatenation of the current mouse button codes. 
     *  \remarks This function can be used to get information on the currently pressed mouse buttons.
     *  \par Example
     *  Extract the mouse button codes: 
     *  \code
     *  UINT DOLLY = _LEFT_MOUSE_BTN | _MIDDLE_MOUSE_BTN; 
     *  [...] 
     *  switch (m_mouseInput.getButtonState())
     *  {
     *    case DOLLY:
     *      [...]
     *      break;
     *    [...]
     *  }
     *  \endcode
     *  \sa mouseButtonDown, mouseButtonUp */
    UINT getButtonState() const;

    /*! \brief Handle mouse button press events. 
     *  \param btn The mouse button code (or concatenation) of the pressed button(s).
     *  \param x Current x-position of the mouse cursor in window coordinates.
     *  \param y Current y-position of the mouse cursor in window coordinates.
     *  \remarks The application or manipulator can use this function to inform the MouseInput object 
     *  about pressed mouse buttons by providing the button code of the newly pressed mouse button
     *  and the current mouse cursor position in window coordinates. 
     *  (lastPosition = startPosition = currentPosition)\n
     *  The button code is defined by the application 
     *  or Manipulator. See the description of MouseInput for button codes.
     *  \sa mouseButtonUp, getButtonState, MouseInput*/
    NVSG_API void mouseButtonDown( UINT btn, int x, int y );

    /*! \brief Handle mouse button release events. 
     *  \param btn The mouse button code (or concatenation) of the released button(s).
     *  \param x Current x-position of the mouse cursor in window coordinates.
     *  \param y Current y-position of the mouse cursor in window coordinates.
     *  \remarks The application or manipulator can use this function to inform the MouseInput object 
     *  about released mouse buttons by providing the button code of the newly released mouse button
     *  and the current mouse cursor position in window coordinates.\n
     *  The button code is defined by the application 
     *  or Manipulator. See the description of MouseInput for button codes.
     *  \sa mouseButtonDown, getButtonState, MouseInput */    
    NVSG_API void mouseButtonUp( UINT btn, int x, int y );

    /*! \brief Handle mouse motion.
     *  \param x Current x-position of the mouse cursor in window coordinates.
     *  \param y Current y-position of the mouse cursor in window coordinates.
     *  The application or manipulator must tell the MouseInput object the mouse motion 
     *  events. The MouseInput object stores the current 
     *  mouse position (window coordinates). */
    NVSG_API void mouseMotion( int x, int y ); 

    /*! \brief Flag current mouse motion information as handled by the manipulator.
     *  \remarks The manipulators need the motion delta between every frame, 
     *  not from mouse motion to mouse motion.
     *  Calling this method tells the MouseInput object that the motion data 
     *  was used for the last frame and that a new motion delta needs 
     *  to be tracked for the next frame. (lastPosition = currentPosition)
     *  \sa mouseMotion*/
    NVSG_API void motionHandled();


    /*! \brief Get the x-coordinate of the last and not handled mouse position.
     *  \return X-coordinate of the position in window coordinates.
     *  \remarks The manipulators need the motion delta between every frame, 
     *  not from mouse motion to mouse motion. This function returns only the 
     *  not handled mouse position. The Manipulator needs to call motionHandled to
     *  mark the mouse position as handled. 
     *  \sa getLastY, mouseButtonDown, mouseMotion, motionHandled*/
    NVSG_API int getLastX() const;

    /*! \brief Get the y-coordinate of the last and not handled mouse position.
     *  \return Y-coordinate of the position in window coordinates.
     *  \remarks The manipulators need the motion delta between every frame, 
     *  not from mouse motion to mouse motion. This function returns only the 
     *  not handled mouse position. The Manipulator needs to call motionHandled to
     *  mark the mouse position as handled. 
     *  \sa getLastY, mouseButtonDown, mouseMotion, motionHandled*/
    NVSG_API int getLastY() const;

    /*! \brief Get the x-coordinate of the current mouse position.
     *  \return X-coordinate of the position in window coordinates.
     *  \remarks The current position does not care if it was handled. 
     *  It is simply the current position of the mouse cursor while a button is pressed.
     *  \sa getCurrentY, mouseMotion */
    NVSG_API int getCurrentX() const;

    /*! \brief Get the y-coordinate of the current mouse position.
     *  \return Y-coordinate of the position in window coordinates.
     *  \remarks The current position does not care if it was handled. 
     *  It is simply the current position of the mouse cursor while a button is pressed.
     *  \sa getCurrentX, mouseMotion */
    NVSG_API int getCurrentY() const;

    /*! \brief Get the x-coordinate of the mouse position where a button was pressed.
     *  \return X-coordinate of the position in window coordinates
     *  \remarks The last mouse position represents the cursor position of the mouse 
     *  where the last button was pressed.
     *  \sa getLastY, mouseButtonDown */
    NVSG_API int getStartX() const;

    /*! \brief Get the y-coordinate of the mouse position where a button was pressed.
     *  \return Y-coordinate of the position in window coordinates
     *  \remarks The last mouse position represents the cursor position of the mouse 
     *  where the last button was pressed.
     *  \sa getLastY, mouseButtonDown */
    NVSG_API int getStartY() const;

    /*! \brief Assignment operator.
     *  \param rhs Reference to an MouseInput object.
     *  \return Reference to an MouseInput object.
     *  \remarks Use the assignment operator to easily assign one MouseInput object to 
     *  another one.*/
    NVSG_API MouseInput & operator= ( const MouseInput &rhs );
    
    /*! \brief Equality operator.
     *  \param rhs Reference to an MouseInput object - object to compare with.
     *  \return When the data of both objects are equal, the return value is true.
     *  \remarks Compare the two objects on equality. Internally only the mouse button state mask
     *  containing the concatenated button codes and all the position information are compared.
     *  \sa operator!=() */
    NVSG_API bool operator==( const MouseInput &rhs ) const;

    /*! \brief Inequality operator.
     *  \param rhs Reference to an MouseInput object - object to compare with.
     *  \return When the data of both objects are unequal, the return value is true.
     *  \remarks Compare the two objects on inequality. Internally only the mouse button state mask
     *  containing the concatenated button codes and all the position information are compared.
     *  \sa operator!=() */
    NVSG_API bool operator!=( const MouseInput &rhs ) const;

  protected:
    UINT m_buttonState; //!< Current button mask containing the concatenation of mouse button codes.

    int m_currentX;     //!< Current mouse x-position.
    int m_currentY;     //!< Current mouse y-position.

    int m_lastX;        //!< Last and not handled mouse x-position for motion delta.
    int m_lastY;        //!< Last and not handled mouse y-position for motion delta. 

    int m_startX;       //!< Mouse start x-position where the last button was pressed.
    int m_startY;       //!< Mouse start y-position where the last button was pressed.
  };

  inline int MouseInput::getLastX() const
  {
    NVSG_TRACE();
    return m_lastX;
  }

  inline int MouseInput::getLastY() const
  {
    NVSG_TRACE();
    return m_lastY;
  }

  inline int MouseInput::getCurrentX() const
  {
    NVSG_TRACE();
    return m_currentX;
  }

  inline int MouseInput::getCurrentY() const
  {
    NVSG_TRACE();
    return m_currentY;
  }

  inline int MouseInput::getStartX() const
  {
    NVSG_TRACE();
    return m_startX;
  }

  inline int MouseInput::getStartY() const
  {
    NVSG_TRACE();
    return m_startY;
  }

  inline void MouseInput::resetPositions()
  {
    m_lastX = m_lastY = m_currentX = m_currentY = m_startX = m_startY = 0;
  }

  inline void MouseInput::resetButtonState()
  {
    NVSG_TRACE();
    m_buttonState = 0;
  }

  inline UINT MouseInput::getButtonState() const
  {
    NVSG_TRACE();
    return( m_buttonState );
  }
} //namespace nvui