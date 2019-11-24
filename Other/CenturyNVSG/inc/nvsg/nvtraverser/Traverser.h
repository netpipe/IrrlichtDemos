// Copyright NVIDIA Corporation 2002-2006
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
#include "nvmath/Vec3f.h"
#include "nvsg/CoreTypes.h" // forward declarations of core type
#include "nvsg/Object.h" // ObjectCode defines
#include "nvsg/VolumeBrick.h"
#include "nvutil/BitMask.h"
#include "nvutil/Conversion.h"
#include "nvutil/MemFunTbl.h"
#include "nvutil/ProblemLog.h"
#include "nvutil/RCObject.h" // base class definition
#include <stack>


namespace nvtraverser
{
  /*! \brief Virtual base class providing the common interface part of ExclusiveTraverser and SharedTraverser.
   *  \remarks A traverser serves as a link between a scene graph and a defined operation to be
   *  performed on the scene graph. That is, according to a certain scene graph's hierarchy, a
   *  traverser iterates over all scene graph components and performs a defined operation for
   *  each by means of an overloadable handler routine.
   *  The Traverser class offers base functionality for all traversers. There are two traversers
   *  derived from Traverser: SharedTraverser and ExclusiveTraverser, that provide read-only
   *  access and read-write access, respectively, to the elements of the scene graph.
   *  \sa ExclusiveTraverser, SharedTraverser */
  class Traverser : public nvutil::RCObject
  {
    public:
      /*! \brief Set a Scene to be used with this Traverser.
       *  \param scene A pointer to the SceneHandle to be used.
       *  \remarks If \a scene is a different pointer than the current Scene pointer, its
       *  reference count is incremented and the reference count of a previously set Scene is
       *  decremented. Any caches for the Scene are marked as invalid.
       *  \sa setViewState */
      NVSG_API void setScene( nvsg::SceneHandle * scene );

      /*! \brief Set a ViewState to be used with this Traverser.
       *  \param viewState A pointer to the ViewStateHandle to be used.
       *  \remarks If \a viewState is a different pointer than the current ViewState pointer, its
       *  reference count is incremented and the reference count of a previously set ViewState is
       *  decremented. Any caches for the ViewState and the cull tree are marked as invalid.
       *  \sa setScene */
      NVSG_API void setViewState( nvsg::ViewStateHandle * viewState );

      /*! \brief Start traversal from a specified node.
       *  \param root A pointer to the NodeHandle to start traversal with.
       *  \remarks If preApply is successful, the Scene and the ViewState are locked, the
       *  traversal is executed by calling doApply, the Scene and the ViewState are unlocked
       *  again, and postApply is called with \a root.
       *  \note Some traverser need a valid ViewState object which has to be set via setViewState.
       *  \sa apply */
      NVSG_API void apply( nvsg::NodeHandle * root );

      /*! \brief Start the traversal of a Scene.
       *  \param scene A pointer to the SceneHandle to traverse.
       *  \remarks This convenience function simply sets the SceneHandle pointer \a scene via
       *  setScene, and initiates the traversal from the root node of \a scene.
       *  \sa setScene, apply */
      NVSG_API void apply( nvsg::SceneHandle * scene );

      /*! \brief Immediately interrupt the current traversal.
       *  \remarks After this function has been called, no more Nodes are visited in the current
       *  traversal. The traversal point is back-tracked to the root without initiating any new
       *  actions.
       *  \note This function is especially useful in a multithreaded environment, where one
       *  thread traverses a Scene while an other thread is allowed to interrupt the traversal
       *  under some circumstances.
       *  \sa wasTraversalInterrupted */
      NVSG_API void interruptTraversal();

      /*! \brief Check for traversal interruption.
       *  \return \c true if the traversal was interrupted, otherwise \c false.
       *  \note This function is especially useful if an operation on a Scene is a complex one,
       *  that needs multiple traversals. If one of those traversals was interrupted, the
       *  following traversals should be omitted.
       *  \sa interruptTraversal */
      NVSG_API bool wasTraversalInterrupted();

    protected:
      /*! \brief Default constructor.
       *  \note The constructor is protected, and hence, a Traverser object cannot be
       *  instantiated directly, but only by deriving from Traverser. */
      NVSG_API Traverser();

      /*! \brief Protected destructor to prevent instantiation. */
      NVSG_API virtual ~Traverser();

      /*! \brief Interface for locking the ViewState.
       *  \remarks This function is called from the framework when the ViewState set by
       *  setViewState is to be locked.
       *  \sa setViewState, unlockViewState */
      NVSG_API virtual void lockViewState() = 0;

      /*! \brief Interface for unlocking the ViewState.
       *  \remarks This function is called from the framework when the ViewState set by
       *  setViewState is to be unlocked. 
       *  \sa setViewState, lockViewState */
      NVSG_API virtual void unlockViewState() = 0;

      /*! \brief Interface for locking the Scene.
       *  \remarks This function is called from the framework when the Scene set by setScene is
       *  to be locked.
       *  \sa setScene, unlockScene */
      NVSG_API virtual void lockScene() = 0;

      /*! \brief Interface for unlocking the Scene.
       *  \remarks This function is called from the framework when the Scene set by setScene is
       *  to be unlocked.
       *  \sa setScene, lockScene */
      NVSG_API virtual void unlockScene() = 0;

      /*! \brief Interface for initiating the traversal.
       *  \param root A pointer to the NodeHandle to start traversal at.
       *  \remarks This function is called from the framework after the Scene and the ViewState,
       *  if set, are locked. The actual traversal is assumed to be done here.
       *  \sa preApply, postApply, lockScene, unlockScene, lockViewState, unlockViewState */
      NVSG_API virtual void doApply( nvsg::NodeHandle * root ) = 0;

      /*! \brief Interface for locking an Object.
       *  \param obj A pointer to the ObjectHandle to lock.
       *  \return A pointer to a constant Object.
       *  \remarks This function is called from the framework on traversing the ObjectHandle \a
       *  obj.
       *  \sa unlockObject, ExclusiveTraverser::lockObject, SharedTraverser::lockObject */
      NVSG_API virtual const nvsg::Object * lockObject( nvsg::ObjectHandle * obj ) = 0;

      /*! \brief Interface for unlocking an Object.
       *  \param obj A pointer to the ObjectHandle to unlock.
       *  \remarks This function is called from the framework right after traversal of \a obj.
       *  \sa lockObject */
      NVSG_API virtual void unlockObject( nvsg::ObjectHandle * obj ) = 0;

      /*! \brief Start traversing an ObjectHandle.
       *  \param obj A pointer to the ObjectHandle to traverse.
       *  \remarks If the traversal was interrupted, the function immediately returns. Otherwise
       *  the Object is locked using lockObject, traversed by calling the corresponding handler
       *  function, and unlocked again using unlockObject.
       *  \note The behavior is undefined if \a obj points to an invalid location.
       *  \sa lockObject, unlockObject, traverseObject */
      NVSG_API void traverseObject( nvsg::ObjectHandle * obj );

      /*! \brief Template to start traversal of an ObjectHandle with one arbitrary argument.
       *  \param obj A pointer to the ObjectHandle to traverse.
       *  \param arg The value to use as the second argument on traversal.
       *  \remarks If the traversal was interrupted, the function immediately returns. Otherwise
       *  the Object is locked using lockObject, traversed by calling the corresponding handler
       *  function, and unlocked again using unlockObject.
       *  \note The behavior is undefined if \a obj points to an invalid location.
       *  \sa lockObject, unlockObject, traverseObject */
      template<typename T> void traverseObject(nvsg::ObjectHandle * obj, T arg);

      /*! \brief Notifier called from the framework directly before traversal starts.
       *  \param root A pointer to NodeHandle to be traversed.
       *  \return The default implementation always return \c true in order to continue traversal
       *  of \a root to proceed.
       *  \remarks Custom traversers might override this notifier to process some pre-traversal
       *  work. A customized preApply notifier should return \c true to force the traversal of
       *  the specified root node to proceed. Returning \c false prevents the specified root node
       *  from being traversed.\n
       *  At the time of call, a custom override can rely on the optional Scene and the optional 
       *  ViewState both _not_ being locked for read or write access by the calling thread. 
       *  \note If the function returns \c false, traversal of \a root is skipped.
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \par Example:
       *  \code
       *    MyTraverser::doApply( nvsg::NodeHandle * root )
       *    {
       *      //  Do some pre-traversal bookholding or whatever
       *      //  ...
       *
       *      //  call the base implementation and return
       *      return( Traverser::preApply( root ) );
       *    }
       *  \endcode
       *  \sa postApply */
      NVSG_API virtual bool preApply( nvsg::NodeHandle * root );

      /*! \brief Notifier called from the framework directly after traversal.
       *  \param root A pointer to a NodeHandle that was just traversed.
       *  \remarks Custom traversers might override this notifier to process some post-traversal
       *  work. It is strongly recommended for custom overrides to call the base implementation
       *  in their code.\n
       *  At the time of call, a custom override can rely on the optional Scene and the optional 
       *  ViewState both _not_ being locked for read or write access by the calling thread. 
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \sa preApply */
      NVSG_API virtual void postApply( nvsg::NodeHandle * root );

      /*! \brief Notifier called from the framework directly before a Group is traversed.
       *  \param grp A pointer to the constant Group to traverse.
       *  \return The default implementation always returns \c true, in order to continue
       *  traversal.
       *  \remarks Custom traversers can override this function for any tasks that need to be
       *  done directly before the Group \a grp is traversed.
       *  \note If the function returns \c false, traversal of \a grp is skipped.
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \sa postTraverseGroup */
      NVSG_API virtual bool preTraverseGroup( const nvsg::Group * grp );

      /*! \brief Notifier called from the framework directly after a Group has been traversed.
       *  \param grp A pointer to the constant Group that was just traversed.
       *  \remarks Custom traversers can override this function for any tasks that need to be
       *  done directly after the Group \a grp has been traversed.
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \sa preTraverseGroup */
      NVSG_API virtual void postTraverseGroup( const nvsg::Group * grp );

