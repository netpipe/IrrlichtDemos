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

#include <vector>
#include <map>
#include "nvutil/Singleton.h"

namespace nvsg
{
class DALServer;
class DALData;

/*! \brief Handle to a Device Abstraction Link (DAL) */
typedef unsigned long HDAL;
/*! \brief Indicates a invalid handle value */
extern const HDAL HDAL_INVALID; 

/*! \brief Represents an object capable to host a Device Abstraction Link
  * \par Namespace: nvsg
  * \remarks
  * A DALHost is intended to host a Device Abstraction Link (DAL) to attach arbitrary DALData.
  * A DAL links device or platform dependent DALData created by a DALDataCreator to an independent 
  * DALHost.
  * \n\n
  * An Object, for example, 'is a' DALHost.
  */
class DALHost
{
#if !defined(DOXYGEN_IGNORE)
  friend class DALServer;
#endif

public:
  /*! \brief Stores Device Abstraction Link Data.
    * \param
    * dataID Identifies the kind of data to store. This is so the user can identify the kind
    * of data, especially when stored data will be retrieved later.
    * \param
    * data Pointer to the DALData to store.
    * \return
    * The function returns a handle that identifies the Device Abstraction Link the stored data is
    * associated with. If the function fails, HDAL_INVALID will be returned.
    * \remarks
    * The function attaches the DALData identified by \a dataID, and pointed to by \a data. 
    * Different DALDataCreators can store data with the same \a dataID. In turn, a certain 
    * DALDataCreator can retrieve DALData stored by a different DALDataCreator. That is, two or
    * more DALDataCreators can share DALData if appropriate. For example, a particular OpenGL
    * renderer (nvutil::GLTraverser) might use OpenGL specific texture information that was stored
    * to a certain TextureImage by another OpenGL renderer, provided the two renderers share their
    * render contexts.
    * \n\n
    * Use DALHost::getDeviceAbstractionLinkData to retrieve particular data from the DALHost.
    * \n\n
    * The handle to the Device Abstraction Link (DAL) that will be returned by the function must be 
    * stored by the DALDataCreator. This handle is used, for example, to communicate to the 
    * DALDataCreator that a certain DAL hosted by a DALHost has been destroyed, and hence, stored 
    * DALData has been released as a consequence (DALDataCreator::onReleaseDAL). Or, in turn, if a 
    * DALDataCreator needs to dismiss stored DALData, this handle is used to identify the DAL that
    * is associated to that particular data. 
    * \n\n
    * The behavior is undefined if invalid DALData is passed. The behavior also is undefined if
    * the DALData pointed to by \a data already is hosted by another host!
    * \sa DALHost::getDeviceAbstractionLinkData, DALData, DALDataCreator
    */
  NVSG_API HDAL storeDeviceAbstractionLinkData(unsigned int dataID, DALData * data) const;
  
  /*! \brief Releases Device Abstraction Link data.
    * \remarks
    * The function releases the host's internal Device Abstraction link with all DALData attached 
    * to it. In turn, the particular DALDataCreator will be informed that the DALData they stored
    * with this host became invalid, so the creator can dismiss the stored handles they received
    * at the time they stored the data using DALHost::storeDeviceAbstractionLinkData.
    * \sa DALHost::storeDeviceAbstractionLinkData
    */
  NVSG_API void releaseDeviceAbstractionLinkData();
  
  /*! \brief Retrieves Devices Abstraction Link data from the DALHost.
    * \param
    * dataID Identifies the kind of data to retrieve.
    * \param
    * cache Reference to a pointer to DALData that receives the pointer to the stored DALData. 
    * \param
    * test A test function of the form 'bool test(const DALDataType*)' to be applied on the 
    * requested DALData.
    * \return
    * The function returns \c true if the DALData could have been retrieved successfully.
    * Otherwise, the function returns \c false.
    * \remarks
    * DALDataCreators can use this function to retrieve back DALData they previously stored using
    * DALHost::storeDeviceAbstractionLinkData. For this, a creator needs to pass the same \a dataID
    * as used with DALHost::storeDeviceAbstractionLinkData. In addition, the creator must specify a
    * test function of the form 'bool test(const DALDataType *)'. 
    * \n\n
    * The function looks up the DALData by the \a dataID and, if found, passes that data to the 
    * \a test function. If the test function returns \c true, the pointer to the particular DALData 
    * will be assigned to \a cache and the function returns \c true. 
    * \n\n
    * The reason why the test function is needed here is so that different creators can store 
    * different data having the same data ID. The test function verifies that specific data
    * is the right data the caller is looking for. A concrete example would be some texture 
    * information that was created by one renderer and used by some other renderer.
    * \sa DALHost::getLastRecentlyUsedDeviceAbstractionLinkData
    */
  template<typename DALDataType, typename TestFunc>
  bool getDeviceAbstractionLinkData(unsigned int dataID, DALDataType *& cache, TestFunc test) const;

