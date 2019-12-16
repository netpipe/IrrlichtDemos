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

/*	Title: Horde3D API */

#pragma once

#ifndef DLL
#	if defined( WIN32 ) || defined( _WINDOWS )
#		define DLL extern "C" __declspec( dllimport )
#	else
#		define DLL extern "C"
#	endif
#endif


/*	Topic: Conventions
		Some conventions for the API.
	
	Horde3D uses a right-handed coordinate system, where y is the up-axis and the positive z-axis is
	pointing out of the screen. The rotations are specified in degrees and rotation direction is
	counter-clockwise when looking down the axis from the the positive end towards the origin. View
	vectors like the camera or light direction are always pointing along the negative z-axis when no
	transformation is applied. Matrices in Horde are stored in a column-major memory layout.
*/


// *************************************************************************************************
// Core Library
// *************************************************************************************************

/* Group: Typedefs and constants */

/*	Constants: Typedefs
	ResHandle	- handle to resource (int)
	NodeHandle	- handle to scene node (int)
*/
typedef int ResHandle;
typedef int NodeHandle;


/*	Constants: Predefined constants
	RootNode		- Scene root node handle
*/
const NodeHandle RootNode = 1;


/* Group: Enumerations */
struct EngineOptions
{
	/*	Enum: EngineOptions
			The available engine option parameters.
		
		MaxLogLevel			- Defines the maximum log level; only messages which are smaller or equal to this value
							  (hence more important) are published in the message queue. (Default: 4)
		MaxNumMessages      - Defines the maximum number of messages held in the queue until getMessage is called (Default 512)
		TrilinearFiltering	- Enables or disables trilinear filtering for textures; only affects textures
							  that are loaded after setting the option. (Values: 0, 1; Default: 1)
		AnisotropyFactor	- Sets the quality for anisotropic filtering; only affects textures that
							  are loaded after setting the option. (Values: 1, 2, 4, 8; Default: 1)
		TexCompression		- Enables or disables texture compression; only affects textures that are
							  loaded after setting the option. (Values: 0, 1; Default: 0)
		LoadTextures		- Enables or disables loading of texture images; option can be used to
							  minimize loading times for testing. (Values: 0, 1; Default: 1)
		FastAnimation		- Disables or enables inter-frame interpolation for animations. (Values: 0, 1; Default: 1)
		ShadowMapSize		- Sets the size of the shadow map buffer (Values: 128, 256, 512, 1024, 2048; Default: 1024)
		SampleCount			- Maximum number of samples used for multisampled render targets; only affects pipelines
							  that are loaded after setting the option. (Values: 0, 2, 4, 8, 16; Default: 0)
		WireframeMode		- Enables or disables wireframe rendering
		DebugViewMode		- Enables or disables debug view where geometry is rendered in wireframe without shaders and
							  lights are visualized using their screen space bounding box. (Values: 0, 1; Default: 0)
	*/
	enum List
	{
		MaxLogLevel = 1,
		MaxNumMessages,
		TrilinearFiltering,
		AnisotropyFactor,
		TexCompression,
		LoadTextures,
		FastAnimation,
		ShadowMapSize,
		SampleCount,
		WireframeMode,
		DebugViewMode
	};
};

struct EngineStats
{
	/*	Enum: EngineStats
			The available engine statistic parameters.
		
		TriCount		- Number of triangles that were pushed to the renderer
		BatchCount		- Number of batches (draw calls)
		LightPassCount	- Number of lighting passes
	*/
	enum List
	{
		TriCount = 100,
		BatchCount,
		LightPassCount
	};
};

struct ResourceTypes
{
	/*	Enum: ResourceTypes
			The available resource types.
		
		Undefined	- An undefined resource, returned by getResourceType in case of error
		SceneGraph	- Scene graph subtree stored in XML format
		Geometry	- Geometrical data containing bones, vertices and triangles
		Animation	- Animation data
		Material	- Material script
		Code		- Text block containing shader source code
		Shader		- Shader program
		Texture2D	- Two-dimensional texture map
		TextureCube	- Cube map texture
		Effect		- Particle configuration
		Pipeline	- Rendering pipeline
	*/
	enum List
	{
		Undefined = 0,
		SceneGraph,
		Geometry,
		Animation,
		Material,
		Code,
		Shader,
		Texture2D,
		TextureCube,
		Effect,
		Pipeline
	};
};

struct ResourceFlags
{
	/*	Enum: ResourceFlags
			The available flags used when adding a resource.
			
		NoQuery				- Excludes resource from being listed by queryUnloadedResource function.
		NoTexPOTConversion	- Disables texture conversion to power-of-two dimensions on hardware without NPOT-support.
		NoTexCompression	- Disables texture compression for Texture2D or TextureCube resource.
		NoTexMipmaps		- Disables generation of mipmaps for textures.
		NoTexFiltering		- Disables bilinear filtering for textures.
		NoTexRepeat			- Disables tiling (repeat mode) for textures and enables clamping instead.
	*/
	enum Flags
	{
		NoQuery = 1,
		NoTexPOTConversion = 2,
		NoTexCompression = 4,
		NoTexMipmaps = 8,
		NoTexFiltering = 16,
		NoTexRepeat = 32
	};
};

struct GeometryResParams
{
	/*	Enum: GeometryResParams
		   The available Geometry resource parameters.	  
	   
		VertexCount	- Number of vertices; valid for getResourceParami
		IndexCount	- Number of triangle indices; valid for getResourceParami
		VertexData	- Vertex positon data (pointer to float); valid for getResourceData
		IndexData	- Triangle indices (pointer to uint); valid for getResourceData
	 */
	enum List
	{
		VertexCount = 200,
		IndexCount,
		VertexData,
		IndexData
	};
};

struct AnimationResParams
{
	/*	Enum: AnimationResParams
			The available Animation resource parameters.
	  
	   FrameCount	- Number of animation frames; valid for getResourceParami
	*/
	enum List
	{
		FrameCount = 300
	};
};

struct MaterialResParams
{
	/*	Enum: MaterialResParams
			The available Material resource parameters.

		Class   	- Hierarchical class name (Default: empty string);
					  valid for get-/setResourceParamstr
		Link	    - Link to other material resource;
					  valid for get-/setResourceParami
		Shader      - Shader resource used for rendering;
					  valid for get-/setResourceParami
		TexUnit_0	- Texture resource for the first unit;	
					  valid for get-/setResourceParami
		TexUnit_1	- Texture resource for the second unit;
					  valid for get-/setResourceParami
		TexUnit_2	- Texture resource for the third unit;
					  valid for get-/setResourceParami
		TexUnit_3	- Texture resource for the fourth unit;
					  valid for get-/setResourceParami
		TexUnit_4	- Texture resource for the fifth unit;
					  valid for get-/setResourceParami
		TexUnit_5	- Texture resource for the sixth unit;
					  valid for get-/setResourceParami
		TexUnit_6	- Texture resource for the seventh unit;
					  valid for get-/setResourceParami
		TexUnit_7	- Texture resource for the eighth unit;
					  valid for get-/setResourceParami
		TexUnit_8	- Texture resource for the ninth unit;
					  valid for get-/setResourceParami
		TexUnit_9	- Texture resource for the tenth unit;
					  valid for get-/setResourceParami
		TexUnit_10	- Texture resource for the eleventh unit;
					  valid for get-/setResourceParami
		TexUnit_11	- Texture resource for the twelfth unit;
					  valid for get-/setResourceParami
	 */
	enum List
	{
		Class = 400,
		Link,
		Shader,
		TexUnit_0,
		TexUnit_1,
		TexUnit_2,
		TexUnit_3,
		TexUnit_4,
		TexUnit_5,
		TexUnit_6,
		TexUnit_7,
		TexUnit_8,
		TexUnit_9,
		TexUnit_10,
		TexUnit_11
	};	
};