      /*! \brief Notifier called from the framework directly before the Drawable objects
       *  associated with one StateSet are traversed.
       *  \param gnode A pointer to the constant GeoNode currently traversed.
       *  \param geomIndex Index of the StateSet in \a gnode associated with the Drawable objects
       *  that are going to be traversed.
       *  \return The default implementation always returns \c true to continue traversal.
       *  \remarks Custom traversers can override this function for any tasks that need to be
       *  done directly before the Drawable objects of the GeoNode \a gnode associated with the
       *  StateSet of \a gnode at index \a geomIndex are traversed.
       *  \note If the function returns \c false, traversal of the specified Drawable objects is
       *  skipped.
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \sa postTraverseDrawables */
      NVSG_API virtual bool preTraverseDrawables( const nvsg::GeoNode* gnode, size_t geomIndex );

      /*! \brief Notifier called from the framework directly after the Drawable objects
       *  associated with one StateSet have been traversed.
       *  \param gnode A pointer to the constant GeoNode currently traversed.
       *  \param geomIndex Index of the StateSet in \a gnode associated with the Drawable objects
       *  that were just traversed.
       *  \remarks Custom traversers can override this function for any tasks that need to be
       *  done directly after the Drawable objects of the GeoNode \a gnode associated with the
       *  StateSet of \a gnode at index \a geomIndex are traversed.
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \sa preTraverseDrawables */
      NVSG_API virtual void postTraverseDrawables( const nvsg::GeoNode* gnode, size_t geomIndex );

      /*! \brief Notifier called from the framework directly before a Map as part of a MapArray
       *  is traversed.
       *  \param ma A pointer to the constant MapArray currently traversed.
       *  \param index The index of the Map in \a ma that is going to be traversed.
       *  \remarks Custom traversers can override this function for any tasks needed directly
       *  before a Map as part of a MapArray is traversed.
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \sa postTraverseMap, preTraverseMap */
      NVSG_API virtual void preTraverseMap( const nvsg::MapArray * ma, size_t index );

      /*! \brief Notifier called from the framework directly after a Map as part of a MapArray
       *  has been traversed.
       *  \param ma A pointer to the constant MapArray currently traversed.
       *  \param index The index of the Map in \a ma that just has been traversed.
       *  \remarks Custom traversers can override this function for any tasks needed directly
       *  after a Map as part of a MapArray is traversed.
       *  \note Not calling the base implementation might result in undefined runtime behavior.
       *  \sa preTraverseMap, postTraverseMap */
      NVSG_API virtual void postTraverseMap( const nvsg::MapArray * ma, size_t index );

      /*! \brief Notifier called from the framework directly before a MapColumn of a MapMatrix is
       *  traversed.
       *  \param mm A pointer to the constant MapMatrix currently traversed.
       *  \param col The index of the column of Map objects that is going to be traversed.
       *  \remarks A MapMatrix is traversed in column-row order.
       *  \sa postTraverserMapColumn */
      NVSG_API virtual void preTraverseMapColumn( const nvsg::MapMatrix * mm, size_t col );

      /*! \brief Notifier called from the framework directly after a MapColumn of a MapMatrix has
       *  been traversed.
       *  \param mm A pointer to the constant MapMatrix currently traversed.
       *  \param col The index of the column of Map objects that has just been traversed.
       *  \remarks A MapMatrix is traversed in column-row order.
       *  \sa preTraverseMapColumn */
      NVSG_API virtual void postTraverseMapColumn( const nvsg::MapMatrix * mm, size_t col );

      /*! \brief Notifier called from the framework directly before a Map as part of a MapMatrix
       *  is traversed.
       *  \param mm A pointer to the constant MapMatrix currently traversed.
       *  \param col The column index of the Map that is going to be traversed.
       *  \param row The row index of the Map that is going to be traversed.
       *  \remarks A MapMatrix is traversed in column-row order.
       *  \sa postTraverseMap, preTraverseMapColumn, postTraverseMapColumn */
      NVSG_API virtual void preTraverseMap( const nvsg::MapMatrix * mm, size_t col, size_t row );

      /*! \brief Notifier called from the framework directly after a Map as part of a MapMatrix
       *  has been traversed.
       *  \param mm A pointer to the constant MapMatrix currently traversed.
       *  \param col The column index of the Map that has just been traversed.
       *  \param row The row index of the Map that has just been traversed.
       *  \remarks A MapMatrix is traversed in column-row order.
       *  \sa preTraverseMap, preTraverseMapColumn, postTraverseMapColumn */
      NVSG_API virtual void postTraverseMap( const nvsg::MapMatrix * mm, size_t col, size_t row );

      /*! \brief Dispatcher to the particular overrides to traverse Group-derived Nodes.
       *  \param grp A pointer to the constant Group-derived Node to traverse.
       *  \remarks If preTraverseGroup() returned true, the lights contained in \a grp are
       *  traversed, and then, depending on the cull state of \a grp and it's actual type a
       *  special traversal is performed. After that, postTraverseGroup() is called.
       *  \sa preTraverseGroup, postTraverseGroup */
      template <typename T> void traverseGroup( const T * grp );

      /*! \brief Traversal of a GeoNode.
       *  \param gnode A pointer to the constant GeoNode to traverse.
       *  \remarks Depending on the cull state of \a gnode, all Drawable objects of the GeoNode,
       *  that are at least partially visible, are traversed.
       *  \sa preTraverseDrawables, postTraverseDrawables */
      NVSG_API void traverseGeoNode( const nvsg::GeoNode * gnode );

      /*! \brief Traversal of a Shape.
       *  \param shape A pointer to the constant Shape to traverse.
       *  \remarks Traverses all PrimitiveSet objects of \a shape. */
      NVSG_API void traverseShape( const nvsg::Shape * shape );

      /*! \brief Traversal of a StateVariant.
       *  \param variant A pointer to the constant StateVariant to traverse.
       *  \remarks Traverses all StatePass objects of \a variant.
       *  \sa traverseStatePass */
      NVSG_API void traverseStateVariant( const nvsg::StateVariant * variant );

      /*! \brief Traversal of a StatePass.
       *  \param pass A pointer to the constant StatePass to traverse.
       *  \remarks Traverses all StateAttribute objects of \a pass.
       *  \sa traverseStateVariant */
      NVSG_API void traverseStatePass( const nvsg::StatePass * pass );

      /*! \brief Traversal of a TextureAttribute.
       *  \param texAttrib A pointer to the constant TextureAttribute to traverse.
       *  \remarks Traverses all TextureAttributeItem objects of \a texAttrib. */
      NVSG_API void traverseTextureAttribute( const nvsg::TextureAttribute * texAttrib );

      /*! \brief Traversal of a MapArray.
       *  \param mapArray A pointer to the constant MapArray to traverse.
       *  \remarks Traverses all Map objects of \a mapArray. The traversal direction is determined
       *  by the MapArray.
       *  \sa MapArray::getTraversalDirection */
      NVSG_API void traverseMapArray( const nvsg::MapArray * mapArray );

      /*! \brief Traversal of a MapMatrix.
       *  \param mapMatrix A pointer to the constant MapMatrix to traverse.
       *  \remarks Traverses all Map objects of \a mapMatrix. Both the horizontal and vertical
       *  traversal direction is determined by the MapMatrix.
       *  \sa MapMatrix::getTraversalDirection, preTraverseMapColumn, postTraverseMapColumn */
      NVSG_API void traverseMapMatrix( const nvsg::MapMatrix * mapMatrix );

      /*! \brief Traversal of a MapList.
       *  \param mapList A pointer to the constant MapList to traverse.
       *  \remarks A ViewState holds a MapList, containing two-dimensional maps, that can be used
       *  to render, for example, overlays for menus or textual information to top of the scene.
       *  That MapList is traversed here.
       *  \sa ViewState::setMapList, ViewState::getMapList, traverseMapArray, traverseMapMatrix */
      NVSG_API void traverseMapList( const nvsg::MapList * mapList );

      /*! \brief Traversal of a VolumeNode.
       *  \param volNode A pointer to the constant VolumeNode to traverse.
       *  \remarks Traverses the associated shader (a CgFx object) and the VolumeBrick objects
       *  that are at least partially visible. */
      NVSG_API void traverseVolumeNode( const nvsg::VolumeNode * volNode );

      /*! \brief Evaluate the current state of the cull tree.
       *  \param viewState A pointer to the constant ViewState whose cull tree is evaluated
       *  \remarks This function is used in the implementation of ExclusiveTraverser and
       *  SharedTraverser. */
      NVSG_API void evaluateCullTree( const nvsg::ViewState * viewState );

    protected:
      // traverser bits
      enum
      {
        // tree flags
        NVSG_SCENE_DIRTY           = BIT0
      , NVSG_ROOT_DIRTY            = BIT1
      , NVSG_CULLTREE_DIRTY        = BIT2
      , NVSG_TREE_DIRTY_MASK       = NVSG_SCENE_DIRTY | NVSG_ROOT_DIRTY | NVSG_CULLTREE_DIRTY
        // viewstate flags
      , NVSG_VIEWSTATE_DIRTY       = BIT3
      // traversal flags
      , NVSG_TRAVERSAL_INTERRUPTED = BIT4
      };

      std::stack<const nvsg::CullData *>    m_cullStack;      //!< The stack of CullData corresponding to the currently traversed tree.
      std::stack<size_t>                    m_lodStack;       //!< The stack of LOD level; top is the current level
      nvutil::MemFunTbl<Traverser>          m_mftbl;          //!< The table of handler functions.
      nvsg::StateSetHandle                * m_nullStateSet;   //!< A pointer to an empty StateSetHandle, serving as the default StateSet.
      nvutil::ProblemLog                    m_problemLog;     //!< Problems occurred during construction go here.
      nvsg::NodeHandle                    * m_root;           //!< A pointer to the current root node.
      nvsg::SceneHandle                   * m_scene;          //!< A pointer to the current scene.
      unsigned int                          m_traverserBits;  //!< The bitfield of flags for a Traverser.
      nvsg::ViewStateHandle               * m_viewState;      //!< A pointer to the current view state.

      // NOTE: must not encode this into m_traverserBits, because it must remain volatile
      bool volatile m_interrupt;    //!< A flag, indicating an interrupt request

