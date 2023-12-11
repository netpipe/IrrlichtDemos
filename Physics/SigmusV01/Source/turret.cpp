#include "turret.h"

CTurret::CTurret(const stringw Name, float TurnRate, IAnimatedMeshSceneNode* Node, ISceneManager *smgr, IVideoDriver *driver, IGUIEnvironment* gui, ITimer* Timer, CPlayer* Player, CPhysics* Physics, vector3df Muzzle1, vector3df Muzzle2)
{
	CTurret::TurnRate = TurnRate;
	CTurret::Position = Node->getAbsolutePosition();
	CTurret::smgr = smgr;
	CTurret::driver = driver;
	CTurret::gui = gui;
	CTurret::Timer = Timer;
	CTurret::Name = Name;
	CTurret::Node = Node;
	CTurret::Physics = Physics;
	CTurret::Player = Player;

	CTurret::Muzzle1 = smgr->addCubeSceneNode(0,Node,-1,Muzzle1);
	CTurret::Muzzle2 = smgr->addCubeSceneNode(0,Node,-1,Muzzle2);
	CTurret::Back = smgr->addCubeSceneNode(0, Node, -1, vector3df(0,0,-10));

	CTurret::shotSize = 20;
	CTurret::shotMass = 150;
	CTurret::shotRate = 1000;
	CTurret::shotSpeed = 2000;
	CTurret::health = 100;
	CTurret::Damage = 5;

	CTurret::Active = true;
	CTurret::DrawDebug = true;
	CTurret::HUDEnabled = true;
	CTurret::useOverrideText = false;
	CTurret::OverrideText = L"Neutralized";

	CTurret::HUDIsActive = CTurret::gui->addStaticText(L" ",rect<s32>(0,0,0,0));
	CTurret::HUDIsActive->setOverrideColor(SColor(255,255,255,255));
	CTurret::HUDActiveColor = SColor(255,255,0,0);
	CTurret::HUDInactiveColor = SColor(255,255,255,255);
	CTurret::DebugColor = SColor(255,255,255,255);

	CTurret::shotTexture = CTurret::driver->getTexture("./Media/Textures/check.jpg");


    //Add Collision

	//CTurret::TurretPhysics = CTurret::Physics->CreateConvex(Node, (btVector3&)Node->getPosition(), Node->getScale(), 100);

};

void CTurret::setDamage(int Damage)
{
	CTurret::Damage = Damage;
};

void CTurret::setShotRate(u32 Milliseconds)
{
	CTurret::shotRate = Milliseconds;
};

void CTurret::setOverrideText(stringw Text)
{
	CTurret::OverrideText = Text;
};

void CTurret::setOverrideTextActive(bool Active)
{
	CTurret::useOverrideText = Active;
};

void CTurret::setHUDActive(bool Active)
{
	CTurret::HUDEnabled = Active;
};

void CTurret::setShotTexture(ITexture* Texture)
{
	CTurret::shotTexture = Texture;
};

void CTurret::setHUDColors(SColor Active, SColor Inactive)
{
	CTurret::HUDActiveColor = Active;
	CTurret::HUDInactiveColor = Inactive;
};

void CTurret::setDebugColor(SColor Color)
{
	CTurret::DebugColor = Color;
};

int CTurret::getHealth()
{
	return CTurret::health;
};

void CTurret::setHealth(int Health)
{
	CTurret::health = Health;
};

bool CTurret::checkShotLast()
{
	return CTurret::shotLast;
};

void CTurret::UpdateVisuals()
{
	if(!CTurret::useOverrideText)
		if(CTurret::checkShotLast())
		{
			stringw Text = L"Active -- Health ";
			Text += CTurret::health;
			CTurret::HUDIsActive->setOverrideColor(SColor(255,255,0,0));
			CTurret::HUDIsActive->setText(Text.c_str());
			CTurret::driver->draw2DRectangleOutline(CTurret::HUDSquare,CTurret::HUDActiveColor);
		}
		else
		{
			stringw Text = L"Inactive -- Health ";
			Text += CTurret::health;
			CTurret::HUDIsActive->setOverrideColor(SColor(255,255,255,255));
			CTurret::HUDIsActive->setText(Text.c_str());
			CTurret::driver->draw2DRectangleOutline(CTurret::HUDSquare,CTurret::HUDInactiveColor);
		}
	else
	{
		CTurret::HUDIsActive->setOverrideColor(SColor(255,0,255,0));
		CTurret::HUDIsActive->setText(CTurret::OverrideText.c_str());
	}
	if(CTurret::DrawDebug)
		CTurret::driver->draw3DLine(CTurret::ray.start,CTurret::ray.end,CTurret::DebugColor);
};

