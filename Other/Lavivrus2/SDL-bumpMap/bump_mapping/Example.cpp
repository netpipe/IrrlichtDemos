/**************************************
*                                     *
*   Jeff Molofee's Basecode Example   *
*          nehe.gamedev.net           *
*                2001                 *
*                                     *
**************************************/

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glaux.h>											// Header File For The GLaux Library
#include "NeHeGL.h"												// Header File For NeHeGL
#include "math\All.h"											// Header File For Vector/Matrix math
#include "Extensions.h"

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking
#pragma comment( lib, "glaux.lib" )								// Search For GLaux.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

vec3 light_position;

GLuint base_map;												// Base Color Texture
GLuint bump_map;												// Bump Map Texture
GLuint normalization_cube_map;									// Normalization Cube Map

GL_Window*	g_window;
Keys*		g_keys;

// User Defined Variables
float		angle;												// Used To Rotate The Triangles
int			rot1, rot2;											// Counter Variables

BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;

	// Start Of User Initialization
	angle		= 0.0f;											// Set Starting Angle To Zero

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate

	if (!BuildTexture("Images\\Rock.jpg", base_map))				// Loading Base Map
		return FALSE;											// Return False If Loading Failed

	if (!BuildTexture("Images\\RockNormal.bmp", bump_map))		// Loading Bump Map
		return FALSE;											// Return False If Loading Failed

	GenNormalizationCubeMap(128, normalization_cube_map);		// Generate Cubemap with a size of 128x128 each face

	SetUp_GL_ARB_multitexture();								// Setting up Multitexturing

	return TRUE;												// Return TRUE (Initialization Successful)
}

void Deinitialize (void)										// Any User DeInitialization Goes Here
{
}

void Update (DWORD milliseconds)								// Perform Motion Updates Here
{
	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}

	angle += (float)(milliseconds) / 5.0f;						// Update angle Based On The Clock
}

void Draw (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity ();											// Reset The Modelview Matrix
	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set The First Texture Unit To Normalize Our Vector From The Surface To The Light.
	// Set The Texture Environment Of The First Texture Unit To Replace It With The
	// Sampled Value Of The Normalization Cube Map.
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, normalization_cube_map);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE) ;
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE) ;

	// Set The Second Unit To The Bump Map.
	// Set The Texture Environment Of The Second Texture Unit To Perform A Dot3
	// Operation With The Value Of The Previous Texture Unit (The Normalized
	// Vector Form The Surface To The Light) And The Sampled Texture Value (The
	// Normalized Normal Vector Of Our Bump Map).
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bump_map);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB) ;
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS) ;
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE) ;

	// Set The Third Texture Unit To Our Texture.
	// Set The Texture Environment Of The Third Texture Unit To Modulate
	// (Multiply) The Result Of Our Dot3 Operation With The Texture Value.
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, base_map);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Now We Draw Our Object (Remember That We First Have To Calculate The
	// (UnNormalized) Vector From Each Vertex To Our Light).

	vec3 vertex_to_light;
	vec3 vertex_position;

	// setting the light position (light rotates around z-axis)
	vec3 light_position = get_rot_z_mat3(angle) * get_vec3(0.7f, 0.0f, -0.5f);

	glBegin(GL_QUADS);

		// lower left Vertex
		vertex_position = get_vec3(-1.0f, -1.0f, -1.0f);
		// calculate to vector from the vertex to the light
		vertex_to_light = light_position - vertex_position;

		// passing vector from vertex to the light to texture unit 0
		glMultiTexCoord3f(GL_TEXTURE0, vertex_to_light[0], vertex_to_light[1], vertex_to_light[2]);
		// passing texture coordinate to texture unit 1 and 2
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);

		// rendering vertex
		glVertex3f(vertex_position[0], vertex_position[1], vertex_position[2]);


		// lower right Vertex
		vertex_position = get_vec3(1.0f, -1.0f, -1.0f);
		// calculate to vector from the vertex to the light
		vertex_to_light = light_position - vertex_position;

		// passing vector from vertex to the light to texture unit 0
		glMultiTexCoord3f(GL_TEXTURE0, vertex_to_light[0], vertex_to_light[1], vertex_to_light[2]);
		// passing texture coordinate to texture unit 1 and 2
		glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);

		// rendering vertex
		glVertex3f(vertex_position[0], vertex_position[1], vertex_position[2]);


		// upper right Vertex
        vertex_position = get_vec3(1.0f, 1.0f, -1.0f);
		// calculate to vector from the vertex to the light
		vertex_to_light = light_position - vertex_position;

		// passing vector from vertex to the light to texture unit 0
		glMultiTexCoord3f(GL_TEXTURE0, vertex_to_light[0], vertex_to_light[1], vertex_to_light[2]);
		// passing texture coordinate to texture unit 1 and 2
		glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);

		// rendering vertex
		glVertex3f(vertex_position[0], vertex_position[1], vertex_position[2]);


		// upper left Vertex
		vertex_position = get_vec3(-1.0f, 1.0f, -1.0f);
		// calculate to vector from the vertex to the light
		vertex_to_light = light_position - vertex_position;

		// passing vector from vertex to the light to texture unit 0
		glMultiTexCoord3f(GL_TEXTURE0, vertex_to_light[0], vertex_to_light[1], vertex_to_light[2]);
		// passing texture coordinate to texture unit 1 and 2
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);

		// rendering vertex
		glVertex3f(vertex_position[0], vertex_position[1], vertex_position[2]);

	glEnd();

	glFlush ();													// Flush The GL Rendering Pipeline
}
