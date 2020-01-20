// NewtonBoid.cpp: implementation of the NewtonBoid class.
//
//////////////////////////////////////////////////////////////////////
#include <steerlibrary.h>
#include "NewtonBoid.h"
#include "NewtonLeader.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>
extern vector3df getRandomForce();
extern KeyItem keyMap[14];
extern bool g_wander;
extern int getrand(int x);
//extern ObstacleGroup g_obstacles;
extern NewtonLeader g_leader;
extern float DegreesToRadians(float i);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace OpenSteer;

int NewtonBoid::isKindOf(){
	return IS_KIND_OF_FLY;
}

NewtonBoid::NewtonBoid(){
	m_firstTime = false;
	m_followCam = NULL;
	m_neighbors = NULL;
	m_id = -1;
	m_speed = 0;
	m_neighborCount = 0;
	m_target = Vec3(0,0,0);

	m_leverDist = 0.5f; //lever dist for x,y,z;
	m_scaleSteerX = m_scaleSteerY =  0.0001f; // force scaling
	m_leverUpDist = 1.0f; // lever dist for the up vector correction
	m_scaleUp = 0.0001f;  // force scaling for the up vector correction
    m_scaleSteerZ = 0.0001f;
	m_sepangle = DegreesToRadians(-45);
	m_sepdist = 15.0f;





}

NewtonBoid::~NewtonBoid(){

}

void  NewtonBoid::FinishContactHandler (const NewtonMaterial* material){

}

int  NewtonBoid::BeginContactHandler(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1){
 	return 1;

}

int NewtonBoid::ProcessContactHandler(const NewtonMaterial* material, const NewtonContact* contact){
	return 1;

}

void  NewtonBoid::ApplyForceEventHandler (const NewtonBody* body){

  float timestep, timestepInv, mass, Ixx, Iyy, Izz;
   static double ang =0 ;
   NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
   timestep = NewtonGetTimeStep(m_World);
   timestepInv = 1.0f/timestep;
   vector3df velo;
   matrix4 mat;
   mat.makeIdentity();
 //  m_appliedForce.X = m_appliedForce.Y = m_appliedForce.Z = 0.0f;

   NewtonBodyGetMatrix(body,mat.M);
   NewtonBodyGetVelocity(body,&m_velocity.x);
   velo.X = m_velocity.x; velo.Y = m_velocity.y; velo.Z = m_velocity.z;
   m_speed = velo.getLength();
   vector3df omegaVelocity;
   vector3df omegaForce;
   NewtonBodyGetVelocity(body,&omegaVelocity.X);
 	ang = ang + 1.0f;
	double r = ang*3.14159f/180.0f;

   m_futurePosition = m_velocity*timestep  ;

  	Vec3 coh;
    Vec3 sep  = steerForSeparation (m_sepdist,m_sepangle,avNeighbors);
 	Vec3 dest = steerForSeek(m_target);
	coh = steerForCohesion(1,-0.714f,avNeighbors);
	Vec3 tForce = ( 0.5f* sep +0.5* dest)*4.0f;



	if(g_wander)
	{
		sep = steerForSeparation (5.5f,-0.7f,avNeighbors)+  steerForWander(14);
		coh = steerForCohesion(1,-0.714f,avNeighbors);

		tForce = (sep);




	}

    vector3df mForce(tForce.x,tForce.y,tForce.z);
	ApplySteerForce(mForce,body);


    NewtonBodyGetForce(body,&m_appliedForce.X);

}

void  NewtonBoid::SetMeshTransformEventHandler(const NewtonBody* body, const float* matrix){

   matrix4 mat, fixmat, unfixmat, rotmat;
   memcpy(mat.M, matrix, sizeof(float)*16);
   setPosition(mat.getTranslation().X,mat.getTranslation().Y, mat.getTranslation().Z);
   vector3df place = mat.getTranslation()* NewtonToIrr ;
    m_animatedBoxNode->setPosition(place);
   m_animatedBoxNode->setRotation(mat.getRotationDegrees());
	m_animatedBoxNode->updateAbsolutePosition();
    vector3df pos = m_animatedBoxNode->getAbsolutePosition();
	vector3df tVec(0,0,0);
 	mat.transformVect(vector3df(0,0,-8),tVec);
 	m_followCam->setPosition(pos+tVec);
	m_followCam->setTarget(g_leader.getPosition());





}