    private:
      NVSG_API unsigned int     getObjectTraversalCode( const nvsg::Object * object );  // NOTE: used inline, therefore needs to be exported via NVSG_API
      void                      traverseLockedObject( const nvsg::Object * obj );
      template<typename T> void traverseLockedObject(const nvsg::Object * obj, T arg);

      // traverse overrides for Group derived (have only read-only access to objects)
      void traverse( const nvsg::Group * );
      void traverse( const nvsg::Group *, const nvsg::CullData * );
      void traverse( const nvsg::Switch * );
      void traverse( const nvsg::Switch *, const nvsg::CullData * );
      void traverse( const nvsg::LOD * );
      void traverse( const nvsg::LOD *, const nvsg::CullData * );
      // specialized helper with GeoNode traversal
      void traverse( const nvsg::GeoNode * );

    private:
      // several incarnation counters to monitor related changes
      nvutil::Incarnation m_rootIncarnation;
      nvutil::Incarnation m_cullTreeIncarnation;
  };

  /*! \brief Base class providing an interface for read-only traversing of a given scene graph.
  *  \remarks The SharedTraverser class offers base functionality for all traversers that intend
  *  to provide read-only operations on a given scene graph. For all known and concrete \link
  *  nvsg::Object Objects \endlink this class provides a set of overloadable handler routines,
  *  each ensuring that all components following the actual object in the graph hierarchy will be
  *  traversed correctly. Hence, for derived traversers it is recommended to always call the base
  *  implementation of an overloaded handler routine for traversing purposes.\n
  *  To provide new read-only operations to be applied on known and concrete components or
  *  objects arranged in a scene graph, it is sufficient to derive a new traverser from either
  *  SharedTraverser or one of its derived traversers, and overload the corresponding handler
  *  routines as needed.
  *  \sa ExclusiveTraverser */
  class SharedTraverser : public Traverser
  {
    public:
      /*! \brief Default constructor. */
      NVSG_API SharedTraverser();

    protected:
      /*! \brief Protected Destructor to prevent instantiation. */
      NVSG_API virtual ~SharedTraverser() {}
      
      /*! \brief Template function to add a handler routine for a new class derived from Object.
       *  \param objectCode Object code to identify an object type at run-time.
       *  \param handler Specifies the address of the handler routine.
       *  \remarks The function registers the handler routine specified by \a handler to handle a
       *  concrete class, derived from Object, that is explicitly identified by \a objectCode.\n
       *  A handler routine must be a member function of a Traverser-derived class. It must return
       *  void and expect a pointer to the concrete object as a first parameter, and can have one
       *  additional (optional) argument of arbitrary type. A handler routine must not have a
       *  default argument! If a handler routine is intended to remain overloadable, it should be
       *  declared virtual.\n
       *  For derived classes intended to provide new operations for known objects arranged in a
       *  scene graph, it is strongly recommended not to add new handler routines for those objects
       *  but to overload the corresponding handler routine. In this context, we refer to 'known
       *  objects' as objects that are known by the Traverser base class and for which a
       *  corresponding handler routine is provided.\n
       *  However, \c addObjectHandler is indispensable if newly invented objects, and hence, 
       *  objects that are unknown by the Traverser base class, need to be considered for
       *  traversing.
       *  \note Any previously registered handler routine for the specified object code will be
       *  overridden by the new handler.
       *  \par Example:
       *  \code
       *    addObjectHandler( 
       *      OC_NEWGROUP   // object code for the new object NewGroup
       *                    // this must be provided by the author of NewGroup
       *      , &MySharedTraverser::handleNewGroup // handler function  
       *    );    
       *  \endcode */
      template <typename T, typename U> 
      void addObjectHandler( unsigned int objectCode, void (T::*handler)(const U*) );

      /*! \brief Template function to add a handler routine for a new class derived from Object.
       *  \param objectCode Object code to identify an object type at run-time.
       *  \param handler Specifies the address of the handler routine.
       *  \remarks The function registers the handler routine specified by \a handler to handle a
       *  concrete class, derived from Object, that is explicitly identified by \a objectCode.\n
       *  A handler routine must be a member function of a Traverser-derived class. It must return
       *  void and expect a pointer to the concrete object as a first parameter, and can have one
       *  additional (optional) argument of arbitrary type. A handler routine must not have a
       *  default argument! If a handler routine is intended to remain overloadable, it should be
       *  declared virtual.\n
       *  For derived classes intended to provide new operations for known objects arranged in a
       *  scene graph, it is strongly recommended not to add new handler routines for those objects
       *  but to overload the corresponding handler routine. In this context, we refer to 'known
       *  objects' as objects that are known by the Traverser base class and for which a
       *  corresponding handler routine is provided.\n
       *  However, \c addObjectHandler is indispensable if newly invented objects, and hence, 
       *  objects that are unknown by the Traverser base class, need to be considered for
       *  traversing.
       *  \note Any previously registered handler routine for the specified object code will be
       *  overridden by the new handler.
       *  \par Example:
       *  \code
       *    addObjectHandler( 
       *      OC_NEWGROUP   // object code for the new object NewGroup
       *                    // this must be provided by the author of NewGroup
       *      , &MySharedTraverser::handleNewGroup // handler function  
       *    );    
       *  \endcode */
      template <typename T, typename U, typename V> 
      void addObjectHandler( unsigned int objectCode, void (T::*handler)(const U*, V) );

      /*! \brief Override of the object locking interface.
       *  \param obj A pointer to the ObjectHandle to lock.
       *  \return A pointer to a read-locked Object.
       *  \remarks This function is called from the framework just before traversal of \a obj. The
       *  SharedTraverser locks the Object for reading.
       *  \sa unlockObject */
      NVSG_API virtual const nvsg::Object * lockObject( nvsg::ObjectHandle * obj );

      /*! \brief Override of the object unlocking interface.
       *  \param obj A pointer to the ObjectHandle to unlock.
       *  \remarks This function is called from the framework right after traversal of \a obj. The
       *  SharedTraverser unlocks the Object from reading.
       *  \sa lockObject */
      NVSG_API virtual void unlockObject( nvsg::ObjectHandle * obj );

      /*! \brief Override of the ViewState locking interface.
       *  \remarks This function is called from the framework to lock the current ViewState, if any,
       *  for reading.
       *  \sa setViewState, unlockViewState */
      NVSG_API virtual void lockViewState();

      /*! \brief Override of the ViewState unlocking interface.
       *  \remarks This function is called from the framework to unlock the current ViewState, if
       *  any, from reading.
       *  \sa setViewState, lockViewState */
      NVSG_API virtual void unlockViewState();

      /*! \brief Override of the Scene locking interface.
       *  \remarks This function is called from the framework to lock the current Scene, if any, for
       *  reading.
       *  \sa setScene, unlockScene */
      NVSG_API virtual void lockScene();

      /*! \brief Override of the Scene unlocking interface.
       *  \remarks This function is called from the framework to unlock the current Scene, if any,
       *  from reading.
       *  \sa setScene, lockScene */
      NVSG_API virtual void unlockScene();

      /*! \brief Override of the traversal initiating interface.
       *  \param root A pointer to the NodeHandle to start traversal at.
       *  \remarks This function is called from the framework after the Scene and the ViewState,
       *  if set, are locked. The actual traversal is done here.
       *  \sa preApply, postApply, lockScene, unlockScene, lockViewState, unlockViewState */
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      /*! \brief Handler function for a ParallelCamera.
       *  \param camera A pointer to the read-locked ParallelCamera being traversed.
       *  \remarks This function is called from the framework whenever a ParallelCamera is
       *  encountered on traversal. The ParallelCamera \a camera is already read-locked.\n
       *  The base implementation traverses through all the head lights of this camera.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa handlePerspectiveCamera */
      NVSG_API virtual void handleParallelCamera( const nvsg::ParallelCamera * camera );

      /*! \brief Handler function for a PerspectiveCamera.
       *  \param camera A pointer to the read-locked PerspectiveCamera being traversed.
       *  \remarks This function is called from the framework whenever a PerspectiveCamera is
       *  encountered on traversal. The PerspectiveCamera \a camera is already read-locked.\n
       *  The base implementation traverses through all the head lights of this camera.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa handleParallelCamera */
      NVSG_API virtual void handlePerspectiveCamera( const nvsg::PerspectiveCamera * camera );

      /*! \brief Handler function for a Group.
       *  \param group A pointer to the read-locked Group being traversed.
       *  \remarks This function is called from the framework whenever a Group is encountered on
       *  traversal. The Group \a group is already read-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup */
      NVSG_API virtual void handleGroup( const nvsg::Group * group );

      /*! \brief Handler function for a Transform.
       *  \param trafo A pointer to the read-locked Transform being traversed.
       *  \remarks This function is called from the framework whenever a Transform is encountered
       *  on traversal. The Transform \a trafo is already read-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleTransform( const nvsg::Transform * trafo );

      /*! \brief Handler function for an AnimatedTransform.
       *  \param trafo A pointer to the read-locked AnimatedTransform being traversed.
       *  \remarks This function is called from the framework whenever an AnimatedTransform is
       *  encountered on traversal. The AnimatedTransform \a trafo is already read-locked.\n
       *  The base implementation first traverses the associated TrafoAnimation, and then calls
       *  Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup, TrafoAnimation */
      NVSG_API virtual void handleAnimatedTransform( const nvsg::AnimatedTransform * trafo );

      /*! \brief Handler function for an LOD.
       *  \param lod A pointer to the read-locked LOD being traversed.
       *  \remarks This function is called from the framework whenever an LOD is encountered on
       *  traversal. The LOD \a lod is already read-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleLOD( const nvsg::LOD * lod );

      /*! \brief Handler function for a Switch.
       *  \param swtch A pointer to the read-locked Switch being traversed.
       *  \remarks This function is called from the framework whenever a Switch is encountered on
       *  traversal. The Switch \a switch is already read-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleSwitch( const nvsg::Switch * swtch );

      /*! \brief Handler function for a Billboard.
       *  \param billboard A pointer to the read-locked Billboard being traversed.
       *  \remarks This function is called from the framework whenever a Billboard is encountered on
       *  traversal. The Billboard \a billboard is already read-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleBillboard( const nvsg::Billboard * billboard );

      /*! \brief Handler function for a DirectedLight.
       *  \param light A pointer to the read-locked DirectedLight being traversed.
       *  \remarks This function is called from the framework whenever a DirectedLight is
       *  encountered on traversal. The DirectedLight \a light is already read-locked.\n
       *  The base implementation just calls traverseLight().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseLight */
      NVSG_API virtual void handleDirectedLight( const nvsg::DirectedLight * light );

