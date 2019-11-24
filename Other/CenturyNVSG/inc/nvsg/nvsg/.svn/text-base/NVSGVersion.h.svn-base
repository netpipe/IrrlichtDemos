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

#if !defined( DOXYGEN_IGNORE )    //  no need to document the version header
//---------------------------------------------------------------------------
// Version Number
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - This is the ONLY place to edit the version number.
// - There must be no unnecessary leading zeros in the numbers.
//   (e.g.: don't use '02' - use '2' instead)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// The build group should read this before they start 
// compiling the solution and set the environment variable 
// NVSGVERSIONTAG regarding this value. The version tag is the major version number.
// NVSGVERSIONTAG=1 
#define NVSG_VER_MARKETING   3
#define NVSG_VER_MAJOR       2
#define NVSG_VER_MINOR       0  
#define NVSG_VER_BUGFIX     13

//#define NVSG_ALPHA
//#define NVSG_BETA

#ifdef NVSG_ALPHA
  #define ALPHA
#endif

#ifdef NVSG_BETA
  #define BETA
#endif

// MaKe STRing helper macro
#define MKSTR(s)    MKSTR_i(s)
#define MKSTR_i(s)  MKSTR_ii(s)
#define MKSTR_ii(s) #s

// conditionally add a leading zero to the single parts of the version string
#if NVSG_VER_MARKETING < 10
#define NVSG_VER_MARKETING_STR  "0" MKSTR(NVSG_VER_MARKETING)
#else
#define NVSG_VER_MARKETING_STR      MKSTR(NVSG_VER_MARKETING)
#endif
#if NVSG_VER_MAJOR < 10
#define NVSG_VER_MAJOR_STR      "0" MKSTR(NVSG_VER_MAJOR)
#else
#define NVSG_VER_MAJOR_STR          MKSTR(NVSG_VER_MAJOR)
#endif
#if NVSG_VER_MINOR < 10
#define NVSG_VER_MINOR_STR      "0" MKSTR(NVSG_VER_MINOR)
#else
#define NVSG_VER_MINOR_STR          MKSTR(NVSG_VER_MINOR)
#endif
#if NVSG_VER_BUGFIX < 10
#define NVSG_VER_BUGFIX_STR     "0" MKSTR(NVSG_VER_BUGFIX)
#else
#define NVSG_VER_BUGFIX_STR         MKSTR(NVSG_VER_BUGFIX)
#endif

#define SDK_VENDOR "NVIDIA"
#define SDK_NAME   "NVSG"

// internal version string
#define _VER_STR   NVSG_VER_MARKETING_STR "."\
                   NVSG_VER_MAJOR_STR "."\
                   NVSG_VER_MINOR_STR "."\
                   NVSG_VER_BUGFIX_STR

// no need to update these
//
#ifdef ALPHA
  #if !defined(NDEBUG)
    #define VERSION_STR   _VER_STR " alpha" " (DEBUG)"
  #else  //DEBUG
    #define VERSION_STR   _VER_STR " alpha" 
  #endif //DEBUG
#else
  #ifdef BETA
    #if !defined(NDEBUG)
      #define VERSION_STR   _VER_STR " beta (DEBUG)"
    #else  //DEBUG
      #define VERSION_STR   _VER_STR " beta"
    #endif //DEBUG
  #else  //BETA
    #if !defined(NDEBUG)
      #define VERSION_STR   _VER_STR " (DEBUG)"
    #else  //DEBUG
      #define VERSION_STR   _VER_STR
    #endif //DEBUG
  #endif //BETA
#endif

#define COPYRIGHT	 SDK_VENDOR" "SDK_NAME" Version "VERSION_STR"\n© Copyright 2005 NVIDIA Corporation\n";

#endif  //  DOXYGEN_IGNORE
