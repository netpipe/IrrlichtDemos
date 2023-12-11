#pragma once

#include "nvsg/nvsgapi.h" // NVSG storage-class defines
#include "nvui/RenderArea.h"
#include "nvgl/GLContext.h"
#include "nvsg/CoreTypes.h"

#include <GL/glx.h>
#include <GL/gl.h>

#include <X11/Xlib.h>



class GLXRenderArea :
  public nvui::RenderArea
{
public:
  GLXRenderArea(void);
  virtual ~GLXRenderArea(void);
  
  // --- The GLXRenderArea interface:

  //! Attach a window to the render area.
  /** You have to create an OpenGL capable window and attach it before calling init(),
      or you can derive from this class to integrate the functionality.  */          
  NVSG_API virtual bool attachWindow( Display* dpy,  //!< The X-Server connection
                                      Window wnd     //!< The window handle of an existing window
                                     );
  //! Create the render area.
  /** This method creates and initializes the render area.
    * \return true - everything went fine
    * \note Create and attach a valid Window before calling this function. */
  NVSG_API virtual bool init( XVisualInfo* vi,               //!< Visual Information structure for the attached window
                              GLXRenderArea * shareArea = NULL  //!< Pointer to a render area with which to share display lists, textures, vertex programs, etc.
                            );
  //! Destroy the render area.
  /** This method destroys and cleans up the render area. */
  NVSG_API virtual void destroy( void );
  
  //! Make OpenGL context current.
  /** This is a more clever way to make the context current. 
    * This method minimizes wglMakeCurrent calls. 
    * \note Use only this method instead of wglMakeCurrent 
    * so we save time. */
  NVSG_API void  makeCurrent( void );

  //! Handle the viewport resizing
  /** Resize the viewport. React on parent window size changes. 
    * Report to OpenGL, to the render window, or to the camera manipulator. */
  NVSG_API virtual void setViewportSize( size_t x  //!< Width of the viewport
                                       , size_t y  //!< Height of the viewport
                                       );

  //! Render the scene.
  /** Do the rendering of the entire scene. (app, cull, draw the whole tree) 
    * If you want to trigger this method, use TriggerRedraw() to force a redraw 
    * of the scene. */
  NVSG_API virtual void renderScene( void );  
  
  //!< Trigger a redraw of the scene
  /** \note Use this method to trigger a redraw 
    * instead of calling the drawing method directly! */
  NVSG_API virtual void triggerRedraw( void );       
  

protected:
    
  GLXContext    m_hglrc;
  Display*      m_dpy;    
  GLXDrawable   m_drawable;  
    
};
