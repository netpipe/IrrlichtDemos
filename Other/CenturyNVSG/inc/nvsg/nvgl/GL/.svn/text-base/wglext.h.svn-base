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

#ifdef __cplusplus
extern "C" {
#endif

/*
** Copyright 1998-2002, NVIDIA Corporation.
** All Rights Reserved.
** 
** THE INFORMATION CONTAINED HEREIN IS PROPRIETARY AND CONFIDENTIAL TO
** NVIDIA, CORPORATION.  USE, REPRODUCTION OR DISCLOSURE TO ANY THIRD PARTY
** IS SUBJECT TO WRITTEN PRE-APPROVAL BY NVIDIA, CORPORATION.
** 
** 
** Copyright 1992-1999, Silicon Graphics, Inc.
** All Rights Reserved.
** 
** Portions of this file are UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon
** Graphics, Inc.; the contents of this file may not be disclosed to third
** parties, copied or duplicated in any form, in whole or in part, without
** the prior written permission of Silicon Graphics, Inc.
** 
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to
** restrictions as set forth in subdivision (c)(1)(ii) of the Rights in
** Technical Data and Computer Software clause at DFARS 252.227-7013,
** and/or in similar or successor clauses in the FAR, DOD or NASA FAR
** Supplement.  Unpublished - rights reserved under the Copyright Laws of
** the United States.
*/

#ifdef _WIN32
# include "nvsgwindefs.h"
# ifndef NOMINMAX
#  define NOMINMAX // avoid problems with Microsoft definitions in windef.h and min/max from stl
# endif
# include <windows.h>
# undef NOMINMAX
#endif

#ifndef GLAPI
# define GLAPI __stdcall
# define __DEFINED_GLAPI
#endif

DECLARE_HANDLE(HPBUFFERARB);
DECLARE_HANDLE(HPBUFFEREXT);
DECLARE_HANDLE(HPVIDEODEVICE);
DECLARE_HANDLE(HGPUNV);

    /* WGL_NV_gpu_affinity */ 
typedef struct _GPU_DEVICE {
    DWORD  cb;
    CHAR   DeviceName[32];
    CHAR   DeviceString[128];
    DWORD  Flags;
    RECT   rcVirtualScreen;
} GPU_DEVICE, *PGPU_DEVICE;
/*************************************************************/

/* Extensions */
#define WGL_ARB_buffer_region              1
#define WGL_ARB_extensions_string          1
#define WGL_ARB_make_current_read          1
#define WGL_ARB_multisample                1
#define WGL_ARB_pbuffer                    1
#define WGL_ARB_pixel_format               1
#define WGL_EXT_pixel_format               1
#define WGL_ARB_render_texture             1
#define WGL_ATI_pixel_format_float         1
#define WGL_EXT_extensions_string          1
#define WGL_EXT_pbuffer                    1
#define WGL_EXT_swap_control               1
#define WGL_NV_allocate_memory             1
#define WGL_NV_float_buffer                1
#define WGL_NV_render_depth_texture        1
#define WGL_NV_render_texture_rectangle    1
#define WGL_NV_video_output                1
#define WGL_KTX_buffer_region              1
#define WGL_I3D_genlock                    1
#define WGL_NV_swap_group                  1
#define WGL_NV_gpu_affinity                1

/* ARB_buffer_region */
#define WGL_FRONT_COLOR_BUFFER_BIT_ARB     0x00000001
#define WGL_BACK_COLOR_BUFFER_BIT_ARB      0x00000002
#define WGL_DEPTH_BUFFER_BIT_ARB           0x00000004
#define WGL_STENCIL_BUFFER_BIT_ARB         0x00000008

/* ARB_pixel_format */
#define WGL_NUMBER_PIXEL_FORMATS_ARB       0x2000
#define WGL_DRAW_TO_WINDOW_ARB             0x2001
#define WGL_DRAW_TO_BITMAP_ARB             0x2002
#define WGL_ACCELERATION_ARB               0x2003
#define WGL_NEED_PALETTE_ARB               0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB        0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB         0x2006
#define WGL_SWAP_METHOD_ARB                0x2007
#define WGL_NUMBER_OVERLAYS_ARB            0x2008
#define WGL_NUMBER_UNDERLAYS_ARB           0x2009
#define WGL_TRANSPARENT_ARB                0x200A
#define WGL_SHARE_DEPTH_ARB                0x200C
#define WGL_SHARE_STENCIL_ARB              0x200D
#define WGL_SHARE_ACCUM_ARB                0x200E
#define WGL_SUPPORT_GDI_ARB                0x200F
#define WGL_SUPPORT_OPENGL_ARB             0x2010
#define WGL_DOUBLE_BUFFER_ARB              0x2011
#define WGL_STEREO_ARB                     0x2012
#define WGL_PIXEL_TYPE_ARB                 0x2013
#define WGL_COLOR_BITS_ARB                 0x2014
#define WGL_RED_BITS_ARB                   0x2015
#define WGL_RED_SHIFT_ARB                  0x2016
#define WGL_GREEN_BITS_ARB                 0x2017
#define WGL_GREEN_SHIFT_ARB                0x2018
#define WGL_BLUE_BITS_ARB                  0x2019
#define WGL_BLUE_SHIFT_ARB                 0x201A
#define WGL_ALPHA_BITS_ARB                 0x201B
#define WGL_ALPHA_SHIFT_ARB                0x201C
#define WGL_ACCUM_BITS_ARB                 0x201D
#define WGL_ACCUM_RED_BITS_ARB             0x201E
#define WGL_ACCUM_GREEN_BITS_ARB           0x201F
#define WGL_ACCUM_BLUE_BITS_ARB            0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB           0x2021
#define WGL_DEPTH_BITS_ARB                 0x2022
#define WGL_STENCIL_BITS_ARB               0x2023
#define WGL_AUX_BUFFERS_ARB                0x2024
#define WGL_NO_ACCELERATION_ARB            0x2025
#define WGL_GENERIC_ACCELERATION_ARB       0x2026
#define WGL_FULL_ACCELERATION_ARB          0x2027
#define WGL_SWAP_EXCHANGE_ARB              0x2028
#define WGL_SWAP_COPY_ARB                  0x2029
#define WGL_SWAP_UNDEFINED_ARB             0x202A
#define WGL_TYPE_RGBA_ARB                  0x202B
#define WGL_TYPE_COLORINDEX_ARB            0x202C
#define WGL_TRANSPARENT_RED_VALUE_ARB      0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB    0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB     0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB    0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB    0x203B

/* EXT_pixel_format */
#define WGL_NUMBER_PIXEL_FORMATS_EXT       0x2000
#define WGL_DRAW_TO_WINDOW_EXT             0x2001
#define WGL_DRAW_TO_BITMAP_EXT             0x2002
#define WGL_ACCELERATION_EXT               0x2003
#define WGL_NEED_PALETTE_EXT               0x2004
#define WGL_NEED_SYSTEM_PALETTE_EXT        0x2005
#define WGL_SWAP_LAYER_BUFFERS_EXT         0x2006
#define WGL_SWAP_METHOD_EXT                0x2007
#define WGL_NUMBER_OVERLAYS_EXT            0x2008
#define WGL_NUMBER_UNDERLAYS_EXT           0x2009
#define WGL_TRANSPARENT_EXT                0x200A
#define WGL_SHARE_DEPTH_EXT                0x200C
#define WGL_SHARE_STENCIL_EXT              0x200D
#define WGL_SHARE_ACCUM_EXT                0x200E
#define WGL_SUPPORT_GDI_EXT                0x200F
#define WGL_SUPPORT_OPENGL_EXT             0x2010
#define WGL_DOUBLE_BUFFER_EXT              0x2011
#define WGL_STEREO_EXT                     0x2012
#define WGL_PIXEL_TYPE_EXT                 0x2013
#define WGL_COLOR_BITS_EXT                 0x2014
#define WGL_RED_BITS_EXT                   0x2015
#define WGL_RED_SHIFT_EXT                  0x2016
#define WGL_GREEN_BITS_EXT                 0x2017
#define WGL_GREEN_SHIFT_EXT                0x2018
#define WGL_BLUE_BITS_EXT                  0x2019
#define WGL_BLUE_SHIFT_EXT                 0x201A
#define WGL_ALPHA_BITS_EXT                 0x201B
#define WGL_ALPHA_SHIFT_EXT                0x201C
#define WGL_ACCUM_BITS_EXT                 0x201D
#define WGL_ACCUM_RED_BITS_EXT             0x201E
#define WGL_ACCUM_GREEN_BITS_EXT           0x201F
#define WGL_ACCUM_BLUE_BITS_EXT            0x2020
#define WGL_ACCUM_ALPHA_BITS_EXT           0x2021
#define WGL_DEPTH_BITS_EXT                 0x2022
#define WGL_STENCIL_BITS_EXT               0x2023
#define WGL_AUX_BUFFERS_EXT                0x2024
#define WGL_NO_ACCELERATION_EXT            0x2025
#define WGL_GENERIC_ACCELERATION_EXT       0x2026
#define WGL_FULL_ACCELERATION_EXT          0x2027
#define WGL_SWAP_EXCHANGE_EXT              0x2028
#define WGL_SWAP_COPY_EXT                  0x2029
#define WGL_SWAP_UNDEFINED_EXT             0x202A
#define WGL_TYPE_RGBA_EXT                  0x202B
#define WGL_TYPE_COLORINDEX_EXT            0x202C
#define WGL_TRANSPARENT_RED_VALUE_EXT      0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_EXT    0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_EXT     0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_EXT    0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_EXT    0x203B

/* ARB_pbuffer */
#define WGL_DRAW_TO_PBUFFER_ARB            0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB         0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB          0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB         0x2030
#define WGL_PBUFFER_LARGEST_ARB            0x2033
#define WGL_PBUFFER_WIDTH_ARB              0x2034
#define WGL_PBUFFER_HEIGHT_ARB             0x2035
#define WGL_PBUFFER_LOST_ARB               0x2036

/* EXT_pbuffer */
#define WGL_DRAW_TO_PBUFFER_EXT            0x202D
#define WGL_MAX_PBUFFER_PIXELS_EXT         0x202E
#define WGL_MAX_PBUFFER_WIDTH_EXT          0x202F
#define WGL_MAX_PBUFFER_HEIGHT_EXT         0x2030
#define WGL_PBUFFER_LARGEST_EXT            0x2033
#define WGL_PBUFFER_WIDTH_EXT              0x2034
#define WGL_PBUFFER_HEIGHT_EXT             0x2035
#define WGL_PBUFFER_LOST_EXT               0x2036

/* ARB_multisample */
#define WGL_SAMPLE_BUFFERS_ARB             0x2041
#define WGL_SAMPLES_ARB                    0x2042

/* I3D_genlock */
#define WGL_GENLOCK_SOURCE_MULTIVIEW_I3D   0x2044
#define WGL_GENLOCK_SOURCE_EXTERNAL_SYNC_I3D 0x2045
#define WGL_GENLOCK_SOURCE_EXTERNAL_FIELD_I3D 0x2046
#define WGL_GENLOCK_SOURCE_EXTERNAL_TTL_I3D 0x2047
#define WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D 0x2048
#define WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D 0x2049
#define WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D 0x204A
#define WGL_GENLOCK_SOURCE_EDGE_RISING_I3D 0x204B
#define WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D   0x204C

/* ARB_render_texture */
#define WGL_BIND_TO_TEXTURE_RGB_ARB        0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB       0x2071
#define WGL_TEXTURE_FORMAT_ARB             0x2072
#define WGL_TEXTURE_TARGET_ARB             0x2073
#define WGL_MIPMAP_TEXTURE_ARB             0x2074
#define WGL_TEXTURE_RGB_ARB                0x2075
#define WGL_TEXTURE_RGBA_ARB               0x2076
#define WGL_NO_TEXTURE_ARB                 0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB           0x2078
#define WGL_TEXTURE_1D_ARB                 0x2079
#define WGL_TEXTURE_2D_ARB                 0x207A
#define WGL_MIPMAP_LEVEL_ARB               0x207B
#define WGL_CUBE_MAP_FACE_ARB              0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x2082
#define WGL_FRONT_LEFT_ARB                 0x2083
#define WGL_FRONT_RIGHT_ARB                0x2084
#define WGL_BACK_LEFT_ARB                  0x2085
#define WGL_BACK_RIGHT_ARB                 0x2086
#define WGL_AUX0_ARB                       0x2087
#define WGL_AUX1_ARB                       0x2088
#define WGL_AUX2_ARB                       0x2089
#define WGL_AUX3_ARB                       0x208A
#define WGL_AUX4_ARB                       0x208B
#define WGL_AUX5_ARB                       0x208C
#define WGL_AUX6_ARB                       0x208D
#define WGL_AUX7_ARB                       0x208E
#define WGL_AUX8_ARB                       0x208F
#define WGL_AUX9_ARB                       0x2090

/* NV_render_depth_texture */
#define WGL_BIND_TO_TEXTURE_DEPTH_NV       0x20A3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_DEPTH_NV 0x20A4
#define WGL_DEPTH_TEXTURE_FORMAT_NV        0x20A5
#define WGL_TEXTURE_DEPTH_COMPONENT_NV     0x20A6
#define WGL_DEPTH_COMPONENT_NV             0x20A7

/* NV_float_buffer */
#define WGL_FLOAT_COMPONENTS_NV            0x20B0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV 0x20B1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV 0x20B2
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV 0x20B3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV 0x20B4
#define WGL_TEXTURE_FLOAT_R_NV             0x20B5
#define WGL_TEXTURE_FLOAT_RG_NV            0x20B6
#define WGL_TEXTURE_FLOAT_RGB_NV           0x20B7
#define WGL_TEXTURE_FLOAT_RGBA_NV          0x20B8

/* NV_render_texture_rectangle */
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGB_NV 0x20A0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV 0x20A1
#define WGL_TEXTURE_RECTANGLE_NV           0x20A2

/* ARB_make_current_read */
#define WGL_ERROR_INVALID_PIXEL_TYPE_ARB   0x2043
#define WGL_ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054

/* ATI_pixel_format_float */
#define WGL_TYPE_RGBA_FLOAT_ATI            0x21A0

/* WGL_NV_video_output */
#define WGL_BIND_TO_VIDEO_RGB_NV           0x20C0
#define WGL_BIND_TO_VIDEO_RGBA_NV          0x20C1
#define WGL_BIND_TO_VIDEO_RGB_AND_DEPTH_NV 0x20C2
#define WGL_VIDEO_OUT_COLOR_NV             0x20C3
#define WGL_VIDEO_OUT_ALPHA_NV             0x20C4
#define WGL_VIDEO_OUT_DEPTH_NV             0x20C5
#define WGL_VIDEO_OUT_COLOR_AND_ALPHA_NV   0x20C6
#define WGL_VIDEO_OUT_COLOR_AND_DEPTH_NV   0x20C7
#define WGL_VIDEO_OUT_FRAME_NV             0x20C8
#define WGL_VIDEO_OUT_FIELD_1_NV           0x20C9
#define WGL_VIDEO_OUT_FIELD_2_NV           0x20CA

/* NV_gpu_affinity */
#define WGL_ERROR_INCOMPATIBLE_AFFINITY_MASKS_NV 0x20D0
#define WGL_ERROR_MISSING_AFFINITY_MASK_NV 0x20D1

/*************************************************************/


/* WGL_ARB_buffer_region */
typedef HANDLE (GLAPI * PFNWGLCREATEBUFFERREGIONARBPROC) (HDC hdc, int iLayerPlane, UINT uType);
typedef void (GLAPI * PFNWGLDELETEBUFFERREGIONARBPROC) (HANDLE hRegion);
typedef BOOL (GLAPI * PFNWGLSAVEBUFFERREGIONARBPROC) (HANDLE hRegion, int x, int y, int width, int height);
typedef BOOL (GLAPI * PFNWGLRESTOREBUFFERREGIONARBPROC) (HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);

/* WGL_ARB_extensions_string */
typedef const char * (GLAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);

/* WGL_ARB_pbuffer */
typedef HPBUFFERARB (GLAPI * PFNWGLCREATEPBUFFERARBPROC) (HDC hdc, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
typedef HDC (GLAPI * PFNWGLGETPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer);
typedef int (GLAPI * PFNWGLRELEASEPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer, HDC hdc);
typedef BOOL (GLAPI * PFNWGLDESTROYPBUFFERARBPROC) (HPBUFFERARB hPbuffer);
typedef BOOL (GLAPI * PFNWGLQUERYPBUFFERARBPROC) (HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

/* WGL_EXT_pbuffer */
typedef HPBUFFEREXT (GLAPI * PFNWGLCREATEPBUFFEREXTPROC) (HDC hdc, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
typedef HDC (GLAPI * PFNWGLGETPBUFFERDCEXTPROC) (HPBUFFEREXT hPbuffer);
typedef int (GLAPI * PFNWGLRELEASEPBUFFERDCEXTPROC) (HPBUFFEREXT hPbuffer, HDC hdc);
typedef BOOL (GLAPI * PFNWGLDESTROYPBUFFEREXTPROC) (HPBUFFEREXT hPbuffer);
typedef BOOL (GLAPI * PFNWGLQUERYPBUFFEREXTPROC) (HPBUFFEREXT hPbuffer, int iAttribute, int *piValue);

/* WGL_ARB_render_texture */
typedef BOOL (GLAPI * PFNWGLBINDTEXIMAGEARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (GLAPI * PFNWGLRELEASETEXIMAGEARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (GLAPI * PFNWGLSETPBUFFERATTRIBARBPROC) (HPBUFFERARB hPbuffer, const int * piAttribList);

/* WGL_NV_video_output */
typedef BOOL (GLAPI * PFNWGLGETVIDEODEVICENVPROC) (HDC hDC, INT iVideoStreams, HPVIDEODEVICE* phpVideoDevice);
typedef BOOL (GLAPI * PFNWGLRELEASEVIDEODEVICENVPROC) (HPVIDEODEVICE hpVideoDevice);
typedef BOOL (GLAPI * PFNWGLBINDVIDEOIMAGENVPROC) (HPVIDEODEVICE hpVideoDevice, HPBUFFERARB hPbuffer, INT iVideoBuffer);
typedef BOOL (GLAPI * PFNWGLRELEASEVIDEOIMAGENVPROC) (HPBUFFERARB hPbuffer, INT iVideoBuffer);
typedef BOOL (GLAPI * PFNWGLSENDPBUFFERTOVIDEONVPROC) (HPBUFFERARB hPbuffer, INT iBufferType, unsigned long * pulCounterPbuffer, BOOL bBlock);
typedef BOOL (GLAPI * PFNWGLGETVIDEOINFONVPROC) (HPVIDEODEVICE hpVideoDevice, unsigned long * pulCounterOutputVideo, unsigned long * pulCounterOutputPbuffer);

/* WGL_ARB_pixel_format */
typedef BOOL (GLAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
typedef BOOL (GLAPI * PFNWGLGETPIXELFORMATATTRIBFVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL (GLAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

/* WGL_EXT_pixel_format */
typedef BOOL (GLAPI * PFNWGLGETPIXELFORMATATTRIBIVEXTPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
typedef BOOL (GLAPI * PFNWGLGETPIXELFORMATATTRIBFVEXTPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL (GLAPI * PFNWGLCHOOSEPIXELFORMATEXTPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

/* WGL_EXT_extensions_string */
typedef const char * (GLAPI * PFNWGLGETEXTENSIONSSTRINGEXTPROC) (void);

/* WGL_EXT_swap_control */
typedef int (GLAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int (GLAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);

/* WGL_NV_allocate_memory */
typedef void * (GLAPI * PFNWGLALLOCATEMEMORYNVPROC) (int size, float readfreq, float writefreq, float priority);
typedef void (GLAPI * PFNWGLFREEMEMORYNVPROC) (void *pointer);

/* WGL_KTX_buffer_region */
typedef UINT (GLAPI * PFNWGLNEWBUFFERREGIONPROC) (UINT type);
typedef void (GLAPI * PFNWGLDELETEBUFFERREGIONPROC) (UINT region);
typedef void (GLAPI * PFNWGLREADBUFFERREGIONPROC) (UINT region, int x, int y, GLint width, GLint height);
typedef void (GLAPI * PFNWGLDRAWBUFFERREGIONPROC) (UINT region, int x, int y, GLint width, GLint height, int xDest, int yDest);
typedef UINT (GLAPI * PFNWGLBUFFERREGIONENABLEDPROC) (void);

/* WGL_I3D_genlock */
typedef BOOL (GLAPI * PFNWGLENABLEGENLOCKI3DPROC) (HDC hdc);
typedef BOOL (GLAPI * PFNWGLDISABLEGENLOCKI3DPROC) (HDC hdc);
typedef BOOL (GLAPI * PFNWGLISENABLEDGENLOCKI3DPROC) (HDC hdc, BOOL * pFlag);
typedef BOOL (GLAPI * PFNWGLGENLOCKSOURCEI3DPROC) (HDC hdc, UINT uSource);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSOURCEI3DPROC) (HDC hdc, UINT * uSource);
typedef BOOL (GLAPI * PFNWGLGENLOCKSOURCEEDGEI3DPROC) (HDC hdc, UINT uEdge);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSOURCEEDGEI3DPROC) (HDC hdc, UINT * uEdge);
typedef BOOL (GLAPI * PFNWGLGENLOCKSAMPLERATEI3DPROC) (HDC hdc, UINT uRate);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSAMPLERATEI3DPROC) (HDC hdc, UINT * uRate);
typedef BOOL (GLAPI * PFNWGLGENLOCKSOURCEDELAYI3DPROC) (HDC hdc, UINT uDelay);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSOURCEDELAYI3DPROC) (HDC hdc, UINT * uDelay);
typedef BOOL (GLAPI * PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC) (HDC hdc, UINT * uMaxLineDelay, UINT * uMaxPixelDelay);

/* WGL_NV_swap_group */
typedef BOOL (GLAPI * PFNWGLJOINSWAPGROUPNVPROC) (HDC hDC, GLuint group);
typedef BOOL (GLAPI * PFNWGLBINDSWAPBARRIERNVPROC) (GLuint group, GLuint barrier);
typedef BOOL (GLAPI * PFNWGLQUERYSWAPGROUPNVPROC) (HDC hDC, GLuint * group, GLuint * barrier);
typedef BOOL (GLAPI * PFNWGLQUERYMAXSWAPGROUPSNVPROC) (HDC hDC, GLuint * maxGroups, GLuint * maxBarriers);
typedef BOOL (GLAPI * PFNWGLQUERYFRAMECOUNTNVPROC) (HDC hDC, GLuint * count);
typedef BOOL (GLAPI * PFNWGLRESETFRAMECOUNTNVPROC) (HDC hDC);

/* WGL_ARB_make_current_read */
typedef BOOL (GLAPI * PFNWGLMAKECONTEXTCURRENTARBPROC) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
typedef HDC (GLAPI * PFNWGLGETCURRENTREADDCARBPROC) (void);

/* WGL_EXT_swap_frame_lock */
typedef BOOL (GLAPI * PFNWGLENABLEFRAMELOCKEXTPROC) (void);
typedef BOOL (GLAPI * PFNWGLDISABLEFRAMELOCKEXTPROC) (void);
typedef BOOL (GLAPI * PFNWGLISENABLEDFRAMELOCKEXTPROC) (BOOL * pFlag);
typedef BOOL (GLAPI * PFNWGLQUERYFRAMELOCKMASTEREXTPROC) (BOOL * pFlag);

/* WGL_EXT_genlock */
#define WGL_GENLOCK_SOURCE_MULTIVIEW_EXT   0x2044
#define WGL_GENLOCK_SOURCE_EXTERNAL_SYNC_EXT 0x2045
#define WGL_GENLOCK_SOURCE_EXTERNAL_FIELD_EXT 0x2046
#define WGL_GENLOCK_SOURCE_EXTERNAL_TTL_EXT 0x2047
#define WGL_GENLOCK_SOURCE_DIGITAL_SYNC_EXT 0x2048
#define WGL_GENLOCK_SOURCE_DIGITAL_FIELD_EXT 0x2049
#define WGL_GENLOCK_SOURCE_EDGE_FALLING_EXT 0x204A
#define WGL_GENLOCK_SOURCE_EDGE_RISING_EXT 0x204B
#define WGL_GENLOCK_SOURCE_EDGE_BOTH_EXT   0x204C

typedef BOOL (GLAPI * PFNWGLENABLEGENLOCKEXTPROC) (HDC hdc);
typedef BOOL (GLAPI * PFNWGLDISABLEGENLOCKEXTPROC) (HDC hdc);
typedef BOOL (GLAPI * PFNWGLISENABLEDGENLOCKEXTPROC) (HDC hdc, BOOL * pFlag);
typedef BOOL (GLAPI * PFNWGLGENLOCKSOURCEEXTPROC) (HDC hdc, UINT uSource);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSOURCEEXTPROC) (HDC hdc, UINT * uSource);
typedef BOOL (GLAPI * PFNWGLGENLOCKSOURCEEDGEEXTPROC) (HDC hdc, UINT uEdge);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSOURCEEDGEEXTPROC) (HDC hdc, UINT * uEdge);
typedef BOOL (GLAPI * PFNWGLGENLOCKSAMPLERATEEXTPROC) (HDC hdc, UINT uRate);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSAMPLERATEEXTPROC) (HDC hdc, UINT * uRate);
typedef BOOL (GLAPI * PFNWGLGENLOCKSOURCEDELAYEXTPROC) (HDC hdc, UINT uDelay);
typedef BOOL (GLAPI * PFNWGLGETGENLOCKSOURCEDELAYEXTPROC) (HDC hdc, UINT * uDelay);
typedef BOOL (GLAPI * PFNWGLQUERYGENLOCKMAXSOURCEDELAYEXTPROC) (HDC hdc, UINT * uMaxLineDelay, UINT * uMaxPixelDelay);

/* WGL_NV_gpu_affinity */
typedef BOOL (GLAPI * PFNWGLENUMGPUSNVPROC) (UINT iIndex, HGPUNV *hGpu);
typedef BOOL (GLAPI * PFNWGLENUMGPUDEVICESNVPROC) (HGPUNV hGpu, UINT iIndex, PGPU_DEVICE pGpuDevice);
typedef HDC  (GLAPI * PFNWGLCREATEAFFINITYDCNVPROC) (const HGPUNV *pGpuList);
typedef BOOL (GLAPI * PFNWGLENUMGPUSFROMAFFINITYDCNVPROC) (HDC hAffinityDC, UINT iIndex, HGPUNV *hGpu);
typedef BOOL (GLAPI * PFNWGLDELETEDCNVPROC) (HDC hAffinityDC);

#ifdef __DEFINED_GLAPI
# undef GLAPI
# undef __DEFINED_GLAPI
#endif

#ifdef __cplusplus
}
#endif

