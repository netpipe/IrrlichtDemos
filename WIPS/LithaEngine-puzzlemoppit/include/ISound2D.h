
#ifndef I_SOUND_2D_H
#define I_SOUND_2D_H

#include "ISound.h"

class ISound2D : public virtual ISound
{
public:
	virtual ~ISound2D(){}
	
	virtual void SetPan(f32 panning) = 0;
};

#endif
