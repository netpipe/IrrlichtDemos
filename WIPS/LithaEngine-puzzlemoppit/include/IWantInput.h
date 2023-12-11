
#ifndef I_WANT_INPUT_H
#define I_WANT_INPUT_H

class IWantInput
{
public:
	virtual ~IWantInput(){}
	
	virtual void OnAxisChange(s32 id, f32 delta){}
	
	virtual void OnButtonDown(s32 id){}
	virtual void OnButtonUp(s32 id){}
};

#endif