struct TextureResParams
{
	/*	Enum: TextureResParams
			The available Texture2D and TextureCube resource parameters.

		PixelData	- Image pixel data (pointer to unsigned char); valid for updateResourceData for Texture2D
		Width		- Image width in pixels; valid for getResourceParami
		Height		- Image height in pixels; valid for getResourceParami
		Comps		- Number of channels in image (e.g. an RGBA image has 4 channels); valid for getResourceParami
		HDR			- Flag indicating whether the texture is a HDR image (returns 1) or a usual 8 bit per channel image (returns 0);
		              valid for getResourceParami
	 */
	enum List
	{
		PixelData = 700,
		Width,
		Height,
		Comps,
		HDR
	};
};

struct EffectResParams
{
	/*	Enum: EffectResParams
			The available Effect resource parameters.
	  
	   LifeMin          - Minimum value for selecting random life time; valid for get-/setResourceParamf
	   LifeMax	        - Maximum value for selecting random life time; valid for get-/setResourceParamf
	   MoveVelMin       - Minimum value for selecting random initial value of velocity defining 
						  how many units per second particle is moving; valid for get-/setResourceParamf 
	   MoveVelMax       - Maximum value for selecting random initial value of velocity defining 
						  how many units per second particle is moving; valid for get-/setResourceParamf
	   MoveVelEndRate   - Percentage of the initial translation velocity value when particle is dying;
						  valid for get-/setResourceParamf
	   RotVelMin        - Minimum value for selecting random initial value of velocity defining 
						  how many degrees per second particle is rotating; valid for get-/setResourceParamf
	   RotVelMax        - Maximum value for selecting random initial value of velocity defining 
					  	  how many degrees per second particle is rotating; valid for get-/setResourceParamf
	   RotVelEndRate    - Percentage of the initial rotation velocity value when particle is dying;		
						  valid for get-/setResourceParamf
	   SizeVelMin       - Minimum value for selecting random initial size value; 
	                      valid for get-/setResourceParamf
	   SizeVelMax       - Maximum value for selecting random initial size value; 
						  valid for get-/setResourceParamf					      
	   SizeVelEndRate   - Percentage of the initial size value when particle is dying;
						  valid for get-/setResourceParamf					      
	   Col_R_Min        - Minimum value for selecting random initial red color value;					     
						  valid for get-/setResourceParamf					      
	   Col_R_Max        - Maximum value for selecting random initial red color value;					      
						  valid for get-/setResourceParamf					      
	   Col_R_EndRate    - Percentage of the initial red value when particle is dying;	   
						  valid for get-/setResourceParamf					      
	   Col_G_Min        - Minimum value for selecting random initial green color value;
						  valid for get-/setResourceParamf					      
	   Col_G_Max        - Maximum value for selecting random initial green color value;
						  valid for get-/setResourceParamf					      
	   Col_G_EndRate    - Percentage of the initial green value when particle is dying;
						  valid for get-/setResourceParamf					      
	   Col_B_Min        - Minimum value for selecting random initial blue color value;
						  valid for get-/setResourceParamf					      
	   Col_B_Max        - Maximum value for selecting random initial blue color value;
						  valid for get-/setResourceParamf					      
	   Col_B_EndRate    - Percentage of the initial blue value when particle is dying;
						  valid for get-/setResourceParamf					      
	   Col_A_Min        - Minimum value for selecting random initial alpha color value;
						  valid for get-/setResourceParamf					      
	   Col_A_Max        - Maximum value for selecting random initial alpha color value;
						  valid for get-/setResourceParamf					      
	   Col_A_EndRate    - Percentage of the initial alpha value when particle is dying;
						  valid for get-/setResourceParamf					      
	*/
	enum List
	{
		LifeMin = 900,
		LifeMax,
		MoveVelMin,
		MoveVelMax,
		MoveVelEndRate,
		RotVelMin,
		RotVelMax,
		RotVelEndRate,
		SizeMin,
		SizeMax,
		SizeEndRate,
		Col_R_Min,
		Col_R_Max,
		Col_R_EndRate,
		Col_G_Min,
		Col_G_Max,
		Col_G_EndRate,
		Col_B_Min,
		Col_B_Max,
		Col_B_EndRate,
		Col_A_Min,
		Col_A_Max,
		Col_A_EndRate
	};
};

struct SceneNodeTypes
{
	/*	Enum: SceneNodeTypes
			The available scene node types.
		
		Undefined	- An undefined node type, returned by getNodeType in case of error
		Group		- Group of different scene nodes
		Model		- 3D model with optional skeleton
		Mesh		- Subgroup of a model with triangles of one material
		Joint		- Joint for skeletal animation
		Light		- Light source
		Camera		- Camera giving view on scene
		Emitter		- Particle system emitter
	*/
	enum List
	{
		Undefined = 0,
		Group,
		Model,
		Mesh,
		Joint,
		Light,
		Camera,
		Emitter
	};
};

struct SceneNodeParams
{
	/*  Enum: SceneNodeParams
	        The available Scene node parameters.

			Name				- The name of a scene node; used e.g. to find a node which 
								  handle may not known to the application [type: string]
		    AttachmentString    - Optional XML data of an 'Attachment' node attached to a scene node [type: string]

	        *Important Note: The pointer is const and allows only read access to the data. Do never try to modify the
					         data of the pointer since that can corrupt the engine's internal states!*

	*/
	enum List
	{
		Name = 1,
		AttachmentString
	};
};

struct GroupNodeParams
{
	/*	Enum: GroupNodeParams
			The available Group node parameters.
	
		MinDist	- Minimal distance from the viewer for the node to be visible
				  (default: 0.0); used for level of detail [type: float]
		MaxDist	- Maximal distance from the viewer for the node to be visible
				  (default: infinite); used for level of detail [type: float]
	*/
	enum List
	{
		MinDist = 100,
		MaxDist
	};
};

struct ModelNodeParams
{
	/*	Enum: ModelNodeParams
			The available Model node parameters

		GeometryRes			- Geometry resource used for the model [type: ResHandle]
		SoftwareSkinning	- Enables or disables software skinning (default: 0) [type: int]
	*/
	enum List
	{
		GeometryRes = 200,
		SoftwareSkinning
	};
};

