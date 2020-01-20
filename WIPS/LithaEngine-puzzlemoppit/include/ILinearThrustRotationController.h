
#ifndef I_LINEAR_THRUST_ROTATION_CONTROLLER_H
#define I_LINEAR_THRUST_ROTATION_CONTROLLER_H

#include "IRotationController.h"

class ILinearThrustRotationController : public IRotationController // or just IController?
{
public:
	virtual ~ILinearThrustRotationController(){}
};

#endif
