#ifndef _STORM_EFFECT_H_
#define _STORM_EFFECT_H_

#include <vector>
#include <irrlicht.h>
#include "shader_cb.h"

using namespace irr;


#ifndef STORM_EFFECT_PROPERTIES
#define STORM_EFFECT_PROPERTIES
#define START_DURATION 5000
#define END_DURATION 4500
#endif

class StormEffect
{

	public:
	
	StormEffect(IrrlichtDevice* device);
	virtual ~StormEffect();

	//ref:
	//http://en.wikipedia.org/wiki/Sigmoid_function
	static
	f32 sigmoid
	(
		f32 x1, f32 x2,
		u32 totalTime,
		u32 time,
		u32 nctime = 6 // normalized complete time
	);

	static
	scene::IMesh* createFrustumMesh
	(
		f32 topRadius,
		f32 bottomRadius,
		f32 height,
		u32 tesselation = 128
	);
	
	void update();
	void showDebugInfo();
	
	void start();
	void stop();
	
	
	protected:
	
	private:
	IrrlichtDevice* Device;
	scene::ISceneManager* Smgr;
	scene::IMeshSceneNode* Frustrum;
	scene::IBillboardSceneNode* Bill;
	ShaderCallBack* SCB;
	
	u32 ElapsedTime;
	u32 LastTime;

	u32 StartTime;
	bool IsStarting;
	
	u32 EndTime;
	bool IsEnding;
	
};







#endif // _STORM_EFFECT_H_