struct MeshNodeParams
{
	/*	Enum: MeshNodeParams
			The available Mesh node parameters.
		
		MaterialRes	- Material resource used for the mesh [type: ResHandle]
		BatchStart	- First triangle index of mesh in Geometry resource of parent Model node [type: int, read-only]
		BatchCount	- Number of triangle indices used for drawing mesh [type: int, read-only]
		VertRStart	- First vertex in Geometry resource of parent Model node [type: int, read-only]
		VertREnd	- Last vertex in Geometry resource of parent Model node [type: int, read-only]
	*/
	enum List
	{
		MaterialRes = 300,
		BatchStart,
		BatchCount,
		VertRStart,
		VertREnd
	};
};

struct JointNodeParams
{
	/*	Enum: JointNodeParams
			The available Joint node parameters.

		JointIndex - Index of joint in Geometry resource of parent Model node [type: int, read-only]
	*/
	enum List
	{
		JointIndex = 400
	};
};

struct LightNodeParams
{
	/*	Enum: LightNodeParams
			The available Light node parameters.
		
		MaterialRes			- Material resource used for the light [type: ResHandle]
		Radius				- Radius of influence (default: 100.0) [type: float]
		FOV					- Field of view (FOV) angle (default: 90.0) [type: float]
		Col_R				- Red component of light diffuse color (default: 1.0) [type: float]
		Col_G				- Green component of light diffuse color (default: 1.0) [type: float]
		Col_B				- Blue component of light diffuse color (default: 1.0) [type: float]
		ShadowMapCount		- Number of shadow maps used for light source (values: 0, 1, 2, 3, 4; default: 0) [type: int]
		ShadowSplitLambda	- Constant determining segmentation of view frustum for Parallel Split Shadow Maps
							  (default: 0.5) [type: float]
		ShadowMapBias		- Bias value for shadow mapping to reduce shadow acne (default: 0.005) [type: float]
	*/
	enum List
	{
		MaterialRes = 500,
		Radius,
		FOV,
		Col_R,
		Col_G,
		Col_B,
		ShadowMapCount,
		ShadowSplitLambda,
		ShadowMapBias
	};
};

struct CameraNodeParams
{
	/*	Enum: CameraNodeParams
			The available Camera node parameters.
		
		PipelineRes			- Pipeline resource used for rendering [type: ResHandle]
		OutputTex			- Texture2D resource used as output buffer (can be 0 to use main framebuffer) (default: 0) [type: ResHandle]
		OutputBufferIndex	- Index of the output buffer for stereo rendering (values: 0 for left eye, 1 for right eye) (default: 0) [type: int]
		LeftPlane			- Coordinate of left plane relative to near plane center (default: -0.055228457) [type: float]
		RightPlane			- Coordinate of right plane relative to near plane center (default: 0.055228457) [type: float]
		BottomPlane			- Coordinate of bottom plane relative to near plane center (default: -0.041421354f) [type: float]
		TopPlane			- Coordinate of top plane relative to near plane center (default: 0.041421354f) [type: float]
		NearPlane			- Distance of near clipping plane (default: 0.1) [type: float]
		FarPlane			- Distance of far clipping plane (default: 1000) [type: float]
		Orthographic		- Flag for setting up an orthographic frustum instead of a perspective one (default: 0) [type:int]
		OcclusionCulling	- Flag for enabling occlusion culling (default: 0) [type:int]
	*/
	enum List
	{
		PipelineRes = 600,
		OutputTex,
		OutputBufferIndex,
		LeftPlane,
		RightPlane,
		BottomPlane,
		TopPlane,
		NearPlane,
		FarPlane,
		Orthographic,
		OcclusionCulling
	};
};

struct EmitterNodeParams
{
	/*	Enum: EmitterNodeParams
			The available Emitter node parameters.
		
		MaterialRes		- Material resource used for rendering [type: ResHandle]
		EffectRes		- Effect resource which configures particle properties [type: ResHandle]
		MaxCount		- Maximal number of particles living at the same time [type: int]
		RespawnCount	- Number of times a single particle is recreated after dying (-1 for infinite) [type: int]
		Delay			- Time in seconds before emitter begins creating particles (default: 0.0) [type: float]
		EmissionRate	- Maximal number of particles to be created per second (default: 0.0) [type: float]
		SpreadAngle		- Angle of cone for random emission direction (default: 0.0) [type: float]
		ForceX			- X-component of force vector applied to particles (default: 0.0) [type: float]
		ForceY			- Y-component of force vector applied to particles (default: 0.0) [type: float]
		ForceZ			- Z-component of force vector applied to particles (default: 0.0) [type: float]
	*/
	enum List
	{
		MaterialRes = 700,
		EffectRes,
		MaxCount,
		RespawnCount,
		Delay,
		EmissionRate,
		SpreadAngle,
		ForceX,
		ForceY,
		ForceZ
	};
};


namespace Horde3D
{
	/* Group: Basic functions */
	/* 	Function: getVersionString
			Returns the engine version string.
		
		This function returns a pointer to a string containing the current version of Horde3D.
		
		Parameters:
			none
			
		Returns:
			pointer to the version string
	*/
	DLL const char *getVersionString();

	/* 	Function: checkExtension
			Checks if an extension is part of the engine library.
		
		This function checks if a specified extension is contained in the DLL/shared object of the engine.
		
		Parameters:
			extensionName	- name of the extension
			
		Returns:
			true if extension is implemented, otherwise false
	*/
	DLL bool checkExtension( const char *extensionName );
	
	/* 	Function: init
			Initializes the engine.
		
		This function initializes the graphics engine and makes it ready for use. It has to be the
		first call to the engine except for getVersionString. In order to successfully initialize
		the engine the calling application must provide a valid OpenGL context. The function can be
		called several times on different rendering contexts in order to initialize them.
		
		Parameters:
			none
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL	bool init();
	
	/* 	Function: release
			Releases the engine.
		
		This function releases the engine and frees all objects and associated memory. It should be
		called when the application is destroyed.
		
		Parameters:
			none
			
		Returns:
			nothing
	*/
	DLL void release();
	
	/* 	Function: resize
			Resizes the viewport.
		
		This function sets the dimensions of the rendering viewport. It has to be called after
		initialization and whenever the viewport size changes.
		
		Parameters:
			x		- the x-position of the viewport in the rendering context
			y		- the y-position of the viewport in the rendering context
			width	- the width of the viewport
			height	- the height of the viewport
			
		Returns:
			nothing
	*/
	DLL void resize( int x, int y, int width, int height );
	
	/* 	Function: render
			Main rendering function.
		
		This is the main function of the engine. It executes all the rendering, animation and other
		tasks. The function can be called several times per frame, for example in order to write to different
		output buffers.
		
		Parameters:
			cameraNode	- camera node used for rendering scene
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool render( NodeHandle cameraNode );
	
	/* 	Function: clear
			Removes all resources and scene nodes.
		
		This function removes all nodes from the scene graph except the root node and releases all resources.
        
        *Warning: All resource and node IDs are invalid after calling this function*
		
		Parameters:
			none
			
		Returns:
			nothing
	*/
	DLL void clear();


	// --- General functions ---
	/* 	Function: getMessage
			Gets the next message from the message queue.
		
		This function returns the next message string from the message queue and writes additional information
        to the specified variables. If no message is left over in the queue an empty string is returned.
		
		Parameters:
			level	- pointer to variable for storing message level indicating importance (can be NULL)
			time	- pointer to variable for storing time when message was added (can be NULL)
			
		Returns:
			message string or empty string if no message is in queue
	*/
	DLL const char *getMessage( int *level, float *time );
	
