#ifndef __S_MATERIAL_SYSTEM_H_INCLUDED__
#define __S_MATERIAL_SYSTEM_H_INCLUDED__

#include "SColor.h"
#include "matrix4.h"
#include "irrArray.h"
#include "EMaterialTypes.h"
#include "IAttributes.h"
#include "IReferenceCounted.h"
#include "IFileSystem.h"
namespace irr
{
namespace video
{
	class SMaterial;
	class ITexture;

/**Interface for material loading.
 
This interface provides methods for loading materials properties from material files.
Material files are XML files with the extension irrmaterial. They support parameterized user 
defined render fallbacks.

Example irrmaterial file:
\verbatim
<!--example.irrmaterial-->
<?xml version='1.0' encoding='UTF-8' standalone='yes'?>
<material>
	<!--this is a material fallback. 
	It takes the format of:
		<material_renderer_name [systemparam ="[{<,<=,>,>=,!}]intvalue" ...]>
			...
		</<material_renderer_name>
	System params ares set by the caller to control loading of materials. When reading 
	material files the material system tests these parameters. If one fails	(or 
	if the material type isn't supported by hardware) this fallback is skipped.-->
	<parallaxmap_solid renderer_d3d = "1" graphic_level = "2">

		<!--
			This is a material attribute. The format is identical to value written out with irr::io::IAttributes.
			Only the following types are supported: float,bool,int,color,colorf,vector3d,position,matrix.
		-->
		<float  name = "Shininess" value = "1.0"/> 

		<!--
			These set the textures for the materials layers. 
			arguments:
			layer : can be a number specifying the layer or a mnemonic name.
			file  : The filename to load. Can also specify a Named texture by prefixing the name with a $ symbol.
			create_normal_map: if set to true. the material system will convert this texture into a normal map upon loading.
			amplitude: When create_normal_map is true this float specifies the height multiplier used. It defaults to 1.
			nomipmaps: if set to true no mipmaps will be created.
			noalpha: if set to true the alpha channel will be ignored when loading.
		-->
		<texture layer = "base" file = "Path\example.png">
			<!--You can set some texture layer specific attributes here. only the following attributes are supported-->
			<vector3d name = "translation" value = "1 1 0"/>
			<vector3d name = "rotation" value = "1 1 0"/>
			<vector3d name = "scale" value = "1 1 0"/>
			<bool name = "BilinearFilter" value = "false"/>
			<bool name = "AnisotropicFilter" value = "false"/>
			<bool name = "TrilinearFilter" value = "false"/>
			<int name = "TextureWrap" value = "0"/>
		</texture>

		<!--if you don't need to specify texture attributes, this format is valid too-->
		<texture layer = "parallaxmap" file = "Path\example_height.png" create_normal_map = "true" amplitude = "9"/>
	
	</parallaxmap_solid>
	
	<normalmap_solid renderer_d3d = "1" graphic_level = "<=1">
		<texture layer = "base" file = "Path\example.png"/>
		<texture layer = "normalmap" file = "Path\example_height.png" create_normal_map = "true" amplitude = "9"/>
	</normalmap_solid>
	
	<!--This material fallback uses a named texture instead of a file.-->
	<solid>
		<texture layer = "base" file = "$NamedTexture"/>
	</solid>

</material>


\endverbatim

Here is the list of all builtin materials and their material names:
<TABLE>
	<TR>
		<TD><B>Material Type</B></TD>
		<TD><B>Material Name</B></TD>
		<TD><B>Layer Name</B></TD>
		<TD><B>Layer Description</B></TD>
	</TR>
	
	<TR>
		<TD>EMT_SOLID</TD>
		<TD>solid</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_SOLID_2_LAYER</TD>
		<TD rowspan = "2">solid_2layer</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>overlay</TD>
		<TD>This layer is blended with base</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_LIGHTMAP</TD>
		<TD rowspan = "2">lightmap</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>lightmap</TD>
		<TD>The lightmap</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_LIGHTMAP_ADD</TD>
		<TD rowspan = "2">lightmap_add</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>lightmap</TD>
		<TD>The lightmap</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_LIGHTMAP_M2</TD>
		<TD rowspan = "2">lightmap_m2</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>lightmap</TD>
		<TD>The lightmap</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_LIGHTMAP_M4</TD>
		<TD rowspan = "2">lightmap_m4</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>lightmap</TD>
		<TD>The lightmap</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_LIGHTMAP_LIGHTING</TD>
		<TD rowspan = "2">lightmap_light</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>lightmap</TD>
		<TD>The lightmap</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_LIGHTMAP_LIGHTING_M2</TD>
		<TD rowspan = "2">lightmap_light_m2</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>lightmap</TD>
		<TD>The lightmap</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_LIGHTMAP_LIGHTING_M4</TD>
		<TD rowspan = "2">lightmap_light_m4</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>lightmap</TD>
		<TD>The lightmap</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_DETAIL_MAP</TD>
		<TD rowspan = "2">detail_map</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>detail</TD>
		<TD>detail map</TD>
	</TR>
	<TR>
		<TD>EMT_SPHERE_MAP</TD>
		<TD>sphere_map</TD>
		<TD>spheremap</TD>
		<TD>Spheremap texture</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_REFLECTION_2_LAYER</TD>
		<TD rowspan = "2">reflection_2layer</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>reflection</TD>
		<TD>Reflection texture</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_TRANSPARENT_REFLECTION_2_LAYER</TD>
		<TD rowspan = "2">trans_reflection_2layer</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>reflection</TD>
		<TD>Reflection texture</TD>
	</TR>
	
	<TR>
		<TD>EMT_TRANSPARENT_ADD_COLOR</TD>
		<TD>trans_add</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	
	<TR>
		<TD>EMT_TRANSPARENT_ALPHA_CHANNEL</TD>
		<TD>trans_alphach</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	
	<TR>
		<TD>EMT_TRANSPARENT_ALPHA_CHANNEL_REF</TD>
		<TD>trans_alphach_ref</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	
	<TR>
		<TD>EMT_TRANSPARENT_VERTEX_ALPHA</TD>
		<TD>trans_vertex_alpha</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>

	<TR>
		<TD rowspan = "2">EMT_NORMAL_MAP_SOLID</TD>
		<TD rowspan = "2">normalmap_solid</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>normalmap</TD>
		<TD>The normal map</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR</TD>
		<TD rowspan = "2">normalmap_trans_add</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>normalmap</TD>
		<TD>The normal map</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA</TD>
		<TD rowspan = "2">normalmap_trans_vertexalpha</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>normalmap</TD>
		<TD>The normal map</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_PARALLAX_MAP_SOLID</TD>
		<TD rowspan = "2">parallaxmap_solid</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>parallaxmap</TD>
		<TD>The normal map</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR</TD>
		<TD rowspan = "2">parallaxmap_trans_add</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>parallaxmap</TD>
		<TD>The normal map</TD>
	</TR>
	
	<TR>
		<TD rowspan = "2">EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA</TD>
		<TD rowspan = "2">parallaxmap_trans_vertexalpha</TD>
		<TD>base</TD>
		<TD>Diffuse texture</TD>
	</TR>
	<TR>
		<TD>parallaxmap</TD>
		<TD>The normal map</TD>
	</TR>
</TABLE>

Builtin System Params:
<TABLE>
	<TR>
		<TD><B>Param</B></TD>
		<TD><B>Meaning</B></TD>
	</TR>
	<TR>
		<TD>renderer_burningsvideo</TD>
		<TD>Set to one if the driver type is EDT_BURNINGSVIDEO.</TD>
	</TR>
	<TR>
		<TD>renderer_software</TD>
		<TD>Set to one if the driver type is EDT_SOFTWARE.</TD>
	</TR>
	<TR>
		<TD>renderer_d3d8</TD>
		<TD>Set to one if the driver type is EDT_DIRECT3D8.</TD>
	</TR>
	<TR>
		<TD>renderer_d3d9</TD>
		<TD>Set to one if the driver type is EDT_DIRECT3D9.</TD>
	</TR>
	<TR>
		<TD>renderer_d3d</TD>
		<TD>Set to one if the driver type is either Direct X versions.</TD>
	</TR>
	<TR>
		<TD>renderer_opengl</TD>
		<TD>Set to one if the driver type is EDT_OPENGL.</TD>
	</TR>
	<TR>
		<TD>feature_render_to_target</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_RENDER_TO_TARGET) is true</TD>
	</TR>
	<TR>
		<TD>feature_hw_tl</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_HARDWARE_TL) is true</TD>
	</TR>
	<TR>
		<TD>feature_multitexture</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_MULTITEXTURE) is true</TD>
	</TR>
	<TR>
		<TD>feature_bilinear_filter</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_BILINEAR_FILTER) is true</TD>
	</TR>
	<TR>
		<TD>feature_mipmaps</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_MIP_MAP) is true</TD>
	</TR>
	<TR>
		<TD>feature_vs_x_x</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_VERTEX_SHADER_x_x) is true.
		x_x is the shader version.</TD>
	</TR>
	<TR>
		<TD>feature_ps_x_x</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_PIXEL_SHADER_x_x) is true. 
		x_x is the shader version</TD>
	</TR>
	<TR>
		<TD>feature_arb_vp_1_0</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_ARB_VERTEX_PROGRAM_1) is true</TD>
	</TR>
	<TR>
		<TD>feature_arb_fp_1_0</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_ARB_FRAGMENT_PROGRAM_1) is true</TD>
	</TR>
	<TR>
		<TD>feature_glsl</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_ARB_GLSL) is true</TD>
	</TR>
	<TR>
		<TD>feature_hlsl</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_HLSL) is true</TD>
	</TR>
	<TR>
		<TD>feature_texture_nsquare</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_TEXTURE_NSQUARE) is true</TD>
	</TR>
	<TR>
		<TD>feature_texture_npot</TD>
		<TD>Set to one if IVideoDriver::queryFeature(EVDF_TEXTURE_NPOT) is true</TD>
	</TR>
</TABLE>
*/
	class IMaterialSystem : public IReferenceCounted
	{

