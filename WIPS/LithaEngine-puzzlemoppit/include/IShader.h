
#ifndef I_SHADER_H
#define I_SHADER_H

#include "litha_internal.h"

class IShader;

class IShaderCallback : public virtual IReferenceCounted
{
public:
	virtual void ShaderOnSetConstants(IShader *shader) = 0;
};

// This is an ugly hack, to add support for low level shaders without modifying
// my shader system. Should really have separate HighLevelShader and LowLevelShader
// derived from a base IShader class. (and two different CreateShader methods)
// And different LowLevelShadersAvailable and HighLevelShadersAvailable methods.
// (or better still, have some kind of even more general purpose shader version fallback system)
class LowLevelShaderRegisterMap
{
public:
	struct RegisterComponentGroup
	{
		core::stringc name;
		u32 size; // number of *components* of a register this takes.
	};
	
private:
	// each index is a single register component.
	// So every 4 indices form a register.
	std::vector<RegisterComponentGroup> registerComponentNames;
	
public:
	// a single register as four separate float values.
	// (a low level shader register has four components x, y, z, w.)
	void PushSingleRegister(const c8 *name1, const c8 *name2, const c8 *name3, const c8 *name4)
	{
		RegisterComponentGroup rcg;
		rcg.size = 1;
		
		rcg.name = name1;
		registerComponentNames.push_back(rcg);
		
		rcg.name = name2;
		registerComponentNames.push_back(rcg);
		
		rcg.name = name3;
		registerComponentNames.push_back(rcg);
		
		rcg.name = name4;
		registerComponentNames.push_back(rcg);
	}
	
	// four registers used for a matrix. (4 components per register, gives 4*4 = 16)
	void PushFourRegisters(const c8 *name)
	{
		RegisterComponentGroup rcg;
		rcg.name = name;
		rcg.size = 16;
		registerComponentNames.push_back(rcg);
	}
	
	// Use a user defined amount of registers.
	// (remember, each single register is four values. so to store 8 floats would require 2 registers)
	// NOT IMPLEMENTED YET AS NOT NEEDED YET.
	/*
	void PushRegisters(const c8 *name, u16 registerCount)
	{
		RegisterComponentGroup rcg;
		rcg.name = name;
		rcg.size = registerCount * 4; // number of float components
		registerComponentNames.push_back(rcg);
	}
	*/
	
	// Used internally...
	const std::vector<RegisterComponentGroup> &GetRegisterComponentNames()
	{
		return registerComponentNames;
	}
};

/*
class ILowLevelShader : public virtual IReferenceCounted
{
public:
	virtual ~ILowLevelShader(){}
	
	virtual void SetFloats(u32 reg, f32 a, f32 b, f32 c, f32 d) = 0;
	
	virtual void SetMatrix4(u32 reg, const core::matrix4 &mat) = 0;
	
	virtual void SetSingleValue(u32 reg, u8 component, f32 value) = 0;
	
	
	... OR?
	
	virtual void SetSingleRegister(u32 reg, f32 a, f32 b, f32 c, f32 d) = 0;
	
	virtual void SetFourRegisters(u32 reg, const core::matrix4 &mat) = 0;
};
*/

// Perhaps ought to have a predefined selection of constants that can be "watched".
// e.g. world matrix etc
// So a callback is not needed for common things.
// IShader->EnableAutoVertexConstant(EAVC_WORLD_MATRIX);
class IShader : public virtual IReferenceCounted
{
public:
	virtual ~IShader(){}
	
	// Ugly hack adding support for low level shaders.
	virtual void SetVertexRegisterMap(const LowLevelShaderRegisterMap &registerMap) = 0;
	virtual void SetPixelRegisterMap(const LowLevelShaderRegisterMap &registerMap) = 0;
	
	// Constants set with the Set..Constant methods are stored permanantly and then set by IShader before each render.
	// If you want a constant to be updated before each render, use a callback and the SetCallback method.
	
	// set a single vertex shader constant
	virtual void SetVertexConstant(const c8 *name, s32 value) = 0;
	virtual void SetVertexConstant(const c8 *name, f32 value) = 0;
	
	// set an array of vertex shader constants
	virtual void SetVertexConstants(const c8 *name, s32 *values, u16 count) = 0;
	virtual void SetVertexConstants(const c8 *name, f32 *values, u16 count) = 0;
	
	// set a single pixel shader constant
	virtual void SetPixelConstant(const c8 *name, s32 value) = 0;
	virtual void SetPixelConstant(const c8 *name, f32 value) = 0;
	
	// set an array of pixel shader constants
	virtual void SetPixelConstants(const c8 *name, s32 *values, u16 count) = 0;
	virtual void SetPixelConstants(const c8 *name, f32 *values, u16 count) = 0;
	
	// Extra methods which cast double to f32.
	// (prevents having to cast a hard coded value to float)
	void SetVertexConstant(const c8 *name, f64 value) { SetVertexConstant(name, (f32)value); }
	void SetPixelConstant(const c8 *name, f64 value) { SetPixelConstant(name, (f32)value); }
	
	// Extra matrix methods
	void SetVertexConstant(const c8 *name, core::matrix4 &mat) { SetVertexConstants(name, mat.pointer(), 16); }
	void SetPixelConstant(const c8 *name, core::matrix4 &mat) { SetPixelConstants(name, mat.pointer(), 16); }
	
	// Use this callback if you want to update any constants before each render.
	virtual void SetCallback(IShaderCallback *callback) = 0;
	
	// Apply this shader to an Irrlicht material.
	// This method is used internally and can be ignored.
	// Shader should not be dropped while a material still references it!
	// MaterialTypeParam2 is overwritten in the material to store an ID to this Shader.
	virtual void ApplyToIrrMaterial(video::SMaterial &material) = 0;
	
	// Remove this shader from an Irrlicht material.
	// Resets the material type and clears MaterialTypeParam2 to zero.
	virtual void ClearFromIrrMaterial(video::SMaterial &material, video::E_MATERIAL_TYPE newMaterialType) = 0;
};

#endif
