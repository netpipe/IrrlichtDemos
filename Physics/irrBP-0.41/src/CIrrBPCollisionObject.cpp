#include "CIrrBPCollisionObject.h"
#include "animator/CIrrBPAnimator.h"

CIrrBPCollisionObject::CIrrBPCollisionObject()
{
	m_BodyStatus = false;
	m_uBodyId = UNIQUE_BODY_ID_GENERATOR++;
}
void CIrrBPCollisionObject::addAnimator(CIrrBPAnimator * anim)
{
	anim->setBody(this);
	m_Animators.push_back(anim);
}
