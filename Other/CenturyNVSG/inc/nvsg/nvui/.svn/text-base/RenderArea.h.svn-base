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

#include "nvsg/Scene.h"
#include "nvsg/ViewState.h"
#include "nvtraverser/AppTraverser.h"
#include "nvtraverser/CullTraverser.h"
#include "nvtraverser/RenderTraverser.h"
#include "nvutil/ProblemLog.h"

#include <vector>

namespace nvui
{ 
  /*! \brief The RenderArea is the place where the actual drawing takes place.
   *  \par Namespace: nvui
   *  \remarks This class provides a generic interface to a render target view that is
   *  independent of device, hardware, and operating system specifics.\n
   *  By default, this base class is created with an nvtraverser::AppTraverser and a 
   *  nvtraverser::FrustumCullTraverser.
   *  Derived classes may also create a platform specific nvtraverser::RenderTraverser by default.\n
   *  The RenderArea always uses the camera of the given nvsg::ViewState. 
   *  \sa GLWinRenderArea, */    
  class RenderArea
  {
    public:
      /*! \brief Destructor of the RenderArea. 
       *  \remarks If the RenderArea still has references to the given ViewState and/or given 
       *  Scene, the objects will be unreferenced. */
      NVSG_API virtual ~RenderArea();

      /*! \brief Create the RenderArea.
       *  \param shareArea Pointer to a RenderArea to share information with. 
       *  If you do not want to share with another RenderArea, provide NULL.
       *  \return When the function succeeds, the return value is true.
       *  \remarks This function creates and initializes the RenderArea. An nvTraverser::AppTraverser 
       *  and a nvTraverser::FrustumCullTraverser will be created by default.
       *  \n\n
       *  This class does not use the shareArea parameter. Derived classes can use the 
       *  shareArea parameter to share data between the two RenderAreas. An OpenGL RenderArea 
       *  can, for example, call GLShareLists to share display lists, textures, vertex programs, etc.
       *  \sa GLWinRenderArea*/
      NVSG_API virtual bool init( RenderArea * shareArea = NULL );  

      /*! \brief Destroy the render area.
       *  \remarks This function destroys and cleans up the RenderArea. It makes sure that all the
       *  registered nvtraverser::Traverser will be unregistered. Overload this function to clean up  
       *  your derived class and call back into the base class.
       *  \sa GLWinRenderArea */
      NVSG_API virtual void destroy( void );
    
      /*! \brief Renders the very next frame. 
       *  \remarks The function initiates rendering of the very next frame of the entire scene that 
       *  is assigned to the RenderArea. It does this by subsequently applying the assigned 
       *  nvtraverser::AppTraverser, nvtraverser::CullTraverser, and nvtraverser::RenderTraverser.\n 
       *  After this functions returns you optionally can call RenderArea::wasRenderInterrupted to
       *  check whether the last frame was rendered completely or not. 
       *  \n\n
       *  Do not call this function directly. Trigger the RenderArea::renderScene 
       *  by calling RenderArea::triggerRedraw , since 
       *  this call is RenderArea specific. This means that the different overloads will do the right 
       *  things to trigger a redraw with respect to your used window manager. 
       *  For example, with MFC you would call something like InvalidateRect(NULL) from inside 
       *  RenderArea::triggerRedraw to force the framework to enter 
       *  the View::OnDraw function from where you can call RenderArea::renderScene. 
       *  \sa GLWinRenderArea, wasRenderInterrupted */
      NVSG_API virtual void renderScene();  

      /*! \brief Forces interruption of current frame.
       *  \remarks This function interrupts the traversal of its assigned traversers. Note that this
       *  function is meaningful only if called from a different thread as the thread actually 
       *  rendering the scene. The render thread then optionally can call wasRenderInterrupted to 
       *  check whether the frame was entirely rendered or whether the rendering was interrupted. 
       *  \sa wasRenderInterrupted */ 
      NVSG_API void interruptRender();

