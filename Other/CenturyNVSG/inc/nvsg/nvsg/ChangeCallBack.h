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

#include "nvutil/RCObject.h"
#include "nvsg/CoreTypes.h"

namespace nvsg
{
  /*! \brief Abstract class for defining a CallBack on an object
    * \par Namespace: nvsg
    * \remarks  
    * This class (in conjunction with the CallBackServer) provides an interface
    * that allows the user to:
    *       -trigger CallBacks on an Object 
    *       and/or
    *       -do Object-Object messaging (Subject-Observer pattern)
  */
  class ChangeCallback : public nvutil::RCObject
  {
  public:

    /*! \brief pure virtual method to call when the subject changes
      * \param subjectHandle Handle to the subject that changed
      * \remarks
      *   -Derive a concrete ChangeCallBack and override this method
      *   -add the call back to the CallBackServer using CallBackServer::addCallback
      * Calling CallBackServer::changed method routes(i.e calls the changed method) for an
      * object and its associated CallBacks
    */
    NVSG_API virtual void changed(nvsg::ObjectHandle * subjectHandle, const void* userData = NULL ) const = 0;
  protected:
    /*! \brief Constructs a ChangeCallback 
    */
    ChangeCallback();

    /*! \brief Destructs a ChangeCallback object.
    */
    virtual ~ChangeCallback();
  };
}
