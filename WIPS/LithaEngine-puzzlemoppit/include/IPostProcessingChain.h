
#ifndef I_POST_PROCESSING_CHAIN_H
#define I_POST_PROCESSING_CHAIN_H

#include "litha_internal.h"

class IShader;

// perhaps need a flag for, if non power of two, falling *up* to POT dimensions *greater*.
// (if very high quality is desired)
class IPostProcessingChain : public virtual IReferenceCounted
{
public:
	virtual ~IPostProcessingChain(){}
	
	// Add an effect. Effects are applied in the order they are added, with the result of one effect
	// being fed as input to the next effect. The initial render is the input to the first effect, and the
	// last effect will output on to the screen. Effect materials and shaders should be set accordingly, for
	// example you may want the last effect to render additively on to the screen.
	// TextureLayer[0] of an effect's material will be set to the input texture.
	virtual void AddEffect(video::SMaterial material, IShader *shader, core::dimension2du size) = 0;
	
	// Same as above but size is given as a fraction of the screen dimensions.
	// last parameter is a divisor. e.g. 1 for full screen dimensions, 2 for half size...
	// If exact screen dimensions are not available will fallback as above
	virtual void AddEffect(video::SMaterial material, IShader *shader, u32 screenFraction = 1) = 0;
	
	// Does this post processing chain want the screen to be rendered to?
	// (with the effect rendered over the top of this, e.g. additively).
	// Otherwise, the screen is not rendered at all, and only the result of the post processing
	// is visible.
	virtual bool ShouldRenderScreen() = 0;
	
	// Get the input texture for post processing.
	// RenderTask renders the scene to this texture.
	virtual video::ITexture *GetInputTexture() = 0;
	
	// Render the post processing chain.
	// Called by RenderTask after it has rendered the scene to the chain's input texture.
	virtual void Process() = 0;
	
	// Perform the final step in the chain, rendering to screen.
	virtual void ApplyToScreen() = 0;
};

#endif
