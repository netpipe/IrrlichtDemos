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

/* This file contains the defines commonly used by NVSG before including Windows headers.
*/

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//# define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
# endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
# ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#  define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
# endif

# ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#  define _WIN32_WINNT 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
# endif						

# ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#  define _WIN32_WINDOWS 0x0500 // Change this to the appropriate value to target Windows Me or later.
# endif

# ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#  define _WIN32_IE 0x0600	// Change this to the appropriate value to target IE 5.0 or later.
# endif

#endif // windows
