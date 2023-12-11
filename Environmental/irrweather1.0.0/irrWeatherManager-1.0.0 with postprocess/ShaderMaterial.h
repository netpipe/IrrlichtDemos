#ifndef _SHADERMATERIAL_H
#define _SHADERMATERIAL_H

#include <irr/irrlicht.h>

using namespace irr;

// built in shader constants
// combine using bitwise or
enum E_SHADER_CONSTANT
{
	ESC_TIME =				0x1,
	ESC_RANDOM =			0x2,
	ESC_BUFFERWIDTH =		0x4,
	ESC_BUFFERHEIGHT =		0x8,
	ESC_PROJECTION =		0x10,
	ESC_VIEW =				0x20,
	ESC_WORLD =				0x40,
	ESC_WORLDVIEW =			0x80,
	ESC_WORLDVIEWPROJ =		0x100,
	ESC_CAMERAPOSITION =	0x200,
	ESC_MOUSEPOSITIONX =    0x400,
	ESC_MOUSEPOSITIONY =    0x800
};

struct SUniformDescriptor
{
	SUniformDescriptor() : ParamPointer(0), ParamCount(0) {}
	SUniformDescriptor(const f32* paramPointer, u32 paramCount)
		: ParamPointer(paramPointer), ParamCount(paramCount) {}

	const irr::f32* ParamPointer;
	irr::u32 ParamCount;
};

class CShaderMaterial : public video::IShaderConstantSetCallBack
{
public:
	// constructor
	CShaderMaterial(IrrlichtDevice* device, const core::stringc& name,
		io::path vsFile, core::stringc vsEntry, video::E_VERTEX_SHADER_TYPE vsType,
		io::path psFile, core::stringc psEntry, video::E_PIXEL_SHADER_TYPE psType,
		video::E_MATERIAL_TYPE baseMaterial);

protected:
	// the irrlicht device
	IrrlichtDevice* Device;

	// shader constant maps
	core::map<const core::stringc, f32> PixelShaderConstant;
	core::map<const core::stringc, SUniformDescriptor> PixelShaderUniform;
	core::map<const core::stringc, f32> VertexShaderConstant;
	core::map<const core::stringc, SUniformDescriptor> VertexShaderUniform;

	// texture names
	core::stringc TextureName[video::MATERIAL_MAX_TEXTURES];

	// material id
	core::stringc Name;

	// material
	video::SMaterial Material;

	// additional built in shader constants
	u32 PixelShaderFlags;
	u32 VertexShaderFlags;

public:
	// OnSetConstants callback to transfer shader constant to the gpu program
	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userdata);

	// sets pixel shader constant
	void setPixelShaderConstant(const core::stringc& name, f32 value) { PixelShaderConstant[name] = value; }
	// sets pixel shader constant (keep in mind, that the pointer shows to a persistent variable, usually a member of another object)
	void setPixelShaderConstant(const core::stringc& name, const f32* value, const u32 count) { PixelShaderUniform[name] = SUniformDescriptor(value, count); }

	// returns pixel shader constant
	f32 getPixelShaderConstant(const core::stringc& name) { return PixelShaderConstant[name]; }

	// returns a pixel shader constant iterator
	core::map<const core::stringc, f32>::Iterator getPixelShaderConstantIterator() { return PixelShaderConstant.getIterator(); }

	// sets pixel shader flag (bitwise or combination)
	void setPixelShaderFlag(E_SHADER_CONSTANT flag, bool enabled = true) { PixelShaderFlags = (PixelShaderFlags & (~flag)) | ((((u32)!enabled)-1) & flag); }

	// returns pixel shader flag
	bool getPixelShaderFlag(E_SHADER_CONSTANT flag) const { return (PixelShaderFlags& flag)!=0; }

	// sets vertex shader constants
	void setVertexShaderConstant(const core::stringc& name, f32 value) { VertexShaderConstant[name] = value; }
	// sets vertex shader constant (keep in mind, that the pointer shows to a persistent variable, usually a member of another object)
	void setVertexShaderConstant(const core::stringc& name, const f32* value, const u32 count) { VertexShaderUniform[name] = SUniformDescriptor(value, count); }

	// returns vertex shader constant
	f32 getVertexShaderConstant(const core::stringc& name) { return VertexShaderConstant[name]; }

	// returns a vertex shader constant iterator
	core::map<const core::stringc, f32>::Iterator getVertexShaderConstantIterator() { return VertexShaderConstant.getIterator(); }

	// sets vertex shader flag
	void setVertexShaderFlag(E_SHADER_CONSTANT flag, bool enabled=true) { VertexShaderFlags = (VertexShaderFlags & (~flag)) | ((((u32)!enabled)-1) & flag); }

	// returns vertex shader flag
	bool getVertexShaderFlag(E_SHADER_CONSTANT flag) const { return (VertexShaderFlags& flag)!=0; }

	// sets texture name (used for opengl gpu programs)
	void setTextureName(u32 index, const core::stringc& name) { TextureName[index] = name; }

	// returns the SMaterial struct of the shader material
	virtual video::SMaterial& getMaterial() { return Material; }

	// returns the material type of the shader material
	virtual video::E_MATERIAL_TYPE getMaterialType() const { return Material.MaterialType; }

	// returns the name of the material
	const core::stringc& getName() const { return Name; }
};

#endif