	/* 	Function: getOption
			Gets an option parameter of the engine.
		
		This function gets a specified option parameter and returns its value.
		
		Parameters:
			param	- option parameter
			
		Returns:
			current value of the specified option parameter
	*/
	DLL float getOption( EngineOptions::List param );
	
	/* 	Function: setOption
			Sets an option parameter for the engine.
		
		This function sets a specified option parameter to a specified value.
		
		Parameters:
			param	- option parameter
			value	- value of the option parameter
			
		Returns:
			true if the option could be set to the specified value, otherwise false
	*/
	DLL bool setOption( EngineOptions::List param, float value );

	/* 	Function: getStat
			Gets a statistic value of the engine.
		
		This function returns the value of the specified statistic. The reset flag makes it possible
		to reset the statistic value after reading.
		
		Parameters:
			param	- statistic parameter
			reset	- flag specifying whether statistic value should be reset
			
		Returns:
			current value of the specified statistic parameter
	*/
	DLL float getStat( EngineStats::List param, bool reset );
	
	/* 	Function: showOverlay
			Shows an overlay on the screen.
		
		This function displays an overlay with a specified material at a specified position on the screen.
        An overlay is a 2D image that can be used to render 2D GUI elements. The coordinate system
        used has its origin (0, 0) at the lower left corner of the screen and its maximum (1, 1) at
        the upper right corner. Texture coordinates are using the same system, where the coordinates
        (0, 0) correspond to the lower left corner of the image.
        Overlays can have different layers which describe the order in which they are drawn. Overlays with
        smaller layer numbers are drawn before overlays with higher layer numbers.
        Note that the overlays have to be removed manually using the function clearOverlays.
		
		Parameters:
			x_ll, y_ll, u_ll, v_ll	- position and texture coordinates of the lower left corner
			x_lr, y_lr, u_lr, v_lr	- position and texture coordinates of the lower right corner
			x_ur, y_ur, u_ur, v_ur	- position and texture coordinates of the upper right corner
			x_ul, y_ul, u_ul, v_ul	- position and texture coordinates of the upper left corner
			layer					- layer index of the overlay (Values: from 0 to 7)
			materialRes				- material resource used for rendering
			
		Returns:
			nothing
	*/
	DLL void showOverlay( float x_ll, float y_ll, float u_ll, float v_ll,
						  float x_lr, float y_lr, float u_lr, float v_lr,
						  float x_ur, float y_ur, float u_ur, float v_ur,
						  float x_ul, float y_ul, float u_ul, float v_ul,
						  int layer, ResHandle materialRes );

	/* 	Function: clearOverlays
			Removes all overlays.
		
		This function removes all overlays that were added usig showOverlay.
		
		Parameters:
			none
			
		Returns:
			nothing
	*/
	DLL void clearOverlays();


	/* Group: General resource management functions */
	/* 	Function: getResourceType
			Returns the type of a resource.
		
		This function returns the type of a specified resource. If the resource handle is invalid,
        the function returns the resource type 'Unknown'.
		
		Parameters:
			res	- handle to the resource
			
		Returns:
			type of the resource
	*/
	DLL ResourceTypes::List getResourceType( ResHandle res );

	/* 	Function: getResourceName
			Returns the name of a resource.
		
		This function returns a pointer to the name of a specified resource. If the resource handle
		is invalid, the function returns an empty string.

        *Important Note: The pointer is const and allows only read access to the data. Do never try to modify the
        data of the pointer since that can corrupt the engine's internal states!*
		
		Parameters:
			res	- handle to the resource
			
		Returns:
			name of the resource or empty string in case of failure
	*/
	DLL const char *getResourceName( ResHandle res );
	
	/* 	Function: findResource
			Finds a resource and returns its handle.
		
		This function searches the resource of the specified type and name and returns its handle. If
        the resource is not available in the resource manager a zero handle is returned.
		
		Parameters:
			type	- type of the resource
			name	- name of the resource
			
		Returns:
			handle to the resource or 0 if not found
	*/
	DLL ResHandle findResource( ResourceTypes::List type, const char *name );
	
	/* 	Function: addResource
			Adds a resource.
		
		This function tries to add a resource of a specified type and name to the resource manager. If
        a resource of the same type and name is already existing, the handle to the existing resource is returned
        instead of creating a new one and the user reference count of the resource is increased.

		*Note: The name string may not contain a colon character (:)*
		
		Parameters:
			type	- type of the resource
			name	- name of the resource
			flags	- flags used for creating the resource
			
		Returns:
			handle to the resource to be added or 0 in case of failure
	*/
	DLL ResHandle addResource( ResourceTypes::List type, const char *name, int flags );

	/* 	Function: cloneResource
			Duplicates a resource.
		
		This function duplicates a specified resource. In the cloning process a new resource with the
		specified name is added to the resource manager and filled with the data of the specified source
		resource. If the specified name for the new resource is already in use, the function fails and
		returns 0. If the name string is empty, a unique name for the resource is generated automatically.
		
		*Note: The name string may not contain a colon character (:)*
		
		Parameters:
			sourceRes	- handle to resource to be cloned
			name		- name of new resource (can be empty for auto-naming)
			
		Returns:
			handle to the cloned resource or 0 in case of failure
	*/
	DLL ResHandle cloneResource( ResHandle sourceRes, const char *name );
	
	/* 	Function: removeResource
			Removes a resource.
		
		This function decreases the user reference count of a specified resource. When the user reference
		count is zero and there are no internal references, the resource can be released and removed using
		the API fuction releaseUnusedResources.
		
		Parameters:
			res	- handle to the resource to be removed
			
		Returns:
			the number of references that the application is still holding after removal or -1 in case of an error
	*/
	DLL int removeResource( ResHandle res );
	
	/* 	Function: isResourceLoaded
			Checks if a resource is loaded.
		
		This function checks if the specified resource has been successfully loaded.
		
		Parameters:
			res		- handle to the resource to be checked
			
		Returns:
			true if resource is loaded, otherwise or in case of failure false
	*/
	DLL bool isResourceLoaded( ResHandle res );
	
	/* 	Function: loadResource
			Loads a resource.
		
		This function loads data for a resource that was previously added to the resource manager.
        If data is a NULL-pointer the resource manager is told that the resource doesn't have any data
		(e.g. the corresponding file was not found). In this case, the resource remains in the unloaded state
		but is no more returned when querying unloaded resources. When the specified resource is already loaded,
		the function returns false.
		
        *Important Note: XML-data must be NULL-terminated*
		
		Parameters:
			res		- handle to the resource for which data will be loaded
			data	- pointer to the data to be loaded
			size	- size of the data block
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool loadResource( ResHandle res, const char *data, int size );
	
	/* 	Function: unloadResource
			Unloads a resource.
		
		This function unloads a previously loaded resource and restores the default values
        it had before loading. The state is set back to unloaded which makes it possible to load
        the resource again.
		
		Parameters:
			res	- handle to resource to be unloaded
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool unloadResource( ResHandle res );


	/* 	Function: getResourceParami
			Gets a property of a resource.
		
		This function returns a specified property of the specified resource.
		The property must be of the type int.
 		
		Parameters:
			res		- handle to the resource to be accessed
			param	- parameter to be accessed
			
		Returns:
			value of the parameter
	*/
	DLL int getResourceParami( ResHandle res, int param );

