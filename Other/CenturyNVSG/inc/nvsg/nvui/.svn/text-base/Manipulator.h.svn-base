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

namespace nvui
{
  class RenderArea;

  /*! \brief Base class for all kinds of manipulators.
   *  \par Namespace: nvui
   *  \remarks This purely virtual class serves as the base class for all kinds of 
   *  manipulators for user interaction such as controlling the nvsg::Camera, 
   *  nvsg::Transform, etc.\n 
   *  The Manipulator works closely with the RenderArea 
   *  from where the manipulator code can retrieve the current 
   *  nvsg::ViewState, nvsg::Camera etc.
   *  \n\n
   *  If you use a Manipulator, your application must use a mechanism  
   *  (such as using OnIdle() that calls updateFrame()). If updateFrame() returns true,
   *  you should call triggerRedraw of the render area so the updated frame will be drawn.\n
   *  See the SimpleViewer tutorial source code for details on how to use a manipulator with MFC.\n
   *  See the ConsoleViewer tutorial source code for details on how to use a manipulator.
   *  \n\n
   *  The following code snippets show the Manipulator handling in the idle handler of an MFC 
   *  based application.
   *  \n\n  
   *  The doIdleHandling is called from the OnIdle handler of the CWinApp derived class:
   *  \code
   *  BOOL ViewerApp::OnIdle(LONG lCount)
   *  {
   *    BOOL retval  = CWinApp::OnIdle(lCount);
   *    [...]
   *    if (pActiveView)
   *    {
   *      retval |= pActiveView->doIdleHandling();
   *    }
   *    return retval;
   *  }
   *  \endcode
   *  See the Microsoft documentation of CWinApp::doIdle for the return value description.
   *  \n\n
   *  The following code shows the basic steps that are needed to draw the scene:
   *  \code
   *  BOOL ViewerView::doIdleHandling()
   *  {
   *    if (m_pManip->updateFrame())
   *    {
   *      m_pRA->triggerRedraw();
   *    }
   *    return FALSE;  // Return FALSE, no continues redraw - we are done
   *                   // we only want to draw the next frame when there was mouse 
   *                   // movement or such. Some Manipulators want to do continuous redraw
   *                   // like an AnimationManipulator. These manipulators should return TRUE,
   *                   // so OnIdle is called again.
   *  }  
   *  \endcode
   *  \n
   *  Deriving your own Manipulator class is straight forward. Camera manipulators should be derived 
   *  from this base class. Manipulators working on nvsg::Transform nodes should be derived from 
   *  the TransformManipulator class.\n
   *  See the ManipSampleViewer tutorial source code on how to derive a new Manipulator.
   *  \sa TrackballCameraManipulator,  WalkCameraManipulator, FlightCameraManipulator,
   *  AnimationManipulator, TransformManipulator, TrackballTransformManipulator */ 
  class Manipulator
  {
  public:
    /*! \brief Default constructor.
     *  \remarks Initialize the Manipulator object.*/
    NVSG_API Manipulator();
    
    /*! \brief Default destructor. */
    NVSG_API virtual ~Manipulator();

    /*! \brief Assign the current RenderArea to work with. 
     *  \param renderArea Pointer to the RenderArea object. NULL is a valid input.
     *  \remarks The assigned RenderArea will be used to extract nvui::ViewState and nvsg::Camera 
     *  information. */
    NVSG_API void setRenderArea(nvui::RenderArea * renderArea);
    
    /*! \brief Apply changes for the next frame.
     *  \return If there is a change regarding the last frame this function returns true.
     *  \remarks This function updates the objects which are manipulated by the Manipulator.
     *  If the return value is true, the application should trigger a redraw by calling triggerRedraw.
     *  \par Example
     *  See the sample code in the Manipulator class description.
     *  \sa RenderArea::TriggerRedraw */
    NVSG_API virtual bool updateFrame() = 0;

    /*! \brief Give feedback of the activeness of the manipulator.
     *  \return This function returns true when the manipulator is in active mode. 
     *  \remarks All derived manipulators need to implement this method to reflect the 
     *  manipulator state. The manipulator should return true if it is in a special mode 
     *  such as rotate, pan, etc.. */
    NVSG_API virtual bool isActive() = 0;

  protected:
    nvui::RenderArea *  m_renderArea;  /*!< \brief The asssigned RenderArea containing ViewState etc.
                                            \remarks Member could contain a NULL pointer. */
  };

  inline void Manipulator::setRenderArea(nvui::RenderArea * renderArea)
  {
    NVSG_TRACE();
    m_renderArea = renderArea;
  } 

} // namespace nvui

