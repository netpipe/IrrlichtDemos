#include "main.h"

#ifndef __CPOINTLIGHT__
#define __CPOINTLIGHT__

class CPointLights : public IShaderConstantSetCallBack
{
public:

	// Constructor / Destructor.
	CPointLights();
	~CPointLights();

	void addLight(SColor color,vector3df position,f32 radius);
	void render();
	virtual void OnSetConstants(IMaterialRendererServices* services,s32 userData);

	array<IMeshSceneNode*> meshArray;

	struct fakeLight
	{
		SColor color;
		vector3df pos;
		f32 radius;

		IBillboardSceneNode* billboard;
	};
	array<fakeLight> lightConf;

private:

	int shader;
	ILightSceneNode* light;
	f32 radius;

};

#endif