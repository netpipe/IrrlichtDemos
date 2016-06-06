#include <irrlicht.h>
#include <ode/ode.h>


#define GRAD_PI 57.29577951f
#define GRAD_PI2 0.174532925f

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;

bool bPaused = true;

dSpaceID space;
dWorldID world;
dJointGroupID contacts;
const int MAX_CONTACTS = 4;

vector3df vInitPos = vector3df(8, 4, 8);

float fHeadScale = 0.15f;
vector3df vBodyScale = vector3df(0.5, 0.7, 0.1);
vector3df vLwrArmScale = vector3df(0.3, 0.1, 0.1);
vector3df vUprArmScale = vector3df(0.3, 0.1, 0.1);
vector3df vLwrLegScale = vector3df(0.1, 0.5, 0.1);
vector3df vUprLegScale = vector3df(0.1, 0.5, 0.1);

dBodyID bHead;
dGeomID gHead;
dBodyID bBody;
dGeomID gBody;
dBodyID bLeftLwrArm;
dGeomID gLeftLwrArm;
dBodyID bLeftUprArm;
dGeomID gLeftUprArm;
dBodyID bLeftLwrLeg;
dGeomID gLeftLwrLeg;
dBodyID bLeftUprLeg;
dGeomID gLeftUprLeg;
dBodyID bRightLwrArm;
dGeomID gRightLwrArm;
dBodyID bRightUprArm;
dGeomID gRightUprArm;
dBodyID bRightLwrLeg;
dGeomID gRightLwrLeg;
dBodyID bRightUprLeg;
dGeomID gRightUprLeg;

void addODEBox(dBodyID &bBox, dGeomID &gBox, vector3df vPos, vector3df vBoxSize)
{
    bBox = dBodyCreate(world);
    dBodySetPosition (bBox, vPos.X, vPos.Y, vPos.Z);
    gBox = dCreateBox (space, vBoxSize.X, vBoxSize.Y, vBoxSize.Z);
    dGeomSetBody (gBox, bBox);
    dMass mBox;
    dMassSetBox (&mBox, 1.0, vBoxSize.X, vBoxSize.Y, vBoxSize.Z);
    dBodySetMass (bBox, &mBox);

}

