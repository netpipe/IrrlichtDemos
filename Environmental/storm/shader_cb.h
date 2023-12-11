#ifndef _SHADER_CALLBACK_H_
#define _SHADER_CALLBACK_H_

#include <irrlicht.h>
#include <iostream>

using namespace std;
using namespace irr;


class ShaderCallBack: public video::IShaderConstantSetCallBack
{
	public:
	
	ShaderCallBack(IrrlichtDevice* device)
	:	_device(device),
		_alpha(0.25f),
		_time(0.0f)
	{
	}
	
	
	void setTime(float time)
	{
		_time = time * 0.001f; // in seconds
	}
	float getTime() const { return _time; }
	
	void setAlpha(float alpha)
	{
		if ((alpha < 0.0f) || (alpha > 1.0f))
			return;
		
		_alpha = alpha;
	}
	
	float getAlpha() const { return _alpha; }
	
	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);
	
	private:
	IrrlichtDevice* _device;
	float _alpha;
	float _time;

};

#endif //_SHADER_CALLBACK_H_

