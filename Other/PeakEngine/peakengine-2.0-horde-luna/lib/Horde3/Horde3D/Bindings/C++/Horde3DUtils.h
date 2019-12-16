// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

/*	Title: Horde3D Utility Library */

#pragma once

#include "Horde3D.h"

#ifndef DLL
#	if defined( WIN32 ) || defined( _WINDOWS )
#		define DLL extern "C" __declspec( dllimport )
#	else
#		define DLL extern "C"
#	endif
#endif


/*	Topic: Introduction
		Some words about the Utility Library.
	
	Horde3D has a simple core API which offers all the functionality needed to control the engine. The
	engine core is intended to be as generic as possible to make the complete system lightweight and clean.
	Nevertheless, it is sometimes useful to have more specific functions in order to increase productivity.
	For this reason the Utility Library is offered. It has some very handy functions that can help
	to make your life easier.
*/


namespace Horde3DUtils
{
	/*	Group: General functions */
	/*	Function: freeMem
			Frees memory allocated by the Utility Library.
		
		This utility function frees the memory that was allocated by another function of the Utility Library.
		
		Parameters:
			ptr	- address of a pointer that references to memory allocated by the Utility Library
			
		Returns:
			nothing
	*/
	DLL void freeMem( char **ptr );
	
	/*	Function: dumpMessages
			Writes all messages in the queue to a log file.
		
		This utility function pops all messages from the message queue and writes them to a HTML formated
        log file 'EngineLog.html'.
		
		Parameters:
			none
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool dumpMessages();
	
	
	/*	Group: OpenGL-related functions */
	/* 	Function: initOpenGL
			Initializes OpenGL.
		
		This utility function initializes an OpenGL rendering context in a specified window component.
        *Currently this function is only available on Windows platforms.*
		
		Parameters:
			hDC	- handle to device context for which OpenGL context shall be created
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool initOpenGL( int hDC );
	
	/* 	Function: releaseOpenGL
			Releases OpenGL.
		
		This utility function destroys the previously created OpenGL rendering context.
        *Currently this function is only available on Windows platforms.*
		
		Parameters:
			none
			
		Returns:
			nothing
	*/
	DLL void releaseOpenGL();
	
	/* 	Function: swapBuffers
			Displays the rendered image on the screen.
		
		This utility function displays the image rendered to the previously initialized OpenGL context
		on the screen by copying it from the backbuffer to the frontbuffer.
        *Currently this function is only available on Windows platforms.*
		
		Parameters:
			none
			
		Returns:
			nothing
	*/
	DLL void swapBuffers();

	
	/*	Group: Resource management */
	/* 	Function: getResourcePath
			Returns  the search path of a resource type.
		
		This function returns the search path of a specified resource type.
		
		Parameters:
			type	- type of resource
			
		Returns:
			pointer to the search path string
	*/
	DLL const char *getResourcePath( int type );
	
	/* 	Function: setResourcePath
			Sets the search path for a resource type.
		
		This function sets the search path for a specified resource type.
		
		Parameters:
			type	- type of resource
			path	- path where the resources can be found ((back-)slashes at end are removed)
			
		Returns:
			nothing
	*/
	DLL void setResourcePath( int type, const char *path );
	
	/* 	Function: loadResourcesFromDisk
			Loads previously added resources from a data drive.
		
		This utility function loads previously added and still unloaded resources from the specified
		directories on a data drive. Several search paths can be specified using the pipe character (|)
		as separator. All resource names are directly converted to filenames and the function tries to
		find them in the specified directories using the given order of the search paths.
		
		Parameters:
			contentDir	- directories where data is located on the drive ((back-)slashes at end are removed)
			
		Returns:
			false if at least one resource could not be loaded, otherwise true
	*/
	DLL bool loadResourcesFromDisk( const char *contentDir );
	
	/* 	Function: createTGAImage
			Creates a TGA image in memory.
		
		This utility function allocates memory at the pointer outData and creates a TGA image from the
		specified pixel data. The dimensions of the image have to be specified as well as the bit depth.
		The created TGA-image-data can be used as Texture2D or TexureCube resource in the engine.
		*Note: The memory allocated by this routine has to freed manually using the freeMem function.*
		
		Parameters:
			pixels	- pointer to pixel source data in BGR(A) format from which TGA-image is constructed;
						-memory layout: pixel with position (x, y) in image (origin of image is upper left
						 corner) has memory location (y * width + x) * (bpp / 8) in pixels-array  
			width	- width of source image
			height	- height of source image
			bpp		- color bit depth of source data (valid values: 24, 32)
			outData	- address of a pointer to which the address of the created memory block is written
			outSize	- variable to which to size of the created memory block is written
			
		Returns:
			false if at least one resource could not be loaded, otherwise true
	*/
	DLL bool createTGAImage( const unsigned char *pixels, int width, int height, int bpp,
							 char **outData, int *outSize );
	
	
	/*	Group: Scene graph */
	/* 	Function: pickRay
	 *  	Calculates the ray originating at the specified camera and window coordinates
		
		This utility function takes normalized window coordinates (ranging from 0 to 1 with the
		origin being the bottom left corner of the window) and returns ray origin and direction for the
		given camera. The function is especially useful for selecting objects by clicking
		on them.
		
		Parameters:
			cameraNode	- camera used for picking
			nwx, nwy	- normalized window coordinates
			ox, oy, oz  - calculated ray origin
			dx, dy, dz  - calculated ray direction
			
		Returns:
			nothing
	*/
	DLL void pickRay(NodeHandle cameraNode, float nwx, float nwy, float *ox, float *oy, float *oz, float *dx, float *dy, float *dz );
	
	/* 	Function: pickNode
			Returns the scene node which is at the specified window coordinates.
		
		This utility function takes normalized window coordinates (ranging from 0 to 1 with the
		origin being the bottom left corner of the window) and returns the scene node which is
		visible at that location. The function is especially useful for selecting objects by clicking
		on them. Currently picking is only working for Meshes.
		
		Parameters:
			cameraNode	- camera used for picking
			nwx, nwy	- normalized window coordinates

		Returns:
			handle of picked node or 0 if no node was hit
	*/
	DLL NodeHandle pickNode(NodeHandle cameraNode, float nwx, float nwy );

	/*	Group: Overlays */
	/* 	Function: showText
			Shows text on the screen using a font texture.
		
		This utility function uses overlays to display a text string at a specified position on the screen.
		The font texture of the specified font material has to be a regular 16x16 grid containing all
		ASCII characters in row-major order. The layer corresponds to the layer parameter of overlays.
		
		Parameters:
			text			- text string to be displayed
			x, y			- position of the lower left corner of the first character;
							  for more details on coordinate system see overlay documentation
			size 			- size (scale) factor of the font
			layer			- layer index of the font overlays
			fontMaterialRes	- font material resource used for rendering
			
		Returns:
			nothing
	*/
	DLL void showText( const char *text, float x, float y, float size,
					   int layer, ResHandle fontMaterialRes );

	/* 	Function: showFrameStats
			Shows frame statistics on the screen.
		
		This utility function displays statistics for the current frame in the upper left corner of
		the screen. Since the statistic counters are reset after the call, it should be called exactly
		once per frame to obtain correct values.
		
		Parameters:
			fontMaterialRes	- font material resource used for drawing text
			curFPS			- frames per second with which application is currently running
			
		Returns:
			nothing
	*/
	DLL void showFrameStats( ResHandle fontMaterialRes, float curFPS );
};