      /*! \brief Handler function for a PointLight.
       *  \param light A pointer to the read-locked PointLight being traversed.
       *  \remarks This function is called from the framework whenever a PointLight is encountered
       *  on traversal. The PointLight \a light is already read-locked.\n
       *  The base implementation just calls traverseLight().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseLight */
      NVSG_API virtual void handlePointLight( const nvsg::PointLight * light );

      /*! \brief Handler function for a SpotLight.
       *  \param light A pointer to the read-locked SpotLight being traversed.
       *  \remarks This function is called from the framework whenever a SpotLight is encountered on
       *  traversal. The SpotLight \a light is already read-locked.\n
       *  The base implementation just calls traverseLight().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseLight */
      NVSG_API virtual void handleSpotLight( const nvsg::SpotLight * light );

      /*! \brief Handler function for a GeoNode.
       *  \param gnode A pointer to the read-locked GeoNode being traversed.
       *  \remarks This function is called from the framework whenever a GeoNode is encountered on
       *  traversal. The GeoNode \a gnode is already read-locked.\n
       *  The base implementation just calls Traverser::traverseGeoNode().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGeoNode */
      NVSG_API virtual void handleGeoNode( const nvsg::GeoNode * gnode );

      /*! \brief Handler function for a Quads.
       *  \param quads A pointer to the read-locked Quads being traversed.
       *  \remarks This function is called from the framework whenever a Quads is encountered on
       *  traversal. The Quads \a quads is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleQuads( const nvsg::Quads * quads );

      /*! \brief Handler function for an AnimatedQuads.
       *  \param quads A pointer to the read-locked AnimatedQuads being traversed.
       *  \remarks This function is called from the framework whenever an AnimatedQuads is
       *  encountered on traversal. The AnimatedQuads \a quads is already read-locked.\n
       *  The base implementation first traverses the associated VNVectorAnimation, and then calls
       *  traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleAnimatedQuads( const nvsg::AnimatedQuads * quads );

      /*! \brief Handler function for a Triangles.
       *  \param triangles A pointer to the read-locked Triangles being traversed.
       *  \remarks This function is called from the framework whenever a Triangles is encountered on
       *  traversal. The Triangles \a quads is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleTriangles( const nvsg::Triangles * triangles );

      /*! \brief Handler function for an AnimatedTriangles.
       *  \param triangles A pointer to the read-locked AnimatedTriangles being traversed.
       *  \remarks This function is called from the framework whenever an AnimatedTriangles is
       *  encountered on traversal. The AnimatedTriangles \a quads is already read-locked.\n
       *  The base implementation first traverses the associated VNVectorAnimation, and then calls
       *  traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleAnimatedTriangles( const nvsg::AnimatedTriangles * triangles );

      /*! \brief Handler function for a SkinnedTriangles.
       *  \param triangles A pointer to the read-locked Triangles being traversed.
       *  \remarks This function is called from the framework whenever a SkinnedTriangles is
       *  encountered on traversal. The SkinnedTriangles \a triangles is already read-locked.\n
       *  The base implementation first traverses each TrafoAnimation of each bone of each skin, and
       *  then calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleSkinnedTriangles( const nvsg::SkinnedTriangles * triangles );

      /*! \brief Handler function for a Lines.
       *  \param lines A pointer to the read-locked Lines being traversed.
       *  \remarks This function is called from the framework whenever a Lines is encountered on
       *  traversal. The Lines \a lines is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleLines( const nvsg::Lines * lines );

      /*! \brief Handler function for a Points.
       *  \param points A pointer to the read-locked Points being traversed.
       *  \remarks This function is called from the framework whenever a Points is encountered on
       *  traversal. The Points \a points is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handlePoints( const nvsg::Points * points );

      /*! \brief Handler function for a QuadsMeshes.
       *  \param meshes A pointer to the read-locked QuadsMeshes being traversed.
       *  \remarks This function is called from the framework whenever a QuadsMeshes is encountered
       *  on traversal. The QuadsMeshes \a meshes is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleQuadMeshes( const nvsg::QuadMeshes * meshes );

      /*! \brief Handler function for a QuadStrips.
       *  \param strips A pointer to the read-locked QuadStrips being traversed.
       *  \remarks This function is called from the framework whenever a QuadStrips is encountered
       *  on traversal. The QuadStrips \a strips is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleQuadStrips( const nvsg::QuadStrips * strips );

      /*! \brief Handler function for a TriFans.
       *  \param fans A pointer to the read-locked TriFans being traversed.
       *  \remarks This function is called from the framework whenever a TriFans is encountered on
       *  traversal. The TriFans \a fans is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleTriFans( const nvsg::TriFans * fans );

      /*! \brief Handler function for a TriStrips.
       *  \param strips A pointer to the read-locked TriStrips being traversed.
       *  \remarks This function is called from the framework whenever a TriStrips is encountered on
       *  traversal. The TriStrips \a strips is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleTriStrips( const nvsg::TriStrips * strips );

      /*! \brief Handler function for a LineStrips.
       *  \param strips A pointer to the read-locked LineStrips being traversed.
       *  \remarks This function is called from the framework whenever a LineStrips is encountered
       *  on traversal. The LineStrips \a strips is already read-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleLineStrips( const nvsg::LineStrips * strips );

      /*! \brief Handler function for a Shape.
       *  \param shape A pointer to the read-locked Shape being traversed.
       *  \remarks This function is called from the framework whenever a Shape is encountered on
       *  traversal. The Shape \a shape is already read-locked.\n
       *  The base implementation just calls Traverser::traverseShape().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseShape */
      NVSG_API virtual void handleShape( const nvsg::Shape * shape );

      /*! \brief Handler function for a StateSet.
       *  \param stateSet A pointer to the read-locked StateSet being traversed.
       *  \remarks This function is called from the framework whenever a StateSet is encountered on
       *  traversal. The StateSet \a stateSet is already read-locked.\n
       *  The base implementation just traverses the active StateVariant.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa StateVariant */
      NVSG_API virtual void handleStateSet( const nvsg::StateSet * stateSet );

      /*! \brief Handler function for a StateVariant.
       *  \param variant A pointer to the read-locked StateVariant being traversed.
       *  \remarks This function is called from the framework whenever a StateVariant is encountered
       *  on traversal. The StateVariant \a variant is already read-locked.\n
       *  The base implementation just call Traverser::traverseStateVariant().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseStateVariant */
      NVSG_API virtual void handleStateVariant( const nvsg::StateVariant * variant );

      /*! \brief Handler function for a StatePass.
       *  \param pass A pointer to the read-locked StatePass being traversed.
       *  \remarks This function is called from the framework whenever a StateSet is encountered on
       *  traversal. The StatePass \a pass is already read-locked.\n
       *  The base implementation just call Traverser::traverseStatePass().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseStatePass */
      NVSG_API virtual void handleStatePass( const nvsg::StatePass * pass );

      /*! \brief Handler function for a CgFx.
       *  \param cgfx A pointer to the read-locked CgFx being traversed.
       *  \remarks This function is called from the framework whenever a CgFx is encountered on
       *  traversal. The CgFx \a cgfx is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleCgFx( const nvsg::CgFx * cgfx );

      /*! \brief Handler function for a Material.
       *  \param material A pointer to the read-locked Material being traversed.
       *  \remarks This function is called from the framework whenever a Material is encountered on
       *  traversal. The Material \a material is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleMaterial( const nvsg::Material * material );

      /*! \brief Handler function for a TextureAttribute.
       *  \param texAttrib A pointer to the read-locked TextureAttribute being traversed.
       *  \remarks This function is called from the framework whenever a TextureAttribute is
       *  encountered on traversal. The TextureAttribute \a texAttrib is already read-locked.\n
       *  The base implementation just calls Traverser::traverseTextureAttribute().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseTextureAttribute */
      NVSG_API virtual void handleTextureAttribute( const nvsg::TextureAttribute * texAttrib );

      /*! \brief Handler function for a TextureAttributeItem.
       *  \param texAttribItem A pointer to the read-locked TextureAttributeItem being traversed.
       *  \param tu Index of the texture unit to use.
       *  \remarks This function is called from the framework whenever a TextureAttributeItem is
       *  encountered on traversal. The TextureAttributeItem \a texAttribItem is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleTextureAttributeItem( const nvsg::TextureAttributeItem * texAttribItem
                                                      , size_t tu );

      /*! \brief Handler function for a BlendAttribute.
       *  \param blendAttrib A pointer to the read-locked BlendAttribute being traversed.
       *  \remarks This function is called from the framework whenever a BlendAttribute is
       *  encountered on traversal. The BlendAttribute \a blendAttrib is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleBlendAttribute( const nvsg::BlendAttribute * blendAttrib );

      /*! \brief Handler function for a DepthAttribute.
       *  \param depthAttrib A pointer to the read-locked DepthAttribute being traversed.
       *  \remarks This function is called from the framework whenever a DepthAttribute is
       *  encountered on traversal. The DepthAttribute \a depthAttrib is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleDepthAttribute( const nvsg::DepthAttribute * depthAttrib );

      /*! \brief Handler function for a FaceAttribute.
       *  \param faceAttrib A pointer to the read-locked FaceAttribute being traversed.
       *  \remarks This function is called from the framework whenever a FaceAttribute is
       *  encountered on traversal. The FaceAttribute \a faceAttrib is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleFaceAttribute( const nvsg::FaceAttribute * faceAttrib );

      /*! \brief Handler function for a LineAttribute.
       *  \param lineAttrib A pointer to the read-locked LineAttribute being traversed.
       *  \remarks This function is called from the framework whenever a LineAttribute is
       *  encountered on traversal. The LineAttribute \a lineAttrib is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleLineAttribute( const nvsg::LineAttribute * lineAttrib );

      /*! \brief Handler function for a PointAttribute.
       *  \param pointAttrib A pointer to the read-locked PointAttribute being traversed.
       *  \remarks This function is called from the framework whenever a PointAttribute is
       *  encountered on traversal. The PointAttribute \a pointAttrib is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handlePointAttribute( const nvsg::PointAttribute * pointAttrib );

      /*! \brief Handler function for a MapArray.
       *  \param mapArray A pointer to the read-locked MapArray being traversed.
       *  \remarks This function is called from the framework whenever a MapArray is encountered on
       *  traversal. The MapArray \a mapArray is already read-locked.\n
       *  The base implementation just calls Traverser::traverseMapArray().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseMapArray */
      NVSG_API virtual void handleMapArray( const nvsg::MapArray * mapArray );