      /*! \brief Returns whether the last frame was interrupted.
       *  \return The function returns \c true if rendering of the last frame was interrupted.
       *  Otherwise the function returns \c false, to indicate that the frame was entirely rendered.
       *  \remarks This function optionally can be called from the drawing thread, right after
       *  renderScene, to check whether the rendering of the last frame was interrupted or whether
       *  the frame was entirely rendered. 
       *  \sa renderScene, interruptRender */
      NVSG_API bool wasRenderInterrupted() const;
     
      /*! \brief Handle the viewport resizing.
       *  \param width Width of the viewport. Valid for width > 0.f.
       *  \param height Height of the viewport. Valid for height > 0.f.
       *  \remarks Resize the viewport. React on parent window size changes and 
       *  report the new aspect ratio to the camera of the nvsg::ViewState.
       *  \sa GLWinRenderArea::setViewportSize */
      NVSG_API virtual void setViewportSize( size_t width, size_t height );

      /*! \brief Get viewport height.
       *  \return The height of the viewport in pixels. 
       *  \sa getViewportWidth */
      NVSG_API size_t getViewportHeight() const;
      
      /*! \brief Get viewport width.
       *  \return The width of the viewport in pixels. 
       *  \sa getViewportHeight*/
      NVSG_API size_t getViewportWidth() const;

      /*! \brief Get the currently registered nvtraverser::RenderTraverser. 
       *  \return Pointer to the currently registered nvtraverser::RenderTraverser. If there is 
       *  currently no RenderTraverser registered, this function returns NULL.
       *  \remarks nvtraverser::RenderTraverser may be device dependent. So a secure programming  
       *  technique is not to share one nvtraverser::RenderTraverser with another RenderArea. There 
       *  may be exceptions to this rule so see the documentation of the derived 
       *  nvtraverser::RenderTraverser classes for more detailed information on this issue.
       *  \sa setRenderTraverser, nvtraverser::RenderTraverser */
      NVSG_API nvtraverser::RenderTraverser * getRenderTraverser() const;
      
      /*! \brief Set the new nvtraverser::RenderTraverser.
       *  \param tr Pointer to the new nvtraverser::RenderTraverser.
       *  \remarks Register the new nvtraverser::RenderTraverser. The currently registered 
       *  nvtraverser::RenderTraverser will be unreferenced and replaced by the new one. 
       *  If you provide NULL, because the new nvtraverser::RenderTraverser will be unreferenced 
       *  and the RenderArea has no nvtraverser::RenderTraverser registered, this is    
       *  completely valid.
       *  \n\n
       *  nvtraverser::RenderTraverser may be device dependent. So a secure programming technique 
       *  is not to share one nvtraverser::RenderTraverser with another RenderArea. There may be 
       *  exceptions to this rule so see the documentation of the derived nvtraverser::RenderTraverser 
       *  classes for more detailed information on this issue.
       *  \sa getRenderTraverser, nvtraverser::RenderTraverser */
      NVSG_API void setRenderTraverser( nvtraverser::RenderTraverser * tr );
      
      /*! \brief Set the current nvtraverser::AppTraverser.
       *  \param tr Pointer to the new nvtraverser::AppTraverser.
       *  \remarks Register the new nvtraverser::AppTraverser. The currently registered 
       *  nvtraverser::AppTraverser will 
       *  be unreferenced and replaced by the new one. If you provide NULL, because the new 
       *  nvtraverser::AppTraverser will be unreferenced and the RenderArea has 
       *  no nvtraverser::AppTraverser registered, this is   
       *  completely valid. But you should be aware of the fact the the default
       *  nvtraverser::AppTraverser is needed to bring information into the tree that is needed on  
       *  a frame by frame basis e.g. animation information etc. \n
       *  Not providing an nvtraverser::AppTraverser can lead to unexpected rendering results.
       *  \sa getAppTraverser, nvtraverser::AppTraverser */
      NVSG_API void setAppTraverser( nvtraverser::AppTraverser * tr );

