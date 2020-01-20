
#ifndef I_GRAPHIC_H
#define I_GRAPHIC_H

#include "ITransformable.h"

// A renderable graphic.
// Render() is called at a variable FPS (as opposed to all other game logic which is run
// at a fixed FPS). Graphic position/rotation is interpolated smoothly behind the scenes.
class IGraphic : public ITransformable
{
public:
	virtual ~IGraphic(){}
	
	// Called just before rendering with the interpolated and absolute positions.
	// Use to set any wrapped Irrlicht scene node graphics for rendering etc...
	// This takes into account the transformation of any parents.
	virtual void ReceiveRenderPosition(core::vector3df pos) = 0;
	virtual void ReceiveRenderRotation(core::vector3df rot) = 0;
	
	// Override if more than one render pass is required.
	virtual u16 GetPassCount() { return 1; }
	
	// Prepare for rendering a particular pass.
	// This will be called even for passes this graphic doesn't have, in order for
	// any Irrlicht scene nodes that shouldn't be rendered to be hidden.
	virtual void SetCurrentPass(u16 pass) {}
	
	// Render the graphic. Render is called GetPassCount() number of times per frame,
	// with the pass parameter incremented each call.
	// Could use Irrlicht's video driver drawing functions here.
	// However, Render does not have to be implemented, as the "graphic" may actually be 
	// an Irrlicht scene node, and thus may be rendered automatically by Irrlicht.
	// WARNING: No game logic should be performed here (in particular no moving or rotating of this or
	// any other graphics). That would mess up the graphic interpolation system.
	virtual void Render(u16 pass) {}
};

#endif
