#include "player.h"

CPlayer::CPlayer(vector3df Position, vector3df Target, float MovSpeed, float RotSpeed, float JumpSpeed, ISceneManager* smgr, IVideoDriver* driver, IrrlichtDevice* device, ITimer* timer, CMap* Map, CEventReceiver* receiver, CEffectsManager* EffectsManager)
{
    CPlayer::device = device;
    CPlayer::receiver = receiver;
    CPlayer::FarValue = 100000;
    CPlayer::StartPos = Position;
    CPlayer::StartTar = Target;
    CPlayer::MovSpeed = MovSpeed;
    CPlayer::RotSpeed = RotSpeed;
    CPlayer::JumpSpeed = JumpSpeed;
    CPlayer::smgr = smgr;
    CPlayer::driver = driver;
    CPlayer::timer = timer;
    CPlayer::EffectsManager = EffectsManager;
    CPlayer::Active = true;
    CPlayer::isDead = false;
    CPlayer::FinishedLevel = false;

    CPlayer::BloomActive = false;
    CPlayer::InvertActive = false;

	SKeyMap keyMap[6];
	SKeyMap keyMapEMPTY[1];

	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_KEY_W;

	keyMap[1].Action = EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode = KEY_KEY_S;

	keyMap[2].Action = EKA_STRAFE_LEFT;
	keyMap[2].KeyCode = KEY_KEY_A;

	keyMap[3].Action = EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode = KEY_KEY_D;

	keyMap[4].Action = EKA_JUMP_UP;
	keyMap[4].KeyCode = KEY_SPACE;

	keyMap[5].Action = EKA_CROUCH;
	keyMap[5].KeyCode = KEY_KEY_C;

    if(!CC_PHYSICS)
    {
        CPlayer::Camera = smgr->addCameraSceneNodeFPS(0,RotSpeed,MovSpeed,ID_IsPlayer,keyMap,6,true,JumpSpeed);
        CPlayer::Camera->setPosition(Position);
        CPlayer::Camera->setTarget(Target);
        CPlayer::Camera->setFarValue(CPlayer::FarValue);

        ISceneNodeAnimatorCollisionResponse* anim = smgr->createCollisionResponseAnimator(Map->getSelector(),CPlayer::Camera,vector3df(30,50,30),vector3df(0,-10,0),vector3df(0,30,0));
        Map->getSelector()->drop();
        CPlayer::Camera->addAnimator(anim);
    }

    if(CC_PHYSICS)
    {
        CPlayer::Camera = smgr->addCameraSceneNodeFPS(0, 100, 0.5);
        CPlayer::Camera->setPosition(Position);
        CPlayer::Camera->setTarget(Target);
        CPlayer::Camera->setFarValue(CPlayer::FarValue);
    }

	CPlayer::Health = 100;
	CPlayer::shoot = false;
	CPlayer::shoot2 = false;
	CPlayer::CurrentWeapon = WT_NORMAL;
	CPlayer::checkEffect = 0;
	CPlayer::FiringMode = FM_SINGLE;
	CPlayer::ChangedFireMode = false;
	CPlayer::shotForce = 0;

    CPlayer::Ammo[WT_NORMAL] = 0;
    CPlayer::Ammo[WT_EXPLOSIVE] = 0;
    CPlayer::Ammo[WT_ATTRACTIVE] = 0;
}

bool CPlayer::isActive()
{
    return CPlayer::Active;
}

void CPlayer::setActive(bool Active)
{
    CPlayer::Active = Active;

    CPlayer::Camera->setInputReceiverEnabled(Active);

}

IAnimatedMeshSceneNode* CPlayer::getWeapon()
{
    return CPlayer::Weapon;
}

void CPlayer::setAmmo(int Type, int Value)
{
    switch(Type)
    {
        case WT_NORMAL:
            CPlayer::Ammo[WT_NORMAL] = Value;
            break;

        case WT_ATTRACTIVE:
            CPlayer::Ammo[WT_ATTRACTIVE] = Value;
            break;

        case WT_EXPLOSIVE:
            CPlayer::Ammo[WT_EXPLOSIVE] = Value;
            break;
    }
    CPlayer::HUD->setAmmo(CPlayer::Ammo[CPlayer::CurrentWeapon]);
}