      /*! \brief Get the currently registered nvtraverser::AppTraverser. 
       *  \return Pointer to the currently registered nvtraverser::AppTraverser. 
       *  If there is currently no nvtraverser::AppTraverser registered, this function returns NULL.
       *  \remarks Get the currently registered nvtraverser::AppTraverser. 
       *  It is a valid constellation to have no nvtraverser::AppTraverser registered with the 
       *  RenderArea, but you should be aware of the fact the the default nvtraverser::AppTraverser 
       *  is needed to bring information into the tree that 
       *  is needed on a frame by frame basis e.g. animation information etc. \n 
       *  Not providing an nvtraverser::AppTraverser can lead to unexpected rendering results.
       *  \sa setAppTraverser, nvtraverser::AppTraverser */
      NVSG_API nvtraverser::AppTraverser * getAppTraverser() const;

      /*! \brief Add a nvtraverser::CullTraverser to the culling pipeline. 
       *  \param tr Pointer to the new nvtraverser::CullTraverser.
       *  \remarks A RenderArea can have more than one nvtraverser::CullTraverser. You can build 
       *  a type of culling pipeline by stacking together several cull steps. This function appends 
       *  an additional nvtraverser::CullTraverser to the end of the pipeline of this RenderArea. 
       *  \n\n
       *  Internally we increment the reference count of the new nvtraverser::CullTraverser.
       *  \n\n
       *  Do not add the same nvtraverser::CullTraverser more than once to the culling pipeline.
       *  \sa popCullTraverser, getNumberOfCullTraverser, getCullTraverser */
      NVSG_API void pushCullTraverser( nvtraverser::CullTraverser * tr );

      /*! \brief Remove a nvtraverser::CullTraverser from the cull pipeline. 
       *  \remarks This function removes the last nvtraverser::CullTraverser from the end of the 
       *  culling pipeline. The RenderArea also takes care of decrementing the reference count of 
       *  that nvtraverser::CullTraverser.
       *  \sa pushCullTraverser, getNumberOfCullTraverser, getCullTraverser */
      NVSG_API void popCullTraverser();
      
      /*! \brief Get the number of culling stages in the culling pipeline.
       *  \return Number of active culling stages.
       *  \sa pushCullTraverser, popCullTraverser, getCullTraverser */
      NVSG_API size_t getNumberOfCullTraverser() const;

      /*! \brief Get the specified nvtraverser::CullTraverser,
       *  \param position Zero-based position of the nvtraverser::CullTraverser.  
       *  The position must be valid: RenderArea::getNumberOfCullTraverser > position.
       *  \return Pointer to the specified nvtraverser::CullTraverser. 
       *  \remarks Retrieve the nvtraverser::CullTraverser from the specified position of the 
       *  culling pipeline. If you feed this function with an invalid position, the behavior is 
       *  not defined.
       *  \sa pushCullTraverser, popCullTraverser, getNumberOfCullTraverser */
      NVSG_API nvtraverser::CullTraverser * getCullTraverser(size_t position) const;

      /*! \brief Get the current aspect ratio of the RenderArea.
       *  \return The current aspect ratio of the render area. If height or width is not a valid
       *  value, the return value is 0.f.
       *  \remarks This function can be used to retrieve the current aspect ratio of the RenderArea. 
       *  (aspect ratio = width / height). The initial values for height and width is 1.f which 
       *  corresponds to the camera window size definition. 
       *  \sa nvsg::Camera::setWindowSize */
      NVSG_API float getAspectRatio() const;

      /*! \brief Check if stereo is available for the current RenderArea. 
       *  \return true if stereo is available. false indicates that stereo is not available or the 
       *  RenderArea was not initialized and valid.
       *  \remarks The RenderArea must be initialized and valid before you can ask for stereo 
       *  availability.
       *  \sa init*/
      NVSG_API bool isStereoAvailable();

