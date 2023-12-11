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
/** \file */

#include "nvsgcommon.h"

#include "nvui/Manipulator.h"
#include "nvutil/Trackball.h"
#include "nvui/MouseInput.h"
#include "nvui/KeyInput.h"

namespace nvui
{ 
  /*! \brief Simulate a trackball-like camera mouse interaction.
   *  \par Namespace: nvui
   *  \remarks This Manipulator is a special Manipulator that interprets/converts
   *  mouse movement into nvsg::Camera changes. You can orbit around the scene, pan the camera,
   *  and dolly in and out.\n
   *  \n
   *  <b>Keys & Buttons </b>                        \n
   *  Left mouse button           : Orbit.          \n
   *  Middle mouse button         : Pan.            \n
   *  Left & Middle mouse button  : Dolly.          \n
   *  Ctrl  + Left mouse button   : Pan.            \n
   *  Shift + Left mouse button   : Dolly.          \n
   *  \n
   *  When holding the x or the y key while orbiting or panning, the x or the y coordinate are
   *  held constant, respectively. Thus orbiting or panning is done with a fixed corresponding
   *  axis.
   *  If you use this manipulator, your application must use a mechanism,
   *  such as using OnIdle() that calls updateFrame(). If updateFrame() returns true
   *  you should call triggerRedraw of the render area so the updated frame will be drawn.
   *  You will find some code samples in the description of the Manipulator class. Also, 
   *  the SimpleViewer tutorial source code shows how to use a manipulator with MFC,
   *  the ConsoleViewer tutorial source code shows the details on how to use a manipulator, and 
   *  the ManipSampleViewer tutorial source code shows how to derive a new Manipulator class.
   *  \sa Manipulator*/
  class TrackballCameraManipulator : public Manipulator
  {
  public:
    /*! \brief Default constructor.
     *  \remarks Initialize the TrackballCameraManipulator object.*/
    NVSG_API TrackballCameraManipulator();

    /*! \brief Default destructor. */
    NVSG_API virtual ~TrackballCameraManipulator();
    
    /*! \brief Button and key definitions.
     *  \remarks These values are used for the button and key state handling. 
     *  \sa KeyInput, MouseInput */
    enum
    { _NO_BTN_KEY         = 0x0000 //!< No button or key pressed.
    , _LEFT_MOUSE_BTN     = 0x0001 //!< Left mouse button pressed.
    , _MIDDLE_MOUSE_BTN   = 0x0002 //!< Middle mouse button pressed.
    , _SHIFT_KEY          = 0x0004 //!< Shift key pressed.
    , _CONTROL_KEY        = 0x0008 //!< Control key pressed.
    , _X_KEY              = 0x0010 //!< x key pressed: lock x axis
    , _Y_KEY              = 0x0020 //!< y key pressed: lock y axis
    , _Z_KEY              = 0x0040 //!< z key pressed
    };

   /*! \brief Apply changes for the next frame.
    *  \return If there is a change regarding the last frame this function returns true.
    *  \remarks This function updates the camera. The application should call this function 
    *  from, for example, your idle handler. If it returns with a positive result - true - the application 
    *  should call RenderArea::triggerRedraw. 
    *  \par Example
    *  \code
    *  if (m_pTrackballCamManip->updateFrame())
    *  {
    *    m_pRA->triggerRedraw();
    *  }
    *  \endcode
    *  See also the sample code in the Manipulator class description and the ManipSampleViewer, 
    *  SimpleViewer and ConsoleViewer tutorial source code.
    *  \sa RenderArea::triggerRedraw */
    NVSG_API virtual bool updateFrame();

    /*! \brief Give feedback of the activeness of the manipulator.
     *  \return This function returns true when the manipulator is in active mode. 
     *  \remarks The TrackballCameraManipulator is active when he is internally in orbit, pan or 
     *  dolly mode. */
    NVSG_API virtual bool isActive();

    /*! \brief Handle key press events. 
     *  \param key The key code of the pressed key. This Manipulator accepts only the 
     *  _SHIFT_KEY and _CONTROL_KEY. Other key codes will lead to undefined behavior.
     *  \remarks This function routes the key codes to the KeyInput class so the manipulator can 
     *  react on keyboard input.
     *  \sa keyUp, KeyInput::keyDown */
    NVSG_API void keyDown( UINT key  );

    /*! \brief Handle key release events. 
     *  \param key The key code of the released key. This Manipulator accepts only the 
     *  _SHIFT_KEY and _CONTROL_KEY. Other key codes will lead to undefined behavior.
     *  \remarks This function routes the key codes to the KeyInput class so the manipulator can 
     *  react on keyboard input.
     *  \sa keyDown, KeyInput::keyUp*/
    NVSG_API void keyUp( UINT key );

    /*! \brief Handle mouse button press events. 
     *  \param btn The mouse button code (or concatenation) of the pressed button(s). This 
     *  Manipulator accepts only the _LEFT_MOUSE_BTN and _MIDDLE_MOUSE_BTN. Other button codes will
     *  lead to undefined behavior.
     *  \param x Current x-position of the mouse cursor in window coordinates.
     *  \param y Current y-position of the mouse cursor in window coordinates.
     *  \remarks The Manipulator routes the mouse button code and the current mouse cursor position
     *  to the MouseInput object to store the events. Call this function from your mouse button 
     *  handler of your application.
     *  \sa mouseButtonUp, MouseInput::mouseButtonUp */
    NVSG_API void mouseButtonDown( UINT btn, int x, int y );