	/* 	Function: setResourceParami
			Sets a property of a resource.
		
		This function sets a specified property of the specified resource to a specified value.
		The property must be of the type int.
 		
		Parameters:
			node	- handle to the node to be modified
			param	- parameter to be modified
			value	- new value for the specified parameter
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setResourceParami( ResHandle res, int param, int value );

	/* 	Function: getResourceParamf
			Gets a property of a resource.
		
		This function returns a specified property of the specified resource.
		The property must be of the type float.
 		
		Parameters:
			res		- handle to the resource to be accessed
			param	- parameter to be accessed
			
		Returns:
			value of the parameter
	*/
	DLL float getResourceParamf( ResHandle res, int param );

	/* 	Function: setResourceParamf
			Sets a property of a resource.
		
		This function sets a specified property of the specified resource to a specified value.
		The property must be of the type float.
 		
		Parameters:
			node	- handle to the node to be modified
			param	- parameter to be modified
			value	- new value for the specified parameter
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setResourceParamf( ResHandle res, int param, float value );

	/* 	Function: getResourceParamstr
			Gets a property of a resource.
		
		This function returns a specified property of the specified resource.
		The property must be of the type string (const char *).
 		
		Parameters:
			res		- handle to the resource to be accessed
			param	- parameter to be accessed
			
		Returns:
			value of the property or empty string if no such property exists
	*/
	DLL const char *getResourceParamstr( ResHandle res, int param );

	/* 	Function: setResourceParamstr
			Sets a property of a resource.
		
		This function sets a specified property of the specified resource to a specified value.
		The property must be of the type string (const char *).
 		
		Parameters:
			node	- handle to the node to be modified
			param	- parameter to be modified
			value	- new value for the specified parameter
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setResourceParamstr( ResHandle res, int param, const char *value );

	/* 	Function: getResourceData
			Gives access to resource data.
		
		This function returns a pointer to the specified data of a specified resource. For information on the
        format (uint, float, ..) of the pointer see the ResourceData description.
 
        *Important Note: The pointer is const and allows only read access to the data. Do never try to modify the
        data of the pointer since that can corrupt the engine's internal states!*
		
		Parameters:
			res		- handle to the resource to be accessed
			param	- parameter indicating data of the resource that will be accessed
			
		Returns:
			pointer to the specified resource data if it is available, otherwise NULL-pointer
	*/
	DLL const void *getResourceData( ResHandle res, int param );
	
	/* 	Function: updateResourceData
			Updates the data of a resource.
		
		This function updates the content of a resource that was successfully loaded before. The new data must
        have exactly the same data layout as the data that was loaded.
        
        Notes on available ResourceData parameters:
		- Texture2DResData::PixelData
			Sets the image data of a Texture2D resource. The data pointer must point to a memory block that contains
			the pixels of the image. Each pixel needs to have 32 bit color data in BGRA format and the dimensions
			of the image (width, height) must be exactly the same as the dimensions of the image that was
			originally loaded for the resource. The first element in the data array corresponds to the lower left
			corner of the image and subsequent elements progress from left to right in the image.
		
		Parameters:
			res		- handle to the resource for which the data is modified
			param	- data structure which will be updated
			data	- pointer to the new data
			size	- size of the new data block
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool updateResourceData( ResHandle res, int param, const void *data, int size );
	
	/* 	Function: queryUnloadedResource
			Returns handle to an unloaded resource.
		
		This function looks for a resource that is not yet loaded and returns its handle.
        If there are no unloaded resources or the zero based index specified is greater than the number
		of the currently unloaded resources, 0 is returned.
		
		Parameters:
			index    - index of unloaded resource within the internal list of unloaded resources (starting with 0) 
			
		Returns:
			handle to an unloaded resource or 0
	*/
	DLL ResHandle queryUnloadedResource( int index );
	
	/* 	Function: releaseUnusedResources
			Frees resources that are no longer used.
		
		This function releases resources that are no longer used. Unused resources were either told
		to be released by the user calling removeResource or are no more referenced by any other
		engine objects.
		
		Parameters:
			none
			
		Returns:
			nothing
	*/
	DLL void releaseUnusedResources();


	/* Group: Specific resource management functions */
	/* 	Function: createTexture2D
			Adds a Texture2D resource.
		
		This function tries to create and add a Texture2D resource with the specified name to the resource
		manager. If a Texture2D resource with the same name is already existing, the function fails. The
		texture is initialized with the specified dimensions and the resource is declared as loaded. This
		function is especially useful to create dynamic textures (e.g. for displaying videos) or output buffers
		for render-to-texture.

		*Note: The name string may not contain a colon character (:)*
		
		Parameters:
			name		- name of the resource
			flags		- flags used for creating the resource
			width		- width of the texture image
			height		- height of the texture image
			renderable	- flag indicating whether the texture can be used as an output buffer for a Camera node
			
		Returns:
			handle to the created resource or 0 in case of failure
	*/
	DLL ResHandle createTexture2D( const char *name, int flags, int width, int height, bool renderable );
	
	/* 	Function: setShaderPreambles
			Sets preambles of all Shader resources.
		
		This function defines a header that is inserted at the beginning of all shaders. The preamble
		is used when a shader is compiled, so changing it will not affect any shaders that are already
		compiled. The preamble is useful for setting platform-specific defines that can be employed for
		creating several shader code paths, e.g. for supporting different hardware capabilities.
		
		Parameters:
			vertPreamble	- preamble text of vertex shaders (default: empty string)
			fragPreamble	- preamble text of fragment shaders (default: empty string)
			
		Returns:
			nothing
	*/
	DLL void setShaderPreambles( const char *vertPreamble, const char *fragPreamble );
	
