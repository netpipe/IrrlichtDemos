
#include "Task.h"
#include "IRenderSystem.h"

class IEngine;
class World;
class IPostProcessingChain;
class ShaderManager;

class RenderTask : public Task, public IRenderSystem
{
	IEngine *engine;
	World *world;
	
	video::IVideoDriver *driver;
	scene::ISceneManager *smgr;
	gui::IGUIEnvironment *guienv;
	
	ShaderManager *shaderManager;
	
	// currently set post processing chain
	IPostProcessingChain *ppChain;
	bool postProcessingEnabled;
	
	// screen fading state and params
	bool fadeGUI;
	f32 oldScreenFade;
	f32 newScreenFade;
	f32 fadeStartTime;
	f32 fadeFinishTime;
	bool isFadeFinished;
	
	// Updated on each call to RenderFade
	f32 currentScreenFade;
	
	// background colour
	video::SColor backgroundCol;
	
	bool renderInvisible;
	
	void Render(u16 passCount);
	void RenderFade();
	
public:
	RenderTask(World *world);
	~RenderTask();
	
	// Render system methods
	
	IPostProcessingChain *CreatePostProcessingChain(bool renderScreen);
	void SetActivePostProcessingChain(IPostProcessingChain *chain);
	
	void SetShaderLevel(E_SHADER_LEVEL level);
	
	IShader *CreateShader(const c8 *vertexShaderFileName, const c8 *pixelShaderFileName, video::E_MATERIAL_TYPE baseMaterial);
	
	bool ShadersAreAvailable();
	bool PostProcessingEnabled();
	void ForceNoShaders(bool noShaders);
	void ForceNoPostProcessing(bool noPostProcessing);
	
	void ScreenFade(f32 fadeAmount, f32 time, bool fadeGUI);
	void ScreenFadeFromCurrent(f32 fadeAmount, f32 time, bool fadeGUI);
	bool IsFading();
	f32 GetCurrentFade() { return currentScreenFade; }
	
	void SetBackgroundCol(const video::SColor &col) { backgroundCol = col; }
	
	void RenderInvisible();
	
	// Task methods
	void Update(f32 dt);
};
