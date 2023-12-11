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
#include "nvsg/CoreTypes.h"

#include <map>
#include <set>

namespace nvsg 
{
  // forward declaration
  class ChangeCallback;

  /*! \brief CallBackServer for associating CallBacks with objects
    * \par Namespace: nvsg
    * \remarks  
    * This class (in conjunction with the ChangeCallBack) provides an interface
    * that allows the user to:
    *       -Trigger CallBacks on an Object 
    *                 and/or
    *       -Do Object-Object messaging (Subject-Observer pattern)
  */
  class CallbackServer
  {
  public:
    /*! \brief Constructs a CallBackServer 
    */
    NVSG_API CallbackServer();

    /*! \brief Destructs a CallBackServer 
    */
    NVSG_API ~CallbackServer();

    /*! \brief Register an ObjectHandle and associated ChangeCallback with the 
      * \param subjectHandle Handle to the subject Object
      * \param cb Pointer to the CallBack of the Subject
      * \sa CallbackServer::changed, CallbackServer::removeCallback
      * \remarks
      * Behavior is undefined if either or both the parameters are NULL
    */  
    NVSG_API void addCallback( ObjectHandle * subjectHandle, ChangeCallback* cb );

    /*! \brief Un-register an ObjectHandle and associated ChangeCallback with the 
      * \param subjectHandle Handle to the subject Object
      * \param cb Pointer to the CallBack of the Subject
      * \sa CallbackServer::changed, CallbackServer::addCallback
      * \remarks
      * Behavior is undefined if either or both the parameters are NULL
      */  
    NVSG_API void removeCallback( ObjectHandle * subjectHandle, ChangeCallback* cb );

    /*! \brief Trigger CallBacks on the ObjectHandle
      * \param subjectHandle Handle to the subject Object
      * \param userData Pointer to user defined data
      * \sa CallbackServer::addCallback, CallbackServer::removeCallback
      * \remarks
      * calling this function will trigger call the CallBack::changed method on all
      * the CallBacks registered for the SubjectHandle in the CallBackServer
    */  
    NVSG_API void changed( ObjectHandle * subjectHandle, const void* userData = NULL ) const;

  private:
    std::map< ObjectHandle*, std::set<ChangeCallback*> > m_objectsToChangeCallbacks; // map of ObjectHandles 
                                                                                     // to a set of their associated CallBacks

    // convenient type defines
    typedef std::map< ObjectHandle*, std::set<ChangeCallback*> >::iterator I;
    typedef std::map< ObjectHandle*, std::set<ChangeCallback*> >::const_iterator CI;
  };

  //! Type define for the application unique CallBackServer.
  typedef nvutil::Singleton<CallbackServer> CBS;
}
