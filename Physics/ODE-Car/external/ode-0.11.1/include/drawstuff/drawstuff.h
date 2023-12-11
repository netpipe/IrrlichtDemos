/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001-2003 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

/** @defgroup drawstuff DrawStuff

DrawStuff is a library for rendering simple 3D objects in a virtual 
environment, for the purposes of demonstrating the features of ODE.
It is provided for demonstration purposes and is not intended for
production use.

@section Notes

In the virtual world, the z axis is "up" and z=0 is the floor.

The user is able to click+drag in the main window to move the camera:
  * left button - pan and tilt.
  * right button - forward and sideways.
  * left + right button (or middle button) - sideways and up.
*/


#ifndef __DRAWSTUFF_H__
#define __DRAWSTUFF_H__

/* Define a DLL export symbol for those platforms that need it */
#if defined(ODE_PLATFORM_WINDOWS)
  #if defined(DS_DLL)
    #define DS_API __declspec(dllexport)
  #elif !defined(DS_LIB)
    #define DS_DLL_API __declspec(dllimport)
  #endif
#endif
    
#if !defined(DS_API)
  #define DS_API
#endif

#ifdef __cplusplus
extern "C" {
#endif


#include <drawstuff/version.h>


/* texture numbers */
  enum DS_TEXTURE_NUMBER
  {
    DS_NONE = 0,       /* uses the current color instead of a texture */
    DS_WOOD,
    DS_CHECKERED,
    DS_GROUND,
    DS_SKY,
  };

/* draw modes */

#define DS_POLYFILL  0
#define DS_WIREFRAME 1

/**
 * @struct dsFunctions
 * @brief Set of functions to be used as callbacks by the simulation loop.
 * @ingroup drawstuff
 */
typedef struct dsFunctions {
  int version;			/* put DS_VERSION here */
  /* version 1 data */
  void (*start)();		/* called before sim loop starts */
  void (*step) (int pause);	/* called before every frame */
  void (*command) (int cmd);	/* called if a command key is pressed */
  void (*stop)();		/* called after sim loop exits */
  /* version 2 data */
  const char *path_to_textures;	/* if nonzero, path to texture files */
} dsFunctions;


/**
 * @brief Does the complete simulation.
 * @ingroup drawstuff
 * This function starts running the simulation, and only exits when the simulation is done.
 * Function pointers should be provided for the callbacks.
 * @param argv supports flags like '-notex' '-noshadow' '-pause'
 * @param fn Callback functions.
 */
DS_API void dsSimulationLoop (int argc, char **argv,
		       int window_width, int window_height,
		       struct dsFunctions *fn);

/**
 * @brief exit with error message.
 * @ingroup drawstuff
 * This function displays an error message then exit.
 * @param msg format strin, like printf, without the newline character.
 */
DS_API void dsError (const char *msg, ...);

/**
 * @brief exit with error message and core dump.
 * @ingroup drawstuff
 * this functions tries to dump core or start the debugger.
 * @param msg format strin, like printf, without the newline character.
 */
DS_API void dsDebug (const char *msg, ...);

/**
 * @brief print log message
 * @ingroup drawstuff
 * @param msg format string, like printf, without the \n.
 */
DS_API void dsPrint (const char *msg, ...);

/**
 * @brief Sets the viewpoint
 * @ingroup drawstuff
 * @param xyz camera position.
 * @param hpr contains heading, pitch and roll numbers in degrees. heading=0
 * points along the x axis, pitch=0 is looking towards the horizon, and
 * roll 0 is "unrotated".
 */
DS_API void dsSetViewpoint (float xyz[3], float hpr[3]);


/**
 * @brief Gets the viewpoint
 * @ingroup drawstuff
 * @param xyz position
 * @param hpr heading,pitch,roll.
 */
DS_API void dsGetViewpoint (float xyz[3], float hpr[3]);

/**
 * @brief Stop the simulation loop.
 * @ingroup drawstuff
 * Calling this from within dsSimulationLoop()
 * will cause it to exit and return to the caller. it is the same as if the
 * user used the exit command. using this outside the loop will have no
 * effect.
 */
DS_API void dsStop();

/**
 * @brief Get the elapsed time (on wall-clock)
 * @ingroup drawstuff
 * It returns the nr of seconds since the last call to this function.
 */
DS_API double dsElapsedTime();

/**
 * @brief Toggle the rendering of textures.
 * @ingroup drawstuff
 * It changes the way objects are drawn. these changes will apply to all further
 * dsDrawXXX() functions. 
 * @param the texture number must be a DS_xxx texture constant.
 * The current texture is colored according to the current color.
 * At the start of each frame, the texture is reset to none and the color is
 * reset to white.
 */
DS_API void dsSetTexture (int texture_number);

/**
 * @brief Set the color with which geometry is drawn.
 * @ingroup drawstuff
 * @param red Red component from 0 to 1
 * @param green Green component from 0 to 1
 * @param blue Blue component from 0 to 1
 */
DS_API void dsSetColor (float red, float green, float blue);

/**
 * @brief Set the color and transparency with which geometry is drawn.
 * @ingroup drawstuff
 * @param alpha Note that alpha transparency is a misnomer: it is alpha opacity.
 * 1.0 means fully opaque, and 0.0 means fully transparent.
 */
DS_API void dsSetColorAlpha (float red, float green, float blue, float alpha);

/**
 * @brief Draw a box.
 * @ingroup drawstuff
 * @param pos is the x,y,z of the center of the object.
 * @param R is a 3x3 rotation matrix for the object, stored by row like this:
 *        [ R11 R12 R13 0 ]
 *        [ R21 R22 R23 0 ]
 *        [ R31 R32 R33 0 ]
 * @param sides[] is an array of x,y,z side lengths.
 */
DS_API void dsDrawBox (const float pos[3], const float R[12], const float sides[3]);

/**
 * @brief Draw a sphere.
 * @ingroup drawstuff
 * @param pos Position of center.
 * @param R orientation.
 * @param radius
 */
DS_API void dsDrawSphere (const float pos[3], const float R[12], float radius);

/**
 * @brief Draw a triangle.
 * @ingroup drawstuff
 * @param pos Position of center
 * @param R orientation
 * @param v0 first vertex
 * @param v1 second
 * @param v2 third vertex
 * @param solid set to 0 for wireframe
 */
DS_API void dsDrawTriangle (const float pos[3], const float R[12],
		     const float *v0, const float *v1, const float *v2, int solid);

/**
 * @brief Draw a z-aligned cylinder
 * @ingroup drawstuff
 */
DS_API void dsDrawCylinder (const float pos[3], const float R[12],
		     float length, float radius);

/**
 * @brief Draw a z-aligned capsule
 * @ingroup drawstuff
 */
DS_API void dsDrawCapsule (const float pos[3], const float R[12],
		    float length, float radius);

/**
 * @brief Draw a line.
 * @ingroup drawstuff
 */
DS_API void dsDrawLine (const float pos1[3], const float pos2[3]);

/**
 * @brief Draw a convex shape.
 * @ingroup drawstuff
 */
DS_API void dsDrawConvex(const float pos[3], const float R[12],
		  float *_planes,
		  unsigned int _planecount,
		  float *_points,
		  unsigned int _pointcount,
		  unsigned int *_polygons);

 /* these drawing functions are identical to the ones above, except they take
 * double arrays for `pos' and `R'.
 */
DS_API void dsDrawBoxD (const double pos[3], const double R[12],
		 const double sides[3]);
DS_API void dsDrawSphereD (const double pos[3], const double R[12],
		    const float radius);
DS_API void dsDrawTriangleD (const double pos[3], const double R[12],
		      const double *v0, const double *v1, const double *v2, int solid);
DS_API void dsDrawCylinderD (const double pos[3], const double R[12],
		      float length, float radius);
DS_API void dsDrawCapsuleD (const double pos[3], const double R[12],
		     float length, float radius);
DS_API void dsDrawLineD (const double pos1[3], const double pos2[3]);
DS_API void dsDrawConvexD(const double pos[3], const double R[12],
		  double *_planes,
		  unsigned int _planecount,
		  double *_points,
		  unsigned int _pointcount,
		  unsigned int *_polygons);

/**
 * @brief Set the quality with which curved objects are rendered.
 * @ingroup drawstuff
 * Higher numbers are higher quality, but slower to draw. 
 * This must be set before the first objects are drawn to be effective.
 * Default sphere quality is 1, default capsule quality is 3.
 */
DS_API void dsSetSphereQuality (int n);		/* default = 1 */
DS_API void dsSetCapsuleQuality (int n);		/* default = 3 */

/**
 * @brief Set Drawmode 0=Polygon Fill,1=Wireframe).
 * Use the DS_POLYFILL and DS_WIREFRAME macros.
 * @ingroup drawstuff
 */
DS_API void dsSetDrawMode(int mode);

// Backwards compatible API
#define dsDrawCappedCylinder dsDrawCapsule
#define dsDrawCappedCylinderD dsDrawCapsuleD
#define dsSetCappedCylinderQuality dsSetCapsuleQuality

/* closing bracket for extern "C" */
#ifdef __cplusplus
}
#endif

#endif

