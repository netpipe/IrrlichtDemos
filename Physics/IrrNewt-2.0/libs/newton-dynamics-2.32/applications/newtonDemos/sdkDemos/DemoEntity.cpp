#include "toolbox_stdafx.h"
#include "DemoMesh.h"
#include "DemoEntity.h"

dInitRtti(DemoEntity);

#define DEMO_GRAVITY	10.0f

DemoEntity::DemoEntity(DemoEntity* parent)
	:dHierarchy<DemoEntity>(), 
	dClassInfo(), 
	m_matrix(GetIdentityMatrix()), 
	m_curPosition (0.0f, 0.0f, 0.0f, 1.0f),
	m_nextPosition (0.0f, 0.0f, 0.0f, 1.0f),
	m_curRotation (1.0f, 0.0f, 0.0f, 0.0f),
	m_nextRotation (1.0f, 0.0f, 0.0f, 0.0f),
	m_lock(0), 
	m_mesh (NULL)
{
}


DemoEntity::DemoEntity(
	DemoEntityManager& world, 
	const dScene* scene, 
	dScene::dTreeNode* rootSceneNode, 
	dTree<DemoMesh*, dScene::dTreeNode*>& meshCache, 
	//dTree<DemoEntity*, dScene::dTreeNode*>& entityDictionary, 
	DemoEntityManager::EntityDictionary& entityDictionary,
	DemoEntity* parent)
	:dHierarchy<DemoEntity>(), dClassInfo(), 
	m_matrix(GetIdentityMatrix()), 
	m_curPosition (0.0f, 0.0f, 0.0f, 1.0f),
	m_nextPosition (0.0f, 0.0f, 0.0f, 1.0f),
	m_curRotation (1.0f, 0.0f, 0.0f, 0.0f),
	m_nextRotation (1.0f, 0.0f, 0.0f, 0.0f),
	m_lock (0),
	m_mesh (NULL)
{
	// add this entity to the dictionary
	entityDictionary.Insert(this, rootSceneNode);

	// if this is a child mesh set it as child of th entity
	dMatrix parentMatrix (GetIdentityMatrix());
	if (parent) {
		Attach (parent);

		dScene::dTreeNode* parentNode = scene->FindParentByType(rootSceneNode, dSceneNodeInfo::GetRttiType());
		dSceneNodeInfo* parentInfo = (dSceneNodeInfo*) parentNode;
		parentMatrix = parentInfo->GetTransform();
	}

	dSceneNodeInfo* info = (dSceneNodeInfo*) scene->GetInfoFromNode (rootSceneNode);
//	SetMatrix(info->GetTransform() * parentMatrix.Inverse4x4());
	dMatrix matrix (info->GetTransform() * parentMatrix.Inverse4x4());
	dQuaternion rot (matrix);

	// set the matrix twice in oder to get cur and next position
	SetMatrix(world, rot, matrix.m_posit);
	SetMatrix(world, rot, matrix.m_posit);


	// if this node has a mesh, find it and attach it to this entity
	dScene::dTreeNode* meshNode = scene->FindChildByType(rootSceneNode, dMeshNodeInfo::GetRttiType());
	if (meshNode) {
		DemoMesh* mesh = meshCache.Find(meshNode)->GetInfo();
		SetMesh(mesh);
	}

	// add all of the children nodes as child nodes
	for (void* child = scene->GetFirstChild(rootSceneNode); child; child = scene->GetNextChild (rootSceneNode, child)) {
		dScene::dTreeNode* node = scene->GetNodeFromLink(child);
		dNodeInfo* info = scene->GetInfoFromNode(node);
		if (info->GetTypeId() == dSceneNodeInfo::GetRttiType()) {
			new DemoEntity (world, scene, node, meshCache, entityDictionary, this);
		}
	}
}

DemoEntity::~DemoEntity(void)
{
	SetMesh(NULL);
}


void DemoEntity::SetTransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	DemoEntity* const ent = (DemoEntity*) NewtonBodyGetUserData(body);
	
	dQuaternion rot;
	NewtonBodyGetRotation(body, &rot.m_q0);

	DemoEntityManager* world = (DemoEntityManager*) NewtonBodyGetWorld(body);
	const dMatrix& transform = *((dMatrix*) matrix);
	ent->SetMatrix (*world, rot, transform.m_posit);
}


DemoMesh* DemoEntity::GetMesh() const
{
	return m_mesh;
}

void DemoEntity::SetMesh(DemoMesh* mesh)
{
	if (m_mesh) {
		m_mesh->Release();
	}
	m_mesh = mesh;
	if (mesh) {
		mesh->AddRef();
	}
}

dMatrix DemoEntity::GetCurrentMatrix () const
{
	return dMatrix (m_curRotation, m_curPosition);
}

dMatrix DemoEntity::CalculateGlobalMatrix (const DemoEntity* const root) const
{
	dMatrix matrix (GetIdentityMatrix());
	for (const DemoEntity* ptr = this; ptr != root; ptr = ptr->GetParent()) {
		matrix = matrix * GetCurrentMatrix ();
	}
	return matrix;
}

void DemoEntity::SetMatrix(DemoEntityManager& world, const dQuaternion& rotation, const dVector& position)
{
	// read the data in a critical section to prevent race condition from oteh thread  
	world.Lock(m_lock);

	m_curRotation = m_nextRotation;
	m_nextRotation = rotation;

	m_curPosition = m_nextPosition;
	m_nextPosition = position;

	dFloat angle = m_curRotation.DotProduct(m_nextRotation);
	if (angle < 0.0f) {
		m_curRotation.Scale(-1.0f);
	}

	// release the critical section
	world.Unlock(m_lock);
}

void DemoEntity::InterpolateMatrix (DemoEntityManager& world, dFloat param)
{
	// read the data in a critical section to prevent race condition from oteh thread  
	world.Lock(m_lock);

	dVector p0(m_curPosition);
	dVector p1(m_nextPosition);
	dQuaternion r0 (m_curRotation);
	dQuaternion r1 (m_nextRotation);

	// release the critical section
	world.Unlock(m_lock);

	dVector posit (p0 + (p1 - p0).Scale (param));
	dQuaternion rotation (r0.Slerp(r1, param));

	m_matrix = dMatrix (rotation, posit);
}

void DemoEntity::Render(dFloat timeStep) const
{
	// save the model matrix before changing it Matrix
	glPushMatrix();

	// Set The matrix for this entity Node
	glMultMatrix(&m_matrix[0][0]);

	// Render mesh if there is one 
	if (m_mesh) {
		m_mesh->Render ();
	}

	// restore the matrix before leaving
	glPopMatrix();
	
}