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


#include "nvsgcommon.h" // commonly used stuff
#include "nvsg/CoreTypes.h" // core type declarations and typedefs
#include "nvsg/DAL.h" // base class definition
// more dependencies
#include "nvutil/Incarnation.h"
#include "nvutil/SWMRSync.h"
#include  <typeinfo>

namespace nvsg
{
  /*! \brief 64-bit save DataID.
    * \remarks
    * The DataID is used to uniquely identify an Object's embedded data. 
    * \sa Object::getDataID
    */
  typedef unsigned long long DataID;

  /*! \brief Object Codes for NVSG Object Type Identification 
    * \remarks
    * For a custom traverser it is essential to identify all actual scene graph components while 
    * traversing the scene graph, and to call the corresponding handler routine for a particular 
    * object. For this reason, all concrete NVSG object types must be assigned to a unique object 
    * code. Secure object codes for custom defined NVSG objects start at OC_CUSTOMOBJECT.
    * \note Choosing an object code lower than OC_CUSTOMOBJECT for a custom-defined NVSG object 
    * results in undefined runtime behavior!
    * \sa \ref howtoderiveatraverser \ref howtoderiveanvsgobject 
    */
  enum
  {
    OC_INVALID = 0xFFFFFFFF //!< invalid object code
  , OC_OBJECT = 0           //!< Object
  // nodes (0x00-0x1F)
  , OC_NODE = OC_OBJECT     //!< Node = 0x00 
  , OC_GEONODE              //!< GeoNode
  , OC_GROUP                //!< Group
  , OC_LOD                  //!< LOD
  , OC_SWITCH               //!< Switch
  , OC_TRANSFORM            //!< Transform
  , OC_ANIMATEDTRANSFORM    //!< AnimatedTransform
  , OC_DIRECTEDLIGHT        //!< DirectedLight
  , OC_POINTLIGHT           //!< PointLight
  , OC_SPOTLIGHT            //!< SpotLight
  , OC_BILLBOARD            //!< Billboard
  , OC_VOLUMENODE           //!< VolumeNode
  , OC_VOLUMEBRICK          //!< VolumeBrick
  // drawables (0x20-0x3F)
  , OC_DRAWABLES = 0x20     //!< Drawable = 0x20
  , OC_VERTEX_ATTRIBUTE_SET //!< VertexAttributeSet
  , OC_QUADS                //!< Quads
  , OC_ANIMATEDQUADS        //!< AnimatedQuads
  , OC_QUADSTRIPS           //!< QuadStrips
  , OC_TRIANGLES            //!< Triangles
  , OC_ANIMATEDTRIANGLES    //!< AnimatedTriangles
  , OC_SKINNEDTRIANGLES     //!< SkinnedTriangles
  , OC_TRISTRIPS            //!< TriStrips
  , OC_LINES                //!< Lines
  , OC_LINESTRIPS           //!< LineStrips
  , OC_TRIFANS              //!< TriFans
  , OC_POINTS               //!< Points
  , OC_SHAPE                //!< Shape
  , OC_BITMAP               //!< Bitmap
  , OC_PIXMAP               //!< Pixmap
  , OC_MAPELEMENT           //!< MapElement
  , OC_MAPARRAY             //!< MapArray
  , OC_MAPMATRIX            //!< MapMatrix
  , OC_MAPLIST              //!< MapList
  , OC_QUADMESHES           //!< QuadMeshes
  // states (0x40-0x5F)
  , OC_STATESET = 0x40      //!< StateSet = 0x40
  , OC_STATEVARIANT         //!< StateVariant
  , OC_STATEPASS            //!< StatePass
  , OC_STATEATTRIBUTE       //!< StateAttribute
  , OC_CGFX                 //!< CgFX
  , OC_FACEATTRIBUTE        //!< FaceAttribute
  , OC_MATERIAL             //!< Material
  , OC_TEXTUREATTRIBUTE     //!< TextureAttribute
  , OC_TEXTUREATTRIBUTEITEM //!< TextureAttributeItem
  , OC_STATESETEXT          //!< StateSet for default and override state set
  , OC_LINEATTRIBUTE        //!< LineAttribute
  , OC_POINTATTRIBUTE       //!< PointAttribute
  , OC_BLENDATTRIBUTE       //!< BlendAttribute
  , OC_DEPTHATTRIBUTE       //!< DepthAttribute
  // animations (0x60-0x7F) 
  , OC_ANIMATION = 0x60                       //!< Animation = 0x60
  , OC_FRAMEDTRAFOANIMATION                   //!< FramedTrafoAnimation
  , OC_FRAMEDVNVECTORANIMATION                //!< FramedVectorAnimation
  , OC_LINEARINTERPOLATEDTRAFOANIMATION       //!< LinearAnimatedTrafoAnimation
  , OC_LINEARINTERPOLATEDVNVECTORANIMATION    //!< LinearInterpolatedVNVectorAnimation
  // cameras (0x80-0x9F)  
  , OC_CAMERA = 0x80                          //!< Camera = 0x80              
  , OC_PARALLELCAMERA                         //!< ParallelCamera
  , OC_PERSPECTIVECAMERA                      //!< PerspectiveCamera
  // highlight objects (0xA0-0xBF)
  , OC_HIGHLIGHTOBJECT = 0xA0                 //!< HighlightObject = 0xA0
  , OC_HIGHLIGHTBOX                           //!< HighlightBox
  , OC_HIGHLIGHTEDGES                         //!< HighlightEdges
  // custom (0xC0 and higher)
  , OC_CUSTOMOBJECT = 0xC0                    //!< CustomObjects = 0xC0
  };