void NewtonBoid::Init(ISceneManager* smgr, NewtonWorld* world, vector3df* position, vector3df rotationDegrees, char* filename, float scale, float mass){




	float mm = 8; // md2  milk is 41.5
	m_boxBase = smgr->getMesh(filename);
	m_animatedBoxNode = smgr->addAnimatedMeshSceneNode(m_boxBase);
	m_animatedBoxNode->setMaterialFlag(video::EMF_LIGHTING, false);
	m_animatedBoxNode->setScale(vector3df(scale,scale,scale));
// 	m_animatedBoxNode->setDebugDataVisible(true);

	m_animatedBoxNode->setAnimationSpeed(25);


	m_animatedBoxNode->setLoopMode(true);
	m_animatedBoxNode->setFrameLoop(96,118);

	m_followCam =	smgr->addCameraSceneNode();



	m_smgr = smgr;
	m_World = world;
	m_mass = mass;
	vector3df size, origin,trans;
	vector3df edges[8];
	core::aabbox3d<f32> box;
	NewtonCollision* collision;
	matrix4 matrix,dispmat;
	//size the box in Newton units
 	box = m_boxBase->getMesh(0)->getBoundingBox();
	matrix4 fixmat;
	fixmat.setScale(vector3df(scale,scale,scale));
	fixmat.transformBox(box);
	m_animatedBoxNode->setRotation(rotationDegrees);
	m_animatedBoxNode->updateAbsolutePosition();
	if (position != NULL)
	{
		m_animatedBoxNode->setPosition(*position);
		m_animatedBoxNode->updateAbsolutePosition();
		matrix.setTranslation(m_animatedBoxNode->getPosition()*IrrToNewton);
	}
	else
	{
		matrix.setTranslation(box.getCenter()*IrrToNewton);
	}
	size = box.getExtent();
	size = box.getExtent() * IrrToNewton;
	m_size = size;
	box.getEdges(edges);

	collision = NewtonCreateSphere(m_World, size.X, size.Y, size.Z, NULL);
	m_boxBody = NewtonCreateBody(m_World, collision);
	matrix.setRotationDegrees(rotationDegrees);
    NewtonBodySetLinearDamping(m_boxBody,1.0);
	vector3df rd(1.0f,1.0f,1.0f);
	NewtonBodySetAngularDamping(m_boxBody,&rd.X);
    NewtonBodySetUserData(m_boxBody, this);


	float Iyy = 10*  mass * (size.Y * size.Y )/12;
	float Ixx = 10*  mass * (size.X * size.X )/12;
	float Izz = 10*  mass * (size.Z * size.Z )/12;

	NewtonBodySetMassMatrix (m_boxBody, m_mass, Ixx, Iyy, Izz);
/*
   	NewtonJoint* upVector;
	vector3df upDirection (0.0f, 1.0f, 0.0f);
	upVector = NewtonConstraintCreateUpVector (m_World, &upDirection.X, m_boxBody);
*/



	NewtonBodySetMatrix(m_boxBody, &matrix.M[0]);

	if (mass != 0)
	SetMeshTransformEvent(m_boxBody,&matrix.M[0]);
	NewtonBodySetMaterialGroupID (m_boxBody, m_boxMaterial);


	NewtonReleaseCollision (m_World, collision);

	NewtonBodySetTransformCallback(m_boxBody, SetMeshTransformEvent);
	NewtonBodySetForceAndTorqueCallback(m_boxBody, ApplyForceEvent);
	NewtonBodySetAutoFreeze (m_boxBody , 0) ;
	NewtonWorldUnfreezeBody(m_World, m_boxBody);








}

void NewtonBoid::setMaterials(int materialID){
	m_boxMaterial = materialID;

}

void NewtonBoid::setNeighbors(NewtonBoid **neighbors){
	m_neighbors = neighbors;
	if (m_neighborCount)
	{
		for (int j=0;j<m_neighborCount;j++)
		{

			avNeighbors.push_back(neighbors[j]);
		}


	}
}

void NewtonBoid::setFlyID(int id){
	m_id = id;
}

void NewtonBoid::setNeighborCount(int c){
	m_neighborCount = c;
}

int NewtonBoid::getNeighorCount(){
	return m_neighborCount ;
}

void NewtonBoid::setMaterialData(CMaterialDataBaseClass& m){
//	m_materialData = m;
}