      /*! \brief Handler function for a MapElement.
       *  \param mapElement A pointer to the read-locked MapElement being traversed.
       *  \remarks This function is called from the framework whenever a MapElement is
       *  encountered on traversal. The MapElement \a mapElement is already read-locked.\n
       *  The base implementation just traverses the Map of the MapElement.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa Map */
      NVSG_API virtual void handleMapElement( const nvsg::MapElement * mapElement );

      /*! \brief Handler function for a MapMatrix.
       *  \param mapMatrix A pointer to the read-locked MapMatrix being traversed.
       *  \remarks This function is called from the framework whenever a MapMatrix is encountered on
       *  traversal. The MapMatrix \a mapMatrix is already read-locked.\n
       *  The base implementation just calls Traverser::traverseMapMatrix().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseMapMatrix */
      NVSG_API virtual void handleMapMatrix( const nvsg::MapMatrix * mapMatrix );

      /*! \brief Handler function for a MapList.
       *  \param mapList A pointer to the read-locked MapList being traversed.
       *  \remarks This function is called from the framework whenever a MapList is encountered on
       *  traversal. The MapList \a mapList is already read-locked.\n
       *  The base implementation just calls Traverser::traverseMapList().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseMapList */
      NVSG_API virtual void handleMapList( const nvsg::MapList * mapList );

      /*! \brief Handler function for a Bitmap.
       *  \param bm A pointer to the read-locked Bitmap being traversed.
       *  \remarks This function is called from the framework whenever a Bitmap is encountered on
       *  traversal. The Bitmap \a bm is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleBitmap( const nvsg::Bitmap * bm );

      /*! \brief Handler function for a VertexAttributeSet.
       *  \param vas A pointer to the read-locked VertexAttributeSet being traversed.
       *  \remarks This function is called from the framework whenever a VertexAttributeSet is
       *  encountered on traversal. The VertexAttributeSet \a vas is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleVertexAttributeSet( const nvsg::VertexAttributeSet *vas );

      /*! \brief Handler function for a FramedAnimation<nvmath::Trafo>.
       *  \param trafo A pointer to the read-locked FramedAnimation<nvmath::Trafo> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  FramedAnimation<nvmath::Trafo> is encountered on traversal. The
       *  FramedAnimation<nvmath::Trafo> \a trafo is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleFramedTrafoAnimation( const nvsg::FramedAnimation<nvmath::Trafo> * trafo );

      /*! \brief Handler function for a FramedAnimation<nvsg::VNVector>.
       *  \param vnVector A pointer to the read-locked FramedAnimation<nvsg::VNVector> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  FramedAnimation<nvsg::VNVector> is encountered on traversal. The
       *  FramedAnimation<nvsg::VNVector> \a vnVector is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleFramedVNVectorAnimation( const nvsg::FramedAnimation<nvsg::VNVector> * vnVector );

      /*! \brief Handler function for a LinearInterpolatedAnimation<nvmath::Trafo>.
       *  \param trafo A pointer to the read-locked LinearInterpolatedAnimation<nvmath::Trafo> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  LinearInterpolatedAnimation<nvmath::Trafo> is encountered on traversal. The
       *  LinearInterpolatedAnimation<nvmath::Trafo> \a trafo is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleLinearInterpolatedTrafoAnimation( const nvsg::LinearInterpolatedAnimation<nvmath::Trafo> * trafo );

      /*! \brief Handler function for a LinearInterpolatedAnimation<nvsg::VNVector>.
       *  \param vnVector A pointer to the read-locked LinearInterpolatedAnimation<nvsg::VNVector> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  LinearInterpolatedAnimation<nvsg::VNVector> is encountered on traversal. The
       *  LinearInterpolatedAnimation<nvsg::VNVector> \a vnVector is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleLinearInterpolatedVNVectorAnimation( const nvsg::LinearInterpolatedAnimation<nvsg::VNVector> * vnVector );

      /*! \brief Handler function for a HighlightBox.
       *  \param hlBox A pointer to the read-locked HighlightBox being traversed.
       *  \param pset A pointer to the read-locked PrimitiveSet, that \a hlBox is associated to.
       *  \remarks This function is called from the framework whenever a HighlightBox is
       *  encountered on traversal. The HighlightBox \a hlBox is already read-locked.\n
       *  The base implementation just calls traverseHighlightObject().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseHighlightObject */
      NVSG_API virtual void handleHighlightBox( const nvsg::HighlightBox * hlBox
                                              , const nvsg::PrimitiveSet * pset );

      /*! \brief Handler function for a HighlightEdges.
       *  \param hlEdges A pointer to the read-locked HighlightEdges being traversed.
       *  \param pset A pointer to the read-locked PrimitiveSet, that \a hlEdges is associated to.
       *  \remarks This function is called from the framework whenever a HighlightEdges is
       *  encountered on traversal. The HighlightEdges \a hlEdges is already read-locked.\n
       *  The base implementation just calls traverseHighlightObject().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseHighlightObject */
      NVSG_API virtual void handleHighlightEdges( const nvsg::HighlightEdges * hlEdges
                                                , const nvsg::PrimitiveSet * pset );

      /*! \brief Handler function for a VolumeNode.
       *  \param vnode A pointer to the read-locked VolumeNode being traversed.
       *  \remarks This function is called from the framework whenever a VolumeNode is encountered
       *  on traversal. The VolumeNode \a vnode is already read-locked.\n
       *  The base implementation just calls Traverser::traverseVolumeNode().
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseVolumeNode */
      NVSG_API virtual void handleVolumeNode( const nvsg::VolumeNode * vnode );

      /*! \brief Function for common handling of Camera classes.
       *  \param camera A pointer to the read-locked Camera being traversed.
       *  \remarks This function is called from the framework whenever a Camera is encountered
       *  on traversal. The Camera \a camera is already read-locked.\n
       *  The base implementation traverses the headlights of \a camera.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa LightSource */
      NVSG_API virtual void traverseCamera( const nvsg::Camera * camera );

      /*! \brief Function for common handling of LightSource classes.
       *  \param light A pointer to the read-locked LightSource being traversed.
       *  \remarks This function is called from the framework whenever a LightSource is encountered
       *  on traversal. The LightSource \a light is already read-locked.\n
       *  The base implementation traverses the Animation associated with \a light, if there is one.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa TrafoAnimation */
      NVSG_API virtual void traverseLight( const nvsg::LightSource * light );

      /*! \brief Function for common handling of PrimitiveSet classes.
       *  \param pset A pointer to the read-locked PrimitiveSet being traversed.
       *  \remarks This function is called from the framework whenever a PrimitiveSet is encountered
       *  on traversal. The PrimitiveSet \a pset is already read-locked.\n
       *  The base implementation traverses the VertexAttributeSet of \a pset, if there is one. If
       *  \a pset is highlighted, the associated HighlightObject is traversed.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa VertexAttributeSet, HighlightObject */
      NVSG_API virtual void traversePrimitiveSet( const nvsg::PrimitiveSet * pset );

      /*! \brief Function for common handling of HighlightObject classes.
       *  \param hlObject A pointer to the read-locked HighlightObject being traversed.
       *  \param pset A pointer to the read-locked PrimitiveSet to be highlighted.
       *  \remarks This function is called from the framework whenever a HighlightObject is
       *  encountered on traversal. The HighlightObject \a hlObject is already read-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from SharedTraverser, it is
       *  recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void traverseHighlightObject( const nvsg::HighlightObject * hlObject
                                                   , const nvsg::PrimitiveSet * pset );

    protected:
      const nvsg::Scene * m_lockedScene;          //!< A pointer to the read-locked Scene currently traversed.
      const nvsg::ViewState * m_lockedViewState;  //!< A pointer to the read-locked ViewState used in the current traversal.
  };

/*! \brief Base class providing an interface for read-write traversing of a given scene graph.
 *  \remarks The SharedTraverser class offers base functionality for all traversers that intend
 *  to provide read-write operations on a given scene graph. For all known and concrete \link
 *  nvsg::Object Objects \endlink this class provides a set of overloadable handler routines,
 *  each ensuring that all components following the actual object in the graph hierarchy will be
 *  traversed correctly. Hence, for derived traversers it is recommended to always call the base
 *  implementation of an overloaded handler routine for traversing purposes.\n
 *  To provide new read-write operations to be applied on known and concrete components or
 *  objects arranged in a scene graph, it is sufficient to derive a new traverser from either
 *  ExclusiveTraverser or one of its derived traversers, and overload the corresponding handler
 *  routines as needed.
 *  \sa SharedTraverser */
  class ExclusiveTraverser : public Traverser
  {
    public:
      /*! \brief Default constructor. */
      NVSG_API ExclusiveTraverser();

    protected:
      /*! \brief Protected Destructor to prevent instantiation. */
      NVSG_API virtual ~ExclusiveTraverser() {}