    /*! \brief Handle mouse button press events. 
     *  \param btn The mouse button code (or concatenation) of the released button(s). This 
     *  Manipulator accepts only the _LEFT_MOUSE_BTN and _MIDDLE_MOUSE_BTN. Other button codes will
     *  lead to undefined behavior.
     *  \param x Current x-position of the mouse cursor in window coordinates.
     *  \param y Current y-position of the mouse cursor in window coordinates.
     *  \remarks The Manipulator routes the mouse button code and the current mouse cursor position
     *  to the MouseInput object to store the events. Call this function from your mouse button 
     *  handler of your application.
     *  \sa mouseButtonDown, MouseInput::mouseButtonDown */
    NVSG_API void mouseButtonUp( UINT btn, int x, int y );

    /*! \brief Handle mouse motion.
     *  \param x Current x-position of the mouse cursor in window coordinates.
     *  \param y Current y-position of the mouse cursor in window coordinates.
     *  \remarks The Manipulator routes the mouse motion to the MouseInput object. Call this 
     *  function from your mouse motion handler of your application.
     *  \sa MouseInput::mouseMotion */
    NVSG_API void mouseMotion( int x, int y );

    /*! \brief Reset camera Manipulator object.
     *  \remarks This function resets the key and button states of this Manipulator.
     *  \note This function does not reset the mouse position since the mouse position will be updated
     *  via the mouseMotion function all the time. If you reset the mouse position manually you will  
     *  see undesirable camera jumps.
     *  \sa MouseInput::resetButtonState, KeyInput::resetKeyState */
    NVSG_API void reset();

    /*! \brief Test if the manipulator is in dolly mode. 
     *  \return This function returns true when the Manipulator is in dolly mode. 
     *  \remarks This function tests all the current key and button states to 
     *  determine if this Manipulator is currently in dolly mode.\n
     *  \n
     *  Dolly:\n
     *  - Left & Middle mouse button \n
     *  - Shift + Left mouse button  */
    NVSG_API bool isDolly();
    
    /*! \brief Test if the manipulator is in pan mode.
     *  \return This function returns true when the Manipulator is in pan mode. 
     *  \remarks This function tests all the current key and button states to 
     *  determine if this Manipulator is currently in pan mode. \n
     *  \n
     *  Pan:\n
     *  - Middle mouse button \n
     *  - Ctrl + Left mouse button */
    NVSG_API bool isPan();

    /*! \brief Test if the manipulator is in orbit mode.
     *  \return This function returns true when the Manipulator is in orbit mode. 
     *  \remarks This function tests all the current key and button states to 
     *  determine if this Manipulator is currently in orbit mode. \n
     *  \n
     *  Orbit:\n
     *  - Left mouse button */
    NVSG_API bool isOrbit();

    /*! \brief Test if the manipulator is in rotate mode.
     *  \return This function returns true when the Manipulator is in rotate mode. 
     *  \remarks This function tests all the current key and button states to 
     *  determine if this Manipulator is currently in rotate mode. \n
     *  \n
     *  Orbit:\n
     *  - x key         -> rotate around positive x-axis
     *  - Shift + x key -> rotate around negative x-axis
     *  - y key         -> rotate around positive y-axis
     *  - Shift + y key -> rotate around negative y-axis
     *  - z key         -> rotate around positive z-axis
     *  - Shift + z key -> rotate around negative z-axis */
    NVSG_API bool isRotate();

  protected:
    static NVSG_API const UINT DOLLY1;   //! Button configuration for dolly (_LEFT_MOUSE_BTN | _MIDDLE_MOUSE_BTN).
    static NVSG_API const UINT DOLLY2;   //!< Button configuration for dolly (_LEFT_MOUSE_BTN | _SHIFT_KEY).
    static NVSG_API const UINT PAN1;     //!< Button configuration for pan (_MIDDLE_MOUSE_BTN).
    static NVSG_API const UINT PAN2;     //!< Button configuration for pan (_LEFT_MOUSE_BTN | _CONTROL_KEY).
    static NVSG_API const UINT ORBIT;    //!< Button configuration for orbit (_LEFT_MOUSE_BTN).
    static NVSG_API const UINT ROTATEXP; //!< Button configuration for rotate around positive x-axis.
    static NVSG_API const UINT ROTATEXN; //!< Button configuration for rotate around negative x-axis.
    static NVSG_API const UINT ROTATEYP; //!< Button configuration for rotate around positive y-axis.
    static NVSG_API const UINT ROTATEYN; //!< Button configuration for rotate around negative y-axis.
    static NVSG_API const UINT ROTATEZP; //!< Button configuration for rotate around positive z-axis.
    static NVSG_API const UINT ROTATEZN; //!< Button configuration for rotate around negative z-axis.

    nvui::MouseInput  m_mouseInput;      //!< MouseInput object for tracking mouse events.
    nvui::MouseInput  m_miLast;          //!< Used to compare current and last mouse input.
    nvui::KeyInput    m_keyInput;        //!< KeyInput object for tracking keyboard events.
    nvutil::Trackball m_trackball;       //!< Trackball object that does all the calculations
    nvmath::Vec3f     m_rotationAxis;    //!< Used to rotate
    int               m_trueX, m_trueY;  //!< Used to restart the orbit/pan operation on key down/up

  protected:
    /*! \brief Do the orbit.
     *  \return This function returns true when a redraw is needed. 
     *  \remarks Implementation of the orbit functionality.*/
    virtual bool orbit();

    /*! \brief Do the pan.
     *  \return This function returns true when a redraw is needed. 
     *  \remarks Implementation of the pan functionality.*/
    virtual bool pan();

    /*! \brief Do the dolly.
     *  \return This function returns true when a redraw is needed. 
     *  \remarks Implementation of the dolly functionality.*/
    virtual bool dolly();

    /*! \brief Do the rotate.
     *  \return This function returns true when a redraw is needed. 
     *  \remarks Implementation of the rotate functionality. */
    virtual bool rotate();
  };
} // namespace nvui