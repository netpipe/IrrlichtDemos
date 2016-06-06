#pragma once

namespace irr
{
namespace scene
{


class CBulletChracterAnimator :
	public CBulletObjectAnimator
	       
{
	btVector3 m_raySource[2];
	btVector3 m_rayTarget[2];
	btScalar m_rayLambda[2];
	//btVector3 m_rayNormal[2];
	
public:
	CBulletChracterAnimator(void);
	virtual ~CBulletChracterAnimator(void);

	//! CreateInstance
    static CBulletChracterAnimator* createInstance(
      ISceneManager* pSceneManager,
      ISceneNode* pSceneNode,
      CBulletAnimatorManager* pBulletMgr,
      CBulletObjectAnimatorGeometry* pGeom,
      CBulletObjectAnimatorParams* pPhysicsParam
    );
	
    virtual void animateNode(ISceneNode* node, u32 timeMs);

	bool IsOnGround()
	{
		if(m_rayLambda[0] < 1.0f)
			return true;
		else
			return false;
	}

	//현재 앞으로 향하고있는 방향을구한다.(0,0,1) 기준
	btScalar getBodyFrontAngle();	

	void controlStep_Walker(btVector3 WalkVelocity);
	//! Angle은 라디언값
	void controlStep_Walker(btScalar Angle,btScalar Speed);

	//! WalkVelocity 보행 속도(속력+방향)
	//! bVelforce 관성력 제거
	//! bSyncMoveDirection 이동방향바라보기
	void controlStep(btVector3 WalkVelocity,bool bVelforce=false,bool bSyncMoveDirection=false);

};


}
}