		friend class SMaterial;
		friend class CNullDriver;
	public:
		/**
		Sets a mnemonic for a material type layer.
		this is normally only used when defining your own material types.
		@param mattype The material renderers index.
		@param tex The texture layer.
		@param name the mnemonic for this layer. must not start with a digit or contain whitespace.
		@return true if the name is valid, false otherwise.
		*/
		virtual bool addTextureLayerName(u32 mattype,u32 tex,const c8 *name) = 0;
		
		/**
		loads a material from a IReadFile object
		@param material the material to load results.
		@param filename the file to read from.
		@return true if the material loaded successfully, false otherwise.
		*/
		virtual bool loadMaterial(SMaterial &material,io::IReadFile *filename) = 0;
		
		/**
		loads a material from a file. The usual way to use this function is the pass the filename of an texture file.
		this function then tries to load a material file with the same name in the same directory. if it fails it creates 
		a default material using the specified image. 
		Example:
		@code
		IMaterialSystem *matsys;
		matsys->loadMaterial("Path/prettybirdy.png",true); //this will load Path/prettybirdy.irrmaterial
		@endcode

		@param material the material to load results.
		@param filename the path to an image file or material file.
		@param load_texture_onfail if true it creates a solid material with the texture specified in filename. 
		@return true if the material loaded successfully, false otherwise.
		*/
		virtual bool loadMaterial(SMaterial &material,const c8 *filename,bool load_texture_onfail = true) = 0;

