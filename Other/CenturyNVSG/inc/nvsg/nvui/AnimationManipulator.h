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
#include "nvsg/CoreTypes.h"

namespace nvui
{  
  /*! \brief This Manipulator controls the camera animation.
   *  \par Namespace: nvui
   *  \remarks This Manipulator is a special Manipulator that sets the animation data (frame number)
   *  in the current nvsg::ViewState and nvsg::Camera objects. 
   *  \n
   *  If you use this manipulator, your application must use a mechanism
   *  such as using OnIdle() that calls updateFrame(). If updateFrame() returns true,
   *  you should call triggerRedraw of the render area so the updated frame will be drawn.
   *  You will find some code samples in the description of the Manipulator class. Also, 
   *  the SimpleViewer tutorial source code shows how to use a manipulator with MFC, 
   *  the ConsoleViewer tutorial source code shows the details on how to use a manipulator, and 
   *  the ManipSampleViewer tutorial source code shows how to derive a new Manipulator class.
   *  \sa Manipulator*/
  class AnimationManipulator : public Manipulator 
  {
    public:
      /*! \brief Default constructor.
       *  \remarks Initialize the AnimationManipulator object.*/
      NVSG_API AnimationManipulator();

      /*! \brief Default Destructor 
       *  \remarks The destructor cleans up the AnimationManipulator object and takes care of 
       *  incrementing the reference count of the nvsg::Animation<nvmath::Trafo> object. */
      NVSG_API virtual ~AnimationManipulator();

      /*! \brief Apply changes for the next frame.
       *  \return If there is a change regarding the last frame this function returns true.
       *  \remarks This function updates the nvsg::Camera and the nvsg::ViewState regarding 
       *  m_currentFrame.  The application should call this function 
       *  from your idle handler, for examle. If it returns with a positive result - true - the application 
       *  should call RenderArea::triggerRedraw. 
       *  \par Example
       *  This is a piece of code that demonstrates the usage with MFC. The AnimationManipulator 
       *  needs a method to draw constantly. In MFC - OnIdle() it is enough to return TRUE.
       *  The application can also use a kind of animation thread to provide continuous redraw.
       *  \code
       *  m_pAnimManip->nextFrame();
       *  if (m_pAnimManip->updateFrame())
       *  {
       *    m_pRA->triggerRedraw();   
       *  }
       *  return TRUE;  // Return TRUE, continues redraw - we are not done
       *                // we need to draw the next frame.
       *                // See the return values of OnIdle() in the MSDN documentation
       *  \endcode
       *  See also the sample code in the Manipulator class description and the ManipSampleViewer, 
       *  SimpleViewer and ConsoleViewer tutorial source code.
       *  \sa nextFrame, previousFrame, firstFrame, lastFrame, gotoFrame, 
       *  RenderArea::triggerRedraw */
      NVSG_API virtual bool updateFrame(); 

      /*! \brief Give feedback of the activeness of the manipulator.
       *  \return This Manipulator returns false all the time. 
       *  \remarks The animation manipulator is not an interactive manipulator since there is 
       *  no direct user interaction via an input device. So it is considered to be inactive 
       *  all the time. */
      NVSG_API virtual bool isActive() { return false; };

      /*! \brief Set the number of animation frames.
       *  \param frames The total number of animation frames. 
       *  \remarks Set here the total number of animation frames. Even when there is no camera 
       *  animation set via setCameraAnimation the scene could contain other kinds of animations. 
       *  So no camera animation is not a hint to set this value to NULL.
       *  \par Example
       *  \code
       *  m_pAnimationManip = new AnimationManipulator;
       *  m_pAnimationManip->setRenderArea(m_pRA);
       * 
       *  // We need to ask the scene for the complete numbers of frames.
       *  // Do not forget - there could be other animations even when there is no camera animation!
       *  m_pAnimationManip->setNumberOfFrames(GetDocument()->getScene()->getNumberOfFrames());
       *
       *  // could be NULL, if there is no camera animation
       *  m_pAnimationManip->setCameraAnimation(GetDocument()->getScene()->getCameraAnimation()); 
       *  \endcode */
      NVSG_API void setNumberOfFrames( size_t frames );
      