  /*! \brief Returns the Device Abstraction Link data that was queried last.
    * \return
    * The function returns a pointer to the DALData that was successfully queried last.
    * \remarks
    * If particular, DALData could have queried successfully using 
    * DALHost::getDeviceAbstractionLinkData, so the pointer to that data is cached as the most recently
    * used data. For performance reasons, it is recommended to always call this function prior to
    * DALHost::getDeviceAbstractionLinkData with DALData queries.
    * \par Example:
    * The following code example shows how the nvtraverser::GLTraverser queries a given 
    * nvsg::PrimitiveSet for a probably attached display list information structure.
    * \code
    * // isDLInfoValid is used as test function while querying DLInfo data attached to a DALHost,
    * // which in this particular case is a PrimitiveSet
    * bool GLTraverser::isDLInfoValid(const GLTraverser::DLInfo* info)
    * { // a DLInfo is valid if the name of the display list object
    *   // is known by the current GL render context
    *   return glContext==info->context // we are fine if the current context corresponds the info's context
    *         || !!GLIsList(info->listID); // if not, we need to go deeper
    * }
    *
    * // getCachedDLInfo queries a given PrimitiveSet for DLInfo that might be attached to the 
    * // PrimitiveSet earlier
    * GLTraverser::DLInfo * GLTraverser::getCachedDLInfo( const PrimitiveSet *p ) const
    * {
    *   DLInfo * dlInfo = dynamic_cast<DLInfo*>(p->getLastRecentlyUsedDeviceAbstractionLinkData());
    *
    *   if ( !dlInfo || !isDLInfoValid(dlInfo) )
    *   { 
    *     // the last recently queried data is not valid for us, we need to go deeper
    *     p->getDeviceAbstractionLinkData(GL_DL, dlInfo, &GLTraverser::isDLInfoValid);
    *   }
    *   return( dlInfo );
    * }
    * \endcode
    * \sa DALHost::getDeviceAbstractionLinkData
    */
  NVSG_API DALData * getLastRecentlyUsedDeviceAbstractionLinkData() const;

#if !defined(DOXYGEN_IGNORE)
public:
  DALHost();
  DALHost(const DALHost&);
  ~DALHost();
protected:
  DALHost & operator=(const DALHost & rhs) {return *this;}
private:
  mutable HDAL m_hDAL;
  mutable DALData * m_lastRecentlyUsed;
#endif
};

/*! \brief Represents the interface required for a creator of DALData.
  * \par Namespace: nvsg
  * \remarks 
  * Objects that create DALData to attach it to a Device Abstraction Link (DAL) hosted by a DALHost
  * must derive from this base class.
  */
class DALDataCreator
{
public:
#if !defined(DOXYGEN_IGNORE)
  virtual ~DALDataCreator() {}
#endif
  /*! \brief Called from the framework immediately before the Device Abstraction Link (DAL) becomes 
    * invalid.
    * \param
    * hDAL Handle to the Device Abstraction Link that is about to become invalid.
    * \remarks
    * Called from the framework to inform creators that the DAL identified by \a hDAL immediately 
    * becomes invalid. For example, this is the case when the object that hosts the DAL will be 
    * deleted. 
    * \n\n
    * A concrete DALDataCreator usually overrides this function to dismiss all temporary stored 
    * handles to the DALs that the DALDataCreator has attached DALData to. A DAL handle will be returned
    * to a DALDataCreator when it attaches DALdata to the DALHost through
    * DALHost::storeDeviceAbstractionLinkData. 
    * \n\n
    * Note that the creator must not explicitly delete the attached data! The deletion of the 
    * attached data will be performed by the framework, immediately after this function was called.
    */
  virtual void onReleaseDAL(HDAL hDAL) {}

protected:
#if !defined(DOXYGEN_IGNORE)
  // DALDataCreator serves as base class only. 
  DALDataCreator() {}
#endif

  /*! \brief Releases Device Abstraction Link Data.
    * \param
    * hDAL Handle to the Device Abstraction Link the data to release is attached to.
    * \param
    * dataID Identifies the kind of data to release.
    * \remarks
    * Releases the data identified by \a dataID that was attached to the 
    * Device Abstraction Link identified by \a hDAL.
    * \n\n
    * This function should be called by the creator immediately before this creator becomes invalid, 
    * to free all the data of kind \a dataID that was created by this DALDataCreator and attached 
    * to the DAL identified by \a hDAL. The function deletes the data and removes the according 
    * entry from the DAL. 
    */
  NVSG_API void releaseDeviceAbstractionLinkData(HDAL hDAL, unsigned int dataID);
};

/*! \brief Device Abstraction Link Data.
  * \par Namespace: nvsg
  * \remarks
  * DALData can be used to abstract device dependent data that will be linked to device independent
  * data via a Device Abstraction Link (DAL). 
  * \n\n
  * Note that only a DALDataCreator is permitted to create DALData. DALData can be attached to a DAL
  * using the interface of a DALHost.
  * \sa DALHost, DALDataCreator 
  */
class DALData
{
#if !defined(DOXYGEN_IGNORE)
  friend class DALServer;
  friend class DALDataCreator;
  friend class DALHost;
protected:
  virtual ~DALData();
#endif

protected:
  /*! \brief Constructs a DALData object.
    * \param
    * creator Pointer to the DALDataCreator that creates, end hence, owns the data.
    * \remarks
    * After DALData has been created it can be attached to Device Abstraction Link hosted by a 
    * DALHost using DALHost::storeDeviceAbstractionLink.
    */
  DALData(DALDataCreator * creator);