	/* 	Function: setMaterialUniform
			Sets a shader uniform of a Material resource.
		
		This function sets the specified shader uniform of the specified material to the specified values.
		
		Parameters:
			materialRes	- handle to the Material resource to be accessed
			name		- name of the uniform as defined in Material resource
			a, b, c, d	- values of the four components
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool setMaterialUniform( ResHandle materialRes, const char *name, float a, float b, float c, float d );

	/* 	Function: setPipelineStageActivation
			Sets the activation state of a pipeline stage.
		
		This function enables or disables a specified stage of the specified pipeline resource.
		
		Parameters:
			pipelineRes	- handle to the Pipeline resource to be accessed
			stageName	- name of the stage to be modified
			enabled		- flag indicating whether the stage shall be enabled or disabled
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool setPipelineStageActivation( ResHandle pipelineRes, const char *stageName, bool enabled );
	
	/* 	Function: getPipelineRenderTargetData
			Reads the pixel data of a pipeline render target buffer.
		
		This function reads the pixels of the specified buffer of the specified render target from the specified
		pipeline resource and stores it in the specified float array. To calculate the size required for the array this
		function can be called with a NULL pointer for dataBuffer and pointers to variables where the width,
		height and number of (color) components (e.g. 4 for RGBA or 1 for depth) will be stored.
		The function is not intended to be used for real-time scene rendering but rather as a tool for debugging.
		For more information about the render buffers please refer to the Pipeline documentation.
		
		Parameters:
			pipelineRes	- handle to pipeline resource
			targetName	- unique name of render target to access
			bufIndex	- index of buffer to be accessed
			width		- pointer to variable where the width of the buffer will be stored (can be NULL)
			height		- pointer to variable where the height of the buffer will be stored (can be NULL)
			compCount	- pointer to variable where the number of components will be stored (can be NULL)
			dataBuffer	- pointer to float array where the pixel data will be stored (can be NULL)
			bufferSize	- size of dataBuffer array in bytes
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool getPipelineRenderTargetData( ResHandle pipelineRes, const char *targetName,
										  int bufIndex, int *width, int *height, int *compCount,
										  float *dataBuffer, int bufferSize );


	/* Group: General scene graph functions */
	/* 	Function: getNodeType
			Returns the type of a scene node.
		
		This function returns the type of a specified scene node. If the node handle is invalid,
        the function returns the node type 'Unknown'.
		
		Parameters:
			node	- handle to the scene node
			
		Returns:
			type of the scene node
	*/
	DLL int getNodeType( NodeHandle node );
		
	/* 	Function: getNodeParent
			Returns the parent of a scene node.
		
		This function returns the handle to the parent node of a specified scene node. If the specified
		node handle is invalid or the root node, 0 is returned.
		
		Parameters:
			node	- handle to the scene node
			
		Returns:
			handle to parent node or 0 in case of failure
	*/
	DLL NodeHandle getNodeParent( NodeHandle node );


	/* 	Function: setNodeParent
			Relocates a node in the scene graph.
		
		This function relocates a scene node. It detaches the node from its current parent and attaches
		it to the specified new parent node. If the attachment to the new parent is not possible, the
		function returns false. Relocation is not possible for the RootNode.
		
		Parameters:
			node	- handle to the scene node to be relocated
			parent	- handle to the new parent node
			
		Returns:
			true in case of success, otherwise false
	*/
	DLL bool setNodeParent( NodeHandle node, NodeHandle parent );
	
	/* 	Function: getNodeChild
			Returns the handle to a child node.
		
		This function looks for the n-th (index) child node of a specified node and returns its handle. If the child
		doesn't exist, the function returns 0.
		
		Parameters:
			node	- handle to the parent node
			index	- index of the child node
			
		Returns:
			handle to the child node or 0 if child doesn't exist
	*/
	DLL NodeHandle getNodeChild( NodeHandle node, int index );


	
	/* 	Function: addNodes
			Adds nodes from a SceneGraph resource to the scene.
		
		This function creates several new nodes as described in a SceneGraph resource and
        attaches them to a specified parent node. If an invalid scenegraph resource is specified
		or the scenegraph resource is unloaded, the function returns 0.
		
		Parameters:
			parent			- handle to parent node to which the root of the new nodes will be attached
			sceneGraphRes	- handle to loaded SceneGraph resource
			
		Returns:
			handle to the root of the created nodes or 0 in case of failure
	*/
	DLL NodeHandle addNodes( NodeHandle parent, ResHandle sceneGraphRes );
	
	/* 	Function: removeNode
			Removes a node from the scene.
		
		This function removes the specified node and all of it's children from the scene.
		
		Parameters:
			node	- handle to the node to be removed
			
		Returns:
			true in case of success otherwise false
	*/
	DLL bool removeNode( NodeHandle node );
	
	/* 	Function: setNodeActivation
			Sets the activation (visibility) state of a node.
		
		This function sets the activation state of the specified node to active or inactive. Inactive
        nodes with all their children are excluded from rendering.
		
		Parameters:
			node	- handle to the node to be modified
			active	- boolean value indicating whether node shall be active or inactive
			
		Returns:
			true in case of success otherwise false
	*/
	DLL bool setNodeActivation( NodeHandle node, bool active );

	/* 	Function: checkNodeTransformFlag
			Checks if a scene node has been transformed by the engine.
		
		This function checks if a scene node has been transformed by the engine since the last
		time the transformation flag was reset. Therefore, it stores a flag that is set to true when a
		setTransformation function is called explicitely by the application or when the node transformation
		has been updated by the animation system. The function also makes it possible to reset the
		transformation flag.
		
		Parameters:
			node	- handle to the node to be accessed
			reset	- flag indicating whether transformation flag shall be reset
			
		Returns:
			true if node has been transformed, otherwise false
	*/
	DLL bool checkNodeTransformFlag( NodeHandle node, bool reset );
	
	/* 	Function: getNodeTransform
			Gets the relative transformation of a node.
		
		This function gets the translation, rotation and scale of a specified scene node object. The
		coordinates are in local space and contain the transformation of the node relative to its parent.
		
		Parameters:
			node		- handle to the node which will be accessed
			tx, ty, tz	- pointers to variables where translation of the node will be stored (can be NULL)
			rx, ry, rz	- pointers to variables where rotation of the node in Euler angles
						  will be stored (can be NULL)
			sx, sy, sz	- pointers to variables where scale of the node will be stored (can be NULL)
			
		Returns:
			true in case of success otherwise false
	*/
	DLL bool getNodeTransform( NodeHandle node, float *tx, float *ty, float *tz,
							   float *rx, float *ry, float *rz, float *sx, float *sy, float *sz );
	
	/* 	Function: setNodeTransform
			Sets the relative transformation of a node.
		
		This function sets the relative translation, rotation and scale of a specified scene node object.
		The coordinates are in local space and contain the transformation of the node relative to its parent.
		
		Parameters:
			node		- handle to the node which will be modified
			tx, ty, tz	- translation of the node
			rx, ry, rz	- rotation of the node in Euler angles
			sx, sy, sz	- scale of the node
			
		Returns:
			true in case of success otherwise false
	*/
	DLL bool setNodeTransform( NodeHandle node, float tx, float ty, float tz,
							   float rx, float ry, float rz, float sx, float sy, float sz );
	
	/* 	Function: getNodeTransformMatrices
			Returns the transformation matrices of a node.
		
		This function stores a pointer to the relative and absolute transformation matrices
        of the specified node in the specified pointer varaibles.
		
		Parameters:
			node	- handle to the scene node to be accessed
			relMat	- pointer to a variable where the address of the relative transformation matrix will be stored
                      (can be NULL if matrix is not required)
			absMat	- pointer to a variable where the address of the absolute transformation matrix will be stored
                      (can be NULL if matrix is not required)
			
			
		Returns:
			true in case of success otherwise false
	*/
	DLL bool getNodeTransformMatrices( NodeHandle node, const float **relMat, const float **absMat );
	
	/* 	Function: setNodeTransformMatrix
			Sets the relative transformation matrix of a node.
		
		This function sets the relative transformation matrix of the specified scene node. It is basically the
        same as setNodeTransform but takes directly a matrix instead of individual transformation parameters.
		
		Parameters:
			node	- handle to the node which will be modified
			mat4x4	- pointer to a 4x4 matrix in column major order
			
		Returns:
			true in case of success otherwise false
	*/
	DLL bool setNodeTransformMatrix( NodeHandle node, const float *mat4x4 );

