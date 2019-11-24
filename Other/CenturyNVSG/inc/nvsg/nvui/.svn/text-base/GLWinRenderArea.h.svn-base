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
#ifdef _WIN32

#pragma once
/** \file */

#include "nvsgcommon.h"

#include "nvgl/GLContext.h"
#include "nvui/RenderArea.h"

namespace nvui
{
  /*! \brief RenderArea class for MS Windows OpenGL
   *  \par Namespace: nvui
   *  \remarks This class is responsible for MS Windows specific OpenGL functionality.
   *  It sets up and destroys the OpenGL context, handles the creation, 
   *  pixel format, refresh,  etc. \n
   *  The GLWinRenderArea creates an nvtraverser::GLTraverser as the default 
   *  nvtraverser::RenderTraverser during initialization.\n
   *  The user can request stereo support - quad-buffered OpenGL stereo - and hardware full-scene 
   *  antialiasing. */   
  class GLWinRenderArea : public RenderArea
  {
    public:
      /*! \brief Default constructor.
       */
      NVSG_API GLWinRenderArea(void);
      
      /*! \brief Default destructor.
       */
      NVSG_API virtual ~GLWinRenderArea(void);

      /*! \brief Attach a window to the render area.
       *  \param hWnd The window handle of an existing OpenGL-aware window.
       *  \return When the function succeeds, the return value is true.
       *  \remarks You have to create an OpenGL capable (WS_CLIPCHILDREN | WS_CLIPSIBLINGS) 
       *  window and attach it before calling GLWinRenderArea::init or you can derive from this 
       *  class to integrate the functionality.
       *  \par Example
       *  This sample code demonstrates how to set up a simple Renderer without MFC. 
       *  For more details, see the ConsoleViewer tutorial source code.\n
       *  The Renderer class is derived from GLWinRenderArea. CreateWindow creates a window object
       *  that is used as the RenderAreas OpenGL output window.
       *  \code
       *  bool Renderer::setupRenderArea()
       *  { 
       *    [...]
       *    // Create a simple window for the OpenGL context.
       *    m_hWnd = CreateWindow( "VIEWER" 
       *                         , "NVSG Renderer" 
       *                         , WS_CLIPCHILDREN | WS_CLIPSIBLINGS
       *                         , 100, 100
       *                         , 250, 250
       *                         , NULL
       *                         , NULL
       *                         , m_hInst
       *                         , NULL);
       * 
       *    attachWindow(m_hWnd);
       *    init();
       *    setViewportSize(100, 100);
       *    [...]
       *  }
       *  \endcode
       *  \sa RenderArea::attachWindow, GLMFCRAWnd, GLWinRenderArea */
      NVSG_API virtual bool attachWindow( HWND hWnd );

      /*! \brief Initialize the GLWinRenderArea.
       *  \param shareArea Pointer to a RenderArea to share information with.
       *  If you do not want to share with another RenderArea, provide NULL.
       *  \return When the function succeeds, the return value is true.
       *  \remarks This overload initializes the GLWinRenderArea. 
       *  In addition to the base class implementation it takes care of choosing the right pixel 
       *  format by calling GLWinRenderArea::determinePixelFormat and creating an OpenGL rendering
       *  context. Additionally, it creates an nvtraverser::GLTraverser as 
       *  the nvtraverser::RenderTraverser for scene rendering.
       *  You can derive from this class and overwrite this function if you, for example, want to implement a 
       *  RenderArea that renders into a PBuffer.
       *  \n\n
       *  If the return value is false you can examine the root cause by calling 
       *  RenderArea::getProblems and evaluating the nvutil::ProblemLogList. 
       *  \sa RenderArea::init, determinePixelFormat */
      NVSG_API virtual bool init( RenderArea * shareArea = NULL );
      
      /*! \brief Destroy the render area.
       *  \remarks This function destroys and cleans up the GLWinRenderArea. It calls the base class
       *  implementation RenderArea::destroy to clean up all the registered traversers.
       *  It also destroys the OpenGL context and releases all used resources.
       *  \sa RenderArea::destroy */
      NVSG_API virtual void destroy( void );
      
      /*! \brief Make OpenGL context current.
       *  \remarks This is a more clever way to make the context current. 
       *  This function minimizes wglMakeCurrent calls. 
       *  \n\n 
       *  Do not use wglMakeCurrent. Use makeCurrent since this is optimized for NVSG usage. 
       *  \sa nvgl::WGLMakeCurrent */
      NVSG_API void  makeCurrent( void );

      /*! \brief Handle the viewport resizing
       *  \param x Width of the viewport
       *  \param y Height of the viewport
       *  \remarks Resize the viewport. React on parent window size changes and report 
       *  to OpenGL by setting the GLViewport and call the base class implementation.
       *  \sa RenderArea::setViewportSize */
      NVSG_API virtual void setViewportSize( size_t x, size_t y );

