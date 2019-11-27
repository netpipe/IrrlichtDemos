/*

Physic class - cpp

*/

#include "Physic.h"

// constructor
cPhysic::cPhysic() {
	playerStillJumping = false;
	previewTime = 0;
	abc = 0.f;
}

// destructor
cPhysic::~cPhysic() {
}

// Initialisation
void cPhysic::Initialize(cInit *setInit) {

	init = setInit;
	device = init->getDevice();
	smgr = device->getSceneManager();
	eventReceiver = init->getEventReceiver();
	einstein.Initialize();
	
}

// creating collision between world/player/rocket
void cPhysic::createWorld(cWorld *level, cPlayer *player, cRocket *rocket) {
	
	TA::DynamicObject *characterController;
	TA::DynamicObject *rocketController;

	scene::ISceneNode *levelNode = level->getLevelSceneNode();
	scene::ISceneNode *rocketNode = rocket->getRocketSceneNode();
	scene::ISceneNode *rocketDummy = rocket->getDummySceneNode();

	scene::IAnimatedMesh *physicMesh = device->getSceneManager()->getMesh("media/map/map_phys.b3d");
	smgr->getMeshManipulator()->scaleMesh(physicMesh, level->getScaleFactor());

	// init TrueAxis
	TA::AABB world(TA::Vec3(0,0,0),TA::Vec3(1000,1000,1000));
	TA::Physics::CreateInstance(world);
	TA::Physics& physics = TA::Physics::GetInstance();

	einstein.createLevelTA(physics, physicMesh, core::vector3df(0,0,0));

	einstein.createPlayerTA(player->getSkeletonDummy(), player->getSkeletonSceneNode());
	einstein.createRocketTA(rocketDummy, rocketNode);
	
	characterController = einstein.getCharacterController();
	rocketController = einstein.getRocketController();
	rocketController->SetPosition(TA::Vec3(60,10,60));
	
	// setting this option true, to allow checking for collision iterators
	characterController->SetCollisionListEnabled(true);
	
	characterController->SetUserGroupItemId(1);
	rocketController->SetUserGroupItemId(2);
	
	characterController->AllowCollisionWithUserGroupItemId(2);
	rocketController->AllowCollisionWithUserGroupItemId(1);
	
	// create poles
	scene::IAnimatedMesh *pole = (IAnimatedMesh*) smgr->getMesh("media/pole/pole_phys.3ds");
	smgr->getMeshManipulator()->scaleMesh(pole->getMesh(0), level->getScaleFactor());
	scene::IAnimatedMeshSceneNode *poleNode1 = smgr->addAnimatedMeshSceneNode(pole);
	poleNode1->setPosition(core::vector3df(-20.5f,4.5f,0.5f));

	scene::IAnimatedMeshSceneNode *poleNode2 = smgr->addAnimatedMeshSceneNode(pole);
	poleNode2->setPosition(core::vector3df(578.5f,4.5f,.5f));

	TA::StaticObject *p1 = einstein.createPoleTA(physics, pole, poleNode1->getPosition());
	poles.addPole(p1, poleNode1->getPosition());

	TA::StaticObject *p2 = einstein.createPoleTA(physics, pole, poleNode2->getPosition());
	poles.addPole(p2, poleNode2->getPosition());
	
	poleNode1->setVisible(false);
	poleNode2->setVisible(false);
}

