/*

Einstein class - cpp

*/

#include "Einstein.h"


// constructor
cEinstein::cEinstein() {
}

// destructor
cEinstein::~cEinstein() {
}

// Initialisation
void cEinstein::Initialize() {
}

// create trueaxis static object of the level
TA::StaticObject *cEinstein::createLevelTA(TA::Physics &physics, IAnimatedMesh *imag, core::vector3df offset) {
	
	IMesh *mesh = imag->getMesh(0);
	
	const u32 cnt = mesh->getMeshBufferCount();
	
	TA::StaticObject *pStaticObject[cnt];
	TA::CollisionObjectAABBMesh *pStaticCollisionObject[cnt];

	irr::u16 *indices;
	scene::IMeshBuffer *buf;
	irr::s32 vertexCount=0;
	irr::s32 indexCount=0;
	video::S3DVertex *vtx;
	TA::Vec3 vec;
	int polygonList[3];

	for (irr::u32 i=0; i<cnt; i++) {

		pStaticObject[i] = TA::StaticObject::CreateNew();
		pStaticCollisionObject[i] = TA::CollisionObjectAABBMesh::CreateNew();

		buf = mesh->getMeshBuffer(i);
		vertexCount = buf->getVertexCount();
		indexCount = buf->getIndexCount();

		pStaticCollisionObject[i]->Initialise(vertexCount,indexCount/3,indexCount);

		vtx = (video::S3DVertex*) buf->getVertices();

		for(irr::s32 k=0;k<vertexCount;k++) { // copy to TA vertices buffer
			vtx[k].Pos += offset;
			vec = CSceneNodeAnimatorTA::convertIrrlichtPos(vtx[k].Pos);
			pStaticCollisionObject[i]->AddVertex(vec);
		}

		indices = buf->getIndices();
		for (irr::s32 j=0; j<indexCount; j+=3) { // convert to TA triangles format
			polygonList[0]=indices[j+2];
			polygonList[1]=indices[j+1];
			polygonList[2]=indices[j+0];
			pStaticCollisionObject[i]->AddPolygon(3,polygonList);
		}
	
		pStaticCollisionObject[i]->FinishedAddingGeometry();
		pStaticObject[i]->Initialise(pStaticCollisionObject[i]);
		pStaticCollisionObject[i]->Release();
		physics.AddStaticObject(pStaticObject[i]);
		pStaticObject[i]->Release();	
		
	}

	return pStaticObject[0];
}

// create trueaxis dynamic object for player
void *cEinstein::createPlayerTA(ISceneNode *dummyNode, ISceneNode *skeletonNode) {
	CSceneNodeAnimatorTA *tok = new CSceneNodeAnimatorTA(dummyNode, skeletonNode, .7f, .8f, true); 
	dummyNode->addAnimator(tok); 
	tok->drop();
	characterController = tok->m_pDynamicObject;
	characterController->SetRotationDisabled(true);
	characterController->SetGravityMult(15.f);
}

// converts irrlicht vector3d in trueaxis vec3
TA::Vec3 cEinstein::convertIrrlichtPos(irr::core::vector3df pos) {
	// convert to TA vector
	TA::Vec3 tokpos(pos.X,pos.Y,pos.Z);
	return tokpos;
}

// create trueaxis dynamic object for rocket
void *cEinstein::createRocketTA(ISceneNode *dummyNode, ISceneNode *rocketNode) {
	CSceneNodeAnimatorTA *tok = new CSceneNodeAnimatorTA(dummyNode, rocketNode, 1.f, 1.f, false);
	dummyNode->addAnimator(tok);
	tok->drop();
	rocketController = tok->m_pDynamicObject;
	rocketController->SetGravityMult(20.f);
}

// create player/rocket TA
void cEinstein::createPlayerRocketTA(cPlayer *player, cRocket *rocket) {
	if (characterController && rocketController) {

		TA::Physics &physics = TA::Physics::GetInstance();
		
		characterController->Clear();
		characterController->SetPosition(characterController->GetPosition()+TA::Vec3(0,15,5));

		TA::Vec3 pos = rocketController->GetPosition();
		rocketController->Clear();
		rocketController->SetPosition(pos);
	
		characterController->SetRotationDisabled(true);
		characterController->SetMovementDisabled(true);
		rocketController->SetRotationDisabled(false);
		rocketController->SetGravityMult(0.f);
	}
}

