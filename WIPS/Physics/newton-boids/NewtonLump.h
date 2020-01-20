// NewtonLump.h: interface for the NewtonLump class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWTONLUMP_H__CC793455_9F39_437D_9D08_3D07319428B7__INCLUDED_)
#define AFX_NEWTONLUMP_H__CC793455_9F39_437D_9D08_3D07319428B7__INCLUDED_

#include "NewtonBoid.h"
//#include <opensteer\obstacle.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NewtonBaseObject.h"

class NewtonLump : public NewtonBoid 
{
public:
	NewtonLump();
	virtual ~NewtonLump();



public:
 float radius() const;
void Init(ISceneManager* smgr,
		NewtonWorld* world, 
		vector3df* position, vector3df rotationDegrees,
		char* filename,
		float scale,
		float mass);
int isKindOf();
void  SetMeshTransformEventHandler(const NewtonBody* body, const float* matrix);
void  ApplyForceEventHandler (const NewtonBody* body);


void setMaterialData(CMaterialDataBaseClass& m);


protected:
void  FinishContactHandler (const NewtonMaterial* material);
int  BeginContactHandler(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
int ProcessContactHandler(const NewtonMaterial* material, const NewtonContact* contact) ;








};

#endif // !defined(AFX_NEWTONLUMP_H__CC793455_9F39_437D_9D08_3D07319428B7__INCLUDED_)