	/* 	Function: getNodeParamf
			Gets a property of a scene node.
		
		This function returns a specified property of the specified node.
		The property must be of the type float.
		
		Parameters:
			node	- handle to the node to be accessed
			param	- parameter to be accessed
			
		Returns:
			 value of the parameter
	*/
	DLL float getNodeParamf( NodeHandle node, int param );
	
	/* 	Function: setNodeParamf
			Sets a property of a scene node.
		
		This function sets a specified property of the specified node to a specified value.
		The property must be of the type float.
		
		Parameters:
			node	- handle to the node to be modified
			param	- parameter to be modified
			value	- new value for the specified parameter
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setNodeParamf( NodeHandle node, int param, float value );

	/* 	Function: getNodeParami
			Gets a property of a scene node.
		
		This function returns a specified property of the specified node.
		The property must be of the type int or ResHandle.
		
		Parameters:
			node	- handle to the node to be accessed
			param	- parameter to be accessed
			
		Returns:
			 value of the parameter
	*/
	DLL int getNodeParami( NodeHandle node, int param );
	
	/* 	Function: setNodeParami
			Sets a property of a scene node.
		
		This function sets a specified property of the specified node to a specified value.
		The property must be of the type int or ResHandle.
		
		Parameters:
			node	- handle to the node to be modified
			param	- parameter to be modified
			value	- new value for the specified parameter
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setNodeParami( NodeHandle node, int param, int value );

	/* 	Function: getNodeParamstr
			Gets a property of a scene node.
		
		This function returns a specified property of the specified node.
		The property must be of the type string (const char *).
		
		Parameters:
			node	- handle to the node to be accessed
			param	- parameter to be accessed
			
		Returns:
			 value of the property or empty string if no such property exists
	*/
	DLL const char *getNodeParamstr( NodeHandle node, int param );
	
	/* 	Function: setNodeParamstr
			Sets a property of a scene node.
		
		This function sets a specified property of the specified node to a specified value.
		The property must be of the type string (const char *).
		
		Parameters:
			node	- handle to the node to be modified
			param	- parameter to be modified
			value	- new value for the specified parameter
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setNodeParamstr( NodeHandle node, int param, const char *value );


	/* 	Function: getNodeAABB
			Gets the bounding box of a scene node.
		
		This function stores the world coordinates of the axis aligned bounding box of a specified node in
		the specified variables. The bounding box is represented using the minimum and maximum coordinates
		on all three axes.
		
		Parameters:
			node				- handle to the node which will be accessed
			minX, minY, minZ	- pointers to variables where minimum coordinates will be stored
			maxX, maxY, maxZ	- pointers to variables where maximum coordinates will be stored
			
		Returns:
			true in case of success otherwise false
	*/
	DLL bool getNodeAABB( NodeHandle node, float *minX, float *minY, float *minZ,
						  float *maxX, float *maxY, float *maxZ );
	
	/* 	Function: findNodes
			Finds scene nodes with the specified properties.
		
		This function loops recursively over all children of startNode and adds them to an internal list
		of results if they match the specified name and type. The result list is cleared each time this
		function is called. The function returns the number of nodes which were found and added to the list.
		
		Parameters:
			startNode	- handle to the node where the search begins
			name		- name of nodes to be searched (empty string for all nodes)
			type		- type of nodes to be searched (SceneNodeTypes::Undefined for all types)
			
		Returns:
			number of search results
	*/
	DLL int findNodes( NodeHandle startNode, const char *name, int type );
	
	/* 	Function: getNodeFindResult
			Gets a result from the findNodes query.
		
		This function returns the n-th (index) result of a previous findNodes query. The result is the handle
		to a scene node with the poperties specified at the findNodes query. If the index doesn't exist in the
		result list the function returns 0.
		
		Parameters:
			index	- index of search result
			
		Returns:
			handle to scene node from findNodes query or 0 if result doesn't exist
	*/
	DLL NodeHandle getNodeFindResult( int index );
	
	/* 	Function: castRay
			Performs a recursive ray collision query.
		
		This function checks recursively if the specified ray intersects the specified node or one of its children.
        The function finds intersections relative to the ray origin and returns the number of intersecting scene
        nodes. The ray is a line segment and is specified by a starting point (the origin) and a finite direction
		vector which also defines its length. Currently this function is limited to returning intersections with Meshes.
		
		Parameters:
			node		- node at which intersection check is beginning
			ox, oy, oz	- ray origin
			dx, dy, dz	- ray direction vector also specifying ray length
			numNearest	- maximum number of results to return or 0 for all
			
		Returns:
			number of intersections
		*/
	DLL int castRay( NodeHandle node, float ox, float oy, float oz, float dx, float dy, float dz, int numNearest );


	/*	Function: getCastRayResult
			Returns results of a previous castRay query

		This functions copies the results for the specified index into the provided variables.

		Parameters:
			index			- index in range 0 until return value of castRay exclusive
			node			- intersecting node
			distance		- distance from ray origin to intersection
			intersection	- point of intersection, float[3] array

		Returns:
			true if index was valid and data could be copied
	*/
	DLL bool getCastRayResult( int index, NodeHandle *node, float *distance, float *intersection );


	/* Group: Group-specific scene graph functions */
	/* 	Function: addGroupNode
			Adds a Group node to the scene.
		
		This function creates a new Group node and attaches it to the specified parent node.
		
		Parameters:
			parent	- handle to parent node to which the new node will be attached
			name	- name of the node
			
		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addGroupNode( NodeHandle parent, const char *name );
	
	
	/* Group: Model-specific scene graph functions */
	/* 	Function: addModelNode
			Adds a Model node to the scene.
		
		This function creates a new Model node and attaches it to the specified parent node.
		
		Parameters:
			parent		- handle to parent node to which the new node will be attached
			name		- name of the node
			geometryRes	- Geometry resource used by Model node
			
		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addModelNode( NodeHandle parent, const char *name, ResHandle geometryRes );
	
	/* 	Function: setupModelAnimStage
			Configures an animation stage of a Model node.
		
		This function is used to setup the specified animation stage (channel) of the specified Model node.
        
        The function is used for animation blending. There is a fixed number of stages (by default 16) on
        which different animations can be played. The start node determines the first node (Joint or Mesh)
        to which the animation is recursively applied. If the start node is an empty string, the animation
        affects all animatable nodes (Joints and Meshes) of the model. If a NULL-handle is used for animationRes,
		the stage is cleared and the previous animation is removed.
        
        A simple way to do animation mixing is using additive animations. If a stage is configured to be
        additive  the engine calculates the difference between the current frame and the first frame in the
        animation and adds this delta to the current transformation of the joints or meshes.
		
		Parameters:
			modelNode		- handle to the Model node to be modified
			stage			- index of the animation stage to be configured
			animationRes	- handle to Animation resource (can be 0)
			startNode		- name of first node to which animation shall be applied (or empty string)
			additive		- flag indicating whether stage is additive
			
		Returns:
			 true in case of success, otherwise false
	*/
	DLL bool setupModelAnimStage( NodeHandle modelNode, int stage, ResHandle animationRes,
								  const char *startNode, bool additive );
	
