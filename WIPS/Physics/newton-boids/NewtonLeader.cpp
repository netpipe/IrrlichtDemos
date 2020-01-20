// NewtonLeader.cpp: implementation of the NewtonLeader class.
//
//////////////////////////////////////////////////////////////////////

#include "NewtonLeader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NewtonLeader::NewtonLeader(){

}

NewtonLeader::~NewtonLeader(){

}

void  NewtonLeader::ApplyForceEventHandler (const NewtonBody* body){

   static double ang =0 ;
	ang = ang + 20*(3.14/180);

	matrix4 fixmat,finalmat;
	finalmat.makeIdentity();

    m_appliedForce.X = m_appliedForce.Y = m_appliedForce.Z = 0.0f;



	fixmat.makeIdentity();
	fixmat.setRotationDegrees(vector3df(0,ang,0));
	finalmat = finalmat* fixmat;
	vector3df rForce(1,0,0);
	finalmat.transformVect(rForce);
	rForce = rForce.normalize();


/*

  float timestep, timestepInv, mass, Ixx, Iyy, Izz;
   NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
   timestep = NewtonGetTimeStep(m_World);
   timestepInv = 1.0f/timestep;
   vector3df velo;
   matrix4 mat;
   mat.makeIdentity();

   NewtonBodyGetMatrix(body,mat.M);
   NewtonBodyGetVelocity(body,&m_velocity.x);
   m_speed = 1;
   Vec3 rand = steerForWander(1);
   rand = rand.normalize();

 	Vec3 tForce = (rand)*m_speed;
 	NewtonBodyAddForce(body,&tForce.x);


//	printf("r %3.4f,%3.4f,%3.4f\n",rForce.X,rForce.Y,rForce.Z);
	NewtonBodyAddForce(body,&rForce.X);




*/

   Vec3 rand = steerForWander(35);

	vector3df r2(rand.x,rand.y,rand.z);
	rForce = (r2*0.35f + rForce*0.65f)*35;
	ApplySteerForce(rForce,body);
    NewtonBodyGetForce(body,&m_appliedForce.X);


}

void NewtonLeader::setMaterialData(CMaterialDataBaseClass& m){
//	m_materialData = m;
}

void NewtonLeader::Init(ISceneManager* smgr, NewtonWorld* world, vector3df* position, vector3df rotationDegrees, char* filename, float scale, float mass){
	NewtonBoid::Init(smgr,world,position,rotationDegrees,filename,scale,mass);
	m_animatedBoxNode->setMaterialTexture(0,
		m_smgr->getVideoDriver()->getTexture("assets\\leader.jpg"));


}