  /*! \brief Induces the deletion of this DALData object.
    * \remarks
    * This function will be called from the DALServer framework when this DALData object is going 
    * to be released from a Device Abstraction Link hosted by a DALHost.
    * \n\n
    * The default implementation calls delete on this pointer. If a custom memory manager is 
    * used, in particular a memory manager that does not issue new and delete for creating and 
    * deleting a DALData object, it is recommended to override this function so that the deletion of 
    * the DALData object will be performed correctly. 
    */
  virtual void deleteThis() { delete this; }

#if !defined(DOXYGEN_IGNORE)
private:
  DALDataCreator * m_creator;
  const DALHost * m_host;
#endif
};

#if !defined(DOXYGEN_IGNORE)
// Device Abstraction Link Server - implementation details
class DALServer
{
  friend class DALDataCreator;
  friend class DALHost;

public:
  // Initializes the DALServer at instantiation
  DALServer();
  // Cleanup after deletion
  ~DALServer();
private:
  // Attaches the data identified by \a dataID and pointed to by \a data 
  // to the Device Abstraction Link (DAL) identified by \a hDAL.
  // If HDAL_INVALID is passed for \a hDAL, a new DAL will be created first.
  // \returns A handle to the Device Abstraction Link or DAL_INVALID if the function fails.
  HDAL storeDALData(HDAL hDAL, unsigned int dataID, DALData* data);
  // Releases the data identified by \a dataID that was attached to the DAL identified by \a hDAL 
  // by the creator pointed to by \a creator.
  // If 0xFFFFFFFF is passed for \a dataID, all data that was attached to the DAL identified by \a hDAL
  // by the creator pointed to by \a creator will be released.
  // If NULL is passed for \a creator, all data attached to the DAL identified by \a hDAL will be released
  // and the DAL will be deleted afterwards.
  void releaseDALData(HDAL hDAL, DALDataCreator * creator=NULL, unsigned int dataID=0xFFFFFFFF);
  // Provides all addresses of the data objects identified by \a dataID that are currently stored 
  // with the DAL identified by \a hDAL. 
  // The addresses of the data objects will be copied to the vector referenced by \a data.
  bool getDALData(HDAL hDAL, unsigned int dataID, std::vector<DALData*>& data) const;

  // Calls delete this on the data pointer passed 
  void deleteDALData(DALData * data);

private:
  std::map<HDAL, std::multimap<unsigned int, DALData*> > m_DALMap; // map of DALs
  std::vector<HDAL> m_freeHDALs; // free DAL handles can be re-used
  HDAL m_hDALNext; // use this if no handle is available for re-use
};

typedef nvutil::Singleton<DALServer> DALS;
#endif // DOXYGEN_IGNORE

inline HDAL DALHost::storeDeviceAbstractionLinkData(unsigned int dataID, DALData * data) const
{
  NVSG_ASSERT(data); // do not like bad data
  NVSG_ASSERT(data->m_host==NULL); // data must not be hosted by anyone else
  m_hDAL = DALS::instance()->storeDALData(m_hDAL, dataID, data);
  if ( HDAL_INVALID != m_hDAL )
  {
    data->m_host = this;  
    m_lastRecentlyUsed = data;
  }
  return m_hDAL;
}

inline void DALHost::releaseDeviceAbstractionLinkData()
{
  if ( m_hDAL != HDAL_INVALID )
  {
    DALS::instance()->releaseDALData(m_hDAL);
    m_hDAL = HDAL_INVALID;
    m_lastRecentlyUsed = NULL;
  }
}

template <typename DALDataType, typename TestFunc> 
inline bool DALHost::getDeviceAbstractionLinkData(unsigned int dataID, DALDataType *& data, TestFunc test) const
{
  std::vector<DALData*> caches;
  if ( DALS::instance()->getDALData(m_hDAL, dataID, caches) )
  {
    for ( std::vector<DALData*>::iterator i = caches.begin(); i!=caches.end(); ++i )
    {
      data = dynamic_cast<DALDataType*>(*i);
      if ( data && test(data) )
      {
        m_lastRecentlyUsed = data;
        return true;
      }
    }
  }
  return false;
}

inline DALData * DALHost::getLastRecentlyUsedDeviceAbstractionLinkData() const
{
  return m_lastRecentlyUsed;
}

inline void DALDataCreator::releaseDeviceAbstractionLinkData(HDAL hDAL, unsigned int dataID)
{
  NVSG_ASSERT(hDAL != HDAL_INVALID);
  DALS::instance()->releaseDALData(hDAL, this, dataID);
}

} // namespace nvsg
