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
#include "nvui/KeyInput.h"
#include "nvui/MouseInput.h"
#include "nvmath/Vec3f.h"

namespace nvui
{  
 
  /*! \brief Simulate a walk-through camera-mouse interaction.
   *  \par Namespace: nvui
   *  \remarks This manipulator is a special Manipulator that interprets/converts mouse movement
   *  into walk-through camera movements. The user can simulate walk forward, walk backward, 
   *  increase speed, decrease speed, tilt and translate.\n
   *  \n
   *  <b>Keys & Buttons </b>                         \n
   *  Left mouse button           : Walk forward.    \n
   *  Middle mouse button         : Walk backward.   \n
   *  W-key                       : Increase speed.  \n
   *  S-key                       : Decrease speed.  \n
   *  Ctrl + Left mouse button    : Tilt.            \n
   *  Ctrl + Middle mouse button  : Translate.       \n
   *  \n
   *  When the Manipulator is in walk mode (forward or backward) you can determine the 
   *  walk direction by moving the mouse to the left or right. \n
   *  You can look into the sky or onto the floor by going into the tilt mode. 
   *  Hold down the Ctrl key and the left mouse button at the same time and move the mouse 
   *  forward to look into the sky, backward to look onto the floor, left to make a look to 
   *  the left and right to make a look to the right.\n
   *  To translate the viewing position to another location (e.g. elevate to another 
   *  floor in a building) simply press the Ctrl key and the middle mouse button and move 
   *  the mouse into the direction you want to move the camera.\n
   *  \n
   *  If you use this manipulator your application must use a mechanism,
   *  such as using OnIdle() that calls updateFrame(). If updateFrame() returns true
   *  you should call triggerRedraw of the render area so the updated frame will be drawn.
   *  You will find some code samples in the description of the Manipulator class. Also, 
   *  the SimpleViewer tutorial source code shows how to use a manipulator with MFC,
   *  the ConsoleViewer tutorial source code shows the details on how to use a manipulator and 
   *  the ManipSampleViewer tutorial source code shows how to derive a new Manipulator class.\n
   *  The WalkCameraManipulator needs a kind of continuous redraw to work. See the sample code 
   *  in the documentation on WalkCameraManipulator::updateFrame.
   *  \sa Manipulator*/
  class WalkCameraManipulator : public Manipulator
  {
    public:
      /*! \brief Constructor .
       *  \param screenSizeX Screen x-resolution in pixel - default is 1280 pixels. 
       *  The value must be greater than 0.
       *  \param screenSizeY Screen y-resolution in pixel - default is 1024 pixels. 
       *  The value must be greater than 0.
       *  \remarks Initialize the WalkCameraManipulator object. The screen resolution is used 
       *  for the input sensitivity of the manipulator. The default for the world up direction is
       *  (0.f, 1.f, 0.f) so by default you are walking in the x-z-plane and the ceiling/heaven 
       *  is expected to be in the y-direction. */
      NVSG_API WalkCameraManipulator(size_t screenSizeX = 1280, size_t screenSizeY = 1024);

      /*! \brief Default destructor. */
      NVSG_API ~WalkCameraManipulator();

      //! Keys for walking.
      enum
      { _NO_BTN_KEY         = 0x0000 //!< No button or key pressed.
      , _LEFT_MOUSE_BTN     = 0x0001 //!< Left mouse button.
      , _MIDDLE_MOUSE_BTN   = 0x0002 //!< Middle mouse button.
      , _CONTROL_KEY        = 0x0004 //!< Control key.
      , _W_KEY              = 0x0008 //!< Speed up.
      , _S_KEY              = 0x0010 //!< Slow down.
      };

      /*! \brief Apply changes for the next frame.
       *  \return If there is a change regarding the last frame this function returns true.
       *  \remarks This function updates the nvsg::Camera. The application should call this function 
       *  from, for example, your idle handler. If it returns with a positive result - true - the application 
       *  should call RenderArea::triggerRedraw. 
       *  \par Example
       *  This is a piece of code that demonstrates the usage with MFC. The AnimationManipulator 
       *  needs a method to draw constantly. In MFC - OnIdle() it is enough to return TRUE.
       *  The application can also use a kind of animation thread to provide continuous redraw.
       *  \code
       *  m_pWalkManip->nextFrame();
       *  if (m_pWalkManip->updateFrame())
       *  {
       *    m_pRA->triggerRedraw();   
       *  }
       *  return TRUE;  // Return TRUE, continues redraw - we are not done
       *                // we need to draw the next frame.
       *                // See the return values of OnIdle() in the MSDN documentation
       *  \endcode
       *  See also the sample code in the Manipulator class description and the ManipSampleViewer, 
       *  SimpleViewer and ConsoleViewer tutorial source code.
       *  \sa RenderArea::triggerRedraw */
      NVSG_API virtual bool updateFrame(); 