void CTurret::UpdateTurret()
{
	CTurret::shotTimeNow = CTurret::Timer->getTime();
	CTurret::shotLast = false;
	CTurret::Position = CTurret::Node->getPosition();

	//HUD
	aabbox3d<f32> HUDbox = CTurret::getNode()->getTransformedBoundingBox();
    vector3df Position1 = HUDbox.getCenter();
	vector3df Position2 = HUDbox.getCenter();
	Position1 = vector3df(Position1.X-(HUDbox.getExtent().X/2),Position1.Y-(HUDbox.getExtent().Y/2),Position1.Z-(HUDbox.getExtent().Z/2));
	Position2 = vector3df(Position2.X+(HUDbox.getExtent().X/2),Position2.Y+(HUDbox.getExtent().Y/2),Position2.Z+(HUDbox.getExtent().Z/2));
	vector2d<s32> _hpos1 = CTurret::smgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(Position1);
	vector2d<s32> _hpos2 = CTurret::smgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(Position2);
	recti hpos1;
	hpos1.UpperLeftCorner.X = _hpos1.X;
	hpos1.UpperLeftCorner.Y = _hpos1.Y;
	hpos1.LowerRightCorner.X = _hpos2.X;
	hpos1.LowerRightCorner.Y = _hpos2.Y;
	CTurret::HUDSquare = hpos1;
	CTurret::HUDIsActive->setRelativePosition(rect<s32>(_hpos1.X,_hpos1.Y,_hpos1.X+200,_hpos1.Y+15));

	//Tracking Bullets
	if(CTurret::Projectiles.getSize() != 0)
		for(list<CTurret::SProjectiles *>::Iterator Iterator = CTurret::Projectiles.begin(); Iterator != CTurret::Projectiles.end(); ++Iterator)
		{
			if(CTurret::CheckProjectiles(*Iterator))
				CTurret::Player->setHealth(CTurret::Player->getHealth()-CTurret::Damage);
		};

	line3d<f32> ray;

	ray.start = CTurret::Position;
	ray.end = ray.start + (CTurret::Player->getWeapon()->getAbsolutePosition()-ray.start).normalize() * 1000.0f;
	CTurret::ray = ray;

	ISceneNode* selectedSceneNode = CTurret::smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(ray,ID_IsPlayer);
	if(selectedSceneNode)
            if(selectedSceneNode->getID() == ID_IsPlayer)
            {
                if(CTurret::Active)
                {
                    long double x1, z1, x2, z2;
                    long double dx, dz;
                    float theta;
                    float ResponseSpeed = CTurret::TurnRate;

                    x1 = CTurret::Node->getPosition().X;
                    z1 = CTurret::Node->getPosition().Z;

                    x2 = CTurret::Player->getWeapon()->getAbsolutePosition().X;
                    z2 = CTurret::Player->getWeapon()->getAbsolutePosition().Z;

                    dx = x2-x1;
                    dz = z2-z1;

                    theta = atan2(dx,dz)*180/PI;

                    if(theta<0)
                        theta = 360+theta;

                    //btVector3 EulerRotation;
                    //QuaternionToEuler(CTurret::TurretPhysics->getOrientation(), EulerRotation);
                    vector3df Rotation = CTurret::Node->getRotation();

                    float rotValue=0;

                    if(Rotation.Y != theta)
                    {
                        if(Rotation.Y<(theta-1))
                            rotValue=ResponseSpeed;
                        if(Rotation.Y>(theta+1))
                            rotValue=-ResponseSpeed;
                    }

                    //if(rotValue!=0)
                    //	if((theta-Rotation.Y)>180)
                    //		rotValue = -rotValue;


                    Rotation = vector3df(Rotation.X,Rotation.Y+rotValue,Rotation.Z);
                    CTurret::Node->setRotation(Rotation);

        //			btTransform transform;
        //			transform.setIdentity();
        //			transform.setOrigin(CTurret::TurretPhysics->getWorldTransform().getOrigin());
        //
        //			btQuaternion quat;
        //			quat.setEuler(Rotation.Y,Rotation.X,Rotation.Z);
        //
        //			transform.setRotation(quat);
        //
        //			CTurret::TurretPhysics->setCenterOfMassTransform(transform);


                    if(rotValue==0 & (CTurret::shotTimeNow-CTurret::shotTimeLast)>CTurret::shotRate)
                    {
                        btVector3 Position = btVector3(CTurret::Node->getAbsolutePosition().X, CTurret::Node->getAbsolutePosition().Y, CTurret::Node->getAbsolutePosition().Z);
                        btVector3 Target = btVector3(CTurret::Player->getWeapon()->getAbsolutePosition().X, CTurret::Player->getWeapon()->getAbsolutePosition().Y, CTurret::Player->getWeapon()->getAbsolutePosition().Z);
                        ISceneNode* Node = CTurret::smgr->addSphereSceneNode(CTurret::shotSize, 32);
                        Node->setMaterialFlag(EMF_LIGHTING, LIGHTING);
                        Node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
                        Node->setMaterialTexture(0, CTurret::shotTexture);
                        Node->updateAbsolutePosition();

                        btTransform Transform;
                        Transform.setIdentity();
                        Transform.setOrigin(Position);

                        btDefaultMotionState* MotionState = new btDefaultMotionState(Transform);

                        btCollisionShape* Shape = new btSphereShape(CTurret::shotSize);

                        btVector3 LocalInertia;
                        Shape->calculateLocalInertia(150, LocalInertia);

                        btRigidBody* TurretShot = new btRigidBody(CTurret::shotMass, MotionState, Shape, LocalInertia);

                        TurretShot->setUserPointer((void *)(Node));

                        CTurret::Physics->getWorld()->addRigidBody(TurretShot);
                        CTurret::Physics->Objects.push_back(TurretShot);

                        vector3df _FireD = CTurret::Muzzle1->getAbsolutePosition()-CTurret::Muzzle2->getAbsolutePosition();
                        _FireD.normalize();
                        btVector3 FireDirection = btVector3(_FireD.X,_FireD.Y,_FireD.Z);

                        TurretShot->setLinearVelocity(btVector3(-FireDirection)*CTurret::shotSpeed);
                        CTurret::SProjectiles* Proj = new CTurret::SProjectiles;
                        Proj->rigidBody = TurretShot;
                        Proj->oldPos = Node->getPosition();
                        Proj->newPos = Node->getPosition();
                        Proj->Node = Node;
                        CTurret::Projectiles.push_back(Proj);
                        CTurret::shotTimeLast = CTurret::Timer->getTime();
                    }
                    CTurret::shotLast = true;
                }
            }
};

