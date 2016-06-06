//CollisionObjects.cpp
//Implementation of all the collision shapes

#include "CollisionObjectMgr.h"
#include "irrlicht.h"
#include "utils.h"
#include "CollisionObjects.h"
using namespace irr;
using namespace core;

extern IrrlichtDevice* g_pIrrDevice;

/////////////////////////////////////////////////////////////////////////////////
//Box shape (dynamic)
CBox::CBox(const char* name)
{
	//Get Irrlicht to load the mesh
	if (!LoadMeshFromFile(name)) return;
	//should really have some proper error handling code here

	//Get the collision shape for this mesh, if already created
	// - collision shapes are shared between identical meshes -
	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {

		//first load for this mesh, so create collision shape
		aabbox3d<f32> box=GetBoundingBox();
		vector3df extent=box.getExtent()/2;

		btCollisionShape* pShape = new btBoxShape(btVector3(extent.X, extent.Y, extent.Z));

		//save collision shape
		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);
	}
	m_Mass = 1.f;
 	InitPhysics(GetMaxExtent() / 2);
 }

/////////////////////////////////////////////////////////////////////////////////
//Cone shape (dynamic)
CCone::CCone(const char* name)
{
	if (!LoadMeshFromFile(name)) return;
	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {
		aabbox3d<f32> box=GetBoundingBox();
		vector3df extent=box.getExtent()/2;
		btCollisionShape* pShape = new btConeShape(extent.X, extent.Y*2);
		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);

	}
	m_Mass = 1.f;
 	InitPhysics(GetMaxExtent() / 2);
}

/////////////////////////////////////////////////////////////////////////////////
//Convex hull shape - can't have 'dents' but is dynamic
CConvex::CConvex(const char* name)
{
	if (!LoadMeshFromFile(name)) return;
	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {
		btTriangleMesh* pMesh = GetTriangleMesh(m_pMesh->getMesh(0));
		btCollisionShape* pShape = new btConvexTriangleMeshShape(pMesh);
		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);
	}
	m_Mass = 1.f;
 	InitPhysics(GetMaxExtent() / 2);
}


/////////////////////////////////////////////////////////////////////////////////
//Cylinder shape (dynamic)
CCylinder::CCylinder(const char* name)
{
	if (!LoadMeshFromFile(name)) return;
	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {
		aabbox3d<f32> box=GetBoundingBox();
		vector3df extent=box.getExtent()/2;
		btCollisionShape* pShape = new btCylinderShape(btVector3(extent.X, extent.Y, extent.Z));
		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);
	}
	m_Mass = 1.f;
	InitPhysics(GetMaxExtent() / 2);
}
/////////////////////////////////////////////////////////////////////////////////
//Concave shape (static) - unoptimized, good for simple objects that could be added / removed
// during game
CConcave::CConcave(const char* name)
{
	if (!LoadMeshFromFile(name)) return;
	return;
	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {

		btTriangleMesh* pMesh = GetTriangleMesh(m_pMesh->getMesh(0));
//		btCollisionShape* pShape = new btTriangleMeshShape(pMesh);
//		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);
	}
	m_Mass = 0.f; //this is a static collision type
 	InitPhysics(GetMaxExtent()/2);
}


/////////////////////////////////////////////////////////////////////////////////
//won't bother to implement this. would need a mesh of some sort,
// and we could just create that externally and import it.
//But would be useful for a non-graphical 'safety net'
CPlane::CPlane(const char* name)
{
}

/////////////////////////////////////////////////////////////////////////////////
//Sphere shape (dynamic)
CSphere::CSphere(const char* name)
{
	if (!LoadMeshFromFile(name)) return;
	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {

		aabbox3d<f32> box=GetBoundingBox();
		vector3df extent=box.getExtent()/2;
		btCollisionShape* pShape = new btSphereShape(extent.X);
		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);

	}
	m_Mass = 1.f;
	InitPhysics(GetMaxExtent() / 2);

}

/////////////////////////////////////////////////////////////////////////////////
//Terrain - optimized static mesh
CTerrain::CTerrain(const char* name)
{
	GET_SMGR;
	m_pMesh = smgr->getMesh(name); //returns a IAnimatedMesh*, not a IMesh* as docs say
	m_pNode = smgr->addOctTreeSceneNode(m_pMesh->getMesh(0));
	m_pNode->setMaterialFlag(video::EMF_LIGHTING, false);
	m_Mass = 0.f;
	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {
		btTriangleMesh* pMesh = GetTriangleMesh(m_pMesh->getMesh(0));
		btCollisionShape* pShape = new btBvhTriangleMeshShape((btStridingMeshInterface*)pMesh, true);
		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);
	}
	InitPhysics(GetMaxExtent() / 2);
}

/////////////////////////////////////////////////////////////////////////////////
//Compound shape (dynamic)
//NOTE - at time of writing this code, only shape supported is SPHERE

//There is also a btMultiSphere shape, useful for capsules etc.

enum CompoundObjects
{
	SPHERE,
	BOX,
	CONE,
	CYLINDER
};

//this is just for example, would want a better system to manage compounds than this
CCompound::CCompound(const char* name)
{
	//we are making a compound of 2 shapes. First the shapes we will use
	const s32 obj1 = SPHERE;
	const s32 obj2 = SPHERE;

	//relative positions of each shape
	vector3df pos1(-0.2f,0.0f,0.0f);
	vector3df pos2(0.2f,0.0f,0.0f);

	//first make a compound of 2 meshes, just for rendering
	GET_SMGR;
	//we need a single scene node for both graphic images
	ISceneNode* pParentNode = smgr->addEmptySceneNode();
	pParentNode->setPosition(vector3df(0,0,0));

	//load a mesh
	if (!LoadMeshFromFile(name)) return;
	aabbox3d<f32> box=GetBoundingBox();
	vector3df extent1 = box.getExtent()/2;

	m_pNode->setPosition(pos1);
	//make this mesh a child node of the new node
	m_pNode->setParent(pParentNode);

	if (!LoadMeshFromFile(name)) return;
	box=GetBoundingBox();
	vector3df extent2 = box.getExtent()/2;

	m_pNode->setPosition(pos2);
	m_pNode->setParent(pParentNode);

	//now set m_pNode as the parent node, so it's position, rotation can be updated
	m_pNode=pParentNode;

	//now make the compound physics object
	btTransform trans;

	m_pCollisionObject=g_pCollisionObjectMgr->Get((void*)m_pMesh);
	if (!m_pCollisionObject) {

		btCollisionShape* pShape = new btCompoundShape();
		trans.setOrigin(btVector3(pos1.X,pos1.Y,pos1.Z));
		((btCompoundShape*)pShape)->addChildShape(trans,GetCollisionShape(obj1, extent1));

		trans.setOrigin(btVector3(pos2.X, pos2.Y, pos2.Z));
		((btCompoundShape*)pShape)->addChildShape(trans,GetCollisionShape(obj2, extent2));
		m_pCollisionObject=g_pCollisionObjectMgr->Add(pShape, 0, (void*) m_pMesh);

	}
	m_Mass = 1.f;
 	InitPhysics(GetMaxExtent() / 2);
}


//make a collision shape
btCollisionShape* CCompound::GetCollisionShape(const s32 shapeType, vector3df& extent)
{
	switch (shapeType) {
		case SPHERE:		return new btSphereShape(extent.X);
		case BOX:			return new btBoxShape(btVector3(extent.X, extent.Y, extent.Z));
		case CONE:			return new btConeShape(extent.X, extent.Y*2);
		case CYLINDER:		return new btCylinderShape(btVector3(extent.X, extent.Y, extent.Z));
	}
}

