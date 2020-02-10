// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_COMPILE_CONFIG_H_INCLUDED__
#define __IRR_COMPILE_CONFIG_H_INCLUDED__

//! The defines for different operating system are:
//! _XBOX for XBox
//! WIN32 for Windows32
//! WIN64 for Windows64
//! _IRR_WINDOWS_ for all irrlicht supported windows versions
//! OS2 for OS/2
//! MACOSX for Mac OS X
//! LINUX for linux and unix (it is defined here if no other os is defined)

#if defined(WIN32) || defined(WIN64)
#define _IRR_WINDOWS_
#endif

#if !defined(_IRR_WINDOWS_) && !defined(_XBOX) && !defined(OS2) && !defined(MACOSX)
#define LINUX
#endif

//! Define _IRR_COMPILE_WITH_DIRECT3D_8_ and _IRR_COMPILE_WITH_DIRECT3D_9_ to compile 
//! the Irrlicht engine with Direct3D8 and/or DIRECT3D9.
/** If you only want to use the software device or opengl this can be useful.
This switch is mostly disabled because people do not get the g++ compiler compile
directX header files, and directX is only available on windows platforms. If you
are using Dev-Cpp, and want to compile this using a DX dev pack, you can define
_IRR_COMPILE_WITH_DX9_DEV_PACK_. So you simply need to add something like this
to the compiler settings: -DIRR_COMPILE_WITH_DX9_DEV_PACK
and this to the linker settings: -ld3dx9 -ld3dx8 **/
#if (defined(_IRR_WINDOWS_) || defined(_XBOX)) && \
 (!defined(__GNUC__) || defined(IRR_COMPILE_WITH_DX9_DEV_PACK))

#define _IRR_COMPILE_WITH_DIRECT3D_8_
#define _IRR_COMPILE_WITH_DIRECT3D_9_

#endif

//! Define _IRR_COMPILE_WITH_OPENGL_ to compile the irrlicht engine with OpenGL.
/** If you do not wish the engine to be compiled with OpengGL, out comment this
define. */
#if !defined(_XBOX)

#define _IRR_COMPILE_WITH_OPENGL_

#endif // ! _XBOX


//! Define LINUX_OPENGL_USE_EXTENSIONS if the OpenGL renderer should use OpenGL extensions.
/** For being able to do things like multi texturing. It is useful
 to comment out this define to disable opengl extensions in linux because
 on some linux versions, these extensions does not exist. */
#if !defined(_IRR_WINDOWS_) && !defined(_XBOX) && !defined(OS2) && !defined(MACOSX)
#define _IRR_LINUX_OPENGL_USE_EXTENSIONS_
#define _IRR_LINUX_X11_VIDMODE_
#endif


//! Define _IRR_COMPILE_WITH_ZLIB_ to enable compiling the engine using zlib.
/** This enables the engine to read from compressed .zip archives. If you
disable this feature, the engine can still read archives, but only uncompressed
ones. */
#define _IRR_COMPILE_WITH_ZLIB_

//! Define _IRR_USE_NON_SYSTEM_ZLIB_ to let irrlicht use the zlib which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the zlib installed in the system.
 This is only used when _IRR_COMPILE_WITH_ZLIB_ is defined. */
#define _IRR_USE_NON_SYSTEM_ZLIB_


//! Define _IRR_COMPILE_WITH_JPEGLIB_ to enable compiling the engine using libjpeg.
/** This enables the engine to read jpeg images. If you comment this out,
the engine will no longer read .jpeg images. */
#define _IRR_COMPILE_WITH_LIBJPEG_

//! Define _IRR_USE_NON_SYSTEM_JPEG_LIB_ to let irrlicht use the jpeglib which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the jpeg lib installed in the system.
 This is only used when _IRR_COMPILE_WITH_LIBJPEG_ is defined. */
#define _IRR_USE_NON_SYSTEM_JPEG_LIB_


//! Define _IRR_COMPILE_WITH_LIBPNG_ to enable compiling the engine using libpng.
/** This enables the engine to read png images. If you comment this out,
the engine will no longer read .png images. */
#define _IRR_COMPILE_WITH_LIBPNG_

//! Define _IRR_USE_NON_SYSTEM_LIBPNG_ to let irrlicht use the libpng which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the libpng installed in the system.
 This is only used when _IRR_COMPILE_WITH_LIBPNG_ is defined. */
#define _IRR_USE_NON_SYSTEM_LIB_PNG_


//! Define _IRR_D3D_NO_SHADER_DEBUGGING to disable shader debugging in D3D9
/** If _IRR_D3D_NO_SHADER_DEBUGGING is undefined in IrrCompileConfig.h,
it is possible to debug all D3D9 shaders in VisualStudio. All shaders 
(which have been generated in memory or read from archives for example) will be emitted
into a temporary file at runtime for this purpose. To debug your shaders, choose  
Debug->Direct3D->StartWithDirect3DDebugging in Visual Studio, and for every shader a
file named 'irr_dbg_shader_%%.vsh' or 'irr_dbg_shader_%%.psh' will be created. Drag'n'drop
the file you want to debug into visual studio. That's it. You can now set breakpoints and
watch registers, variables etc. This works with ASM, HLSL, and both with pixel and vertex shaders.
Note that the engine will run in D3D REF for this, which is a lot slower than HAL. */
#define _IRR_D3D_NO_SHADER_DEBUGGING 


// We need to disable DIRECT3D9 support for Visual Studio 6.0 because 
// those $%&$!! disabled support for it since Dec. 2004 and users are complaining
// about linker errors. Comment this out only if you are knowing what you are 
// doing. (Which means you have an old DX9 SDK and VisualStudio6).
#ifdef _MSC_VER
#if (_MSC_VER < 1300 && !defined(__GNUC__)) 
#undef _IRR_COMPILE_WITH_DIRECT3D_9_
#pragma message("Compiling Irrlicht with Visual Studio 6.0, support for DX9 is disabled.")
#endif
#endif

#endif
