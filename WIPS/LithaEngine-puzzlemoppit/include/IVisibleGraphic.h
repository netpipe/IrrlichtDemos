
#ifndef I_VISIBLE_GRAPHIC_H
#define I_VISIBLE_GRAPHIC_H

#include "IGraphic.h"

class IShader;

// A graphic with materials and shaders.
class IVisibleGraphic : public IGraphic
{
public:
	virtual ~IVisibleGraphic(){}
	
	// Same number of materials in each pass.
	virtual u32 GetMaterialCount() = 0;
	
	// Get the material of a specific rendering pass.
	// NOTE: If you are using Shaders, you should not modify the MaterialTypeParam2 of a material.
	// (it is used internally as an id for a Shader object)
	virtual video::SMaterial &GetMaterial(u32 material, u16 pass = 0) = 0;
	
	// Set a single material to be rendered using a shader. This will overwrite the material's MaterialType.
	// Can set NULL to clear shader, this will cause the material's MaterialType to be reset
	// to video::EMT_SOLID.
	virtual void SetShader(u32 material, IShader *shader, u16 pass = 0) = 0;
	
	// Set all materials in a pass to the same shader.
	virtual void SetAllShaders(IShader *shader, u16 pass = 0) = 0;
	
	// Permanently disable rendering of a particular material.
	// This is used if only some materials want to be rendered in a pass.
	// (some materials might not be multipass)
	virtual void DisableMaterial(u32 material, u16 pass = 0) = 0;
	
	// Disable all materials in a pass. Now why ever would we want to do that?
	virtual void DisableAllMaterials(u16 pass) = 0;
	
	// Show or hide this graphic.
	virtual void SetVisible(bool visible) = 0;
};

#endif