void CPlayer::addAmmo(int Type, int Value)
{
    switch(Type)
    {
        case WT_NORMAL:
            CPlayer::Ammo[WT_NORMAL] += Value;
            break;

        case WT_ATTRACTIVE:
            CPlayer::Ammo[WT_ATTRACTIVE] += Value;
            break;

        case WT_EXPLOSIVE:
            CPlayer::Ammo[WT_EXPLOSIVE] += Value;
            break;
    }
    CPlayer::HUD->setAmmo(CPlayer::Ammo[CPlayer::CurrentWeapon]);
}

void CPlayer::subAmmo(int Type, int Value)
{
    switch(Type)
    {
        case WT_NORMAL:
            CPlayer::Ammo[WT_NORMAL] -= Value;
            break;

        case WT_ATTRACTIVE:
            CPlayer::Ammo[WT_ATTRACTIVE] -= Value;
            break;

        case WT_EXPLOSIVE:
            CPlayer::Ammo[WT_EXPLOSIVE] -= Value;
            break;
    }
    CPlayer::HUD->setAmmo(CPlayer::Ammo[CPlayer::CurrentWeapon]);
}

void CPlayer::setHealth(int Health)
{
    CPlayer::Health = Health;
    CPlayer::HUD->setPlayerHealth(Health);
}

int CPlayer::getHealth()
{
    return CPlayer::Health;
}

ICameraSceneNode* CPlayer::getCamera()
{
    return CPlayer::Camera;
}
void CPlayer::setPhysics(CPhysics* Physics)
{
    CPlayer::Physics = Physics;
}

void CPlayer::setupPlayerPhysics()
{
    if(CC_PHYSICS)
    {
        CPlayer::CController = new CCharacterController(Physics, CPlayer::Camera, CPlayer::timer, CPlayer::smgr);
    }

    CPlayer::Weapon = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)CPlayer::smgr->getMesh("./Media/Weapons/Lazer/lazer.3ds"),CPlayer::Camera);
    CPlayer::Weapon->setMaterialFlag(EMF_LIGHTING,LIGHTING);  //
    CPlayer::Weapon->setMaterialFlag(EMF_FOG_ENABLE,true);
    CPlayer::Weapon->setPosition(vector3df(2.149f,-1.529f,3.73f));
    CPlayer::Weapon->setScale(vector3df(0.87f,0.87f,0.87f));
    CPlayer::Weapon->getMaterial(0).NormalizeNormals=true;
    CPlayer::AimNear = smgr->addCubeSceneNode(0,CPlayer::Weapon,-1,vector3df(0,0.5,50));
    CPlayer::AimFar = smgr->addCubeSceneNode(0,CPlayer::Weapon,-1,vector3df(0,0.5,200));
    CPlayer::Weapon->setID(ID_IsPlayer);
}

void CPlayer::CheckProjectilesStart(CPlayer::SProjectiles* Projectiles)
{
    Projectiles->rigidBody->activate(false);
    Projectiles->ShotChecker->setWorldTransform(Projectiles->rigidBody->getWorldTransform());
}

void CPlayer::CheckProjectilesEnd(CPlayer::SProjectiles* Projectiles)
{
    for(int i = 0; i < Projectiles->ShotChecker->getNumOverlappingObjects(); i++)
    {
        btRigidBody* pRigidBody = dynamic_cast<btRigidBody *>(Projectiles->ShotChecker->getOverlappingObject(i));
        if(pRigidBody != NULL && pRigidBody != Projectiles->rigidBody)
        {
            pRigidBody->setActivationState(DISABLE_DEACTIVATION);
            btVector3 Target = pRigidBody->getCenterOfMassPosition();
            btVector3 Position = Projectiles->rigidBody->getCenterOfMassPosition();
            if(Projectiles->type == WT_EXPLOSIVE)
            {
                pRigidBody->setLinearVelocity((btVector3(Target-Position)*250)*(2/Position.distance(Target)));
            }
            if(Projectiles->type == WT_ATTRACTIVE)
            {
                pRigidBody->setLinearVelocity(-(btVector3(Target-Position)*250)*(2/Position.distance(Target)));
            }
            pRigidBody->setActivationState(ISLAND_SLEEPING);
        }
    }
    Projectiles->rigidBody->activate(true);
}

