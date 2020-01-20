// NewtonBaseObject.h: interface for the CNewtonBaseObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWTONBASEOBJECT_H__E63482F5_C751_4601_8EA4_280462EB1F17__INCLUDED_)
#define AFX_NEWTONBASEOBJECT_H__E63482F5_C751_4601_8EA4_280462EB1F17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "main.h"
#include "Quaternion.h"
#include "MaterialDataBaseClass.h"

class CNewtonBaseObject{
public:
	CNewtonBaseObject();
	virtual ~CNewtonBaseObject();
    void drawAppliedForce(IVideoDriver *driver,float scale=0.01f,SColor color=SColor(255,255,0,255));
	virtual int isKindOf()=0;
    aabbox3d<f32> getBoundingBox(){return m_animatedBoxNode->getBoundingBox();}
	vector3df getPosition(){return m_animatedBoxNode->getPosition();}
	virtual void Init(ISceneManager* smgr,
	NewtonWorld* world,
	vector3df* position, vector3df rotationDegrees,
	char* filename,
	float scale,
	float mass)=0;
	virtual void setMaterialData(CMaterialDataBaseClass& data)=0;
	void displayAxes(IVideoDriver *driver);


static int  _cdecl BeginContact (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
static void _cdecl FinishContact (const NewtonMaterial* material);
static int  _cdecl ProcessContact (const NewtonMaterial* material, const NewtonContact* contact);
static void _cdecl SetMeshTransformEvent(const NewtonBody* body, const float* matrix);
static void _cdecl ApplyForceEvent (const NewtonBody* body);

protected:
	vector3df LocalToGlobal(vector3df localVec);
	vector3df GlobalToLocal(vector3df globalVec);
	void  ApplySteerForce (vector3df globalNewtonForce, const NewtonBody* body);
	scene::IAnimatedMesh* m_boxBase ;
    scene::IAnimatedMeshSceneNode* m_animatedBoxNode;
	ISceneManager* m_smgr;
	vector3df m_size;
	NewtonBody* m_boxBody;
	float m_mass;
	NewtonWorld* m_World;
	int m_boxMaterial;
	virtual void  SetMeshTransformEventHandler(const NewtonBody* body, const float* matrix)=0;
	virtual void  ApplyForceEventHandler (const NewtonBody* body)=0;
	virtual int   BeginContactHandler (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)=0;
	virtual void  FinishContactHandler (const NewtonMaterial* material)=0;
	virtual int   ProcessContactHandler (const NewtonMaterial* material, const NewtonContact* contact)=0;
	vector3df m_velocity, m_appliedForce, m_rotation;
	float m_leverDist; // distance offset for torque application
	float m_scaleSteerX, m_scaleSteerY, m_scaleSteerZ,m_scaleUp;
	float m_leverUpDist; //offset torque for up vector;

void AddGlobalForce (vector3df Force, vector3df Point, matrix4 BodyMatrix,const NewtonBody* body);
void AddLocalForce (vector3df LocalForce, vector3df LocalPoint, matrix4 BodyMatrix,const NewtonBody* body);





};

#endif // !defined(AFX_NEWTONBASEOBJECT_H__E63482F5_C751_4601_8EA4_280462EB1F17__INCLUDED_)