      /*! \brief Share the OpenGL lists.
       *  \param ra Pointer to the GLWinRenderArea to share with.
       *  \return When the function succeeds, the return value is true. 
       *  \remarks Share display lists, textures, vertex programs, etc. 
       *  with another rendering context. It basically makes use of wglShareLists.
       *  \sa nvgl::WGLShareLists */
      NVSG_API bool shareLists( GLWinRenderArea* ra ); 

      /*! \brief Render the scene. 
       *  \remarks Render the entire scene. This overload makes the OpenGL context current, clears 
       *  the depth and the color buffer, calls the registered nvtraverser::AppTraverser, 
       *  nvtraverser::CullTraverser and nvtraverser::RenderTraverser through the base class and
       *  swaps the buffers so the result will be visible.
       *  \n\n
       *  Do not call this function directly. See RenderArea::renderScene for more details on this topic.
       *  \sa RenderArea::renderScene */
      NVSG_API virtual void renderScene( void );  
      
      /*! \brief Trigger a redraw of the scene.
       *  \remarks This overload does the Windows-specific InvalidateRect(NULL) call to trigger
       *  a call to the OnDraw-handler where you then can call GLWinRenderArea::renderScene.
       *  See RenderArea::triggerRedraw for more information on this topic.
       *  \sa RenderArea::triggerRedraw, RenderArea::renderScene */
      NVSG_API virtual void triggerRedraw( void );      

      /*! \brief Turn on HWFSAA.
       *  \remarks If you have requested a multi-sample pixel format, you can turn on HWFSAA.
       *  Be sure to have a multi-sample format by using RenderArea::isHWFSAAAvailable to check it.
       *  \n\n
       *  No redraw is triggered; the caller must call GLWinRenderArea::triggerRedraw.
       *  \n\n
       *  If the GL_NV_multisample_filter_hint extension is available we set it to quality mode. 
       *  \sa disableHWFSAA, isHWFSAAEnabled, isHWFSAAAvailable, requestHWFSAA */
      NVSG_API void enableHWFSAA();

      /*! \brief Turn off HWFSAA.
       *  \remarks If you have requested a multi-sample pixel format you can turn off HWFSAA.
       *  \n\n
       *  No redraw is triggered; the caller must call triggerRedraw.
       *  \sa enableHWFSAA, isHWFSAAEnabled, isHWFSAAAvailable, requestHWFSAA */
      NVSG_API void disableHWFSAA();
      
      /*! \brief Check if hardware accelerated full-scene anti-aliasing (FSAA) is turned on.
       *  \return When hardware accelerated FSAA is tuned on, the return value is true. 
       *  \remarks Determine if hardware accelerated FSAA is turned on or off. 
       *  Make sure to have a multi-sample format by using RenderArea::isHWFSAAAvailable to check it.
       *  \sa enableHWFSAA, isHWFSAAAvailable, requestHWFSAA*/
      NVSG_API bool isHWFSAAEnabled();
      
    protected:
      HGLRC     m_hglrc;  //!< Our OpenGL context
      HDC       m_hdc;    //!< Our device context

      /*! \brief Initialize additional OpenGL features.
       *  \return When the function succeeds, the return value is true.
       *  \remarks This is a handy function that is called from GLWinRenderArea::init. 
       *  Overload this virtual function to extend the initialization of additional OpenGL 
       *  features (e.g. framelock extensions, ...).
       *  \sa init */
      NVSG_API virtual bool initGL( void );

      /*! \brief Internal pixel format chooser.
       *  \return The pixel format index. A return value larger than 0 is valid. 
       *  A NULL or negative value indicates a problem.  
       *  No valid pixel format: 0, no stereo: -1, no FSAA: -2
       *  \remarks Determine the OpenGL pixelformat index with respect to the 
       *  requested features, such as stereo, multisample, etc... \n
       *  We use the WGL_ARB_pixelformat extension to find the appropriate pixel format.
       *  To use this extension we need to create a dummy OpenGL context.
       *  We even check this dummy context for stereo (if needed) and hardware acceleration.
       *  Missing hardware acceleration is a knock out for the render area. 
       *  If we can't get a stereo pixel format for the dummy context we report this back to the 
       *  caller, so init so the caller can restart the pixel format search without 
       *  a stereo request.\n
       *  If we can create the dummy context we check for the WGL_ARB_pixelformat 
       *  extension and try to find a valid pixel format. If WGL_SAMPLE_BUFFERS_ARB (HWFSAA) was
       *  requested but not found, we also report this to the caller and the caller can also 
       *  restart the search without the the HWFSAA request. */
      NVSG_API int determinePixelFormat( void );
    
  private:
      HWND      m_hWindow;      // Our Window handle
};

}

#endif //_WIN32