void CPlayer::RemoveProjectile(CPlayer::SProjectiles* Projectiles)
{
    Projectiles->rigidBody->setUserPointer(NULL);
    CPlayer::Physics->getWorld()->removeRigidBody(Projectiles->rigidBody);
    CPlayer::Physics->getWorld()->removeCollisionObject(Projectiles->ShotChecker);
    Projectiles->Node->remove();
}

void CPlayer::FireProjectile()
{
    btVector3 Position = btVector3(CPlayer::AimNear->getAbsolutePosition().X, CPlayer::AimNear->getAbsolutePosition().Y, CPlayer::AimNear->getAbsolutePosition().Z);
    btVector3 Target = btVector3(CPlayer::AimFar->getAbsolutePosition().X, CPlayer::AimFar->getAbsolutePosition().Y, CPlayer::AimFar->getAbsolutePosition().Z);

    vector3df TScale = vector3df(50,50,50);

    btRigidBody* PlayerShot = CPlayer::Physics->loadConvex("./Media/Models/Primitives/icosahedron.obj", Position, TScale, 50);
    btGhostObject* Ghost = new btGhostObject();
    Ghost->setCollisionShape(new btSphereShape(300));    //Needs to be bigger than projectile
    CPlayer::Physics->getWorld()->addCollisionObject(Ghost);
    Ghost->setCollisionFlags(CF_NO_CONTACT_RESPONSE);
    PlayerShot->setLinearVelocity(btVector3(Target-Position)*shotForce*0.1f);

    ITexture* texture;

    switch(CPlayer::CurrentWeapon)
    {
        case WT_NORMAL:
            texture = CPlayer::driver->getTexture("./Media/Models/Primitives/Normal.jpg");
            break;

       case WT_ATTRACTIVE:
            texture = CPlayer::driver->getTexture("./Media/Models/Primitives/Attractive.jpg");
            break;

        case WT_EXPLOSIVE:
            texture = CPlayer::driver->getTexture("./Media/Models/Primitives/Explosive.jpg");
            break;
    }

    SProjectiles* Proj = new SProjectiles;
    Proj->ShotChecker = Ghost;
    Proj->type = CPlayer::CurrentWeapon;
    Proj->rigidBody = PlayerShot;
    Proj->oldPos = static_cast<ISceneNode *>(PlayerShot->getUserPointer())->getPosition();
    Proj->newPos = static_cast<ISceneNode *>(PlayerShot->getUserPointer())->getPosition();
    Proj->Node = static_cast<ISceneNode *>(PlayerShot->getUserPointer());
    Proj->used = false;
    Proj->Kill = false;
    CPlayer::Projectiles.push_back(Proj);

    Proj->Node->setMaterialTexture(0, texture);
    Proj->Node->setMaterialFlag(EMF_LIGHTING, LIGHTING);
}

CEventReceiver* CPlayer::getReceiver()
{
    return CPlayer::receiver;
}

