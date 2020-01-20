// NewtonLump.cpp: implementation of the NewtonLump class.
//
//////////////////////////////////////////////////////////////////////

#include "NewtonLump.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NewtonLump::NewtonLump()
{

}

NewtonLump::~NewtonLump()
{

}


void  NewtonLump::FinishContactHandler (const NewtonMaterial* material)
{


}






int  NewtonLump::BeginContactHandler(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{
 	return 1;

}
int NewtonLump::ProcessContactHandler(const NewtonMaterial* material, const NewtonContact* contact)
{
	return 1;

}


void  NewtonLump::ApplyForceEventHandler (const NewtonBody* body)
{


}

int NewtonLump::isKindOf()
{
	return IS_KIND_OF_LUMP;
}



void NewtonLump::setMaterialData(CMaterialDataBaseClass& m)
{
//	m_materialData = m;
}


float NewtonLump::radius (void) const {return m_size.X*2.0f/2;}


void NewtonLump::Init(ISceneManager* smgr,
		NewtonWorld* world,
		vector3df* position, vector3df rotationDegrees,
		char* filename,
		float scale,
		float mass)

{



	m_boxBase = smgr->getMesh(filename);
	m_animatedBoxNode = smgr->addAnimatedMeshSceneNode(m_boxBase);
	m_animatedBoxNode->setMaterialFlag(video::EMF_LIGHTING, false);
	m_animatedBoxNode->setScale(vector3df(scale,scale,scale));

	mass = 0;

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
		matrix.setTranslation(box.getCenter()*IrrToNewton);
		size = box.getExtent();
		size = box.getExtent() * IrrToNewton;
		m_size = size;
		box.getEdges(edges);

	collision = NewtonCreateBox(m_World, size.X, size.Y, size.Z, NULL);
	m_boxBody = NewtonCreateBody(m_World, collision);
	matrix.setRotationDegrees(rotationDegrees);
    NewtonBodySetLinearDamping(m_boxBody,1.0f);
	vector3df rd(1,1,1);
	NewtonBodySetAngularDamping(m_boxBody,&rd.X);
    NewtonBodySetUserData(m_boxBody, this);


	float Iyy =  2*mass * (size.Y * size.Y )/5;
	float Ixx =  2*mass * (size.X * size.X )/5;
	float Izz =  2*mass * (size.Z * size.Z )/5;

	NewtonBodySetMassMatrix (m_boxBody, m_mass, Ixx, Iyy, Izz);

	NewtonBodySetMatrix(m_boxBody, &matrix.M[0]);

	if (mass != 0)
	SetMeshTransformEvent(m_boxBody,&matrix.M[0]);
	NewtonBodySetMaterialGroupID (m_boxBody, m_boxMaterial);


	NewtonReleaseCollision (m_World, collision);

	NewtonBodySetTransformCallback(m_boxBody, SetMeshTransformEvent);
	NewtonBodySetForceAndTorqueCallback(m_boxBody, ApplyForceEvent);
//	NewtonBodySetAutoFreeze (m_boxBody , 0) ;
//	NewtonWorldUnfreezeBody(m_World, m_boxBody);



}



void  NewtonLump::SetMeshTransformEventHandler(const NewtonBody* body, const float* matrix)
{


}
