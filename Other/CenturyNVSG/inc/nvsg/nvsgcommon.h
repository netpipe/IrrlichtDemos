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

/* This is the common header file, that should be included by all NVSG headers.
*/

#pragma once

#pragma warning(disable:4100) // unreference formal parameter

#include <stdlib.h>

// Linux type defines
#if defined(LINUX)
# include "nvsg/LinuxTypes.h"
#endif

// project header
#include "nvsg/nvsgapi.h"  // storage-class defines
#include "nvutil/Assert.h" // assert
#include "nvutil/Trace.h"  // support for function level tracing
#include "nvutil/Log.h"    // logging
#include "nvutil/nvutil.h" // utility CRT functions