void CPlayer::Update()
{
    if(CPlayer::Active)
    {
        if(CC_PHYSICS)
        {
            bool Left, Right, Forwards, Backwards, Jump;
            Left = Right = Forwards = Backwards = Jump = false;

            if(CPlayer::receiver->IsKeyDown(KEY_KEY_W))
                Forwards = true;
            if(CPlayer::receiver->IsKeyDown(KEY_KEY_S))
                Backwards = true;
            if(CPlayer::receiver->IsKeyDown(KEY_KEY_A))
                Left = true;
            if(CPlayer::receiver->IsKeyDown(KEY_KEY_D))
                Right = true;
            if(CPlayer::receiver->IsKeyDown(KEY_SPACE))
                Jump = true;

            if(CPlayer::receiver->IsKeyDown(KEY_SHIFT))
                if(CPlayer::CController->getSpeed() != CS_RUN)
                    CPlayer::CController->setSpeed(CS_RUN);

            if(CPlayer::receiver->IsKeyDown(KEY_CONTROL))
                if(CPlayer::CController->getSpeed() != CS_CROUCH)
                    CPlayer::CController->setSpeed(CS_CROUCH);

            if(!CPlayer::receiver->IsKeyDown(KEY_CONTROL) && !CPlayer::receiver->IsKeyDown(KEY_SHIFT))
                if(CPlayer::CController->getSpeed() != CS_NORMAL)
                    CPlayer::CController->setSpeed(CS_NORMAL);

            CPlayer::CController->Update(Left, Right, Forwards, Backwards, Jump);
        }

        CPlayer::HUD->updateDebug(CPlayer::Camera->getPosition());
        if(CPlayer::receiver->GetMouseState().LeftButtonDown && CPlayer::shoot==false)
        {
            if(CPlayer::FiringMode == FM_SINGLE)
                if(CPlayer::Projectiles.getSize() < 1)
                {
                    CPlayer::shoot = true;
                    CPlayer::HUD->setFireForce(shotForce);
                }
            if(CPlayer::FiringMode == FM_TRIPLE)
                if(CPlayer::Projectiles.getSize() < 3)
                {
                    CPlayer::shoot = true;
                    CPlayer::HUD->setFireForce(shotForce);
                }
            if(CPlayer::FiringMode == FM_LIMITED)
                {
                    CPlayer::shoot = true;
                    CPlayer::HUD->setFireForce(shotForce);
                }
        }

        if(CPlayer::receiver->GetMouseState().LeftButtonDown && CPlayer::shoot==true)
            if(CPlayer::shotForce < 100)
            {
                CPlayer::shotForce += 1.5f;
                CPlayer::HUD->setFireForce(shotForce);
            }

        if(!CPlayer::receiver->GetMouseState().RightButtonDown && CPlayer::checkEffect == 2)
            CPlayer::checkEffect = 0;

        if(CPlayer::checkEffect == 1)
        {
            for(list<SProjectiles *>::Iterator Iterator = CPlayer::Projectiles.begin(); Iterator != CPlayer::Projectiles.end();)
            {
                CPlayer::CheckProjectilesEnd(*Iterator);
                CPlayer::EffectsManager->CreateExplosion((*Iterator)->Node->getPosition());
                CPlayer::RemoveProjectile(*Iterator);
                Iterator = CPlayer::Projectiles.erase(Iterator);
            }
            CPlayer::checkEffect = 2;
        }

        if(CPlayer::receiver->GetMouseState().RightButtonDown && CPlayer::checkEffect == 0)
        {
            CPlayer::checkEffect = 1;
            for(list<SProjectiles *>::Iterator Iterator = CPlayer::Projectiles.begin(); Iterator != CPlayer::Projectiles.end(); ++Iterator)
                CPlayer::CheckProjectilesStart(*Iterator);
        }

        if(!CPlayer::receiver->GetMouseState().LeftButtonDown)
        {
            if(CPlayer::shoot==true)
            {
                if(CPlayer::FiringMode == FM_SINGLE)
                    if(CPlayer::Projectiles.getSize() < 1)
                        if(CPlayer::Ammo[CPlayer::CurrentWeapon] > 0)
                        {
                            CPlayer::FireProjectile();
                            --CPlayer::Ammo[CPlayer::CurrentWeapon];
                            CPlayer::HUD->setAmmo(CPlayer::Ammo[CPlayer::CurrentWeapon]);
                        }
                if(CPlayer::FiringMode == FM_TRIPLE)
                    if(CPlayer::Projectiles.getSize() < 3)
                        if(CPlayer::Ammo[CPlayer::CurrentWeapon] > 0)
                        {
                            CPlayer::FireProjectile();
                            --CPlayer::Ammo[CPlayer::CurrentWeapon];
                            CPlayer::HUD->setAmmo(CPlayer::Ammo[CPlayer::CurrentWeapon]);
                        }
                if(CPlayer::FiringMode == FM_LIMITED)
                        if(CPlayer::Ammo[CPlayer::CurrentWeapon] > 0)
                        {
                            CPlayer::FireProjectile();
                            --CPlayer::Ammo[CPlayer::CurrentWeapon];
                            CPlayer::HUD->setAmmo(CPlayer::Ammo[CPlayer::CurrentWeapon]);
                        }
            }
            CPlayer::shoot=false;
            CPlayer::shotForce = 0;
            CPlayer::HUD->setFireForce(shotForce);
        }

        if(CPlayer::receiver->MouseDelta()>0)
        {
            if(CPlayer::CurrentWeapon == WT_EXPLOSIVE)
                CPlayer::CurrentWeapon = WT_NORMAL;
            else
                ++CPlayer::CurrentWeapon;
            CPlayer::setPlayerWeapon(CPlayer::CurrentWeapon);
            CPlayer::receiver->ClearMouseDelta();
        }
        if(CPlayer::receiver->MouseDelta()<0)
        {
            if(CPlayer::CurrentWeapon == WT_NORMAL)
                CPlayer::CurrentWeapon = WT_EXPLOSIVE;
            else
                --CPlayer::CurrentWeapon;
            CPlayer::setPlayerWeapon(CPlayer::CurrentWeapon);
            CPlayer::receiver->ClearMouseDelta();
        }

        if(CPlayer::Health>100)
            CPlayer::Health=100;
        if(CPlayer::Health>0)
            CPlayer::HUD->setPlayerHealth(CPlayer::Health);
        else
            CPlayer::HUD->setPlayerHealth(0);

        if(!CPlayer::receiver->IsKeyDown(KEY_KEY_Q) && CPlayer::ChangedFireMode == true)
            CPlayer::ChangedFireMode = false;

        if(CPlayer::receiver->IsKeyDown(KEY_KEY_Q) && CPlayer::ChangedFireMode == false)
        {
            CPlayer::ChangedFireMode = true;
            if(CPlayer::FiringMode == FM_LIMITED)
                CPlayer::FiringMode = FM_SINGLE;
            else
                ++CPlayer::FiringMode;
            CPlayer::HUD->setFiringMode(CPlayer::FiringMode);
        }
    }
};

