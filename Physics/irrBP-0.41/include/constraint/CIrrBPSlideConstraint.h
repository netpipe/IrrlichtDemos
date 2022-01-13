#ifndef CIrrBP_SLIDER_CONST_H_
#define CIrrBP_SLIDER_CONST_H_
#include "constraint/CIrrBPConstraint.h"


class CIrrBPSlideConstraint: public CIrrBPConstraint
{
public:
	/**Constructor.<br>
	The last 2 parameters will only works if there is a static object.<br>
	Please also note that due to internal conversions, if you place the moving body to a 0.0f position, the object will be moved toward a NEAR 0 value.<br>

	@param bodyA The first body
	@param bodyB The second body
	@param pivotInA The constraint position in A (0,0,0 by default)
	@param pivotInB The constraint position in B (0,0,0 by default)
	@param autoadapt If one body is static, and this flag is setted to false. The Slide will be only orthogonal
	@param rotatepiston If setted to true, the dynamic object (piston) will be rotated in the slide direction.
	*/
	CIrrBPSlideConstraint(CIrrBPRigidBody * bodyA,CIrrBPRigidBody * bodyB,const irr::core::vector3df & pivotInA=irr::core::vector3df(0,0,0),const irr::core::vector3df & pivotInB=irr::core::vector3df(0,0,0),bool autoadapt=true, bool rotatepiston=true);
	
