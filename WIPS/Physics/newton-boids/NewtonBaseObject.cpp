// NewtonBaseObject.cpp: implementation of the CNewtonBaseObject class.
//

#include "NewtonBaseObject.h"
extern 	 float RadiansToDegrees( float testvar);
#include "SpecialEffectStruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewtonBaseObject::CNewtonBaseObject()
{
	m_boxBase = NULL;
	m_smgr = NULL;
	m_mass = 0.0f;
    m_animatedBoxNode = NULL;

	m_leverDist = 1.0f; //lever dist for x,y,z;
	m_scaleSteerX = m_scaleSteerY = m_scaleSteerZ = 0.0001f; // force scaling
	m_leverUpDist = 10.0f; // lever dist for the up vector correction
	m_scaleUp = 0.00001f;  // force scaling for the up vector correction


}

CNewtonBaseObject::~CNewtonBaseObject()
{
}


/*
void CNewtonBaseObject::Init(ISceneManager* smgr,
		NewtonWorld* world,
		vector3df* position,
		char* filename,
		float scale,
		float mass,CMaterialData* data)
{

	m_boxBase = smgr->getMesh(filename);
	m_boxNode = smgr->addMeshSceneNode(m_boxBase->getMesh(0));
	m_animatedBoxNode = smgr->addAnimatedMeshSceneNode(m_boxBase);
	m_animatedBoxNode->setMaterialFlag(video::EMF_LIGHTING, false);
	m_animatedBoxNode->setScale(vector3df(scale,scale,scale));
 	m_animatedBoxNode->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("assets\\target.png"));

	m_materialData =  data;
  	m_animatedBoxNode->setDebugDataVisible(true);

	m_smgr = smgr;
	m_World = world;
	m_mass = mass;
	vector3df size, origin,trans;
	vector3df edges[8];
	core::aabbox3d<f32> box;
	NewtonCollision* collision;
	matrix4 matrix,dispmat;
	//size the box in Newton units
	int count = m_boxBase->getFrameCount();
 	box = m_boxBase->getMesh(0)->getBoundingBox();
	matrix4 fixmat;
	fixmat.setScale(vector3df(scale,scale,scale));
	fixmat.transformBox(box);
	if (position != NULL)
		matrix.setTranslation(m_animatedBoxNode->getPosition()*IrrToNewton);
	else
		matrix.setTranslation(box.getCenter()*IrrToNewton);
	size = box.getExtent();
	size = box.getExtent() * IrrToNewton;
	m_size = size;
    box.getEdges(edges);

	collision = NewtonCreateSphere(m_World, size.X*scale, size.Y*scale, size.Z*scale, NULL);
	m_boxBody = NewtonCreateBody(m_World, collision);

    NewtonBodySetUserData(m_boxBody, this);

//	m_boxMaterial = m_materialData->m_groundMaterial;
	m_boxMaterial = NewtonMaterialCreateGroupID(m_World);

	float Iyy =  mass * (size.Y * size.Y )/12;
	float Ixx =  mass * (size.X * size.X )/12;
	float Izz =  mass * (size.Z * size.Z )/12;

	NewtonBodySetMassMatrix (m_boxBody, m_mass, Ixx, Iyy, Izz);

	NewtonBodySetMatrix(m_boxBody, &matrix.M[0]);

	if (mass != 0)
	SetMeshTransformEvent(m_boxBody,&matrix.M[0]);
	NewtonBodySetMaterialGroupID (m_boxBody, m_boxMaterial);


	NewtonReleaseCollision (m_World, collision);












}
*/


void _cdecl CNewtonBaseObject::SetMeshTransformEvent(const NewtonBody* body, const float* matrix)
{
  CNewtonBaseObject* controller = (CNewtonBaseObject*) NewtonBodyGetUserData(body);
  if( controller != NULL)
	controller->SetMeshTransformEventHandler(body,matrix);


}

void _cdecl CNewtonBaseObject::ApplyForceEvent (const NewtonBody* body)
{

  CNewtonBaseObject* controller = (CNewtonBaseObject*) NewtonBodyGetUserData(body);
  if( controller != NULL)

	controller->ApplyForceEventHandler(body);


}