void CPlayer::setPlayerWeapon(int type)
{
    CPlayer::HUD->setWeaponType(type);
    CPlayer::HUD->setAmmo(CPlayer::Ammo[CPlayer::CurrentWeapon]);
}

void CPlayer::setPlayerHUD(CHUD* HUD)
{
    CPlayer::HUD = HUD;
}

int CPlayer::getCurrentWeapon()
{
    return CPlayer::CurrentWeapon;
}

void CPlayer::remove()
{
    CPlayer::AimNear->remove();
    CPlayer::AimFar->remove();
    CPlayer::Weapon->remove();

    for(list<SProjectiles *>::Iterator Iterator = CPlayer::Projectiles.begin(); Iterator != CPlayer::Projectiles.end();)
        CPlayer::RemoveProjectile((*Iterator));

    CPlayer::Camera->remove();

    if(DEBUG_CONSOLE)
        cout<<"Removed Player"<<endl;
}

////
////
////
////
CCharacterController::CCharacterController(CPhysics* Physics, ICameraSceneNode* Camera, ITimer* Timer, ISceneManager* smgr)
{
    CCharacterController::Timer = Timer;
    CCharacterController::smgr = smgr;
    CCharacterController::Physics = Physics;
    CCharacterController::Camera = Camera;
    CCharacterController::Speed = CS_NORMAL;
    CCharacterController::isJumping = false;
    CCharacterController::JumpSpeed = 5.0f;

    CCharacterController::Ghost = new btPairCachingGhostObject();
    btConvexShape* characterShape = new btCapsuleShape(50,70); //Size of character
    btTransform trans;
    trans.setIdentity();
    vector3df Pos = Camera->getPosition();
    trans.setOrigin(btVector3(Pos.X, Pos.Y, Pos.Z));
    CCharacterController::Ghost->setWorldTransform(trans);
    CCharacterController::Ghost->setCollisionShape(characterShape);
    btScalar stepHeight = 2;
    int upAxis = 1;
    CCharacterController::Character = new btKinematicCharacterController (CCharacterController::Ghost,characterShape,stepHeight, upAxis);
    CCharacterController::Physics->getWorld()->addCollisionObject(CCharacterController::Ghost,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
    CCharacterController::Physics->getWorld()->addCharacter(CCharacterController::Character);

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(Pos.X, Pos.Y, Pos.Z));
    btDefaultMotionState* MotionState = new btDefaultMotionState(Transform);
    btVector3 LocalInertia;
    characterShape->calculateLocalInertia(0.01f, LocalInertia);
    CCharacterController::JumpBody = new btRigidBody(0.01f, MotionState, characterShape, LocalInertia);
    CCharacterController::JumpBody->setAngularFactor(0);
    CCharacterController::Physics->getWorld()->addRigidBody(CCharacterController::JumpBody);
    CCharacterController::JumpBody->getBroadphaseProxy()->m_collisionFilterGroup = btBroadphaseProxy::DebrisFilter;
}

