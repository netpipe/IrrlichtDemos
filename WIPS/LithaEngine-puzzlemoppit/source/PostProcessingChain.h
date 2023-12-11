
#ifndef POST_PROCESSING_CHAIN_H
#define POST_PROCESSING_CHAIN_H

#include "IPostProcessingChain.h"
#include <vector>

struct Effect
{
	video::SMaterial material;
	IShader *shader;
};

class PostProcessingChain : public IPostProcessingChain
{
	video::IVideoDriver *driver;
	
	bool renderScreen;
	
	std::vector<Effect> effects;
	
	// Screen quad used for rendering effects
	video::S3DVertex quadVertices[4];
	u16 quadIndices[6];
	
	void AddEffect(video::SMaterial &material, IShader *shader, video::ITexture *rt);
	
public:
	PostProcessingChain(video::IVideoDriver *driver, bool renderScreen);
	~PostProcessingChain();
	
	void AddEffect(video::SMaterial material, IShader *shader, core::dimension2du size);
	
	void AddEffect(video::SMaterial material, IShader *shader, u32 screenFraction = 1);
	
	
	bool ShouldRenderScreen() { return renderScreen; }
	
	video::ITexture *GetInputTexture();
	
	void Process();
	
	void ApplyToScreen();
};

#endif
