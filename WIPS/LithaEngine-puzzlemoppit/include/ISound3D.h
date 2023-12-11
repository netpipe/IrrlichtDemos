
#ifndef I_SOUND_3D_H
#define I_SOUND_3D_H

#include "ISound.h"

class ISound3D : public virtual ISound
{
public:
	virtual ~ISound3D(){}
	
	// 3D position
	virtual void SetPosition(core::vector3df pos) = 0;
	
	// For doppler
	virtual void SetVelocity(core::vector3df vel) = 0;
	
	// Set the proximity to this sound at which max volume is reached.
	// Distances closer than this point will not get any louder.
	// Default 1.0
	virtual void SetMaxVolumePoint(f32 proximity) = 0;
};

#endif