      /*! \brief Request stereo support.
       *  \param stereo Stereo required for this RenderArea.
       *  \remarks Request stereo support for this RenderArea. Call this function prior to the 
       *  RenderArea::init call. So the init call can make sure to create a RenderArea that 
       *  supports stereo. By default, the RenderArea::init call will not request stereo.
       *  \n\n
       *  For rendering stereo images your RenderArea and your nvtraverser::RenderTraverser need 
       *  to support this special feature. To turn stereo rendering on or off you need to call 
       *  nvsg::ViewState::toggleStereo. 
       *  \n\n
       *  If you know that you do not need a RenderArea that supports stereo avoid requesting 
       *  stereo. This avoids wasting resources and rendering performance. 
       *  \sa nvsg::ViewState::toggleStereo */
      NVSG_API void requestStereoSupport( bool stereo );

      /*! \brief Check if hardware full-scene antialiasing (FSAA) is available for the current 
       *  RenderArea. 
       *  \return true if hardware full-scene antialiasing (FSAA) is available. false indicates that 
       *  hardware full-scene antialiasing (FSAA) is not available or the 
       *  RenderArea was not initialized and is not valid.
       *  \remarks The RenderArea must be initialized and valid before you can ask for hardware
       *  full-scene antialiasing (FSAA) availability.
       *  \sa init*/
      NVSG_API bool isHWFSAAAvailable();

      /*! \brief Request hardware full-scene antialiasing (FSAA) support.
       *  \param on Hardware FSAA is required for this RenderArea.
       *  \param numBuffers Hardware FSAA level / number of multisample buffers.
       *  \remarks Request hardware full-scene antialiasing (FSAA) support for this RenderArea. 
       *  Call this function prior to the init call. So the init call can make sure to 
       *  create a RenderArea that supports hardware full-scene antialiasing (FSAA) with the specified 
       *  number of multi sample buffers. By default the RenderArea::init call will not request 
       *  hardware full-scene anti-aliasing.
       *  \n\n
       *  You can combine hardware FSAA with software FSAA. To turn on software FSAA you need to call
       *  nvtraverser::RenderTraverser::setJitterTechnique and nvtraverser::RenderTraverser::setJitter.
       *  \sa nvtraverser::RenderTraverser::setJitterTechnique, 
       *  nvtraverser::RenderTraverser::setJitter */
      NVSG_API void requestHWFSAA( bool on, unsigned int numBuffers );

      /*! \brief Register the Scene and the nvsg::ViewState with this RenderArea. 
       *  \param scene Scene to replace the RenderArea's current Scene. 
       *  Passing in NULL leads to undefined behavior.
       *  \param viewState ViewState to replace the RenderArea's current ViewState. 
       *  Passing in NULL leads to undefined behavior.
       *  \remarks The function replaces the RenderArea's current Scene and ViewState. 
       *  The reference count of the already contained nScene and ViewState will be 
       *  decremented where the reference count of the new Scene and ViewState will 
       *  be incremented.
       *  \sa setViewState, getViewState, setScene, getScene */
      NVSG_API void setSceneData( nvsg::SceneHandle * scene
                                , nvsg::ViewStateHandle * viewState );  
      
      /*! \brief Get the current nvsg::ViewState. 
       *  \return Pointer to a ViewStateHandle specifying the ViewState. 
       *  If no ViewState was registered prior to this call the result will be NULL;
       *  \remarks Get the currently registered ViewState of the RenderArea.
       *  \sa setViewState, setSceneData */
      NVSG_API nvsg::ViewStateHandle * getViewState() const;
            
