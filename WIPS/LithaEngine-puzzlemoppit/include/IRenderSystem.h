
#ifndef I_RENDER_SYSTEM_H
#define I_RENDER_SYSTEM_H

#include "litha_internal.h"

class IShader;
class IPostProcessingChain;

enum E_SHADER_LEVEL
{
	ESL_LOW = 0, // arb asm
	ESL_HIGH, // GLSL vs_1_1 and ps_1_1
};

class IRenderSystem : public virtual IReferenceCounted
{
public:
	virtual ~IRenderSystem(){}
	
	// Create a post processing chain.
	// renderScreen defines whether the screen should still be rendered to as normal (useful if the
	// post processing is added on top of the screen). If false, then only the result of post processing
	// is renderered.
	virtual IPostProcessingChain *CreatePostProcessingChain(bool renderScreen = true) = 0;
	virtual void SetActivePostProcessingChain(IPostProcessingChain *chain) = 0;
	
	// Set the type of shaders to be used. Defaults to high level.
	// This will affect all future calls to ShadersAreAvailable or CreateShader, so should
	// generally be called before those functions.
	virtual void SetShaderLevel(E_SHADER_LEVEL level) = 0;
	
	// OpenGL high level shaders only (GLSL). May return NULL if shaders are not available.
	// Targets vs_1_1 and ps_1_1.
	virtual IShader *CreateShader(const c8 *vertexShaderFileName, const c8 *pixelShaderFileName,
			video::E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID) = 0;
	
	virtual bool ShadersAreAvailable() = 0;
	virtual bool PostProcessingEnabled() = 0;
	
	// Simply forces ShadersAreAvailable to always return false irrespective of the truth.
	virtual void ForceNoShaders(bool noShaders) = 0;
	
	// Force post processing to be disabled. Simply means the method SetActivePostProcessingChain() will fail.
	virtual void ForceNoPostProcessing(bool noPostProcessing) = 0;
	
	// Fade the entire screen to a percentage of black.
	// fadeAmount - 0.0 is totally black, 1.0 is no fade (normal screen)
	// time - how long in seconds the fade should take. 0 for instant.
	// fadeGUI - should the GUI be faded as well? (otherwise just the 3D rendered world is faded)
	// fade start and finish events are sent... finish event will not be sent if a new fade is started
	// while the old fade is still ocurring.
	virtual void ScreenFade(f32 fadeAmount, f32 time = 0.0, bool fadeGUI = false) = 0;
	
	// A more intelligent fade function.
	// Fades from the current fade, so can be used if part way through a previous fade.
	// Time taken is reduced appropriately if the previous fade was not completed.
	// e.g. first fade 0 to 1 over 5 seconds.
	// part way through, at 2.5 seconds we trigger a second fade of 5 seconds, 1 to 0.
	// since the previous fade only reach 0.5, second fade's time will be reduced to 2.5 seconds.
	// and the desired fade speed is maintained.
	virtual void ScreenFadeFromCurrent(f32 fadeAmount, f32 time = 0.0, bool fadeGUI = false) = 0;
	
	// Is a screen fade in progress?
	virtual bool IsFading() = 0;
	
	virtual f32 GetCurrentFade() = 0;
	
	// Set the background colour...
	virtual void SetBackgroundCol(const video::SColor &col) = 0;
	
	// Render everything, but don't show the user...
	// Useful for hackish purposes.
	virtual void RenderInvisible() = 0;
};

#endif