      /*! \brief Give feedback of the activeness of the Manipulator.
       *  \return This function returns true when the Manipulator is in active mode. 
       *  \remarks The WalkCameraManipulator is active when he is internally in walk, translate or 
       *  tilt mode. */
      NVSG_API virtual bool isActive();

      /*! \brief Test if the Manipulator is in walk mode.
       *  \return This function returns true when the manipulator is in walk forward or 
       *  walk backward mode 
       *  \remarks This function tests all the current key and button states to 
       *  determine if this Manipulator is currently in walk mode.\n
       *  \n
       *  Walk:\n
       *  - Left mouse button (walk forward)
       *  - Middle mouse button (walk backward) */
      NVSG_API virtual bool isWalk();

      /*! \brief Test if the Manipulator is in walk forward mode.
      *  \return This function returns true when the manipulator is in walk forward mode.
      *  \remarks This function tests all the current key and button states to 
      *  determine if this Manipulator is currently in walk forward mode.\n
      *  \n
      *  Walk forward: Left mouse button (walk forward) */
      NVSG_API virtual bool isWalkForward();

      /*! \brief Test if the Manipulator is in walk backward mode.
      *  \return This function returns true when the manipulator is in walk backward mode.
      *  \remarks This function tests all the current key and button states to 
      *  determine if this Manipulator is currently in walk backward mode.\n
      *  \n
      *  Walk backward: Middle mouse button (walk backward) */
      NVSG_API virtual bool isWalkBackward();

      /*! \brief Test if the Manipulator is in translate mode
       *  \return This function returns true when the manipulator is in translate mode
       *  \remarks This function tests all the current key and button states to 
       *  determine if this Manipulator is currently in translate mode.\n
       *  \n
       *  Translate:\n
       *  Ctrl + Middle mouse button \n */
      NVSG_API virtual bool isTranslate();

      /*! \brief Test if the Manipulator is in tilt mode
       *  \return This function returns true when the manipulator is in tilt mode
       *  \remarks This function tests all the current key and button states to 
       *  determine if this Manipulator is currently in tilt mode.\n
       *  \n
       *  Tilt:\n
       *  Ctrl + Left mouse button \n */
      NVSG_API virtual bool isTilt();
      
      /*! \brief Define the up direction of the world. 
       *  \param up The vector that points into the up direction of the world. 
       *  (0,0,0) is an invalid value.
       *  \remarks This function sets the world's up direction. The default up direction is the
       *  (0,1,0)-direction, meaning the y-axis points to the 'sky' and you are walking in 
       *  the x-z-plane. It is a right handed coordinate system.
       * \sa getWorldUpVector*/
      void setWorldUpVector(nvmath::Vec3f up);

      /*! \brief Get the current up vector of the world.
       *  \return The current up vector of the world.
       *  \remarks The world's up vector is the vector that points into direction of the 'sky'.
       *  It is a right handed coordinate system. 
       *  \sa setWorldUpVector*/
      const nvmath::Vec3f& getWorldUpVector();

      /*! \brief Define speed increase factor for walking.
       *  \param factor Speed increase factor - default is 2.f. This factor must not be 0.f!
       *  \remarks The current speed will be multiplied or divided by this factor. So with the
       *  default increase factor of 2.f, the current walking speed will be doubled whenever the 
       *  user accelerates the walking speed by, for example, pressing the W-key or halved when pressing 
       *  the S-key.
       *  \sa getSpeedIncreaseFactor*/
      void setSpeedIncreaseFactor(float factor);
      
      /*! \brief Get the current speed increase factor for walking.
       *  \remarks The current speed will be multiplied or divided by this factor. So with the
       *  default increase factor of 2.f, the current walking speed will be doubled whenever the 
       *  user accelerates the walking speed by, for example, pressing the W-key or halved when pressing 
       *  the S-key.
       *  \sa setSpeedIncreaseFactor*/
      float getSpeedIncreaseFactor();
      