	irr::f32 getLowerLinLimit () { return m_fixedConstraint->getLowerLinLimit();}
	void setLowerLinLimit(irr::f32 lowerLimit) {m_fixedConstraint->setLowerLinLimit(lowerLimit);}
	irr::f32 getUpperLinLimit() { return m_fixedConstraint->getUpperLinLimit();}
	void setUpperLinLimit(irr::f32 upperLimit) {m_fixedConstraint->setUpperLinLimit(upperLimit);}
	irr::f32 getLowerAngLimit() { return m_fixedConstraint->getLowerAngLimit();}
	void setLowerAngLimit(irr::f32 lowerLimit)  { m_fixedConstraint->setLowerAngLimit(lowerLimit);}
	irr::f32 getUpperAngLimit() { return m_fixedConstraint->getUpperAngLimit(); }
	void setUpperAngLimit(irr::f32 upperLimit) { m_fixedConstraint->setUpperAngLimit(upperLimit); }

	
	irr::f32 getSoftnessDirLin() { return m_fixedConstraint->getSoftnessDirLin(); }
	irr::f32 getRestitutionDirLin() { return m_fixedConstraint->getRestitutionDirLin(); }
	irr::f32 getDampingDirLin() { return  m_fixedConstraint->getDampingDirLin(); }
	irr::f32 getSoftnessDirAng() { return m_fixedConstraint->getSoftnessDirAng(); }
	irr::f32 getRestitutionDirAng() { return m_fixedConstraint->getRestitutionDirAng(); }
	irr::f32 getDampingDirAng() { return m_fixedConstraint->getDampingDirAng(); }
	irr::f32 getSoftnessLimLin() { return m_fixedConstraint->getSoftnessLimLin(); }
	irr::f32 getRestitutionLimLin() { return m_fixedConstraint->getRestitutionLimLin(); }
	irr::f32 getDampingLimLin() { return m_fixedConstraint->getDampingLimLin(); }
	irr::f32 getSoftnessLimAng() { return m_fixedConstraint->getSoftnessLimAng(); }
	irr::f32 getRestitutionLimAng() { return m_fixedConstraint->getRestitutionLimAng(); }
	irr::f32 getDampingLimAng() { return m_fixedConstraint->getDampingLimAng(); }
	irr::f32 getSoftnessOrthoLin() { return m_fixedConstraint->getSoftnessOrthoLin(); }
	irr::f32 getRestitutionOrthoLin() { return m_fixedConstraint->getRestitutionOrthoLin(); }
	irr::f32 getDampingOrthoLin() { return m_fixedConstraint->getDampingOrthoLin(); }
	irr::f32 getSoftnessOrthoAng() { return m_fixedConstraint->getSoftnessOrthoAng(); }
	irr::f32 getRestitutionOrthoAng() { return m_fixedConstraint->getRestitutionOrthoAng(); }
	irr::f32 getDampingOrthoAng() { return m_fixedConstraint->getDampingOrthoAng(); }
	void setSoftnessDirLin(irr::f32 softnessDirLin) { m_fixedConstraint->setSoftnessDirLin(softnessDirLin); }
	void setRestitutionDirLin(irr::f32 restitutionDirLin) { m_fixedConstraint->setRestitutionDirLin(restitutionDirLin); }
	void setDampingDirLin(irr::f32 dampingDirLin) { m_fixedConstraint->setDampingDirLin(dampingDirLin); }
	void setSoftnessDirAng(irr::f32 softnessDirAng) { m_fixedConstraint->setSoftnessDirAng(softnessDirAng); }
	void setRestitutionDirAng(irr::f32 restitutionDirAng) { m_fixedConstraint->setRestitutionDirAng(restitutionDirAng); }
	void setDampingDirAng(irr::f32 dampingDirAng) { m_fixedConstraint->setDampingDirAng( dampingDirAng); }
	void setSoftnessLimLin(irr::f32 softnessLimLin) { m_fixedConstraint->setSoftnessLimLin(softnessLimLin); }
	void setRestitutionLimLin(irr::f32 restitutionLimLin) { m_fixedConstraint->setRestitutionLimLin(restitutionLimLin) ; }
	void setDampingLimLin(irr::f32 dampingLimLin) { m_fixedConstraint->setDampingLimLin( dampingLimLin); }
	void setSoftnessLimAng(irr::f32 softnessLimAng) { m_fixedConstraint->setSoftnessLimAng(softnessLimAng) ; }
	void setRestitutionLimAng(irr::f32 restitutionLimAng) { m_fixedConstraint->setRestitutionLimAng(restitutionLimAng); }
	void setDampingLimAng(irr::f32 dampingLimAng) { m_fixedConstraint->setDampingLimAng(dampingLimAng); }
	void setSoftnessOrthoLin(irr::f32 softnessOrthoLin) { m_fixedConstraint->setSoftnessOrthoLin(softnessOrthoLin); }
	void setRestitutionOrthoLin(irr::f32 restitutionOrthoLin) { m_fixedConstraint->setRestitutionOrthoLin(restitutionOrthoLin); }
	void setDampingOrthoLin(irr::f32 dampingOrthoLin) { m_fixedConstraint->setDampingOrthoLin(dampingOrthoLin); }
	void setSoftnessOrthoAng(irr::f32 softnessOrthoAng) { m_fixedConstraint->setSoftnessOrthoAng(softnessOrthoAng); }
	void setRestitutionOrthoAng(irr::f32 restitutionOrthoAng) { m_fixedConstraint->setRestitutionOrthoAng(restitutionOrthoAng); }
	void setDampingOrthoAng(irr::f32 dampingOrthoAng) { m_fixedConstraint->setDampingOrthoAng(dampingOrthoAng); }
	void setPoweredLinMotor(bool onOff) { m_fixedConstraint->setPoweredLinMotor(onOff); }
	bool getPoweredLinMotor() { return m_fixedConstraint->getPoweredLinMotor(); }
	void setTargetLinMotorVelocity(irr::f32 targetLinMotorVelocity) { m_fixedConstraint->setTargetLinMotorVelocity(targetLinMotorVelocity); }
	irr::f32 getTargetLinMotorVelocity() { return  m_fixedConstraint->getTargetLinMotorVelocity(); }
	void setMaxLinMotorForce(irr::f32 maxLinMotorForce) {  m_fixedConstraint->setMaxLinMotorForce(maxLinMotorForce); }
	irr::f32 getMaxLinMotorForce() { return  m_fixedConstraint->getMaxLinMotorForce(); }
	void setPoweredAngMotor(bool onOff) {  m_fixedConstraint->setPoweredAngMotor(onOff); }
	bool getPoweredAngMotor() { return  m_fixedConstraint->getPoweredAngMotor(); }
	void setTargetAngMotorVelocity(irr::f32 targetAngMotorVelocity) { m_fixedConstraint-> setTargetAngMotorVelocity(targetAngMotorVelocity); }
	irr::f32 getTargetAngMotorVelocity() { return  m_fixedConstraint->getTargetAngMotorVelocity(); }
	void setMaxAngMotorForce(irr::f32 maxAngMotorForce) {  m_fixedConstraint->setMaxAngMotorForce( maxAngMotorForce); }
	irr::f32 getMaxAngMotorForce() { return m_fixedConstraint-> getMaxAngMotorForce(); }
	irr::f32 getLinearPos() { return m_fixedConstraint-> getLinearPos(); }
	
	void drop(){delete this;}
protected:
	btSliderConstraint * m_fixedConstraint;
};
#endif