// start physic world
void cPhysic::beginPhysic(cPlayer *player, cRocket *rocket) {

	TA::Physics &physics = TA::Physics::GetInstance();
	TA::DynamicObject *characterController = einstein.getCharacterController();
	TA::DynamicObject *rocketController = einstein.getRocketController();

	irr::f32 playerVelo = player->getPlayerVelocity();
	core::vector3df lookAt = player->getLookAt();
	scene::IAnimatedMeshSceneNode *skeletonNode = player->getSkeletonSceneNode();

	// get collision iterator to check collisions
	TA::DynamicObject::CollisionIterator it = characterController->GetCollisionIterator();

	// standing on the map ----------------------------------------------
	if (player->getPlayerMode()) {
		physics.SetDynamicObjectIsMoving(characterController);
		player->setLastPlayerMode(player->getPlayerMode());
		
		if (player->isPlayerJumpOffRocket()) {
			einstein.disconnectPlayerRocketTA(player);
			return;
		}

		// check for collision with ground
		core::vector3df v = player->getSkeletonSceneNode()->getAbsolutePosition();
		TA::Vec3 posTA = einstein.convertIrrlichtPos(v);
		TA::Vec3 normalTA = einstein.convertIrrlichtPos(core::vector3df(0,-14.1f,0));
		normalTA += posTA;

		TA::Collision collide = physics.TestLineForCollision(posTA, normalTA);
		if (collide.CollisionOccurred() == true) {

			if (!playerStillJumping && playerVelo > 0.f) {
				lookAt.Y=0.f;
				TA::Vec3 velocity = einstein.convertIrrlichtPos(60.f*playerVelo*lookAt);
				characterController->SetLinearVelocity(velocity);
			}

		} else {
			playerStillJumping=false;
			return;
		}

		// jump routine
		if (player->getPlayerJump() && playerStillJumping == false) {
			playerStillJumping = true;

			TA::Vec3 v = characterController->GetPosition();
			characterController->Clear();
			characterController->SetPosition(v);

			physics.SetDynamicObjectIsMoving(characterController);
			vector3df impulse = 50.f*(lookAt.normalize() + core::vector3df(0,1.f,0));
			characterController->ApplyLinearImpulse(einstein.convertIrrlichtPos(impulse));
		}
		
		for (; !it.AtEnd(); ++it) {
			
			const TA::Collision& collision = *it;
			TA::DynamicObject *collide = collision.GetObjectB();
			
			// player colliding with rocket
			if (collide)
				if (collide == rocketController) {

					TA::MFrame frame = rocketController->GetFrame();
					player->setPlayerMode(false, rocket);
					einstein.createPlayerRocketTA(player, rocket);
					einstein.createClawTA(rocket);
					rocketController->SetFrame(frame);
					
					// ensure that the rocket will not rotate after playermode change
					rocket->setLastVelocity(0.f);
					
				}
		}
		
	// sitting on the rocket ----------------------------------------------
	} else {
		
		if (player->getLastPlayerMode()) {
			rocketController->SetUserGroupItemId(2);
			rocketController->DisallowCollisionWithUserGroupItemId(3);
		}
	
		// checking for cable collision
		if (rocket->isCableOut()) {

			core::line3d<f32> cable;
			core::vector3df distance;
			u32 pole;
			if (rocket->isRightCableOut() && rocket->isRightCableLinked()==false) {
			
				cable = rocket->getRightCable();
				pole = poles.checkForCollision(physics, cable);
				if (pole) {
					rocket->setCableLinked(true, true, poles.getPolePosition(pole));
					distance = rocket->getRocketSceneNode()->getAbsolutePosition()-poles.getPolePosition(pole);
					rocket->setDistanceRocketPole(distance.getLength());
				}
			}
			
			if (rocket->isLeftCableOut() && rocket->isLeftCableLinked()==false) {

				cable = rocket->getLeftCable();
				pole = poles.checkForCollision(physics, cable);
				if (pole) {
					rocket->setCableLinked(false, true, poles.getPolePosition(pole));
					distance = rocket->getRocketSceneNode()->getAbsolutePosition()-poles.getPolePosition(pole);
					rocket->setDistanceRocketPole(distance.getLength());
				}
			}					
		}

		// check for high acceleration => explosion
		TA::Vec3 velo = rocketController->GetLinearVelocity();
		core::vector3df vec = core::vector3df(velo.x,velo.y,velo.z);
		f32 v = vec.getLength();
		
		f32 delta = rocket->getLastVelocity() - v;
		TA::Vec3 explosion = TA::Vec3(0,0,0);
		if (delta > 60.f) {
			explosion.y = 150.f;
		}
		rocket->setLastVelocity(v);

		// setting velocity
		TA::Vec3 vector = TA::Vec3(0,0,0);
		physics.SetDynamicObjectIsMoving(rocketController);
		if (playerVelo > 0.f) {
			lookAt.normalize();
			vector += einstein.convertIrrlichtPos(150.f*playerVelo*lookAt);
		}

		// disable up component when rocket is linked to pole
		if (rocket->isLeftCableLinked() || rocket->isRightCableLinked())
			vector.y = 0.f;
		
		// pole and keyboard rotation
		if (rocket->isRightCableLinked() || rocket->isLeftCableLinked()) {
			
			f32 mult;
			(rocket->isRightCableLinked()) ? (mult=.8f) : (mult=-.8f);
			
			//calculate angular velocity
			core::vector3df vector = rocket->getRocketSceneNode()->getAbsolutePosition();
			vector -= rocket->getLinkedPolePosition();
			
			vector = poles.calculateAngularVelocity(lookAt, vector, rocket->getDistanceRocketPole());
			TA::Vec3 angVelo = einstein.convertIrrlichtPos(vector);
			rocketController->SetAngularVelocity(mult*angVelo);
			
		} else {
		
			irr::f32 rot = player->getPlayerRot();
			TA::Vec3 r = TA::Vec3(0,rot/10.f,0);
			if (explosion.y > 0.f) {
				rocketController->SetAngularVelocity(explosion);
				vector = TA::Vec3(0,0,0);
			}
			else
				rocketController->SetAngularVelocity(r);
			
		}

		rocketController->SetLinearVelocity(vector);
	}

}

// end physic world
void cPhysic::endPhysic() {

	TA::Physics &physics = TA::Physics::GetInstance();
	physics.Update((f32) (1.0f/(60.f)));

}

void cPhysic::fireCube() {
	scene::ICameraSceneNode* cam = smgr->getActiveCamera();
	scene::IAnimatedMeshSceneNode *cube = smgr->addAnimatedMeshSceneNode(smgr->getMesh("cube.3ds"),0,-1,cam->getPosition()); 
	cube->addShadowVolumeSceneNode(-1,false);
	cube->setMaterialFlag(video::EMF_LIGHTING, false);
	cube->setMaterialTexture(0,device->getVideoDriver()->getTexture("rocket.png"));
	CSceneNodeAnimatorTA *tok = new CSceneNodeAnimatorTA(cube, cube, 1000.0f, RIGID_BODY);
	cube->addAnimator(tok);
	tok->drop();	
	core::vector3df forward = (cam->getTarget()-cam->getPosition() + core::vector3df(20,0,20)).normalize();
	tok->m_pDynamicObject->SetLinearVelocity(CSceneNodeAnimatorTA::convertIrrlichtPos(forward*100.0f));
}

// returns position of all poles
const core::array<core::vector3df> cPhysic::getPolePositionArray() {
	return poles.getPolePosition();
}

// return pole class
cPole *cPhysic::getPoles() {
	return &poles;
}