      /*! \brief Define horizontal sensitivity of the mouse.
       *  \param sensitivity Sensitivity factor - default is 1.f. 
       *  \remarks The horizontal sensitivity factor is used to control how 
       *  sensitive the mouse reacts when you are in walk mode and move your mouse to the 
       *  left or right. The larger the factor, the greater the sensitivity and the smaller 
       *  the curve you can walk with a minimum of mouse movement.
       *  \sa getHSensitivity */
      void setHSensitivity(float sensitivity);
      
      /*! \brief Get the horizontal sensitivity of the mouse.
       *  \return The current sensitivity of the mouse. 
       *  \remarks The horizontal sensitivity factor is used to control how 
       *  sensitive the mouse reacts when you are in walk mode and move your mouse to the 
       *  left or right. The larger the factor, the greater the sensitivity and the smaller 
       *  the curve you can walk with a minimum of mouse movement.
       *  \sa setHSensitivity */
      float getHSensitivity();

      /*! \brief Set the walking speed. 
       *  \param speed The walking speed - default is 1.f.
       *  \remarks The application can set the walking speed via this function.
       *  \sa getSpeed */
      void setSpeed(float speed);

      /*! \brief Get the current walking speed.
       *  \return The current walking speed.
       *  \sa setSpeed */
      float getSpeed();

      /*! \brief Set the screen size. 
       *  \param x Screen x-resolution in pixel - default is 1280 pixels.
       *  The value must be grater than 0.
       *  \param y Screen y-resolution in pixel - default is 1024 pixels. 
       *  The value must be grater than 0.
       *  \remarks The application can set the screen resolution of the system. 
       *  The resolution is used inside the WalkCameraManipulator to define the overall 
       *  Manipulator sensitivity. Even with other screen resolutions the default value 
       *  (1280x1024) works quite good. */
      void setScreenSize(UINT x, UINT y);

      /*! \brief Reset the camera manipulator to the default values. 
       *  \remarks This function resets all the internal states to default 
       *  (e.g. speed, speed increase factor, key and button states, etc...)
       *  The screen resolution and the mouse position will not be reseted.\n
       *  This function does not reset the mouse position since the mouse position will be updated
       *  via the mouseMotion function all the time. If you reset the mouse position manually you will  
       *  see undesirable camera jumps. */
      NVSG_API void reset();

      /*! \brief Handle key press events. 
       *  \param key The key code of the pressed key. This Manipulator accepts only the 
       *  _CONTROL_KEY, _W_KEY and _S_KEY. Other key codes will lead to undefined behavior.
       *  \remarks This function routes the key codes to the KeyInput class so the manipulator can 
       *  react on keyboard input.
       *  \sa keyUp, KeyInput::keyDown */
      NVSG_API void keyDown(UINT key);

      /*! \brief Handle key release events. 
       *  \param key The key code of the released key. This Manipulator accepts only the 
       *  _CONTROL_KEY, _W_KEY and _S_KEY. Other key codes will lead to undefined behavior.
       *  \remarks This function routes the key codes to the KeyInput class so the manipulator can 
       *  react on keyboard input.
       *  \sa keyDown, KeyInput::keyUp*/
      NVSG_API void keyUp(UINT key);
      
      /*! \brief Handle mouse button press events. 
       *  \param btn The mouse button code of the pressed button(s). This 
       *  Manipulator accepts only the _LEFT_MOUSE_BTN and _MIDDLE_MOUSE_BTN. Other button codes will
       *  lead to undefined behavior.
       *  \param x Current x-position of the mouse cursor in window coordinates.
       *  \param y Current y-position of the mouse cursor in window coordinates.
       *  \remarks The Manipulator routes the mouse button code and the current mouse cursor position
       *  to the MouseInput object to store the events. Call this function from your mouse button 
       *  handler of your application.
       *  \sa mouseButtonUp, MouseInput::mouseButtonUp */
      NVSG_API void mouseButtonDown(UINT btn, int x, int y);

      /*! \brief Handle mouse button press events. 
       *  \param btn The mouse button code of the released button(s). This 
       *  Manipulator accepts only the _LEFT_MOUSE_BTN and _MIDDLE_MOUSE_BTN. Other button codes will
       *  lead to undefined behavior.
       *  \param x Current x-position of the mouse cursor in window coordinates.
       *  \param y Current y-position of the mouse cursor in window coordinates.
       *  \remarks The Manipulator routes the mouse button code and the current mouse cursor position
       *  to the MouseInput object to store the events. Call this function from your mouse button 
       *  handler of your application.
       *  \sa mouseButtonDown, MouseInput::mouseButtonDown */
      NVSG_API void mouseButtonUp(UINT btn, int x, int y);