      /*! \brief Register a nvsg::ViewState. 
       *  \param viewState ViewState to replace the RenderArea's current ViewState. 
       *  Passing in NULL as the new nvsg::ViewState leads to undefined behavior. 
       *  \remarks Register a nvsg::ViewState with the RenderArea. If there was 
       *  already a nvsg::ViewState 
       *  registered, the reference count of the old one will be decremented while the reference 
       *  count of the new one will be incremented. 
       *  \sa getViewState, setSceneData */
      NVSG_API void setViewState( nvsg::ViewStateHandle * viewState );

      /*! \brief Get the current nvsg::Scene. 
       *  \return Pointer to a SceneHandle specifying the nvsg::Scene. 
       *  If no nvsg::Scene was registered prior to this call the result will be NULL;
       *  \remarks Get the currently registered nvsg::Scene of the RenderArea.
       *  \sa setScene, setSceneData */
      NVSG_API nvsg::SceneHandle * getScene() const;
            
      /*! \brief Register a nvsg::Scene. 
       *  \param scene Scene to replace the RenderArea's current Scene. 
       *  Passing in NULL leads to undefined behavior. 
       *  \remarks Register a nvsg::Scene with the RenderArea. If there was already a nvsg::Scene 
       *  registered, the reference count of the old one will be decremented while the reference 
       *  count of the new one will be incremented. 
       *  \sa getScene, setSceneData */
      NVSG_API void setScene( nvsg::SceneHandle * scene );

      /*! \brief Create a unique ID for a RenderArea.
       *  \return A unique ID.
       *  \remarks Create a unique ID to identify a RenderArea. Derived classes can use this for 
       *  their creation.
       *  \par Example
       *  \code
       *  GLMFCRAWnd * ra;
       *  ra = new GLMFCRAWnd());
       *  RECT rect = {0,0,0,0};
       *  ra->Create("STATIC", NULL, WS_CHILD, rect, view, (UINT)RenderArea::createNewRenderAreaID());
       *  ra->init();
       *  \endcode
       *  \sa GLMFCRAWnd */
      NVSG_API static size_t createNewRenderAreaID();
      
      /*! \brief Trigger a redraw of the scene.
       *  \remarks You should overload this function in your derived class to trigger a redraw event.
       *  On Windows / MFC you would fill this function with, for example, InvalidateRect(NULL) to trigger
       *  a call to the OnDraw handler where you then can call renderScene from a well defined place.\n
       *  Do not call drawScene directly from any other place. Use triggerRedraw whenever your 
       *  application should redraw the scene.
       *  \sa renderScene */
      NVSG_API virtual void triggerRedraw( void );                

      /*! \brief Get the problem log. 
       *  \return Reference to the nvutil::ProblemLogList. 
       *  \remarks Get a list of all the problems that popped up during initialization of the 
       *  render area. Check this list after calling init to get an overview of 
       *  features that the render area could not provide. 
       *  \sa nvutil::ProblemLogList */
      NVSG_API const nvutil::ProblemLogList & getProblems();

    protected:      
      /*! \brief Default constructor (protected)
       *  \remarks Hidden default constructor to prevent the application to instantiate this class.*/ 
      NVSG_API RenderArea();
      
      //! Possible features of the RenderArea. (mainly influencing the pixel format - OpenGL)
      enum
      { _UNDEFINED    = 0x00000000  /*!< \brief No special feature requested.*/
      , _STEREO       = 0x00000001  /*!< \brief Stereo requested.*/
      , _MULTISAMPLE  = 0x00000002  /*!< \brief Multisample buffer requested.*/
      };

      std::vector<nvtraverser::CullTraverser *>  m_cullTraverser;  /*!< \brief These traversers cull 
                                                                    *   objects in the tree.*/
      nvtraverser::RenderTraverser             * m_renderTraverser;/*!< \brief This traverser renders /
                                                                    *   displays the tree.*/
      nvtraverser::AppTraverser                * m_appTraverser;   /*!< \brief This traverser applies all 
                                                                    *   pending changes to the tree.*/