  /*! \brief Serves as base class for all traversable objects.
    * \par Namespace: nvsg
    * \remarks
    * Besides being reference counted, providing the read/write locking interface, and being host for
    * DAL data, an Object has an object code that identifies its concrete class type. Every concrete
    * class derived from Object has to set an unique object code in its constructors. This object code
    * is used in the Traverser to determine the concrete type of the Object. If there is a handler
    * function registered for that specific object code, this handler is called. Otherwise, a higher
    * level object code is queried, that also has to be provided by the concrete classes derived from
    * Object, until an object code is encountered a handler function is registered for. */
  class Object : public nvutil::HandledObject, public DALHost           // an object can host DAL data
  {
  public:
    /*! \brief Constructs an Object.
      */
    NVSG_API Object();

    /*! \brief Constructs an Object as a copy of another Object.
      */
    NVSG_API Object( const Object& rhs );

    /*! \brief Destructs an Object. 
      */
    NVSG_API virtual ~Object();

    /*! \brief Returns the object code, which is unique per object type.
      * \return
      * The function returns the object code enum for this object.
      * \remarks
      * The object code, which is unique per object type, is used for fast object type 
      * identification at runtime. A nvtraverser::Traverser, for example, uses the object code to 
      * fast lookup the handler function for a particular object type. 
      * \n\n
      * Object-derived classes must override the protected member 
      * \link nvsg::Object::m_objectCode m_objectCode \endlink, with the correct value for the
      * respective type. It is recommended to do this at instantiation time of the customized object.
      * \sa \ref howtoderiveanvsgobject, Object::getHigherLevelObjectCode
      */
    unsigned int getObjectCode() const;

    /*! \brief Returns the higher-level object code for a given object code.
      * \param
      * oc %Object code for which to retrieve the associated higher-level object code.
      * \return
      * The default implementation returns OC_INVALID, which causes the traverser framework to 
      * immediately proceed without handling the object when it comes across an unknown object code.
      * \remarks
      * This function will be called from the object traverser framework if the object code \a oc 
      * is unknown. This would be the case if for a custom object a certain traverser did not 
      * register an appropriate handler function.
      * \n\n
      * The framework expects this function to return a higher-level object code in terms of a 
      * custom object hierarchy. The function will be repeatedly called with the returned 
      * higher-level object code, until either a known object code or OC_INVALID will be returned. 
      * That is - by repeatedly calling this function, the traverser framework moves up the custom 
      * object hierarchy to find an object for which a handler function was registered. If the 
      * traverser framework recognizes a known object code, it calls the appropriate handler 
      * function and proceeds. If OC_INVALID was returned, the traverser framework proceeds without 
      * handling the object.
      * \n\n
      * The runtime behaviour is undefined if the function returns an object code of an object that 
      * is not part of the object hierarchy of the primary object!
      * \n\n
      * The framework might end up in a deadlock if the function never returns either OC_INVALID or 
      * an object code of a concrete object known by the traverser!
      * \sa Object::getObjectCode
      */
    NVSG_API virtual unsigned int getHigherLevelObjectCode(unsigned int oc) const; 

    /*! \brief Returns whether this object shares its embedded data with other objects.
      * \return
      * The function should return \c true if the object shares its embedded data with other 
      * objects. Otherwise, the function should return \c false.
      * \n\n
      * The default implementation always returns \c false.
      * \remarks
      * For Object-derived classes that are capable of sharing their embedded data with other 
      * objects, it is recommended to override this function. For all other objects, the default
      * implementation will be sufficient.
      * \n\n
      * An object, capable of data sharing, shares its data with another object if it was either 
      * instantiated as a copy of the other object, or if its content was re-assigned to by the 
      * content of the other object using the appropriate assignment operator.
      * \n\n
      * Only a few classes of the NVSG core implement data sharing. These mainly are classes where
      * data sharing probably could save a lot of memory because of the large amount of data these
      * classes might contain. VertexAttributeSet, IndependentPrimitiveSet, and StrippedPrimitiveSet
      * are examples of \ref nvsgcoreclasses that implement data sharing.
      * \sa Object::getDataID
      */
    NVSG_API virtual bool isDataShared() const;