      /*! \brief Handle mouse motion.
       *  \param x Current x-position of the mouse cursor in window coordinates.
       *  \param y Current y-position of the mouse cursor in window coordinates.
       *  \remarks The Manipulator routes the mouse motion to the MouseInput object. Call this 
       *  function from your mouse motion handler of your application.
       *  \sa MouseInput::mouseMotion */
      NVSG_API void mouseMotion(int x, int y);

    protected:
      //! Key/button configuration for walking. (_LEFT_MOUSE_BTN)
      static NVSG_API const UINT WALKFORWARD;
      //! Key/button configuration for walking backward. (_MIDDLE_MOUSE_BTN)
      static NVSG_API const UINT WALKBACKWARD;  
      //! Key/button configuration for translating up, down, left and right. (_CONTROL_KEY | _MIDDLE_MOUSE_BTN)
      static NVSG_API const UINT TRANSLATE;     
      //! Key/button configuration for looking left, right, up and down. (_CONTROL_KEY | _LEFT_MOUSE_BTN)
      static NVSG_API const UINT TILT;          
      //! Key/button configuration for increasing the walk speed. (_W_KEY)
      static NVSG_API const UINT INCREASESPEED;
      //! Key/button configuration for decreasing the walk speed. (_S_KEY)
      static NVSG_API const UINT DECREASESPEED; 
      //! Size of the area where the mouse does not trigger the manipulator. (40 pixels)
      static NVSG_API const UINT AREAOFPEACE;   

      UINT  m_ScreenSizeX; //!< Width of the screen in pixel.
      UINT  m_ScreenSizeY; //!< Height of the screen in pixel.

      nvmath::Vec3f m_worldUpVector;  //!< Up-vector of the world. Default (0,1,0). This vector must not be (0,0,0).
      float m_speedIncreaseFactor;    //!< Factor that influences the stepping. Default: 2.f. This factor must not be 0!
      float m_hSensitivity;           //!< Factor that influences the horizontal rotation. Default: 1.f.
      float m_speed;                  //!< Walk speed. Default: 1.f.

      nvui::MouseInput m_mouseInput;  //!< Track mouse input.
      nvui::KeyInput   m_keyInput;    //!< Track keyboard input.

    protected:
     /*! \brief Do the walk.
      *  \return This function returns true when a redraw is needed. 
      *  \remarks Implementation of the walk functionality.
      *  \sa mouseButtonUp, MouseInput::mouseButtonUp, keyDown, KeyInput::keyUp */
      virtual bool walk();

      /*! \brief Do the translate.
      *  \return This function returns true when a redraw is needed. 
      *  \remarks Implementation of the translate functionality.
      *  \sa mouseButtonUp, MouseInput::mouseButtonUp, keyDown, KeyInput::keyUp */
      virtual bool translate();

      /*! \brief Do the tilt.
      *  \return This function returns true when a redraw is needed. 
      *  \remarks Implementation of the tilt functionality.
      *  \sa mouseButtonUp, MouseInput::mouseButtonUp, keyDown, KeyInput::keyUp */
      virtual bool tilt();
  };

  inline void WalkCameraManipulator::setScreenSize(UINT x, UINT y)
  {
    NVSG_ASSERT(x>0);
    NVSG_ASSERT(y>0);
    m_ScreenSizeX = x;
    m_ScreenSizeX = y;
  }

  inline void WalkCameraManipulator::setWorldUpVector( nvmath::Vec3f up )
  {
    m_worldUpVector = up; 
  }

  inline const nvmath::Vec3f& WalkCameraManipulator::getWorldUpVector()
  {
    return m_worldUpVector; 
  }

  inline void WalkCameraManipulator::setSpeedIncreaseFactor(float factor)
  {
    NVSG_ASSERT(fabs(factor) > FLT_EPSILON);
    m_speedIncreaseFactor = factor; 
  }
  
  inline float WalkCameraManipulator::getSpeedIncreaseFactor()
  {
    return m_speedIncreaseFactor; 
  }

  inline void WalkCameraManipulator::setHSensitivity(float sensitivity)
  {
    m_hSensitivity = sensitivity; 
  }
  
  inline float WalkCameraManipulator::getHSensitivity()
  {
    return m_hSensitivity; 
  }

  inline void WalkCameraManipulator::setSpeed(float speed)
  {
    m_speed = speed; 
  }

  inline float WalkCameraManipulator::getSpeed()
  {
    return m_speed;
  }
} //  namespace nvui