      /*! \brief Template function to add a handler routine for a new class derived from Object.
       *  \param objectCode Object code to identify an object type at run-time.
       *  \param handler Specifies the address of the handler routine.
       *  \remarks The function registers the handler routine specified by \a handler to handle a
       *  concrete class, derived from Object, that is explicitly identified by \a objectCode.\n
       *  A handler routine must be a member function of a Traverser-derived class. It must return
       *  void and expect a pointer to the concrete object as a first parameter, and can have one
       *  additional (optional) argument of arbitrary type. A handler routine must not have a
       *  default argument! If a handler routine is intended to remain overloadable, it should be
       *  declared virtual.\n
       *  For derived classes intended to provide new operations for known objects arranged in a
       *  scene graph, it is strongly recommended not to add new handler routines for those objects
       *  but to overload the corresponding handler routine. In this context, we refer to 'known
       *  objects' as objects that are known by the Traverser base class and for which a
       *  corresponding handler routine is provided.\n
       *  However, \c addObjectHandler is indispensable if newly invented objects, and hence, 
       *  objects that are unknown by the Traverser base class, need to be considered for
       *  traversing.
       *  \note Any previously registered handler routine for the specified object code will be
       *  overridden by the new handler.
       *  \par Example:
       *  \code
       *    addObjectHandler( 
       *      OC_NEWGROUP   // object code for the new object NewGroup
       *                    // this must be provided by the author of NewGroup
       *      , &MyExclusiveTraverser::handleNewGroup // handler function  
       *    );    
       *  \endcode */
      template <typename T, typename U> 
      void addObjectHandler( unsigned int objectCode, void (T::*handler)(U*) );

      /*! \brief Template function to add a handler routine for a new class derived from Object.
       *  \param objectCode Object code to identify an object type at run-time.
       *  \param handler Specifies the address of the handler routine.
       *  \remarks The function registers the handler routine specified by \a handler to handle a
       *  concrete class, derived from Object, that is explicitly identified by \a objectCode.\n
       *  A handler routine must be a member function of a Traverser-derived class. It must return
       *  void and expect a pointer to the concrete object as a first parameter, and can have one
       *  additional (optional) argument of arbitrary type. A handler routine must not have a
       *  default argument! If a handler routine is intended to remain overloadable, it should be
       *  declared virtual.\n
       *  For derived classes intended to provide new operations for known objects arranged in a
       *  scene graph, it is strongly recommended not to add new handler routines for those objects
       *  but to overload the corresponding handler routine. In this context, we refer to 'known
       *  objects' as objects that are known by the Traverser base class and for which a
       *  corresponding handler routine is provided.\n
       *  However, \c addObjectHandler is indispensable if newly invented objects, and hence, 
       *  objects that are unknown by the Traverser base class, need to be considered for
       *  traversing.
       *  \note Any previously registered handler routine for the specified object code will be
       *  overridden by the new handler.
       *  \par Example:
       *  \code
       *    addObjectHandler( 
       *      OC_NEWGROUP   // object code for the new object NewGroup
       *                    // this must be provided by the author of NewGroup
       *      , &MyExclusiveTraverser::handleNewGroup // handler function  
       *    );    
       *  \endcode */
      template <typename T, typename U, typename V> 
      void addObjectHandler( unsigned int objectCode, void (T::*handler)(U*, V) );

      /*! \brief Override of the object locking interface.
       *  \param obj A pointer to the ObjectHandle to lock.
       *  \return A pointer to a write-locked Object.
       *  \remarks This function is called from the framework just before traversal of \a obj. The
       *  ExclusiveTraverser locks the Object for writing.
       *  \sa unlockObject */
      NVSG_API virtual nvsg::Object * lockObject( nvsg::ObjectHandle * obj );

      /*! \brief Override of the object unlocking interface.
       *  \param obj A pointer to the ObjectHandle to unlock.
       *  \remarks This function is called from the framework right after traversal of \a obj. The
       *  ExclusiveTraverser unlocks the Object from writing.
       *  \sa lockObject */
      NVSG_API virtual void unlockObject( nvsg::ObjectHandle * obj );

      /*! \brief Override of the ViewState locking interface.
       *  \remarks This function is called from the framework to lock the current ViewState, if any,
       *  for writing.
       *  \sa setViewState, unlockViewState */
      NVSG_API virtual void lockViewState();

      /*! \brief Override of the ViewState unlocking interface.
       *  \remarks This function is called from the framework to unlock the current ViewState, if
       *  any, from writing.
       *  \sa setViewState, lockViewState */
      NVSG_API virtual void unlockViewState();

      /*! \brief Override of the Scene locking interface.
       *  \remarks This function is called from the framework to lock the current Scene, if any, for
       *  writing.
       *  \sa setScene, unlockScene */
      NVSG_API virtual void lockScene();

      /*! \brief Override of the Scene unlocking interface.
       *  \remarks This function is called from the framework to unlock the current Scene, if any,
       *  from writing.
       *  \sa setScene, lockScene */
      NVSG_API virtual void unlockScene();

      /*! \brief Override of the traversal initiating interface.
       *  \param root A pointer to the NodeHandle to start traversal at.
       *  \remarks This function is called from the framework after the Scene and the ViewState,
       *  if set, are locked. The actual traversal is done here.
       *  \sa preApply, postApply, lockScene, unlockScene, lockViewState, unlockViewState */
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      /*! \brief Handler function for a ParallelCamera.
       *  \param camera A pointer to the write-locked ParallelCamera being traversed.
       *  \remarks This function is called from the framework whenever a ParallelCamera is
       *  encountered on traversal. The ParallelCamera \a camera is already write-locked.\n
       *  The base implementation traverses through all the head lights of this camera.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa handlePerspectiveCamera */
      NVSG_API virtual void handleParallelCamera( nvsg::ParallelCamera * camera );

      /*! \brief Handler function for a PerspectiveCamera.
       *  \param camera A pointer to the write-locked PerspectiveCamera being traversed.
       *  \remarks This function is called from the framework whenever a PerspectiveCamera is
       *  encountered on traversal. The PerspectiveCamera \a camera is already write-locked.\n
       *  The base implementation traverses through all the head lights of this camera.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa handleParallelCamera */
      NVSG_API virtual void handlePerspectiveCamera( nvsg::PerspectiveCamera * camera );

      /*! \brief Handler function for a Group.
       *  \param group A pointer to the write-locked Group being traversed.
       *  \remarks This function is called from the framework whenever a Group is encountered on
       *  traversal. The Group \a group is already write-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup */
      NVSG_API virtual void handleGroup( nvsg::Group * group );

      /*! \brief Handler function for a Transform.
       *  \param trafo A pointer to the write-locked Transform being traversed.
       *  \remarks This function is called from the framework whenever a Transform is encountered
       *  on traversal. The Transform \a trafo is already write-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleTransform( nvsg::Transform * trafo );

      /*! \brief Handler function for an AnimatedTransform.
       *  \param trafo A pointer to the write-locked AnimatedTransform being traversed.
       *  \remarks This function is called from the framework whenever an AnimatedTransform is
       *  encountered on traversal. The AnimatedTransform \a trafo is already write-locked.\n
       *  The base implementation first traverses the associated TrafoAnimation, and then calls
       *  Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup, TrafoAnimation */
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform * trafo );

      /*! \brief Handler function for an LOD.
       *  \param lod A pointer to the write-locked LOD being traversed.
       *  \remarks This function is called from the framework whenever an LOD is encountered on
       *  traversal. The LOD \a lod is already write-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleLOD( nvsg::LOD * lod );

      /*! \brief Handler function for a Switch.
       *  \param swtch A pointer to the write-locked Switch being traversed.
       *  \remarks This function is called from the framework whenever a Switch is encountered on
       *  traversal. The Switch \a switch is already write-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleSwitch( nvsg::Switch * swtch );

      /*! \brief Handler function for a Billboard.
       *  \param billboard A pointer to the write-locked Billboard being traversed.
       *  \remarks This function is called from the framework whenever a Billboard is encountered on
       *  traversal. The Billboard \a billboard is already write-locked.\n
       *  The base implementation just calls Traverser::traverseGroup().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGroup, handleGroup */
      NVSG_API virtual void handleBillboard( nvsg::Billboard * billboard );

      /*! \brief Handler function for a DirectedLight.
       *  \param light A pointer to the write-locked DirectedLight being traversed.
       *  \remarks This function is called from the framework whenever a DirectedLight is
       *  encountered on traversal. The DirectedLight \a light is already write-locked.\n
       *  The base implementation just calls traverseLight().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseLight */
      NVSG_API virtual void handleDirectedLight( nvsg::DirectedLight * light );

      /*! \brief Handler function for a PointLight.
       *  \param light A pointer to the write-locked PointLight being traversed.
       *  \remarks This function is called from the framework whenever a PointLight is encountered
       *  on traversal. The PointLight \a light is already write-locked.\n
       *  The base implementation just calls traverseLight().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseLight */
      NVSG_API virtual void handlePointLight( nvsg::PointLight * light );

      /*! \brief Handler function for a SpotLight.
       *  \param light A pointer to the write-locked SpotLight being traversed.
       *  \remarks This function is called from the framework whenever a SpotLight is encountered on
       *  traversal. The SpotLight \a light is already write-locked.\n
       *  The base implementation just calls traverseLight().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseLight */
      NVSG_API virtual void handleSpotLight( nvsg::SpotLight * light );

      /*! \brief Handler function for a GeoNode.
       *  \param gnode A pointer to the write-locked GeoNode being traversed.
       *  \remarks This function is called from the framework whenever a GeoNode is encountered on
       *  traversal. The GeoNode \a gnode is already write-locked.\n
       *  The base implementation just calls Traverser::traverseGeoNode().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseGeoNode */
      NVSG_API virtual void handleGeoNode( nvsg::GeoNode * gnode );

