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
  /*! \brief Simulate flight like camera-mouse interaction.
   *  \par Namespace: nvui
   *  \remarks This manipulator is a special Manipulator that interprets/converts mouse movement
   *  into flight just like camera movements. The user can simulate flying forward, flying backward, 
   *  flying curves, increasing speed, and decreasing speed.\n
   *  \n
   *  <b>Keys & Buttons </b>                  \n
   *  Left mouse button     : Fly forward.    \n
   *  Middle mouse button   : Fly backward.   \n
   *  W-key                 : Increase speed. \n
   *  S-key                 : Decrease speed. \n
   *  \n
   *  When the Manipulator is in flight mode (forward or backward), one can determine the 
   *  flight direction by moving the mouse to the left or right. One can also increase or
   *  decrease the flight speed by pressing the W-key or the S-key on the keyboard.\n
   *  \n
   *  If you use this manipulator your application must use a mechanism,
   *  such as using OnIdle() that calls updateFrame(). If updateFrame() returns true,
   *  you should call triggerRedraw of the render area so the updated frame will be drawn.
   *  You will find some code samples in the description of the Manipulator class. Also 
   *  the SimpleViewer tutorial source code shows how to use a manipulator with MFC,
   *  the ConsoleViewer tutorial source code shows the details on how to use a manipulator, and 
   *  the ManipSampleViewer tutorial source code shows how to derive a new Manipulator class.\n
   *  The FlightCameraManipulator needs a kind of continuous redraw to work. See the sample code 
   *  in the documentation on FlightCameraManipulator::updateFrame.
   *  \sa Manipulator*/
  class FlightCameraManipulator : public Manipulator
  {
    public:
      /*! \brief Constructor .
       *  \param screenSizeX Screen x-resolution in pixel; default is 1280 pixels. 
       *  The value must be greater than 0.
       *  \param screenSizeY Screen y-resolution in pixel; default is 1024 pixels. 
       *  The value must be greater than 0.
       *  \remarks Initialize the FlightCameraManipulator object. The screen resolution is used 
       *  for the input sensitivity of the manipulator. The default for the world up direction is
       *  (0.f, 1.f, 0.f), so by default you are flying in the x-z-plane and the ceiling/heaven 
       *  is expected to be in the y-direction. */
      NVSG_API FlightCameraManipulator(size_t screenSizeX = 1280, size_t screenSizeY = 1024);

      //! Default destructor
      NVSG_API ~FlightCameraManipulator();

      //! Keys for flying.
      enum
      { _NO_BTN_KEY         = 0x0000 //!< No button or key pressed.
      , _LEFT_MOUSE_BTN     = 0x0001 //!< Left mouse button.
      , _MIDDLE_MOUSE_BTN   = 0x0002 //!< Middle mouse button.
      , _W_KEY              = 0x0004 //!< Speed up.
      , _S_KEY              = 0x0008 //!< Slow down.
      };

      /*! \brief Apply changes for the next frame.
       *  \return If there is a change regarding the last frame this function returns true.
       *  \remarks This function updates the nvsg::Camera. The application should call this function 
       *  from your idle handler, for example. If it returns with a positive result - true - the application 
       *  should call RenderArea::triggerRedraw. 
       *  \par Example
       *  This is a piece of code that demonstrates the usage with MFC. The AnimationManipulator 
       *  needs a method to draw constantly. In MFC - OnIdle() it is enough to return TRUE.
       *  The application can also use a kind of animation thread to provide continuous redraw.
       *  \code
       *  m_pFlightManip->nextFrame();
       *  if (m_pFlightManip->updateFrame())
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
       *  \remarks The FlightCameraManipulator is active when he is internally in fly mode. */
      NVSG_API virtual bool isActive();

     /*! \brief Reset the camera manipulator to the default values. 
      *  \remarks This function resets all the internal states to default 
      *  (e.g. speed, speed increase factor, key and button states, etc...)
      *  The screen resolution and the mouse position will not be reset.\n
      *  This function does not reset the mouse position since the mouse position will be updated
      *  via the mouseMotion function all the time. If you reset the mouse position manually, you will  
      *  see undesirable camera jumps. */
      NVSG_API void reset();

      /*! \brief Test if the Manipulator is in fly mode.
       *  \return This function returns true when the manipulator is in fly forward or 
       *  fly backward mode. 
       *  \remarks This function tests all the current key and button states to 
       *  determine if this Manipulator is currently in fly mode.\n
       *  \n
       *  Fly:\n
       *  - Left mouse button (fly forward)
       *  - Middle mouse button (fly backward) */
      NVSG_API virtual bool isFly();

     /*! \brief Test if the Manipulator is in fly forward mode.
      *  \return This function returns true when the manipulator is in fly forward mode.
      *  \remarks This function tests all the current key and button states to 
      *  determine if this Manipulator is currently in fly forward mode.\n
      *  \n
      *  Fly forward: Left mouse button (fly forward) */
      NVSG_API virtual bool isFlyForward();
      
     /*! \brief Test if the Manipulator is in fly backward mode.
      *  \return This function returns true when the manipulator is in fly backward mode.
      *  \remarks This function tests all the current key and button states to 
      *  determine if this Manipulator is currently in fly backward mode.\n
      *  \n
      *  Fly backward: Middle mouse button (fly backward) */     
      NVSG_API virtual bool isFlyBackward();

      /*! \brief Define the up direction of the world. 
       *  \param up The vector that points into the up direction of the world. 
       *  (0,0,0) is an invalid value.
       *  \remarks This function sets the world's up direction. The default up direction is the
       *  (0,1,0)-direction, meaning the y-achsis points to the 'sky' and you are flying in 
       *  the x-z-plane. It is a right handed coordinate system.
       * \sa getWorldUpVector*/
      void setWorldUpVector(nvmath::Vec3f up);
      
      /*! \brief Get the current up vector of the world.
       *  \return The current up vector of the world.
       *  \remarks The world's up vector is the vector that points into direction of the 'sky'.
       *  It is a right handed coordinate system. 
       *  \sa setWorldUpVector*/
      const nvmath::Vec3f& getWorldUpVector();

      /*! \brief Define speed increase factor for flying.
       *  \param factor Speed increase factor - default is 2.f. This factor must not be 0.f!
       *  \remarks The current speed will be multiplied or divided by this factor. So with the
       *  default increase factor of 2.f, the current flying speed will be doubled whenever the 
       *  user accelerates the flying speed by pressing the W-key, for example, or halved when pressing 
       *  the S-key.
       *  \sa getSpeedIncreaseFactor*/
      void setSpeedIncreaseFactor(float factor);
      
      /*! \brief Get the current speed increase factor for flying.
       *  \remarks The current speed will be multiplied or divided by this factor. So with the
       *  default increase factor of 2.f, the current flying speed will be doubled whenever the 
       *  user accelerates the flying speed by pressing the W-key, for example, or halved when pressing 
       *  the S-key.
       *  \sa setSpeedIncreaseFactor*/
      float getSpeedIncreaseFactor();

      /*! \brief Define horizontal sensitivity of the mouse.
       *  \param sensitivity Sensitivity factor - default is 1.f. 
       *  \remarks The horizontal sensitivity factor is used to control how 
       *  sensitive the mouse reacts when you are in fly mode and you move your mouse to the 
       *  left or right. The larger the factor, the greater the sensitivity and the smaller 
       *  the curve you can fly with a minimum of mouse movement.
       *  \sa getHSensitivity */
      void setHSensitivity(float sensitivity);
      
      /*! \brief Get the horizontal sensitivity of the mouse.
       *  \return The current sensitivity of the mouse. 
       *  \remarks The horizontal sensitivity factor is used to control how 
       *  sensitive the mouse reacts when you are in fly mode and move your mouse to the 
       *  left or right. The larger the factor, the greater the sensitivity and the smaller 
       *  the curve you can fly with a minimum of mouse movement.
       *  \sa setHSensitivity */
      float getHSensitivity();

      /*! \brief Define vertical sensitivity of the mouse.
       *  \param sensitivity Sensitivity factor - default is 1.f. 
       *  \remarks The horizontal sensitivity factor is used to control how 
       *  sensitive the mouse reacts when you are in fly mode and move your mouse to the 
       *  left or right. The larger the factor, the greater the sensitivity and the faster
       *  the manipulator simulates the pitch of the virtual aircraft with a minimum of mouse 
       *  movement.
       *  \sa getVSensitivity */
      void setVSensitivity(float sensitivity);

      /*! \brief Get vertical sensitivity of the mouse.
       *  \return The current sensitivity of the mouse. 
       *  \remarks The horizontal sensitivity factor is used to control how 
       *  sensitive the mouse reacts when you are in fly mode and move your mouse to the 
       *  left or right. The larger the factor, the greater the sensitivity and the faster
       *  the manipulator simulates the pitch of the virtual aircraft with a minimum of mouse 
       *  movement.
       *  \sa setVSensitivity */
      float getVSensitivity();
      
      /*! \brief Set the flying speed. 
       *  \param speed The flying speed; default is 1.f.
       *  \remarks The application can set the flying speed via this function.
       *  \sa getSpeed */
      void setSpeed(float speed);

      /*! \brief Get the current flying speed.
       *  \return The current flying speed.
       *  \sa setSpeed */
      float getSpeed();

      /*! \brief Handle key press events. 
       *  \param key The key code of the pressed key. This Manipulator accepts only the 
       *  _W_KEY and _S_KEY. Other key codes will lead to undefined behavior.
       *  \remarks This function routes the key codes to the KeyInput class so the manipulator can 
       *  react on keyboard input.
       *  \sa keyUp, KeyInput::keyDown */
      NVSG_API void keyDown(UINT key);

      /*! \brief Handle key release events. 
       *  \param key The key code of the released key. This Manipulator accepts only the 
       *  _W_KEY and _S_KEY. Other key codes will lead to undefined behavior.
       *  \remarks This function routes the key codes to the KeyInput class so the manipulator can 
       *  react on keyboard input.
       *  \sa keyDown, KeyInput::keyUp*/
      NVSG_API void keyUp( UINT key);

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
       *  function from your mouse motion handler of your application. Call this function only when 
       *  the FlightCameraManipulator is active, meaning in flight mode.
       *  \sa MouseInput::mouseMotion */
      NVSG_API void mouseMotion(int x, int y);

    protected:
      //! Key/button configuration for flying forward (_LEFT_MOUSE_BTN).
      static NVSG_API const UINT FLYFORWARD;     
      //! Key/button configuration for flying backward (_MIDDLE_MOUSE_BTN).
      static NVSG_API const UINT FLYBACKWARD;    
      //! Key/button configuration for increasing speed (_W_KEY).
      static NVSG_API const UINT INCREASESPEED;  
      //! Key/button configuration for decreasing speed (_S_KEY).
      static NVSG_API const UINT DECREASESPEED;  
      //! Size if the area where the mouse does not trigger the manipulator. (40 pixels)
      static NVSG_API const float AREAOFPEACE;   

      UINT  m_ScreenSizeX;    //!< Width of the screen in pixel.
      UINT  m_ScreenSizeY;    //!< Height of the screen in pixel.
      
      nvmath::Vec3f m_worldUpVector;  //!< Up-vector of the world.

      float m_speedIncreaseFactor;  //!< Factor that influences the stepping. Default: 2.0 NOTE: This factor must not be 0!
      float m_hSensitivity;         //!< Factor that influences the horizontal rotation. Default: 1.0
      float m_vSensitivity;         //!< Factor that influences the vertical rotation. Default: 1.0

      float m_speed; //!< //!< Flight speed. Default: 1.f.
      
      nvui::MouseInput m_mouseInput;  //!< Track mouse input.
      nvui::KeyInput   m_keyInput;    //!< Track keyboard input

    protected:
      /*! \brief Do the fly.
      *  \return This function returns true when a redraw is needed. 
      *  \remarks Implementation of the fly functionality.*/
      NVSG_API virtual bool fly();      
  };

  inline void FlightCameraManipulator::setWorldUpVector(nvmath::Vec3f up)
  {
    m_worldUpVector = up; 
  }
  
  inline const nvmath::Vec3f& FlightCameraManipulator::getWorldUpVector()
  {
    return m_worldUpVector; 
  }

  inline void FlightCameraManipulator::setSpeedIncreaseFactor(float factor)
  {
    NVSG_ASSERT(fabs(factor) > FLT_EPSILON);
    m_speedIncreaseFactor = factor; 
  }
  
  inline float FlightCameraManipulator::getSpeedIncreaseFactor()
  {
    return m_speedIncreaseFactor; 
  }

  inline void FlightCameraManipulator::setHSensitivity(float sensitivity)
  {
    m_hSensitivity = sensitivity; 
  }
  
  inline float FlightCameraManipulator::getHSensitivity()
  {
    return m_hSensitivity; 
  }

  inline void FlightCameraManipulator::setVSensitivity(float sensitivity)
  {
    m_vSensitivity = sensitivity; 
  }

  inline float FlightCameraManipulator::getVSensitivity()
  {
    return m_vSensitivity;
  }

  inline void FlightCameraManipulator::setSpeed(float speed)
  {
    m_speed = speed; 
  }

  inline float FlightCameraManipulator::getSpeed()
  {
    return m_speed;
  }

} //  namespace nvui
