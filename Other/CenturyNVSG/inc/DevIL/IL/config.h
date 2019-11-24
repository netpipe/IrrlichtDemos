#ifndef __CONFIG_H__
#define __CONFIG_H__


//#if defined(_WIN64)
//  #define IL_NO_MNG  // dsievers: we need to build all external image libs
//  #define IL_NO_PNG  // for 64bit. for now let us exclude them.
//  #define IL_NO_TIF
//  #define IL_NO_JPG
//#endif

#define IL_NO_LCMS
#define IL_USE_JPEGLIB_UNMODIFIED

//#define IL_NO_PNG
//#define IL_NO_MNG
//#define IL_NO_TIF
//#define IL_NO_JPG
#define IL_NO_PIX
#define IL_NO_PXR
#define IL_NO_XPM

#define ILUT_USE_OPENGL
#define ILUT_USE_DIRECTX8
#define ILUT_USE_WIN32

#endif /* __CONFIG_H__ */