		/**
		Returns the value of a system param.
		If the param  isn't set zero is returned.
		@param param the param name. 
		@return the param value.
		*/
		virtual s32 getSystemParam(const c8 *param) = 0;

		/**
		Sets the value of a system param.
		@param param the param name. Must not contain spaces or any XML proccessing chars(<>?"'&).
		@param value the value.
		*/
		virtual void setSystemParam(const c8 *param,s32 value) = 0;

		/**
		Sets up default values for materials. When a material is loaded the values will be set first.
		@param attributes An IAttributes object containing default values. This method copies all 
		relevant values and does not reference this object again. 
		*/
		virtual void setDefaultValues(io::IAttributes *attributes) = 0;

		/**
		Adds a named texture. This method allows materials to uses system defined textures. This is ideal for render targets. 
		@param name the texture name.
		@param texture the texture.
		@see IMaterialSystem
		*/
		virtual void addNamedTexture(const c8 *name,ITexture *texture) = 0;
		/**
		Removes a named texture.
		@param name the texture name.
		@see IMaterialSystem
		*/
		virtual void removeNamedTexture(const c8 *name) = 0;
	protected:
		virtual void updateStandardParams() = 0;

		IRRLICHT_API static io::IAttributes *createMaterialAttributes();
		IRRLICHT_API static io::IAttributes *copyAttributes(io::IAttributes *src,bool copy_builtin = true);
		IRRLICHT_API static io::E_ATTRIBUTE_TYPE isBuiltinValue(const c8 *valuename,io::E_ATTRIBUTE_TYPE ignore = io::EAT_UNKNOWN);
	};
}
}
#endif