      /*! \brief Handler function for a Quads.
       *  \param quads A pointer to the write-locked Quads being traversed.
       *  \remarks This function is called from the framework whenever a Quads is encountered on
       *  traversal. The Quads \a quads is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleQuads( nvsg::Quads * quads );

      /*! \brief Handler function for an AnimatedQuads.
       *  \param quads A pointer to the write-locked AnimatedQuads being traversed.
       *  \remarks This function is called from the framework whenever an AnimatedQuads is
       *  encountered on traversal. The AnimatedQuads \a quads is already write-locked.\n
       *  The base implementation first traverses the associated VNVectorAnimation, and then calls
       *  traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleAnimatedQuads( nvsg::AnimatedQuads * quads );

      /*! \brief Handler function for a Triangles.
       *  \param triangles A pointer to the write-locked Triangles being traversed.
       *  \remarks This function is called from the framework whenever a Triangles is encountered on
       *  traversal. The Triangles \a quads is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleTriangles( nvsg::Triangles * triangles );

      /*! \brief Handler function for an AnimatedTriangles.
       *  \param triangles A pointer to the write-locked AnimatedTriangles being traversed.
       *  \remarks This function is called from the framework whenever an AnimatedTriangles is
       *  encountered on traversal. The AnimatedTriangles \a quads is already write-locked.\n
       *  The base implementation first traverses the associated VNVectorAnimation, and then calls
       *  traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleAnimatedTriangles( nvsg::AnimatedTriangles * triangles );

      /*! \brief Handler function for a SkinnedTriangles.
       *  \param triangles A pointer to the write-locked Triangles being traversed.
       *  \remarks This function is called from the framework whenever a SkinnedTriangles is
       *  encountered on traversal. The SkinnedTriangles \a triangles is already write-locked.\n
       *  The base implementation first traverses each TrafoAnimation of each bone of each skin, and
       *  then calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleSkinnedTriangles( nvsg::SkinnedTriangles * triangles );

      /*! \brief Handler function for a Lines.
       *  \param lines A pointer to the write-locked Lines being traversed.
       *  \remarks This function is called from the framework whenever a Lines is encountered on
       *  traversal. The Lines \a lines is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleLines( nvsg::Lines * lines );

      /*! \brief Handler function for a Points.
       *  \param points A pointer to the write-locked Points being traversed.
       *  \remarks This function is called from the framework whenever a Points is encountered on
       *  traversal. The Points \a points is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handlePoints( nvsg::Points * points );

      /*! \brief Handler function for a QuadsMeshes.
       *  \param meshes A pointer to the write-locked QuadsMeshes being traversed.
       *  \remarks This function is called from the framework whenever a QuadsMeshes is encountered
       *  on traversal. The QuadsMeshes \a meshes is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleQuadMeshes( nvsg::QuadMeshes * meshes );

      /*! \brief Handler function for a QuadStrips.
       *  \param strips A pointer to the write-locked QuadStrips being traversed.
       *  \remarks This function is called from the framework whenever a QuadStrips is encountered
       *  on traversal. The QuadStrips \a strips is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleQuadStrips( nvsg::QuadStrips * strips );

      /*! \brief Handler function for a TriFans.
       *  \param fans A pointer to the write-locked TriFans being traversed.
       *  \remarks This function is called from the framework whenever a TriFans is encountered on
       *  traversal. The TriFans \a fans is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleTriFans( nvsg::TriFans * fans );

      /*! \brief Handler function for a TriStrips.
       *  \param strips A pointer to the write-locked TriStrips being traversed.
       *  \remarks This function is called from the framework whenever a TriStrips is encountered on
       *  traversal. The TriStrips \a strips is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleTriStrips( nvsg::TriStrips * strips );

      /*! \brief Handler function for a LineStrips.
       *  \param strips A pointer to the write-locked LineStrips being traversed.
       *  \remarks This function is called from the framework whenever a LineStrips is encountered
       *  on traversal. The LineStrips \a strips is already write-locked.\n
       *  The base implementation just calls traversePrimitiveSet().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleLineStrips( nvsg::LineStrips * strips );

      /*! \brief Handler function for a Shape.
       *  \param shape A pointer to the write-locked Shape being traversed.
       *  \remarks This function is called from the framework whenever a Shape is encountered on
       *  traversal. The Shape \a shape is already write-locked.\n
       *  The base implementation just calls Traverser::traverseShape().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseShape */
      NVSG_API virtual void handleShape( nvsg::Shape * shape );

      /*! \brief Handler function for a StateSet.
       *  \param stateSet A pointer to the write-locked StateSet being traversed.
       *  \remarks This function is called from the framework whenever a StateSet is encountered on
       *  traversal. The StateSet \a stateSet is already write-locked.\n
       *  The base implementation just traverses the active StateVariant.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa StateVariant */
      NVSG_API virtual void handleStateSet( nvsg::StateSet * stateSet );

      /*! \brief Handler function for a StateVariant.
       *  \param variant A pointer to the write-locked StateVariant being traversed.
       *  \remarks This function is called from the framework whenever a StateVariant is encountered
       *  on traversal. The StateVariant \a variant is already write-locked.\n
       *  The base implementation just call Traverser::traverseStateVariant().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseStateVariant */
      NVSG_API virtual void handleStateVariant( nvsg::StateVariant * variant );

      /*! \brief Handler function for a StatePass.
       *  \param pass A pointer to the write-locked StatePass being traversed.
       *  \remarks This function is called from the framework whenever a StateSet is encountered on
       *  traversal. The StatePass \a pass is already write-locked.\n
       *  The base implementation just call Traverser::traverseStatePass().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseStatePass */
      NVSG_API virtual void handleStatePass( nvsg::StatePass * pass );

      /*! \brief Handler function for a CgFx.
       *  \param cgfx A pointer to the write-locked CgFx being traversed.
       *  \remarks This function is called from the framework whenever a CgFx is encountered on
       *  traversal. The CgFx \a cgfx is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleCgFx( nvsg::CgFx * cgfx );

      /*! \brief Handler function for a Material.
       *  \param material A pointer to the write-locked Material being traversed.
       *  \remarks This function is called from the framework whenever a Material is encountered on
       *  traversal. The Material \a material is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleMaterial( nvsg::Material * material );

      /*! \brief Handler function for a TextureAttribute.
       *  \param texAttrib A pointer to the write-locked TextureAttribute being traversed.
       *  \remarks This function is called from the framework whenever a TextureAttribute is
       *  encountered on traversal. The TextureAttribute \a texAttrib is already write-locked.\n
       *  The base implementation just calls Traverser::traverseTextureAttribute().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseTextureAttribute */
      NVSG_API virtual void handleTextureAttribute( nvsg::TextureAttribute * texAttrib );

      /*! \brief Handler function for a TextureAttributeItem.
       *  \param texAttribItem A pointer to the write-locked TextureAttributeItem being traversed.
       *  \param tu Index of the texture unit to use.
       *  \remarks This function is called from the framework whenever a TextureAttributeItem is
       *  encountered on traversal. The TextureAttributeItem \a texAttribItem is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleTextureAttributeItem( nvsg::TextureAttributeItem * texAttribItem
                                                      , size_t tu );

      /*! \brief Handler function for a BlendAttribute.
       *  \param blendAttrib A pointer to the write-locked BlendAttribute being traversed.
       *  \remarks This function is called from the framework whenever a BlendAttribute is
       *  encountered on traversal. The BlendAttribute \a blendAttrib is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleBlendAttribute( nvsg::BlendAttribute * blendAttrib );

      /*! \brief Handler function for a DepthAttribute.
       *  \param depthAttrib A pointer to the write-locked DepthAttribute being traversed.
       *  \remarks This function is called from the framework whenever a DepthAttribute is
       *  encountered on traversal. The DepthAttribute \a depthAttrib is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleDepthAttribute( nvsg::DepthAttribute * depthAttrib );

      /*! \brief Handler function for a FaceAttribute.
       *  \param faceAttrib A pointer to the write-locked FaceAttribute being traversed.
       *  \remarks This function is called from the framework whenever a FaceAttribute is
       *  encountered on traversal. The FaceAttribute \a faceAttrib is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleFaceAttribute( nvsg::FaceAttribute * faceAttrib );

      /*! \brief Handler function for a LineAttribute.
       *  \param lineAttrib A pointer to the write-locked LineAttribute being traversed.
       *  \remarks This function is called from the framework whenever a LineAttribute is
       *  encountered on traversal. The LineAttribute \a lineAttrib is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleLineAttribute( nvsg::LineAttribute * lineAttrib );

      /*! \brief Handler function for a PointAttribute.
       *  \param pointAttrib A pointer to the write-locked PointAttribute being traversed.
       *  \remarks This function is called from the framework whenever a PointAttribute is
       *  encountered on traversal. The PointAttribute \a pointAttrib is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handlePointAttribute( nvsg::PointAttribute * pointAttrib );

      /*! \brief Handler function for a MapArray.
       *  \param mapArray A pointer to the write-locked MapArray being traversed.
       *  \remarks This function is called from the framework whenever a MapArray is encountered on
       *  traversal. The MapArray \a mapArray is already write-locked.\n
       *  The base implementation just calls Traverser::traverseMapArray().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseMapArray */
      NVSG_API virtual void handleMapArray( nvsg::MapArray * mapArray );

      /*! \brief Handler function for a MapElement.
       *  \param mapElement A pointer to the write-locked MapElement being traversed.
       *  \remarks This function is called from the framework whenever a MapElement is
       *  encountered on traversal. The MapElement \a mapElement is already write-locked.\n
       *  The base implementation just traverses the Map of the MapElement.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa Map */
      NVSG_API virtual void handleMapElement( nvsg::MapElement * mapElement );

      /*! \brief Handler function for a MapMatrix.
       *  \param mapMatrix A pointer to the write-locked MapMatrix being traversed.
       *  \remarks This function is called from the framework whenever a MapMatrix is encountered on
       *  traversal. The MapMatrix \a mapMatrix is already write-locked.\n
       *  The base implementation just calls Traverser::traverseMapMatrix().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseMapMatrix */
      NVSG_API virtual void handleMapMatrix( nvsg::MapMatrix * mapMatrix );

      /*! \brief Handler function for a MapList.
       *  \param mapList A pointer to the write-locked MapList being traversed.
       *  \remarks This function is called from the framework whenever a MapList is encountered on
       *  traversal. The MapList \a mapList is already write-locked.\n
       *  The base implementation just calls Traverser::traverseMapList().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseMapList */
      NVSG_API virtual void handleMapList( nvsg::MapList * mapList );