      nvsg::SceneHandle * m_scene;            /*!< \brief Pointer to the scene. m_scene could be NULL !! */
      nvsg::ViewStateHandle * m_viewState;    /*!< \brief Pointer to the view specific controls 
                                               *   (camera, animation state,...)*/

      unsigned long       m_features;         /*!< \brief All requested features for this render area 
                                               *   stereo, multisample, ...*/
      UINT                m_numMSBuffers;     /*!< \brief Number of multi sample buffers */
      bool                m_raCreated;        /*!< \brief Signal the Create() status */
      size_t              m_raHeight;         /*!< \brief Height of the render area */
      size_t              m_raWidth;          /*!< \brief Width of the render area */
 
      static size_t       m_idRenderArea;     /*!< \brief The child window ID for the different CWnd derived 
                                               *   render area windows.
                                               *   \remarks Use this ID in the create call 
                                               *   (e.g. GLMFCRAWnd)- see CWnd->Create() -
                                               *   Don't forget to increment the ID before using it.*/
      
      nvutil::ProblemLog m_problemLog;        /*!< \brief All problems during the initialization of the 
                                               *   render area go here.*/

    private:
      bool m_frameInterrupted; // will be evaluated inside renderScene
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline float RenderArea::getAspectRatio() const
  {
    NVSG_ASSERT( m_raHeight > 0 && m_raWidth > 0 );

    if (m_raHeight > 0 && m_raWidth > 0)
    {
      return( (float) m_raWidth / m_raHeight );
    }
    
    return 0.f; // error
  }

  inline nvsg::ViewStateHandle * RenderArea::getViewState() const
  {
    return( m_viewState );
  }

  inline nvsg::SceneHandle * RenderArea::getScene() const
  {
    return( m_scene );
  }

  inline nvtraverser::RenderTraverser * RenderArea::getRenderTraverser() const
  {
    return( m_renderTraverser );
  }

  inline void RenderArea::setRenderTraverser( nvtraverser::RenderTraverser * tr )
  {
    if (m_renderTraverser)
    {
      m_renderTraverser->removeRef();
    }

    m_renderTraverser = tr;
    
    if (m_renderTraverser)
    {
      m_renderTraverser->addRef();
    }
  }

  inline void RenderArea::setAppTraverser(nvtraverser::AppTraverser * tr)
  {
    if (m_appTraverser)
    {
      m_appTraverser->removeRef();
    }

    m_appTraverser = tr;

    if (m_appTraverser)
    {
      m_appTraverser->addRef();
    }
  }

  inline nvtraverser::AppTraverser * RenderArea::getAppTraverser() const
  {
    return m_appTraverser;
  }

  inline void RenderArea::pushCullTraverser(nvtraverser::CullTraverser * tr)
  {
    NVSG_ASSERT(tr);

    m_cullTraverser.push_back(tr);
    tr->addRef();
  }

  inline void RenderArea::popCullTraverser()
  {
    NVSG_ASSERT(!m_cullTraverser.empty());

    m_cullTraverser.back()->removeRef();
    m_cullTraverser.pop_back();
  }
  
  inline size_t RenderArea::getNumberOfCullTraverser() const
  {
    return m_cullTraverser.size();
  }

  inline nvtraverser::CullTraverser * RenderArea::getCullTraverser(size_t position) const
  {
    NVSG_ASSERT(m_cullTraverser.size() > position);

    return m_cullTraverser[position];
  }

  inline size_t RenderArea::createNewRenderAreaID()
  {
    return (++m_idRenderArea);
  }

  inline size_t RenderArea::getViewportHeight() const
  {
    return m_raHeight;  
  }

  inline size_t RenderArea::getViewportWidth() const
  {
    return m_raWidth;
  }

  inline const nvutil::ProblemLogList & RenderArea::getProblems()
  {
    return m_problemLog.getLogList();
  } 

  inline bool RenderArea::wasRenderInterrupted() const
  {
    return m_frameInterrupted;
  }
} //  namespace nvui
