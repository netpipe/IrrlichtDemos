// NewtonBoid.h: interface for the NewtonBoid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWTONBOID_H__E956DB27_EC73_4A04_A73E_E77D64EB4D2E__INCLUDED_)
#define AFX_NEWTONBOID_H__E956DB27_EC73_4A04_A73E_E77D64EB4D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <steerlibrary.h>
#include "NewtonBaseObject.h"

using namespace OpenSteer;

    typedef LocalSpaceMixin<AbstractVehicle> SimpleVehicle_1;


    // SimpleVehicle_2 adds concrete annotation methods to SimpleVehicle_1
//    typedef AnnotationMixin<SimpleVehicle_1> SimpleVehicle_2;


   //  SimpleVehicle_2 adds concrete steering methods to SimpleVehicle_1
     typedef SteerLibraryMixin<SimpleVehicle_1> SimpleVehicle_2;



class NewtonBoid : public CNewtonBaseObject,public SimpleVehicle_2
{
public:
	int getNeighorCount();
	void setNeighborCount(int c);
	void setFlyID(int id);
	void setNeighbors(NewtonBoid** neighbors);
	NewtonBoid();
	virtual ~NewtonBoid();

void setMaterialData(CMaterialDataBaseClass& m);

	Vec3 velocity (void) const {return m_velocity; }//must be updated in force event

       float mass (void) const {return m_mass;}
        float setMass (float s) {return 1;}

        float speed (void) const {return m_speed;}
        float setSpeed (float s) {return m_speed = s;}

        // size of bounding sphere, for obstacle avoidance, etc. neighborhoods
    //    float radius (void) const {return m_size.X*1.5;}
        float setRadius (float m) {return 1.0f;}
         float radius (void) const {return m_size.X;}

        // get/set maxForce
        float maxForce (void) const {return 10000.0f;}
        float setMaxForce (float mf) {return 1.0f;}

        // get/set maxSpeed
        float maxSpeed (void) const {return 100000;}
        float setMaxSpeed (float ms) {return 1.0f;}
    Vec3 predictFuturePosition (const float predictionTime) const
	{ return  m_futurePosition; }


void addToSepAngle(float i){
	m_sepangle += i; }
void addToSepDist(float i){
	m_sepdist += i;
	if (m_sepdist < 0.0f) m_sepdist = 0.0f;
//	printf("dist %4.3f\n",m_sepdist);
}



void Init(ISceneManager* smgr,
		NewtonWorld* world,
		vector3df* position, vector3df rotationDegrees,
		char* filename,
		float scale,
		float mass);
void setMaterials(int materialID);
int isKindOf();
void  SetMeshTransformEventHandler(const NewtonBody* body, const float* matrix);
void  ApplyForceEventHandler (const NewtonBody* body);
Vec3 m_target;

//void AddGlobalForce (vector3df Force, vector3df Point, matrix4 BodyMatrix,const NewtonBody* body);
//void AddLocalForce (vector3df LocalForce, vector3df localPoint, matrix4 BodyMatrix,const NewtonBody* body);

bool m_firstTime ;
ICameraSceneNode*  m_followCam;

protected:


AVGroup avNeighbors;
int m_neighborCount ;
float m_speed;
Vec3 m_velocity;
Vec3 m_futurePosition;
NewtonBoid** m_neighbors;
int m_id ;
void  NewtonBoid::FinishContactHandler (const NewtonMaterial* material);
int   NewtonBoid::BeginContactHandler(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
int   NewtonBoid::ProcessContactHandler(const NewtonMaterial* material, const NewtonContact* contact) ;


private:
	float m_sepangle;
	float m_sepdist;




};

#endif // !defined(AFX_NEWTONBOID_H__E956DB27_EC73_4A04_A73E_E77D64EB4D2E__INCLUDED_)