    /*! \brief Returns the unique data identifier for this object.
      * \return
      * The function returns a 64-bit value which uniquely identifies the embedded data.
      * \remarks
      * This function in particular is useful to identify different objects that share their embedded
      * data. Two objects that share the same data always have the same DataID.
      * \n\n
      * Identifying objects that share their data with other objects is useful for saving objects
      * to a file and later reloading it without losing the data sharing. 
      * \sa Object::isDataShared
      */
    NVSG_API virtual DataID getDataID() const;

    /*! \brief Returns the current incarnation count of this object.
      * \return
      * The function returns a reference to an Incarnation object which is intended to be compared 
      * to a previously stored Incarnation count for this object.
      * \remarks
      * An altered incarnation generally implies that some related object data has changed. 
      * The ability to test an object Incarnation enables user code to do some conditional 
      * evaluation.
      * \sa Object::increaseIncarnation
      */
    NVSG_API const nvutil::Incarnation& getIncarnation() const;

    /*! \brief Increases the Incarnation count of this particular object.
      * \remarks
      * If a particular object gets modified, it must call this function for all owning objects to
      * increase the Incarnation counter of all objects up the hierarchy (bottom to top). This is
      * necessary to enable users to encounter changes without iterating down a tree hierarchy.
      * \sa Object::invalidateIncarnation, Object::validateIncarnation
      */
    NVSG_API void increaseIncarnation();

    /*! \brief Specifies the name for an object.
      * \param
      * name Reference to a STL sting holding the name to specify. 
      * \remarks
      * A previously specified name gets lost when specifying a new name using this function.
      * \n\n
      * Use Object::getName to retrieve the actual name of an Object.  
      */
    NVSG_API void setName(const std::string& name);

    /*! \brief Returns the name of the object.
      * \return
      * A reference to a STL string holding the name of the object. The function returns a reference
      * to an empty string if no name was previously specified for the object. 
      * \remarks
      * The function retrieves the object's name previously specified through Object::setName. 
      */
    NVSG_API const std::string& getName() const;

    /*! \brief Tests whether this object is equivalent to another object.  
      * \param
      * object Pointer to the object to test for equivalence with this object.
      * \param
      * ignoreNames Object names will be ignored while testing if this is \c true.
      * \param
      * deepCompare The function performs a deep-compare if this is \c true.
      * Otherwise the function only performs a shallow compare.
      * \return
      * The function returns \c true if the object pointed to by \a object is detected to be 
      * equivalent to this object. Otherwise the function returns \c false.
      * \remarks
      * The test will be performed considering the optional control parameters ignoreNames and deepCompare. 
      * If you omit these two, the function ignores object names
      * and performs a shallow compare only.
      */
    NVSG_API virtual bool isEquivalent( const Object * object
                                      , bool ignoreNames = true
                                      , bool deepCompare = false) const;

	  /*! \brief Attaches arbitrary user data to the object.
	   * \param 
     * userData Specifies the address of the arbitrary user data.
     * \remarks
     * Use setUserData to store the address to arbitrary user data with the object.
     * The object does not interpret the data, nor does it take over responsibility 
     * for managing the memory occupied by the data behind \a userData. It just keeps 
     * the address to the user data until the address will be overwritten by a 
     * subsequent call to setUserData.
     * \n\n
     * User data will not be considered for storing to a file! 
     * \n\n
     * Use getUserData to get back the address to the user data that was last specified 
     * through setUserData.
     * \sa getUserData
     */
	  NVSG_API void setUserData(const void* userData);
  	
	  /*! \brief Returns a pointer to the attached arbitrary user data.
     * \return
     * The function returns the pointer to the arbitrary user data that was last 
     * specified by a call to setUserData.
     * \sa setUserData
	   */
	  NVSG_API const void* getUserData() const; 

  protected: // accessible for derived classes

    /*! \brief Assigns new content from another Object. 
      * \param
      * rhs Reference to an Object from which to assign the new content.
      * \return
      * A reference to this object.
      * \remarks
      * The assignment operator unreferences the old content before assigning the new content. The new
      * content will be a deep-copy of the content of right-hand-sided object. 
      */
    NVSG_API Object & operator=(const Object & rhs);
  
