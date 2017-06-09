/**************************************************************
file: CShader.h
author: Stephan Gödecker (Masterhawk studios)
version: rev. 5

description: this file was created to achieve a dynamic
management for GLSL shaders with irrlicht. After implementing
this code-snippet the user is able to dynamically load GLSL-
shader in his irrlicht application. The shader just have to
be additionally defined by a special xml-file. You can get
further information at:

http://www.masterhawk.dyndns.org/wordpress?p=267

***************************************************************/

//#include "stdafx.h"
#include <irrlicht.h>
#include "XML/tinyxml.h"

using namespace irr;

enum E_SHADER_VAR_TYPE
{
	ESVT_FLOAT = 1,
	ESVT_VEC2 = 2,
	ESVT_VEC3 = 3,
	ESVT_VEC4 = 4,
	ESVT_MAT4 = 16,
	ESVT_SAMPLER2D,
	ESVT_PREDEFINED
};
enum E_SHADER_PREDEFINED_VARS
{
	ESPV_NONE,
	ESPV_FLOAT_TIME0X,
	ESPV_EYEPOSITION,
	ESPV_MAT4_VIEW,
	ESPV_MAT4_VIEW_PROJ,
	ESPV_RTT_VIEW
};

enum E_RTT_TEXTURE
{
	ERT_VIEW = 0x0001
};

struct S_RTT_Info
{
	E_RTT_TEXTURE type;
	s32 tex_channel;
};

struct SShaderVariable
{
	core::stringc name;
	E_SHADER_VAR_TYPE type;
	E_SHADER_PREDEFINED_VARS predefinition;
	bool b_frag_var;
	float* value;
};

class CShader : public video::IShaderConstantSetCallBack
{
private:
	s32 p_material; //material which has to be applied to a scene_node
	IrrlichtDevice* dev;
	video::IVideoDriver* driver;

	//temporarily
	TiXmlDocument doc;
	core::array<SShaderVariable*> uniform_variables;

	core::array<S_RTT_Info*> rtt_infos;

	f32 f_old_cycle_time;

public:
	CShader(IrrlichtDevice* device, const c8* shader_xml);
	~CShader();

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

	s32 getMaterial();

	S_RTT_Info* getRTTInfo(E_RTT_TEXTURE rtt);
};