void initPhys()
{
dInitODE();
    space = dSimpleSpaceCreate(0);
    contacts = dJointGroupCreate (0);

    world = dWorldCreate();
    dWorldSetGravity(world, 0, -9.81, 0);
    dWorldSetCFM(world, 1e-5);
    dWorldSetAutoDisableFlag(world, 1); //do auto disable
    dWorldSetContactMaxCorrectingVel (world, 0.1);
    dWorldSetContactSurfaceLayer (world, 0.001);

    dCreatePlane(space, 0, 1, 0, 0);
    dGeomID xwall = dCreateBox(space, 20, 20, 20);
    dGeomID zwall = dCreateBox(space, 20, 20, 20);
    dGeomSetPosition(xwall, 20, -1, 0);
    dGeomSetPosition(zwall, 0, -1, 20);

    bHead = dBodyCreate(world);
    dBodySetPosition (bHead, vInitPos.X, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y + fHeadScale, vInitPos.Z);
    gHead = dCreateSphere(space, fHeadScale);
    dGeomSetBody (gHead, bHead);
    dMass mHead;
    dMassSetSphere (&mHead, 1.0, fHeadScale);
    dBodySetMass (bHead, &mHead);

    addODEBox(bBody, gBody, vector3df(vInitPos.X, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y/2, vInitPos.Z), vBodyScale);
    addODEBox(bLeftUprArm, gLeftUprArm, vector3df(vInitPos.X + vBodyScale.X/2 + vUprArmScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z), vUprArmScale);
    addODEBox(bRightUprArm, gRightUprArm, vector3df(vInitPos.X - vBodyScale.X/2 - vUprArmScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z), vUprArmScale);
    addODEBox(bLeftLwrArm, gLeftLwrArm, vector3df(vInitPos.X + vBodyScale.X/2 + vUprArmScale.X + vLwrArmScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z), vLwrArmScale);
    addODEBox(bRightLwrArm, gRightLwrArm, vector3df(vInitPos.X - vBodyScale.X/2 - vUprArmScale.X - vLwrArmScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z), vLwrArmScale);
    addODEBox(bLeftLwrLeg, gLeftLwrLeg, vector3df(vInitPos.X + vBodyScale.X/2 - vLwrLegScale.X/2, vInitPos.Y + vLwrLegScale.Y/2, vInitPos.Z), vLwrLegScale);
    addODEBox(bRightLwrLeg, gRightLwrLeg, vector3df(vInitPos.X - vBodyScale.X/2 + vLwrLegScale.X/2, vInitPos.Y + vLwrLegScale.Y/2, vInitPos.Z), vLwrLegScale);
    addODEBox(bLeftUprLeg, gLeftUprLeg, vector3df(vInitPos.X + vBodyScale.X/2 - vUprLegScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y/2, vInitPos.Z), vUprLegScale);
    addODEBox(bRightUprLeg, gRightUprLeg, vector3df(vInitPos.X - vBodyScale.X/2 + vUprLegScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y/2, vInitPos.Z), vUprLegScale);

    dJointID neck = dJointCreateBall(world, 0);
    dJointAttach(neck, bHead, bBody);
    dJointSetBallAnchor(neck, vInitPos.X, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y, vInitPos.Z);

    dJointID leftshoulder = dJointCreateBall(world, 0);
    dJointAttach(leftshoulder, bBody, bLeftUprArm);
    dJointSetBallAnchor(leftshoulder, vInitPos.X + vBodyScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z);

    dJointID rightshoulder = dJointCreateBall(world, 0);
    dJointAttach(rightshoulder, bBody, bRightUprArm);
    dJointSetBallAnchor(rightshoulder, vInitPos.X - vBodyScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z);

    dJointID leftelbow = dJointCreateBall(world, 0);
    dJointAttach(leftelbow, bLeftUprArm, bLeftLwrArm);
    dJointSetBallAnchor(leftelbow, vInitPos.X + vBodyScale.X/2 + vUprArmScale.X, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z);

    dJointID rightelbow = dJointCreateBall(world, 0);
    dJointAttach(rightelbow, bRightUprArm, bRightLwrArm);
    dJointSetBallAnchor(rightelbow, vInitPos.X - vBodyScale.X/2 - vUprArmScale.X, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y + vBodyScale.Y - vUprArmScale.Y/2, vInitPos.Z);

    dJointID lefthip = dJointCreateBall(world, 0);
    dJointAttach(lefthip, bBody, bLeftUprLeg);
    dJointSetBallAnchor(lefthip, vInitPos.X + vBodyScale.X/2 - vUprLegScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y, vInitPos.Z);

    dJointID righthip = dJointCreateBall(world, 0);
    dJointAttach(righthip, bBody, bRightUprLeg);
    dJointSetBallAnchor(righthip, vInitPos.X - vBodyScale.X/2 + vUprLegScale.X/2, vInitPos.Y + vLwrLegScale.Y + vUprLegScale.Y, vInitPos.Z);

    dJointID leftknee = dJointCreateBall(world, 0);
    dJointAttach(leftknee, bLeftUprLeg, bLeftLwrLeg);
    dJointSetBallAnchor(leftknee, vInitPos.X + vBodyScale.X/2 - vLwrLegScale.X/2, vInitPos.Y + vLwrLegScale.Y, vInitPos.Z);

    dJointID rightknee = dJointCreateBall(world, 0);
    dJointAttach(rightknee, bRightUprLeg, bRightLwrLeg);
    dJointSetBallAnchor(rightknee, vInitPos.X - vBodyScale.X/2 + vLwrLegScale.X/2, vInitPos.Y + vLwrLegScale.Y, vInitPos.Z);

    dBodySetAngularVel(bBody, rand()%10,rand()%10,rand()%10);

}

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{

		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch(event.KeyInput.Key)
			{
			case KEY_KEY_S:
				{
					bPaused = !bPaused;
				}
				return true;
            case KEY_KEY_R:
                {dCloseODE();
                    initPhys();
                }
                return true;
            default: break;

			}
		}

		return false;
	}
};

MyEventReceiver rec;

void initGfx()
{

    device = createDevice(EDT_OPENGL, dimension2d<s32>(1024, 768), 32, false, false, false, &rec);
    device->getCursorControl()->setVisible(false);

    driver = device->getVideoDriver();
    driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

    smgr = device->getSceneManager();

    smgr->addLightSceneNode(0,vector3df(5,5,5), SColor(255,255,255,255), 200.0f, -1);
    ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 5.0f);
    cam->setPosition(vector3df(6,5,2));
    cam->setTarget(vector3df(10,-2,10));

   // IAnimatedMesh *room = smgr->getMesh("room.3ds");
   // IAnimatedMeshSceneNode *snRoom = smgr->addAnimatedMeshSceneNode(room);
   // snRoom->setMaterialFlag(EMF_LIGHTING, false);
   // snRoom->setRotation(vector3df(-90, 0 ,0));

}