	/* 	Function: setModelAnimParams
			Sets the parameters of an animation stage in a Model node.
		
		This function sets the current animation time and weight for a specified stage of the specified model.
        The time corresponds to the frames of the animation and the animation is looped if the
        time is higher than the maximum number of frames in the Animation resource. The weight is used for
        animation blending and determines how much influence the stage has compared to the other active
        stages. When the sum of the weights of all stages is more than one, the animations on the lower
        stages get priority.
		
		Parameters:
			modelNode	- handle to the Model node to be modified
			stage		- index of the animation stage to be modified
			time		- new animation time/frame
			weight		- new blend weight
			
		Returns:
			 true in case of success, otherwise false
	*/
	DLL bool setModelAnimParams( NodeHandle modelNode, int stage, float time, float weight );
	
	/* 	Function: setModelMorpher
			Sets the weight of a morph target.
		
		This function sets the weight of a specified morph target. If the target parameter
        is an empty string the weight of all morph targets in the specified Model node is modified.
        If the specified morph target is not found the function returns false.
		
		Parameters:
			modelNode	- handle to the Model node to be modified
			target		- name of morph target
			weight		- new weight for morph target
			
		Returns:
			 true in case of success, otherwise false
	*/
	DLL bool setModelMorpher( NodeHandle modelNode, const char *target, float weight );
	
	
	/* Group: Mesh-specific scene graph functions */
	/* 	Function: addMeshNode
			Adds a Mesh node to the scene.
		
		This function creates a new Mesh node and attaches it to the specified parent node.
		
		Parameters:
			parent		- handle to parent node to which the new node will be attached
			name		- name of the node
			materialRes	- material resource used by Mesh node
			batchStart	- first triangle index of mesh in Geometry resource of parent Model node
			batchCount	- number of triangle indices used for drawing mesh
			vertRStart	- first vertex in Geometry resource of parent Model node
			vertREnd	- last vertex in Geometry resource of parent Model node
			
		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addMeshNode( NodeHandle parent, const char *name, ResHandle materialRes, 
								int batchStart, int batchCount, int vertRStart, int vertREnd );

	
	/* Group: Joint-specific scene graph functions */
	/* 	Function: addJointNode
			Adds a Joint node to the scene.
		
		This function creates a new Joint node and attaches it to the specified parent node.
		
		Parameters:
			parent		- handle to parent node to which the new node will be attached
			name		- name of the node
			jointIndex	- index of joint in Geometry resource of parent Model node
			
		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addJointNode( NodeHandle parent, const char *name, int jointIndex );
	
	
	/* Group: Light-specific scene graph functions */
	/* 	Function: addLightNode
			Adds a Light node to the scene.
		
		This function creates a new Light node and attaches it to the specified parent node.
        The direction vector of the untransformed light node is pointing along the the negative
        z-axis. The specified material resource can define uniforms and projective textures.
        Furthermore it can contain a shader for doing lighting calculations if deferred shading
        is used. If no material is required the parameter can be zero. The context names
        define which shader contexts are used when rendering shadow maps or doing light calculations for
        forward rendering configurations.
		
		Parameters:
			parent			- handle to parent node to which the new node will be attached
			name			- name of the node
			materialRes		- material resource for light configuration or 0 if not used
			lightingContext	- name of the shader context used for doing light calculations
			shadowContext	- name of the shader context used for doing shadow map rendering
			
		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addLightNode( NodeHandle parent, const char *name, ResHandle materialRes,
								 const char *lightingContext, const char *shadowContext );
	
	/* 	Function: setLightContexts
			Sets the shader contexts used by a light source.
		
		This function sets the lighting and shadow shader contexts of the specified light source. The contexts
        define which shader code is used when doing lighting calculations or rendering the shadow map.
		
		Parameters:
			lightNode		- handle to the Light node to be modified
			lightingContext	- name of the shader context used for performing lighting calculations
			shadowContext	- name of the shader context used for rendering shadow maps
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setLightContexts( NodeHandle lightNode, const char *lightingContext, const char *shadowContext );


	/* Group: Camera-specific scene graph functions */
	/* 	Function: addCameraNode
			Adds a Camera node to the scene.
		
		This function creates a new Camera node and attaches it to the specified parent node.
		
		Parameters:
			parent		- handle to parent node to which the new node will be attached
			name		- name of the node
			pipelineRes	- pipeline resource used for rendering
			
		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addCameraNode( NodeHandle parent, const char *name, ResHandle pipelineRes );
	
	/* 	Function: setupCameraView
			Sets the planes of a camera viewing frustum.
		
		This function calculates the view frustum planes of the specified camera node using the specified view
        parameters.
		
		Parameters:
			cameraNode	- handle to the Camera node which will be modified
			fov			- field of view (FOV) angle
			aspect		- aspect ratio
			nearDist	- distance of near clipping plane
			farDist		- distance of far clipping plane 
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool setupCameraView( NodeHandle cameraNode, float fov, float aspect, float nearDist, float farDist );
	
	/* 	Function: calcCameraProjectionMatrix
			Calculates the camera projection matrix.
		
		This function calculates the camera projection matrix used for bringing the geometry to
        screen space and copies it to the specified array.
		
		Parameters:
			cameraNode	- handle to Camera node
			projMat		- pointer to float array with 16 elements
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool calcCameraProjectionMatrix( NodeHandle cameraNode, float *projMat );


	/* Group: Emitter-specific scene graph functions */
	/* 	Function: addEmitterNode
			Adds a Emitter node to the scene.
		
		This function creates a new Emitter node and attaches it to the specified parent node.
		
		Parameters:
			parent				- handle to parent node to which the new node will be attached
			name				- name of the node
			materialRes			- handle to Material resource used for rendering
			effectRes			- handle to Effect resource used for configuring particle properties
			maxParticleCount	- maximal number of particles living at the same time
			respawnCount		- number of times a single particle is recreated after dying (-1 for infinite)
			
			
		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addEmitterNode( NodeHandle parent, const char *name, ResHandle materialRes,
								   ResHandle effectRes, int maxParticleCount, int respawnCount );
	
	/* 	Function: advanceEmitterTime
			Advances the time value of an Emitter node.
		
		This function advances the simulation time of a particle system and continues the particle simulation
        with timeDelta being the time elapsed since the last call of this function. The specified
        node must be an Emitter node.
		
		Parameters:
			emitterNode	- handle to the Emitter node which will be modified
			timeDelta	- time delta in seconds
			
		Returns:
			 true in case of success otherwise false
	*/
	DLL bool advanceEmitterTime( NodeHandle emitterNode, float timeDelta );

	/* 	Function: hasEmitterFinished
			Checks if an Emitter node is still alive.
		
		This function checks if a particle system is still active and has living particles or
		will spawn new particles. The specified node must be an Emitter node. The function can be
		used to check when a not infinitely running emitter for an effect like an explosion can be
		removed from the scene.
		
		Parameters:
			emitterNode	- handle to the Emitter node which is checked
			
		Returns:
			 true if Emitter will no more emit any particles, otherwise or in case of failure false
	*/
	DLL bool hasEmitterFinished( NodeHandle emitterNode );
}