void CTurret::setShotSpeed(int Speed)
{
	CTurret::shotSpeed = Speed;
};

void CTurret::setProjectileSize(float Size)
{
	CTurret::shotSize = Size;
};

void CTurret::setProjectileMass(float Mass)
{
	CTurret::shotMass = Mass;
};

bool CTurret::CheckProjectiles(CTurret::SProjectiles* Projectil) //Change
{
	ISceneNode* Node = static_cast<ISceneNode *>(Projectil->rigidBody->getUserPointer());
	bool Hit = CTurret::Player->getWeapon()->getTransformedBoundingBox().intersectsWithBox(Projectil->Node->getTransformedBoundingBox());

	return Hit;
};

line3d<f32> CTurret::getDebugRay()
{
	return CTurret::ray;
};

bool CTurret::isActive()
{
	return CTurret::Active;
};

int CTurret::getTurnRate()
{
	return CTurret::TurnRate;
};

bool CTurret::isDebugVisual()
{
	return CTurret::DrawDebug;
};

vector3df CTurret::getPosition()
{
	return CTurret::Position;
};

ISceneNode* CTurret::getNode()
{
	return CTurret::Node;
};

void CTurret::setActive(bool active)
{
	CTurret::Active = active;
};

void CTurret::setTurnRate(int TurnRate)
{
	CTurret::TurnRate = TurnRate;
};

void CTurret::setDebugVisual(bool active)
{
	CTurret::DrawDebug = active;
};

void CTurret::setPosition(irr::core::vector3df Position)
{
	CTurret::Position = Position;
};

void CTurret::RemoveProjectile(CTurret::SProjectiles* Projectiles)
{
    Projectiles->rigidBody->setUserPointer(NULL);
    CTurret::Physics->getWorld()->removeRigidBody(Projectiles->rigidBody);
    Projectiles->Node->remove();
}

void CTurret::remove()
{
    CTurret::Back->remove();
    CTurret::Muzzle1->remove();
    CTurret::Muzzle2->remove();
    CTurret::Node->remove();
    CTurret::HUDIsActive->remove();

    for(list<SProjectiles *>::Iterator Iterator = CTurret::Projectiles.begin(); Iterator != CTurret::Projectiles.end();)
        CTurret::RemoveProjectile((*Iterator));

    if(DEBUG_CONSOLE)
        cout<<"Removed Turret"<<endl;
}