void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
    // exit without doing anything if the two bodies are connected by a joint
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

    dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
    int ii;
    for (ii=0; ii<MAX_CONTACTS; ii++)
    {
        contact[ii].surface.mode = dContactBounce | dContactSoftCFM;
        contact[ii].surface.mu = dInfinity;
        contact[ii].surface.mu2 = 0;
        contact[ii].surface.bounce = 0.1;
        contact[ii].surface.bounce_vel = 0.1;
        contact[ii].surface.soft_cfm = 0.01;
    }
    if (int numc = dCollide (o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
    {
        dMatrix3 RI;
        dRSetIdentity(RI);
        for (ii=0; ii<numc; ii++)
        {
            dJointID c = dJointCreateContact(world, contacts, contact+ii);
            dJointAttach(c, b1, b2);
        }
    }
}

void quaternionToEuler(const dQuaternion quaternion, vector3df &euler)
{
    dReal w,x,y,z;
    w=quaternion[0];
    x=quaternion[1];
    y=quaternion[2];
    z=quaternion[3];
    double sqw = w*w;
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;

    euler.Z = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))*GRAD_PI);
    euler.X = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))*GRAD_PI);
    euler.Y = (irr::f32) (asin(-2.0 * (x*z - y*w))*GRAD_PI);

}

void cleanUp()
{
    device->drop();

    dJointGroupDestroy (contacts);
    dSpaceDestroy (space);
    dWorldDestroy (world);
dCloseODE();
}

ISceneNode *addBox(vector3df scale)
{
    ISceneNode *box = smgr->addCubeSceneNode(1.0f);
    box->setMaterialTexture(0, driver->getTexture("a.jpg"));
    box->setMaterialFlag(EMF_LIGHTING, false);
    box->setScale(scale);
    return box;
}

void renderBody(dGeomID &gBox, dBodyID &bBox, ISceneNode *&box)
{
    const dReal *pos;
    dQuaternion result;
    vector3df rot;

    pos = dBodyGetPosition(bBox);
    dGeomGetQuaternion(gBox, result);
    quaternionToEuler(result, rot);
    box->setPosition(vector3df(pos[0], pos[1], pos[2]));
    box->setRotation(rot);
}

int main()
{
    initPhys();
    initGfx();

    ISceneNode *head = smgr->addSphereSceneNode(fHeadScale);
    head->setMaterialTexture(0, driver->getTexture("a.jpg"));
    head->setMaterialFlag(EMF_LIGHTING, false);

    ISceneNode *body = addBox(vBodyScale);
    ISceneNode *leftuprarm = addBox(vUprArmScale);
    ISceneNode *rightuprarm = addBox(vUprArmScale);
    ISceneNode *leftlwrarm = addBox(vLwrArmScale);
    ISceneNode *rightlwrarm = addBox(vLwrArmScale);
    ISceneNode *leftlwrleg = addBox(vLwrLegScale);
    ISceneNode *rightlwrleg = addBox(vLwrLegScale);
    ISceneNode *leftuprleg = addBox(vUprLegScale);
    ISceneNode *rightuprleg = addBox(vUprLegScale);

    const float dt = 0.01f;

    float currentTime = 0.0f;
    float accumulator = 0.0f;

    while (device->run())
    {
        device->setWindowCaption(L"ODE test");

        float newTime = device->getTimer()->getTime() / 1000.0f; //seconds for ODE
        float deltaTime = newTime - currentTime;
        currentTime = newTime;

        if(!bPaused)
        {
            accumulator += deltaTime;
            while (accumulator>=dt)
            {
                dSpaceCollide (space, 0, &nearCallback);
                dWorldQuickStep (world, dt);
                accumulator -= dt;
                dJointGroupEmpty (contacts);
            }
        }

        renderBody(gHead, bHead, head);
        renderBody(gBody, bBody, body);
        renderBody(gLeftUprArm, bLeftUprArm, leftuprarm);
        renderBody(gRightUprArm, bRightUprArm, rightuprarm);
        renderBody(gLeftLwrArm, bLeftLwrArm, leftlwrarm);
        renderBody(gRightLwrArm, bRightLwrArm, rightlwrarm);
        renderBody(gLeftLwrLeg, bLeftLwrLeg, leftlwrleg);
        renderBody(gRightLwrLeg, bRightLwrLeg, rightlwrleg);
        renderBody(gLeftUprLeg, bLeftUprLeg, leftuprleg);
        renderBody(gRightUprLeg, bRightUprLeg, rightuprleg);

        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        smgr->drawAll();

        driver->endScene();

    }

    cleanUp();

    return 0;
}