      /*! \brief Set the camera animation. 
       *  \param animation Pointer to an Animation object. NULL is a valid value to reset the 
       *  camera animation.
       *  \remarks This function sets the current camera animation. If there is already a valid 
       *  camera animation defined in this Manipulator the reference count of the old 
       *  camera animation object will be decremented, and the reference count of the new 
       *  camera animation object will be incremented.\n
       *  One can use NULL to force the Manipulator to not use any camera animation.
       *  \par Example
       *  See sample code: setNumberOfFrames
       *  \sa nvsg::Animation */
      NVSG_API void setCameraAnimation( nvsg::TrafoAnimationHandle * animation ); 

      /*! \brief Jump to the next animation frame. 
       *  \remarks Simply set the counter to the next frame in the row. 
       *  If the last frame is reached and this function is called, the frame counter will 
       *  be set back to the first frame of the animation. So the application can loop 
       *  through the animation by simply calling nextFrame.
       *  \sa previousFrame, firstFrame, lastFrame, gotoFrame */
      NVSG_API void nextFrame();

      /*! \brief Jump to the previous animation frame. 
       *  \remarks Simply set the counter to the previous frame in the row. 
       *  If the first frame is reached and this function is called, the frame counter will 
       *  be set back to the last frame of the animation. So the application can loop 
       *  through the animation by simply calling previousFrame.
       *  \sa nextFrame, firstFrame, lastFrame, gotoFrame */
      NVSG_API void previousFrame();
      
      /*! \brief Jump to the first animation frame. 
       *  \remarks Simply set the counter to the first frame of the animation. 
       *  \sa nextFrame, previousFrame, lastFrame, gotoFrame */
      NVSG_API void firstFrame();

      /*! \brief Jump to the last animation frame. 
       *  \remarks Simply set the counter to the last frame of the animation. 
       *  \sa nextFrame, previousFrame, lastFrame, gotoFrame */
      NVSG_API void lastFrame();

      /*! \brief Jump to the specified animation frame. 
       *  \param frame Null based index of the animation frame. (0 <= frame < numberOfFrames)
       *  The application has to make sure that the frame index is valid. 
       *  If the parameter is an invalid value, the behavior of 
       *  this function is not defined.
       *  \remarks Simply set the frame index to the specified frame number.
       *  \sa nextFrame, previousFrame, firstFrame, lastFrame */
      NVSG_API void gotoFrame(size_t frame);
      
    protected:
      // we have different kinds of animations: camera animation, 
      // skin / bone animation transformation animation ...
      nvsg::TrafoAnimationHandle  * m_cameraAnimation;  //!< Camera animation information.
      
      size_t m_currentFrame;       //!< Current animation frame.
      size_t m_numberOfFrames;     //!< Maximum number of animation frames.
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  inline void AnimationManipulator::gotoFrame( size_t frame )
  {
    NVSG_TRACE();
    
    NVSG_ASSERT(frame < m_numberOfFrames);
    m_currentFrame = frame;
  }

  inline void AnimationManipulator::nextFrame()
  {
    NVSG_TRACE();
    if (m_currentFrame == 0xFFFFFFFF)
    { 
      firstFrame();
    }
    else
    {
      m_currentFrame = ((++m_currentFrame) % m_numberOfFrames) ? m_currentFrame : 0;
    }
  }

  inline void AnimationManipulator::previousFrame()
  {
    NVSG_TRACE();
    if (m_currentFrame == 0xFFFFFFFF)
    { 
      lastFrame();
    }
    else
    {
      m_currentFrame = ((--m_currentFrame) < 0 ) ? m_numberOfFrames : m_currentFrame;
    }
  }

  inline void AnimationManipulator::firstFrame()
  {
    NVSG_TRACE();
    (m_numberOfFrames > 0) ? m_currentFrame = 0 : m_currentFrame = 0xFFFFFFFF;
  }

  inline void AnimationManipulator::lastFrame()
  {
    NVSG_TRACE();
    (m_numberOfFrames > 0) ? m_currentFrame = m_numberOfFrames-1 : m_currentFrame = 0xFFFFFFFF;
  }

  inline void AnimationManipulator::setNumberOfFrames(size_t frames)
  {
    NVSG_TRACE();
    if (frames == 0)
    {
      m_currentFrame = 0xFFFFFFFF;
    }
    m_numberOfFrames = frames;
  }

} // namespace nvui