void CCharacterController::Update(bool Left, bool Right, bool Forwards, bool Backwards, bool Jump)
{
    float gPlayerForwardBackward = 0.0f;
    float gPlayerSideways = 0.0f;

    if(!CCharacterController::isJumping)
        CCharacterController::JumpBody->setCenterOfMassTransform(CCharacterController::Ghost->getWorldTransform());

    if(Forwards)
        gPlayerForwardBackward = CCharacterController::Speed;

    if(Backwards)
        gPlayerForwardBackward = -CCharacterController::Speed;

    if(Left)
        gPlayerSideways = -CCharacterController::Speed;

    if(Right)
        gPlayerSideways = CCharacterController::Speed;

    if(CCharacterController::isJumping)
    {
        ////
        if(true)
        {
            const btVector3& Pos1 = CCharacterController::JumpBody->getWorldTransform().getOrigin();
            const btVector3& Pos2 = CCharacterController::Ghost->getWorldTransform().getOrigin();
            const btVector3& newPos = btVector3(Pos2[0], Pos1[1], Pos2[2]);
            CCharacterController::Ghost->getWorldTransform().setOrigin(newPos);
            CCharacterController::JumpBody->getWorldTransform().setOrigin(newPos);

            ///////// TEMPROARY FIX
            //CCharacterController::isJumping = false;
            /////////
        }
        else
            CCharacterController::isJumping = false;

        if(CCharacterController::Timer->getTime()-CCharacterController::jumpStart > 1000)
            CCharacterController::isJumping = false;
    }

    if(Jump)
        if(!CCharacterController::isJumping)
        {
            CCharacterController::JumpBody->applyCentralImpulse(btVector3(0,CCharacterController::JumpSpeed,0));
            CCharacterController::isJumping = true;
            CCharacterController::jumpStart = CCharacterController::Timer->getTime();
        }

    btVector3 walkDir(0, 0, 0);
    if (gPlayerForwardBackward != 0)
    {
        vector3df rot = CCharacterController::Camera->getRotation();
        rot.X = 0;
        matrix4 mat;
        mat.setRotationDegrees(rot);
        btVector3 forwardDir(mat[8],mat[9],mat[10]);
        walkDir += forwardDir*gPlayerForwardBackward;
    }
    if (gPlayerSideways != 0)
    {
        vector3df rot = CCharacterController::Camera->getRotation();
        rot.X = 0;
        matrix4 mat;
        mat.setRotationDegrees(rot);
        btVector3 sideWays(mat[0],mat[1],mat[2]);
        walkDir += sideWays*gPlayerSideways;
    }
    CCharacterController::Character->setWalkDirection(walkDir);

    btVector3 c = CCharacterController::Character->getGhostObject()->getWorldTransform().getOrigin();
    vector3df pos (c.getX(),c.getY()+30,c.getZ()); //Character Height = 30
    CCharacterController::Camera->setPosition(pos);
}

int CCharacterController::getHeight()
{
    return CCharacterController::Height;
}

int CCharacterController::getSpeed()
{
    return CCharacterController::Speed;
}

void CCharacterController::setHeight(int Height)
{
    CCharacterController::Height = Height;
}

void CCharacterController::setSpeed(int Speed)
{
    if(Speed != CS_NORMAL && Speed != CS_CROUCH && Speed != CS_RUN)
        Speed = CS_NORMAL;

    CCharacterController::Speed = Speed;
}
