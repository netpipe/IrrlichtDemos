#ifndef _POSTPROCESSMANAGER_H
#define _POSTPROCESSMANAGER_H

#include <irr/irrlicht.h>

using namespace irr;

// forward declarations
class IPostProcess;
class CShaderPostProcess;
class CShaderMaterial;

// available effects
//
// if you want to create a new effect
// 1) add another EPPE_* constant
// 2) define its behaviour in the effect.xml configuration file
// 3) define additional render target textures in the rtt.xml configuration file
// 4) put the used shader programs into the media/shaders/hlsl and media/shaders/glsl subfolders
enum E_POSTPROCESS_EFFECT
{
	EPPE_NO_EFFECT = 0,
	EPPE_FINAL_PASS,
	EPPE_INVERT,
	EPPE_SEPIA,
	EPPE_GRAY_SCALE,
	EPPE_SIMPLE_BLOOM,
	EPPE_ADAPTIVE_BLOOM,
	EPPE_MOTION_BLUR,
	EPPE_NIGHT_VISION,
	EPPE_DREAM_VISION,
	EPPE_POSTERIZE,
	EPPE_SHARPEN,
	EPPE_EMBOSSED,
	EPPE_TILING,
	EPPE_DISPLACEMENT,
	EPPE_SCRATCHED,
	EPPE_PENCIL,
	EPPE_BLUR,
	EPPE_WATER,
	EPPE_COLOR,
	EPPE_PULSING,
	EPPE_SHAKE,
	EPPE_DESATURATE,
	EPPE_RADIAL_BLUR,
	EPPE_DEPTH_OF_FIELD,
	EPPE_VIGNETTE,
	EPPE_UNDER_WATER,
	EPPE_RAIN,
	EPPE_SNOW,
	EPPE_LIGHT,
	EPPE_COUNT
};

class CEffectChain : public core::array<IPostProcess*>
{
protected:
	// name of the effect
	core::stringc Name;

public:
	// set the name of the effect
	virtual void setName(const core::stringc& name) { Name = name; }

	// get the name of the effect
	virtual const core::stringc& getName() const { return Name; }
};

class CPostProcessManager
{
public:
	// constructor
	CPostProcessManager(IrrlichtDevice* device);

	// destructor
	virtual ~CPostProcessManager();

protected:
	// the irrlicht device
	IrrlichtDevice* Device;

	// the default auxiliary buffers
	// they are swaped after completing an effect with all its postprocess passes
	video::ITexture* AuxBuffer[2];

	// additional render target textures (defined in rtt.xml)
	core::map<const core::stringc, video::ITexture*> RenderTargetMap;

	// the effect chains that store the post process passes (defined in effect.xml)
	CEffectChain EffectChain[EPPE_COUNT];

	// list of nodes for depth pass
	core::array<scene::ISceneNode*> DepthPassNodes;

	// material for depth pass
	CShaderMaterial* DepthMaterial;

	// performs aux buffer swaping
	void SwapAuxBuffers();

	// loads the rtt.xml and fills the RenderTargetMap
	void loadRTTConfig(const char* filename);

	// loads the effect.xml and fills the EffectChain
	void loadEffectConfig(const char* filename);

public:
	// performs depth and normal generation pass
	void renderDepth(const video::SColor& defaultDepth = video::SColor(0xFFFFFFFF));

	// prepares the postprocessing by setting the aux0 buffer as the scene render target
	// use this function before calling smgr->drawAll()
	void prepare();

	// renders the desired effect
	void render(E_POSTPROCESS_EFFECT effect);

	// renders the aux buffer to the framebuffer
	// call this function after applying all desired effects and before calling gui->drawAll()
	void update();

	// removes all nodes from depth pass
	void clearDepthPass();

	// adds node to the depth pass
	void addNodeToDepthPass(scene::ISceneNode *node);

	// removes a node from the depth pass
	void removeNodeFromDepthPass(scene::ISceneNode *node);

	// returns a specific effect chain
	CEffectChain& getEffectChain(E_POSTPROCESS_EFFECT effect);
};

#endif
