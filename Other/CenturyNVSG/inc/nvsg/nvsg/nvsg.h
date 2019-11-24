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
#include "nvsg/CgFx.h"
#include "nvutil/BitMask.h"

#include <string>
#include <vector>

#if defined(LINUX)
// Linux specific library entry points
void nvsgLibInit() __attribute__((constructor)); 
void nvsgLibExit() __attribute__((destructor)); 
#endif

//! Scenegrah classes - core NVSG.
/** This namespace contains all core NVSG classes. 
  * Put all the scenegraph and node related classes in here.*/ 
namespace nvsg
{  
  enum NVSGThreadingModelEnum
  { 
    NVSG_SINGLETHREADED = 0
  , NVSG_MULTITHREADED
  };

  enum NVSG3DAPIEnum
  {
    NVSG_3DAPI_GL = 0
#if defined(SUPPORT_D3D9)
  , NVSG_3DAPI_D3D9
#endif
  };

  enum NVSGFeatureSupportedEnum
  {
    NVSG_FAST_TEXTURE_UPLOAD  = BIT0,
    NVSG_VOLUME_RENDERING     = BIT1
  };

  //! Per-application initialization of NVSG global data
  /** \param threadingModel 
    * Specifies whether to run in a multithreaded environment, or not.\n
    * Specify NVSG_SINGLETHREADED to configure NVSG to run in a single-threaded environment.\n
    * Specify NVSG_MULTITHREADED to configure NVSG to run in a multi-threaded environment.\n
    * The default for this parameter is NVSG_SINGLETHREADED.
    * \param graphicsAPI
    * Specifies the 3D graphics API to use for rendering.\n 
    * Specifying NVSG_3DAPI_GL to use OpenGL as 3D API currently is the only choice.
    * This is subject to change with future releases.
    * \remarks
    * An application must call this routine once before any other NVSG API call. 
    * This routine properly initializes all global data required by the NVSG API.
    * \n\n
    * In addition, this routine optionally configures the NVSG library to run safe in a 
    * multithreaded environment. This will be achieved by specifying NVSG_MULTITHREADED for the 
    * optional \a threadingModel parameter. 
    * \n\n
    * Making the NVSG library multi-threaded safe requires some overhead, which might affect
    * the runtime behavior in a single-threaded environment. That is, it is not recommended
    * to configure NVSG to run in a multi-threaded environment, if your application's threading
    * model really is single-threaded only.
    * \n\n
    * The nvsgInitialize entry point in addition lets you specify which 3D graphics API is
    * used for rendering. Note that using another 3D API for rendering than specified here
    * will result in undefined run-time behaviour. 
    * \n\n
    * It is recommended to call this routine inside the application's initial entry point. 
    * \note The nvsg runtime behaviour is undefined if an application issues nvsg API calls
    * without a prior call to this routine. */
  NVSG_API void nvsgInitialize( NVSGThreadingModelEnum threadingModel = NVSG_SINGLETHREADED
                              , NVSG3DAPIEnum graphicsAPI = NVSG_3DAPI_GL );

  //! Per-application termination of nvsg global data
  /** It is strongly recommended to call this routine when the application terminates. 
    * This routine properly frees all global data required by the nvsg API.
    * \note The nvsg runtime behaviour is undefined if an application issues nvsg API calls
    * after having called this routine. */
  NVSG_API void nvsgTerminate();

  //! Get the version string of the NVSGSDK
  NVSG_API void getVersionString(
    std::string & string  //!<  string to hold the NVSGSDK version string
  );

  //! Get the NVSGSDK name string
  NVSG_API void getSDKName(
    std::string & string  //!<  string to hold the NVSGSDK name string
  );

  //! Get the NVSGSDK copyright string
  NVSG_API void getCopyrightString(
    std::string & string  //!<  string to hold the NVSGSDK coppyright string
  );

  //! Get the NVSGSDK vendor name
  NVSG_API void getVendorName(
    std::string & string //!<  string to hold the NVSGSDK vendor name string
  );

  /*! \brief Get a bit field of the supported features.
   *  \return An unsigned int encoding the supported features.
   *  \remarks Currently, there are two features supported on Quadro boards only: fast texture
   *  upload (NVSG_FAST_TEXTURE_UPLOAD) and volume rendering (NVSG_VOLUME_RENDERING). */
  NVSG_API unsigned int getSupportedFeatures();
}