// disable player/rocket TA
void cEinstein::disconnectPlayerRocketTA(cPlayer *player) {
	
	core::vector3df lookAt = player->getLookAt();
	TA::Physics &physics = TA::Physics::GetInstance();

	lookAt.normalize();

	TA::Vec3 p;
	p.x = -30.f*lookAt.X;
	p.y = 0.f;
	p.z = -30.f*lookAt.Z;

	characterController->Clear();
	characterController->SetPosition(rocketController->GetPosition()+TA::Vec3(-25.f*lookAt.X,15,-25.f*lookAt.Z));
	physics.SetDynamicObjectIsMoving(characterController);
	characterController->ApplyLinearImpulse(p+TA::Vec3(0,15,0));
	characterController->SetMovementDisabled(false);
	player->setPlayerJumpOffRocket(false);
	rocketController->SetGravityMult(20.f);

}

// return dynamic object character
TA::DynamicObject *cEinstein::getCharacterController() {
	return characterController;
}
	
// return dynamic object rocket
TA::DynamicObject *cEinstein::getRocketController() {
	return rocketController;
}

// create cable claw TA
void cEinstein::createClawTA(cRocket *rocket) {

	scene::ISceneNode *clawNode;
	CSceneNodeAnimatorTA *tok;
	TA::DynamicObject *clawController;
	
	clawNode = (scene::ISceneNode*) rocket->getRightClawSceneNode();

	tok = new CSceneNodeAnimatorTA(clawNode, clawNode, .1f, .1f); 
	clawNode->addAnimator(tok); 
	tok->drop();
	clawController = tok->m_pDynamicObject;
	clawController->SetRotationDisabled(true);
	clawController->SetMovementDisabled(false);
	clawController->SetGravityMult(0.f);

	rocket->setClawTAController(true, clawController);

	clawNode = (scene::ISceneNode*) rocket->getLeftClawSceneNode();
					
	tok = new CSceneNodeAnimatorTA(clawNode, clawNode, .1f, .1f); 
	clawNode->addAnimator(tok); 
	tok->drop();
	clawController = tok->m_pDynamicObject;
	clawController->SetRotationDisabled(true);
	clawController->SetMovementDisabled(false);
	clawController->SetGravityMult(0.f);

	rocket->setClawTAController(false, clawController);
	
}

// create pole TA
TA::StaticObject *cEinstein::createPoleTA(TA::Physics &physics, scene::IAnimatedMesh *imag, core::vector3df position) {

	IMesh *mesh = imag->getMesh(0);
	
	TA::StaticObject *pStaticObject;
	TA::CollisionObjectAABBMesh *pStaticCollisionObject;

	irr::u16 *indices;
	scene::IMeshBuffer *buf;
	irr::s32 vertexCount=0;
	irr::s32 indexCount=0;
	video::S3DVertex *vtx;
	TA::Vec3 vec,pos;
	int polygonList[3];
	
	pos.x = position.X;
	pos.y = position.Y;
	pos.z = position.Z;
	
	pStaticObject = TA::StaticObject::CreateNew();
	pStaticCollisionObject = TA::CollisionObjectAABBMesh::CreateNew();

	buf = mesh->getMeshBuffer(0);
	vertexCount = buf->getVertexCount();
	indexCount = buf->getIndexCount();

	pStaticCollisionObject->Initialise(vertexCount,indexCount/3,indexCount);

	vtx = (video::S3DVertex*) buf->getVertices();

	for(irr::s32 k=0;k<vertexCount;k++) { // copy to TA vertices buffer
		vec = CSceneNodeAnimatorTA::convertIrrlichtPos(vtx[k].Pos);
		vec += pos;
		pStaticCollisionObject->AddVertex(vec);
	}

	indices = buf->getIndices();
	for (irr::s32 j=0; j<indexCount; j+=3) { // convert to TA triangles format
		polygonList[0]=indices[j+2];
		polygonList[1]=indices[j+1];
		polygonList[2]=indices[j+0];
		pStaticCollisionObject->AddPolygon(3,polygonList);
	}
	
	pStaticCollisionObject->FinishedAddingGeometry();
	pStaticObject->Initialise(pStaticCollisionObject);
	pStaticCollisionObject->Release();
	physics.AddStaticObject(pStaticObject);
	pStaticObject->Release();	
		
	return pStaticObject;
}

// create dynamic object
TA::DynamicObject *cEinstein::createDynamicObject(scene::ISceneNode *node1, scene::ISceneNode *node2) {
	CSceneNodeAnimatorTA *tok = new CSceneNodeAnimatorTA(node1, node2, .7f, .8f); 
	node1->addAnimator(tok); 
	tok->drop();
	TA::DynamicObject *plane = tok->m_pDynamicObject;
	plane->SetRotationDisabled(true);
	plane->SetGravityMult(0.f);
	return plane;
}