      /*! \brief Handler function for a Bitmap.
       *  \param bm A pointer to the write-locked Bitmap being traversed.
       *  \remarks This function is called from the framework whenever a Bitmap is encountered on
       *  traversal. The Bitmap \a bm is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleBitmap( nvsg::Bitmap * bm );

      /*! \brief Handler function for a VertexAttributeSet.
       *  \param vas A pointer to the write-locked VertexAttributeSet being traversed.
       *  \remarks This function is called from the framework whenever a VertexAttributeSet is
       *  encountered on traversal. The VertexAttributeSet \a vas is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleVertexAttributeSet( nvsg::VertexAttributeSet *vas );

      /*! \brief Handler function for a FramedAnimation<nvmath::Trafo>.
       *  \param trafo A pointer to the write-locked FramedAnimation<nvmath::Trafo> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  FramedAnimation<nvmath::Trafo> is encountered on traversal. The
       *  FramedAnimation<nvmath::Trafo> \a trafo is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleFramedTrafoAnimation( nvsg::FramedAnimation<nvmath::Trafo> * trafo );

      /*! \brief Handler function for a FramedAnimation<nvsg::VNVector>.
       *  \param vnVector A pointer to the write-locked FramedAnimation<nvsg::VNVector> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  FramedAnimation<nvsg::VNVector> is encountered on traversal. The
       *  FramedAnimation<nvsg::VNVector> \a vnVector is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleFramedVNVectorAnimation( nvsg::FramedAnimation<nvsg::VNVector> * vnVector );

      /*! \brief Handler function for a LinearInterpolatedAnimation<nvmath::Trafo>.
       *  \param trafo A pointer to the write-locked LinearInterpolatedAnimation<nvmath::Trafo> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  LinearInterpolatedAnimation<nvmath::Trafo> is encountered on traversal. The
       *  LinearInterpolatedAnimation<nvmath::Trafo> \a trafo is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleLinearInterpolatedTrafoAnimation( nvsg::LinearInterpolatedAnimation<nvmath::Trafo> * trafo );

      /*! \brief Handler function for a LinearInterpolatedAnimation<nvsg::VNVector>.
       *  \param vnVector A pointer to the write-locked LinearInterpolatedAnimation<nvsg::VNVector> being traversed.
       *  \remarks This function is called from the framework whenever a
       *  LinearInterpolatedAnimation<nvsg::VNVector> is encountered on traversal. The
       *  LinearInterpolatedAnimation<nvsg::VNVector> \a vnVector is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void handleLinearInterpolatedVNVectorAnimation( nvsg::LinearInterpolatedAnimation<nvsg::VNVector> * vnVector );

      /*! \brief Handler function for a HighlightBox.
       *  \param hlBox A pointer to the write-locked HighlightBox being traversed.
       *  \param pset A pointer to the write-locked PrimitiveSet, that \a hlBox is associated to.
       *  \remarks This function is called from the framework whenever a HighlightBox is
       *  encountered on traversal. The HighlightBox \a hlBox is already write-locked.\n
       *  The base implementation just calls traverseHighlightObject().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseHighlightObject */
      NVSG_API virtual void handleHighlightBox( nvsg::HighlightBox * hlBox
                                              , const nvsg::PrimitiveSet * pset );

      /*! \brief Handler function for a HighlightEdges.
       *  \param hlEdges A pointer to the write-locked HighlightEdges being traversed.
       *  \param pset A pointer to the write-locked PrimitiveSet, that \a hlEdges is associated to.
       *  \remarks This function is called from the framework whenever a HighlightEdges is
       *  encountered on traversal. The HighlightEdges \a hlEdges is already write-locked.\n
       *  The base implementation just calls traverseHighlightObject().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseHighlightObject */
      NVSG_API virtual void handleHighlightEdges( nvsg::HighlightEdges * hlEdges
                                                , const nvsg::PrimitiveSet * pset );

      /*! \brief Handler function for a VolumeNode.
       *  \param vnode A pointer to the write-locked VolumeNode being traversed.
       *  \remarks This function is called from the framework whenever a VolumeNode is encountered
       *  on traversal. The VolumeNode \a vnode is already write-locked.\n
       *  The base implementation just calls Traverser::traverseVolumeNode().
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa traverseVolumeNode */
      NVSG_API virtual void handleVolumeNode( nvsg::VolumeNode * vnode );

      /*! \brief Function for common handling of Camera classes.
       *  \param camera A pointer to the write-locked Camera being traversed.
       *  \remarks This function is called from the framework whenever a Camera is encountered
       *  on traversal. The Camera \a camera is already write-locked.\n
       *  The base implementation traverses the headlights of \a camera.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa LightSource */
      NVSG_API virtual void traverseCamera( nvsg::Camera * camera );

      /*! \brief Function for common handling of LightSource classes.
       *  \param light A pointer to the write-locked LightSource being traversed.
       *  \remarks This function is called from the framework whenever a LightSource is encountered
       *  on traversal. The LightSource \a light is already write-locked.\n
       *  The base implementation traverses the Animation associated with \a light, if there is one.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa TrafoAnimation */
      NVSG_API virtual void traverseLight( nvsg::LightSource * light );

      /*! \brief Function for common handling of PrimitiveSet classes.
       *  \param pset A pointer to the write-locked PrimitiveSet being traversed.
       *  \remarks This function is called from the framework whenever a PrimitiveSet is encountered
       *  on traversal. The PrimitiveSet \a pset is already write-locked.\n
       *  The base implementation traverses the VertexAttributeSet of \a pset, if there is one. If
       *  \a pset is highlighted, the associated HighlightObject is traversed.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal.
       *  \sa VertexAttributeSet, HighlightObject */
      NVSG_API virtual void traversePrimitiveSet( nvsg::PrimitiveSet * pset );

      /*! \brief Function for common handling of HighlightObject classes.
       *  \param hlObject A pointer to the write-locked HighlightObject being traversed.
       *  \param pset A pointer to the read-locked PrimitiveSet to be highlighted.
       *  \remarks This function is called from the framework whenever a HighlightObject is
       *  encountered on traversal. The HighlightObject \a hlObject is already write-locked.\n
       *  The base implementation just does nothing.
       *  \note When this function is overloaded by a traverser derived from ExclusiveTraverser, it
       *  is recommended to always call the base class implementation in order to properly continue
       *  traversal. */
      NVSG_API virtual void traverseHighlightObject( nvsg::HighlightObject * hlObject
                                                   , const nvsg::PrimitiveSet * pset );

    protected:
      nvsg::Scene * m_lockedScene;          //!< A pointer to the write-locked Scene currently traversed.
      nvsg::ViewState * m_lockedViewState;  //!< A pointer to the write-locked ViewState used in the current traversal.
  };


  //--- Inline functions ------------------

  inline void Traverser::interruptTraversal()
  {
    NVSG_TRACE();
    m_interrupt = true;
  }

  inline bool Traverser::wasTraversalInterrupted()
  {
    NVSG_TRACE();
    return !!(m_traverserBits & NVSG_TRAVERSAL_INTERRUPTED);
  }

  inline void Traverser::traverseObject(nvsg::ObjectHandle * obj)
  {
    NVSG_TRACE();
    if ( m_interrupt )
    {
      // there was an interrupt request.
      // directly quit traversal
      m_traverserBits |= NVSG_TRAVERSAL_INTERRUPTED;
      return;
    }
    traverseLockedObject(lockObject(obj));
    unlockObject(obj);
  }

  template <typename T>
  inline void Traverser::traverseObject(nvsg::ObjectHandle * obj, T arg)
  {
    NVSG_TRACE();
    if ( m_interrupt )
    {
      // there was an interrupt request.
      // directly quit traversal
      m_traverserBits |= NVSG_TRAVERSAL_INTERRUPTED;
      return;
    }
    traverseLockedObject(lockObject(obj), arg);
    unlockObject(obj);
  }

  inline void Traverser::traverseLockedObject(const nvsg::Object * obj)
  {
    unsigned int oc = getObjectTraversalCode( obj );
    if ( nvsg::OC_INVALID != oc )
    {
#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      void * __esp__0;
      void * __esp__1;
      __asm mov __esp__0, esp
#endif
        (this->*(void (Traverser::*)(const nvsg::Object*))m_mftbl[oc])(obj);
#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      __asm mov __esp__1, esp
      NVSG_ASSERT(__esp__1==__esp__0);
#endif
    }
  }

  template <typename T>
  inline void Traverser::traverseLockedObject(const nvsg::Object * obj, T arg)
  {
    unsigned int oc = getObjectTraversalCode( obj );
    if ( nvsg::OC_INVALID != oc )
    {
#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      void * __esp__0;
      void * __esp__1;
      __asm mov __esp__0, esp
#endif
        (this->*(void (Traverser::*)(const nvsg::Object*,T))m_mftbl[oc])(obj, arg);
#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      __asm mov __esp__1, esp
      NVSG_ASSERT(__esp__1==__esp__0);
#endif
    }
  }

  template <typename T, typename U> 
  inline void SharedTraverser::addObjectHandler(unsigned int objectCode, void (T::*handler)(const U*))
  {
    NVSG_CTASSERT(SUPERSUBCLASS(SharedTraverser,T));
    m_mftbl.addEntry(objectCode, handler);
  }

  template <typename T, typename U, typename V> 
  inline void SharedTraverser::addObjectHandler(unsigned int objectCode, void (T::*handler)(const U*, V))
  {
    NVSG_CTASSERT(SUPERSUBCLASS(SharedTraverser,T));
    m_mftbl.addEntry(objectCode, handler);
  }

  template <typename T, typename U> 
  inline void ExclusiveTraverser::addObjectHandler(unsigned int objectCode, void (T::*handler)(U*))
  {
    NVSG_CTASSERT(SUPERSUBCLASS(ExclusiveTraverser,T));
    m_mftbl.addEntry(objectCode, handler);
  }

  template <typename T, typename U, typename V> 
  inline void ExclusiveTraverser::addObjectHandler(unsigned int objectCode, void (T::*handler)(U*, V))
  {
    NVSG_CTASSERT(SUPERSUBCLASS(ExclusiveTraverser,T));
    m_mftbl.addEntry(objectCode, handler);
  }

} // namespace nvtraverser}
