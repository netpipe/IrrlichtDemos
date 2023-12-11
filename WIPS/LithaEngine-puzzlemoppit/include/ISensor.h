
#ifndef I_SENSOR_H
#define I_SENSOR_H

#include "ITransformable.h"

class ISensor : public ITransformable
{
public:
	virtual ~ISensor(){}
	
	virtual void Update(f32 dt) = 0;
};

#endif