int  _cdecl CNewtonBaseObject::BeginContact (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{

   CNewtonBaseObject* controller =
	  (CNewtonBaseObject*) NewtonBodyGetUserData(body1);

  return controller->BeginContactHandler(material,body0,body1);



}
void _cdecl CNewtonBaseObject::FinishContact (const NewtonMaterial* material)
{
	SpecialEffectStruct* zzz =
		(SpecialEffectStruct *) NewtonMaterialGetMaterialPairUserData(material);
	CNewtonBaseObject* controller =
		(CNewtonBaseObject*) zzz->controller;

     controller->FinishContactHandler(material);


}
int  _cdecl CNewtonBaseObject::ProcessContact (const NewtonMaterial* material, const NewtonContact* contact)
{
		SpecialEffectStruct* zzz =
		(SpecialEffectStruct *) NewtonMaterialGetMaterialPairUserData(material);
	CNewtonBaseObject* controller =
		(CNewtonBaseObject*) zzz->controller;

   return  controller->ProcessContactHandler(material,contact);


}







vector3df CNewtonBaseObject::LocalToGlobal(vector3df localVec)
{
		vector3df rot = m_animatedBoxNode->getRotation(); //local
		Quaternion q1 = MakeQFromEulerAngles(rot.X,rot.Y,rot.Z); //local quat
		vector3df globalVec = QVRotate(q1,localVec);
		return globalVec;
}

vector3df CNewtonBaseObject::GlobalToLocal(vector3df globalVec)
{

		vector3df rot = m_animatedBoxNode->getRotation(); //local
		Quaternion q1 = MakeQFromEulerAngles(rot.X,rot.Y,rot.Z); //local quat


		float mag = q1.Magnitude();

		if (mag != 0)
			q1 /= mag;

		vector3df localVec = QVRotate(~q1,globalVec);

		return localVec;



}

void  CNewtonBaseObject::ApplySteerForce (vector3df globalNewtonForce, const NewtonBody* body)
{
	float timestep, timestepInv;
	matrix4 mat;
	mat.makeIdentity();
	NewtonBodyGetMatrix(body,mat.pointer());
	timestep = NewtonGetTimeStep(m_World);
	timestepInv = 1.0f/timestep;
	NewtonBodyGetVelocity(body,&m_velocity.X);
	globalNewtonForce *= -1.0f;


	vector3df transXaxis = LocalToGlobal(vector3df(1,0,0));
	transXaxis = transXaxis.normalize();


	vector3df localVel = GlobalToLocal(globalNewtonForce);
	vector3df nForce;
	vector3df angV;
	NewtonBodyGetOmega(body,&angV.X);



	nForce.Y = m_scaleSteerY *  m_mass * (localVel.Y -m_velocity.Y*timestep)/(timestep*timestep);
	nForce.X = m_scaleSteerX *  m_mass * (localVel.X -m_velocity.X*timestep)/(timestep*timestep);
	nForce.Z = m_scaleSteerZ *  m_mass * (localVel.Z -m_velocity.Z*timestep)/(timestep*timestep);

	AddLocalForce(vector3df(0,-1,0)*nForce.Y,vector3df(0,0,m_leverDist*m_size.Y/2),mat,body);
	AddLocalForce(vector3df(0,0,1)*nForce.X,vector3df(m_leverDist*m_size.X/2,0,0),mat,body);
 	AddLocalForce(vector3df(0,0,-1)*nForce.Z,vector3df(0,0,0),mat,body);



///////// y up correction/////////////////////////////////////////////////////

		float mm = -1.0f;
		transXaxis = transXaxis.normalize();
		float testvar = (float) acos(transXaxis.dotProduct(vector3df(0,1,0)));
	 	float testdeg = RadiansToDegrees(testvar);
		float tester = transXaxis.Y;
	    if (tester < 0) mm = 1.0f;
		float rfix = m_mass* (testvar - angV.Z* timestep)/(timestep*timestep);
		rfix = rfix *m_scaleUp;
		float testr = 90.0f - testdeg;
		if (testr < 0) testr = -testr;
	    AddLocalForce(vector3df(0,1,0)*rfix*mm,vector3df(m_leverUpDist*m_size.X/2,0,0),mat,body);

///////////////////////////////////////////////////////////////////////////////


}

void CNewtonBaseObject::AddGlobalForce (vector3df Force, vector3df Point, matrix4 BodyMatrix,const NewtonBody* body)
{
    vector3df LocalPoint = BodyMatrix.getTranslation()*NewtonToIrr;
	vector3df R = Point - BodyMatrix.getTranslation();
//	R.normalize();

     //printf("body %3.4f,%3.4f,%3.4f\n",LocalPoint.X,LocalPoint.Y,LocalPoint.Z);
     //printf("R %3.4f,%3.4f,%3.4f\n",R.X,R.Y,R.Z);

    vector3df Torque = R.crossProduct(Force);
    NewtonBodyAddForce (body,&Force.X);
    NewtonBodyAddTorque (body,&Torque.X);
}


void CNewtonBaseObject::AddLocalForce (vector3df LocalForce, vector3df LocalPoint, matrix4 BodyMatrix,const NewtonBody* body)
{
     vector3df GlobalForce,GlobalPoint;
     BodyMatrix.transformVect(LocalForce,GlobalForce);
    BodyMatrix.transformVect(LocalPoint,GlobalPoint);
	if (LocalForce.X == 0.0f && LocalForce.Y == 0.0f && LocalForce.Z == 0.0f)
	{

	}
	else
	{
		AddGlobalForce (GlobalForce, GlobalPoint,BodyMatrix,body);

	}
}


void CNewtonBaseObject::displayAxes(IVideoDriver *driver){
	if (m_animatedBoxNode){
		vector3df start = m_animatedBoxNode->getPosition(); //global//
		vector3df end = LocalToGlobal(vector3df(0,1,0));
		vector3df upaxis = end;
		upaxis = upaxis.normalize();
		end = end.normalize()*10; // end is global origin global start
		driver->draw3DLine(start,start+end,SColor(255,255,255,0));
		end = LocalToGlobal(vector3df(1,0,0));
		end = end.normalize()*10;
    driver->draw3DLine(start,start+end,SColor(255,0,255,0));
		end = LocalToGlobal(vector3df(0,0,1));
		end = end.normalize()*10;
    driver->draw3DLine(start,start+end,SColor(255,255,0,0));
	}
}

void CNewtonBaseObject::drawAppliedForce(IVideoDriver *driver, float scale, SColor color){
	matrix4 mat;
	SMaterial material;

	material.setTexture(0, 0);
	material.Lighting = false;
 	driver->setTransform(video::ETS_WORLD, mat);
 	driver->setMaterial(material);
	vector3df tpos = m_animatedBoxNode->getAbsolutePosition();
	vector3df nForce = m_appliedForce * scale;
	vector3df ePoint = tpos + nForce;
	driver->draw3DLine(tpos, ePoint, color);

}
