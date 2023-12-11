#include "GLXRenderArea.h"

#include "nvutil/Assert.h"
#include "nvmath/nvmath.h"
#include "nvmath/Vec3f.h"
#include "nvsg/Material.h"
#include "nvsg/PointLight.h"
#include "nvsg/PerspectiveCamera.h"
#include "nvsg/Transform.h"
#include "nvsg/Node.h"
#include "nvsg/Triangles.h"
#include "nvsg/GeoNode.h"
#include "nvsg/FaceAttribute.h"
#include "nvgl/GLContext.h"
#include "nvsg/Scene.h"
#include "nvtraverser/GLTraverser.h"


#include "nvutil/DbgNew.h" // this must be the last include

using namespace nvgl;
using namespace nvsg;
using namespace nvmath;
using namespace nvutil;
using nvtraverser::GLTraverser;
using nvui::RenderArea;


GLXRenderArea::GLXRenderArea(void)
: m_hglrc(NULL)
, m_dpy(NULL)
, m_drawable(NULL)
{
}

GLXRenderArea::~GLXRenderArea(void)
{
}

bool GLXRenderArea::attachWindow( Display* dpy, Window wnd )
{
  NVSG_ASSERT(wnd && dpy);
  NVSG_ASSERT((m_drawable==NULL) && "There is already a window attached to this render area!");
  
  if(m_drawable)
  {
    return false;
  }  

  m_dpy = dpy;
  m_drawable = (GLXDrawable)wnd;

  return true;
}
                                     
bool GLXRenderArea::init( XVisualInfo* vi, GLXRenderArea * shareArea )
{
  // get our own DC and store it
  NVSG_ASSERT(m_drawable && "Must create/attach a window before calling init()!");

  // init non OGL stuff
  if (RenderArea::init(shareArea))
  {   
    // init OGL specific stuff    
    int screen = DefaultScreen(m_dpy);    
    m_hglrc = GLXCreateContext(m_dpy, vi, (shareArea==NULL) ? NULL : shareArea->m_hglrc, GL_TRUE);

    // init the basic OpenGL stuff
    makeCurrent();
    
    if ( m_hglrc )
    {
      // create traverser render traverser
      NVSG_ASSERT(m_renderTraverser == NULL);
      NVSG_VERIFY(m_renderTraverser = new GLTraverser);
      m_renderTraverser->addRef();
            
      // A successfull creation of the RenderArea also depends on whether 
      // the different traversers have been successfully constructed
      // Only cull and render!
      m_raCreated = !m_cullTraverser.empty() && m_renderTraverser;
    }
    else
    {
      // error with OpenGL initialization
      GLXMakeCurrent(m_dpy, None, NULL);
      GLXDestroyContext(m_dpy, m_hglrc);      
    }
        
  }

  return m_raCreated;
}

void GLXRenderArea::destroy()
{ 
  // destroy non-OGL stuff like traversers
  RenderArea::destroy();

  // We need this for the cleanup of e.g. the render traverser
  makeCurrent();
  
  if (m_renderTraverser)
  {
    m_renderTraverser->removeRef();
    m_renderTraverser = NULL;
  }

  // done with the window
  NVSG_ASSERT(m_drawable && m_hglrc);
  GLXMakeCurrent(m_dpy, None, NULL);
  GLXDestroyContext(m_dpy, m_hglrc);  

  m_hglrc = NULL;  
  m_drawable = NULL;
  m_dpy = NULL;
}

void  GLXRenderArea::makeCurrent( void )
{ // check current context and make current if necessary
  if ( GLXGetCurrentContext() !=  m_hglrc )
  {
    NVSG_VERIFY( GLXMakeCurrent( m_dpy, m_drawable, m_hglrc ) );
  }
}

void GLXRenderArea::setViewportSize( size_t x, size_t y )
{
  // GL viewport
  makeCurrent();
  GLViewport( 0, 0, (GLsizei)x, (GLsizei)y );

  RenderArea::setViewportSize(x, y);
}

void GLXRenderArea::renderScene( void )
{
  NVSG_TRACE()

  // GL specific: Make context current and clear the buffers.
  makeCurrent();

  // clear color and depth buffer
  if (m_scene)
  {
    Vec3f backColor = ReadableObject<Scene>(m_scene)->getBackColor();
    GLClearColor( backColor[0], backColor[1], backColor[2], 0.0f );
  }
  
  GLClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Then initiate cull/app/render traversal through base class.
  RenderArea::renderScene();

  glXSwapBuffers(m_dpy, m_drawable);
}

void GLXRenderArea::triggerRedraw()
{
  RenderArea::triggerRedraw();
    
  NVSG_ASSERT(m_drawable);

  // Throw an expose event into the queue, to have the window redrawn

  XExposeEvent expose;
  expose.type = Expose;
  expose.display = m_dpy;
  expose.window = m_drawable;
  expose.count = 0;

  XSendEvent( m_dpy, m_drawable, True, ExposureMask, (XEvent*)&expose );
}