    /*! \brief Marks the Incarnation of this Object as valid.
      * \remarks
      * For Object-derived classes that maintain a kind of parent-child relationship, the 
      * particular implementation need to validate the Incarnations from top to bottom.
      * \sa Object::invalidateIncarnation
      */
    NVSG_API virtual void validateIncarnation() const;


    /*! \brief Marks the Incarnation of this Object as invalid.
      * \remarks
      * If the Incarnation of the object is valid, the default implementation first increments the 
      * Incarnation count and then marks it as invalid.
      * \n\n
      * For Object-derived classes that maintain a kind of parent-child relationship, the 
      * particular implementation need to invalidate the Incarnations from bottom to top.
      * \sa Object::invalidateIncarnation
      */
    NVSG_API virtual void invalidateIncarnation();

    NVSG_API void enterCS() const;
    NVSG_API void leaveCS() const;

  protected:

    /*! \brief Per-object type identifier.
      * \remarks
      * Concrete derived objects must initialize this object code with an identifier,
      * which must be a unique per-object type. It is recommended to do this at object 
      * instantiation.
      * \n\n
      * NVSG uses this object code for fast object type identification at runtime.
      * \n\n
      * <B>Codes from 0x00 to 0xBF are reserved for NVSG core objects,
      * and may not be used for custom object types!</B>
      * \n\n
      * Codes for custom objects should start at OC_CUSTOMOBJECT:
      * \n\n
      * \code
      * // example for custom object code initialization
      *
      * enum
      * {
      *   OC_MYFIRSTOBJECT = nvsg::OC_CUSTOMOBJECT,
      *   OC_MYSECONDOBJECT, // = OC_MYFIRSTOBJECT + 1
      *   OC_MYTHIRDOBJECT.  // = OC_MYFIRSTOBJECT + 2
      * // ... further codes
      * };
      * \endcode
      */
    unsigned int m_objectCode;

    /*! \brief Provides exclusive access to mutable members.
     */

  private:
    std::string * m_name; // optional name

    // helps managing exclusive access to mutable members while reading the object 
    nvutil::SWMRSync m_lock;

    mutable bool m_incarnationValid; // incarnation's valid flag  
    nvutil::Incarnation  m_incarnation; // object's incarnation count 

  	const void * m_userData; // dumb pointer to arbitrary user data
  };

  inline unsigned int Object::getObjectCode() const
  {
    NVSG_TRACE();
    return m_objectCode;
  }

  inline void Object::setUserData(const void * userData)
  {
    NVSG_TRACE();
    m_userData = userData; // no sanity check!
  }

  inline const void * Object::getUserData() const
  {
    NVSG_TRACE();
    return m_userData;
  }

  inline void Object::enterCS() const
  {
    NVSG_TRACE();
    m_lock.lockExclusive();
  }

  inline void Object::leaveCS() const
  {
    NVSG_TRACE();
    m_lock.unlockExclusive();
  }

  //! Detects if the object pointed to by \a p is a pointer to a specified (templated) type.
  /** \returns \c true if \a p is a pointer to the specified type, \c false otherwise. */
  template<typename T> 
  inline bool isTypeOf( ObjectHandle *p )
  {
    return( dynamic_cast<T *>( p ) != NULL );
  }

  //! Detects if the two objects \a p0 and \a p1 are of the same type.
  /** \return \c true if \a p0 and \a p1 are of the same type, \c false otherwise. */
  inline bool areOfSameType( ObjectHandle *p0, ObjectHandle *p1 )
  { // found not initialized object codes if one of the following fires
    nvutil::ReadableObject<Object> obj0(p0);
    nvutil::ReadableObject<Object> obj1(p1);
    NVSG_ASSERT(obj0->getObjectCode()!=OC_INVALID);
    NVSG_ASSERT(obj1->getObjectCode()!=OC_INVALID);
    return( obj0->getObjectCode()==obj1->getObjectCode() );
  }

#if !defined(NDEBUG)
  template <typename T>
  inline void NVSG_ASSERT_OBJECT_WRITE_LOCKED(const T* obj)
  {
    nvutil::getHandle<nvutil::Handle<nvutil::EmptyType> >(obj)->assertWriteLocked();
  }
  template <typename T>
  inline void NVSG_ASSERT_OBJECT_READ_LOCKED(const T* obj)
  {
    nvutil::getHandle<nvutil::Handle<nvutil::EmptyType> >(obj)->assertReadLocked();
  }
#else
# define NVSG_ASSERT_OBJECT_WRITE_LOCKED(x) static_cast<void*>(0);
# define NVSG_ASSERT_OBJECT_READ_LOCKED(x) static_cast<void*>(0);
#endif

} //  namespace nvsg
