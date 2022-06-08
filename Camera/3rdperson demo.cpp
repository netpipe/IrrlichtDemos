#include <irrlicht.h>


#define EPSILON 0.001f

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


enum EPLAYER_ACTION
{
EPA_MOVE_FORWARD,
EPA_MOVE_BACKWARD,
EPA_MOVE_LEFT,
EPA_MOVE_RIGHT,

EPA_COUNT
};

//! Defines player states
enum EPLAYER_STATE
{
EPS_RUNNING,
EPS_STANDING
};

// Global Irrlicht variables
irr::IrrlichtDevice *irrBase=0;

class CSceneNodeAnimator3PCamera : public irr::scene::ISceneNodeAnimator,public irr::IEventReceiver
{

public:

//! Constructor
//! player: The object to follow
//! initDist: The initial distance from the player
//! initAngleY: The initial horizontal rotatation
//! initAngleZ: The initial vertical rotation
//! targetOffset: The offset from the object's center at which the camera looks
//! minDist/maxDist: Distance bounding
//! minAngle/maxAngle: Rotation bounding. -89 ==> looking up at player, 89 ==> looking down
//! boxSize: The size of the box in which mouse movements do not result in camera movements
//! rotationSpeed/zoomSpeed: Speed of movement, in terms of units/millisecond

CSceneNodeAnimator3PCamera(
irr::scene::ISceneManager *manager,
irr::gui::ICursorControl *cursor,
irr::scene::ISceneNode *player,
irr::f32 initDist = 50.0f,
irr::f32 initAngleY = 180.0f,
irr::f32 initAngleZ = 10.0f,
irr::core::vector3df targetOffset = irr::core::vector3df(0,0,0),
irr::f32 minDist = 20.0f,
irr::f32 maxDist = 200.0f,
irr::f32 minAngle = -45.0f,
irr::f32 maxAngle = 89.0f,
irr::f32 boxSize = 0.0f,
irr::f32 rotationSpeed = 60.0f,
irr::f32 zoomSpeed = 2.0f);

//! Destructor
virtual ~CSceneNodeAnimator3PCamera(void);

//! animates the scene node
virtual void animateNode(irr::scene::ISceneNode *node, irr::u32 timeMs);

//! Process an input event
virtual bool OnEvent(irr::SEvent event);

//! Get/set active status
bool isActive();
void setActive(bool status);

//! Get/set box size
irr::f32 getBoxSize();
void setBoxSize(irr::f32 newSize);

//! Map zoom in/zoom out buttons
void mapZoomIn(irr::EKEY_CODE newCode);
void mapZoomOut(irr::EKEY_CODE newCode);

//! Access the camera's current orientation
irr::f32 getOrientation();


private:

int Active;//bool Active;

// current states
irr::f32 Dist;
irr::f32 AngleY;
irr::f32 AngleZ;

// boundaries
irr::core::vector3df TargetOffset;
irr::f32 MinDist;
irr::f32 MaxDist;
irr::f32 MinAngle;
irr::f32 MaxAngle;
irr::f32 BoxSize;

// Motion
irr::f32 RotationSpeed;
irr::f32 ZoomSpeed;

// Zooming keys
irr::EKEY_CODE ZoomInKey;
irr::EKEY_CODE ZoomOutKey;

bool ZoomIn;
bool ZoomOut;

//! Node to follow
irr::scene::ISceneNode *Player;

irr::scene::ISceneManager *Manager;
irr::gui::ICursorControl *Cursor;

//! Puts the cursor back in the box
void updateCursorPosition();
};


CSceneNodeAnimator3PCamera::CSceneNodeAnimator3PCamera(
irr::scene::ISceneManager *manager,
irr::gui::ICursorControl *cursor,
irr::scene::ISceneNode *player,
irr::f32 initDist,irr::f32 initAngleY,
irr::f32 initAngleZ,
irr::core::vector3df targetOffset,
irr::f32 minDist,
irr::f32 maxDist,
irr::f32 minAngle,
irr::f32 maxAngle,
irr::f32 boxSize,
irr::f32 rotationSpeed,
irr::f32 zoomSpeed)
: Active(true), Manager(manager), Cursor(cursor), Player(player),
Dist(initDist),AngleY(initAngleY),AngleZ(initAngleZ),
TargetOffset(targetOffset),
MinDist(minDist), MaxDist(maxDist), MinAngle(-maxAngle), MaxAngle(-minAngle),
BoxSize(boxSize), RotationSpeed(rotationSpeed), ZoomSpeed(zoomSpeed)
{
ZoomIn = false;
ZoomOut = false;

ZoomInKey = irr::KEY_ADD;
ZoomOutKey = irr::KEY_SUBTRACT;

Cursor->setPosition(0.5f,0.5f);

// Ensure Distance is bounded correctly
if(Dist < MinDist) Dist = MinDist;
else if(Dist > MaxDist) Dist = MaxDist;

// Bound MinAngle/MaxAngle to avoid problematic areas
if(MinAngle < -89.0f) MinAngle = -89.0f;
if(MinAngle > 89.0f) MinAngle = 89.0f;
if(MaxAngle < -89.0f) MaxAngle = -89.0f;
if(MaxAngle > 89.0f) MaxAngle = 89.0f;
if(minAngle > maxAngle) MaxAngle = MinAngle+1.0f;

// Ensure Vertical Rotation Angle is bounded correctly
if(AngleZ < MinAngle) AngleZ = MinAngle;
else if(AngleZ > MaxAngle) AngleZ = MaxAngle;
}

//! Destructor
CSceneNodeAnimator3PCamera::~CSceneNodeAnimator3PCamera(void)
{
}

//! Puts the cursor back in the box
void CSceneNodeAnimator3PCamera::updateCursorPosition()
{
irr::core::position2d<irr::f32> pos = Cursor->getRelativePosition();

if(pos.X < (0.5f-BoxSize)) pos.X = (0.5f-BoxSize);
if(pos.X > (0.5f+BoxSize)) pos.X = (0.5f+BoxSize);

if(pos.Y < (0.5f-BoxSize)) pos.Y = (0.5f-BoxSize);
if(pos.Y > (0.5f+BoxSize)) pos.Y = (0.5f+BoxSize);

Cursor->setPosition(pos);
}

//! Process an input event
bool CSceneNodeAnimator3PCamera::OnEvent(irr::SEvent event)
{
if(!Active)
{
return false;
}

if(event.EventType == irr::EET_KEY_INPUT_EVENT)
{
if(event.KeyInput.Key == ZoomInKey)
{
ZoomIn = event.KeyInput.PressedDown;
return true;
}
else if(event.KeyInput.Key == ZoomOutKey)
{
ZoomOut = event.KeyInput.PressedDown;
return true;
}
}

return false;
}

//! Get/set active status
bool CSceneNodeAnimator3PCamera::isActive()
{
return Active;
}

void CSceneNodeAnimator3PCamera::setActive(bool status)
{
// reset the cursor only if we are switching back to active from inactive
if(!Active && status)
{
updateCursorPosition();
}

Active = status;
}

//! Get/set box size
irr::f32 CSceneNodeAnimator3PCamera::getBoxSize()
{
return BoxSize;
}

void CSceneNodeAnimator3PCamera::setBoxSize(irr::f32 newSize)
{
BoxSize = newSize;
updateCursorPosition();
}

//! Map zoom in/zoom out buttons
void CSceneNodeAnimator3PCamera::mapZoomIn(irr::EKEY_CODE newCode)
{
ZoomInKey = newCode;
}

void CSceneNodeAnimator3PCamera::mapZoomOut(irr::EKEY_CODE newCode)
{
ZoomOutKey = newCode;
}

//! Access the camera's current orientation
irr::f32 CSceneNodeAnimator3PCamera::getOrientation()
{
return AngleY;
}


//! animates the scene node
void CSceneNodeAnimator3PCamera::animateNode(irr::scene::ISceneNode *node, irr::u32 timeMs)
{
// make sure you don't go attaching this animator to anything other than a camera
irr::scene::ICameraSceneNode *camera = (irr::scene::ICameraSceneNode*)node;

if(Manager->getActiveCamera() != camera)
{
return;
}

if(Active)
{
// Camera is active, rotate as necessary
irr::core::position2d<irr::f32> pos = Cursor->getRelativePosition();

if(pos.X < 0.5f-BoxSize-EPSILON)
{
AngleY -= (pos.X-(0.5f-BoxSize))*RotationSpeed;//Decrement of this statement gives the correct movement to the mouse.... by smartwhiz
}

if(pos.X > 0.5f+BoxSize+EPSILON)
{
AngleY -= (pos.X-(0.5f+BoxSize))*RotationSpeed;//Decrement of this statement gives the correct movement to the mouse.... by smartwhiz
}

// So we don't get huge rotation numbers
if(AngleY > 360.0f)
{
AngleY += 360.0f;
}

if(AngleY < 0.0f)
{
AngleY -= 360.0f;
}

if(pos.Y < 0.5f-BoxSize-EPSILON)
{
AngleZ -= (pos.Y-(0.5f-BoxSize))*RotationSpeed;
}

if(pos.Y > 0.5f+BoxSize+EPSILON)
{
AngleZ -= (pos.Y-(0.5f+BoxSize))*RotationSpeed;
}

// Ensure Vertical Rotation Angle is bounded correctly
if(AngleZ < MinAngle) AngleZ = MinAngle;
else if(AngleZ > MaxAngle) AngleZ = MaxAngle;

//keep the player in the view angle that of the camera, this is the change made......... by smartwhiz
Player->setRotation(irr::core::vector3df(0,-(AngleY+180),0));

// Zoom only if one or other button is pressed
if((ZoomIn && !ZoomOut) || (!ZoomIn && ZoomOut))
{
int dir = (ZoomIn) ? -1 : 1;
Dist += ZoomSpeed*dir;

// Ensure Distance is bounded correctly
if(Dist < MinDist) Dist = MinDist;
else if(Dist > MaxDist) Dist = MaxDist;
}

updateCursorPosition();
}

// Create translation vector
irr::core::vector3df translation(Dist,0,0);
translation.rotateXYBy(-AngleZ,irr::core::vector3df(0,0,0));
translation.rotateXZBy(AngleY,irr::core::vector3df(0,0,0));

// Assumes the camera is *not* a child of the player node
camera->setTarget(Player->getPosition()+TargetOffset);
camera->setPosition(Player->getPosition()+translation+TargetOffset);
}


class CSceneNodeAnimator3PPlayer :
public irr::scene::ISceneNodeAnimator,
public irr::IEventReceiver
{
public:

//! Constructor
//! camera: The 3PCamera watching this object (should have a 3PC Animator attached to it)
//! cameraAnimator: The animator mentioned above
//! moveSpeed: The speed at which this player moves
CSceneNodeAnimator3PPlayer(
irr::scene::ISceneManager *manager,
irr::scene::ICameraSceneNode *camera,
CSceneNodeAnimator3PCamera *cameraAnimator,
irr::f32 moveSpeed = 0.3f);

virtual ~CSceneNodeAnimator3PPlayer(void);

//! animates the scene node
virtual void animateNode(irr::scene::ISceneNode *node, irr::u32 timeMs);

//! Process an input event
virtual bool OnEvent(irr::SEvent event);

//! Map movement keys
void MapKey(EPLAYER_ACTION action, irr::EKEY_CODE newCode);

//! Retrieve the player's current state
EPLAYER_STATE getState();

private:

irr::f32 MoveSpeed;
irr::u32 PrevTime;

EPLAYER_STATE State;

// action keys
irr::EKEY_CODE Keys[EPA_COUNT];
bool KeyStates[EPA_COUNT];

irr::scene::ICameraSceneNode *Camera;
CSceneNodeAnimator3PCamera *CameraAnimator;

irr::scene::ISceneManager *Manager;
};

CSceneNodeAnimator3PPlayer::CSceneNodeAnimator3PPlayer(
irr::scene::ISceneManager *manager,
irr::scene::ICameraSceneNode *camera,
CSceneNodeAnimator3PCamera *cameraAnimator,
irr::f32 moveSpeed)
: Manager(manager), Camera(camera), CameraAnimator(cameraAnimator), MoveSpeed(moveSpeed)
{
PrevTime = -1;

// Set initial key map
Keys[EPA_MOVE_FORWARD] = irr::KEY_UP;
Keys[EPA_MOVE_BACKWARD] = irr::KEY_DOWN;
Keys[EPA_MOVE_LEFT] = irr::KEY_LEFT;
Keys[EPA_MOVE_RIGHT] = irr::KEY_RIGHT;

// Init states
for(int i=0;i<EPA_COUNT;i++)
{
KeyStates[i] = false;
}

State = EPS_STANDING;
}

CSceneNodeAnimator3PPlayer::~CSceneNodeAnimator3PPlayer(void)
{
}

//! Map movement keys
void CSceneNodeAnimator3PPlayer::MapKey(EPLAYER_ACTION action, irr::EKEY_CODE newCode)
{
Keys[action] = newCode;
}

//! Retrieve the player's current state
EPLAYER_STATE CSceneNodeAnimator3PPlayer::getState()
{
return State;
}

//! Process an input event
bool CSceneNodeAnimator3PPlayer::OnEvent(irr::SEvent event)
{
if(event.EventType == irr::EET_KEY_INPUT_EVENT)
{
for(int i=0;i<EPA_COUNT;i++)
{
if(event.KeyInput.Key == Keys[i])
{
KeyStates[i] = event.KeyInput.PressedDown;
return true;
}
}
}

return false;
}

//! animates the scene node
void CSceneNodeAnimator3PPlayer::animateNode(irr::scene::ISceneNode *node, irr::u32 timeMs)
{
// This would have to be modified for an unanimated node
irr::scene::IAnimatedMeshSceneNode *amNode = (irr::scene::IAnimatedMeshSceneNode*)node;

// first loop
if(PrevTime == -1)
{
PrevTime = timeMs;
}

// Timer operations
irr::f32 timeMsFloat = (irr::f32)(timeMs-PrevTime);
PrevTime = timeMs;


// check if moving this round
if(((KeyStates[EPA_MOVE_FORWARD] && !KeyStates[EPA_MOVE_BACKWARD]) || (!KeyStates[EPA_MOVE_FORWARD] && KeyStates[EPA_MOVE_BACKWARD])) ||
((KeyStates[EPA_MOVE_LEFT] && !KeyStates[EPA_MOVE_RIGHT]) || (!KeyStates[EPA_MOVE_LEFT] && KeyStates[EPA_MOVE_RIGHT])))
{
if(State != EPS_RUNNING)
{
State = EPS_RUNNING;
amNode->setMD2Animation(irr::scene::EMAT_RUN);
}
}
else
{
if(State != EPS_STANDING)
{
State = EPS_STANDING;
amNode->setMD2Animation(irr::scene::EMAT_STAND);
}

return; // no movement
}

// Create movement directions based on camera orientation
irr::core::vector3df forward = Camera->getTarget()-Camera->getPosition();
forward.Y = 0;
irr::core::vector3df left = forward.crossProduct(Camera->getUpVector());

forward = forward.normalize();
left = left.normalize();

// Rotation must be set differently for all 8 possible movement directions.
if(KeyStates[EPA_MOVE_FORWARD] && KeyStates[EPA_MOVE_LEFT])
{
irr::core::vector3df movementVector = forward+left;
movementVector = movementVector.normalize();

amNode->setPosition(amNode->getPosition()+movementVector*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,135.0f-CameraAnimator->getOrientation(),0));
}
else if(KeyStates[EPA_MOVE_FORWARD] && KeyStates[EPA_MOVE_RIGHT])
{
irr::core::vector3df movementVector = forward-left;
movementVector = movementVector.normalize();

amNode->setPosition(amNode->getPosition()+movementVector*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,225.0f-CameraAnimator->getOrientation(),0));
}
else if(KeyStates[EPA_MOVE_BACKWARD] && KeyStates[EPA_MOVE_LEFT])
{
irr::core::vector3df movementVector = left-forward;
movementVector = movementVector.normalize();

amNode->setPosition(amNode->getPosition()+movementVector*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,45.0f-CameraAnimator->getOrientation(),0));
}
else if(KeyStates[EPA_MOVE_BACKWARD] && KeyStates[EPA_MOVE_RIGHT])
{
irr::core::vector3df movementVector = (forward*-1)-left;
movementVector = movementVector.normalize();

amNode->setPosition(amNode->getPosition()+movementVector*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,315.0f-CameraAnimator->getOrientation(),0));
}
else if(KeyStates[EPA_MOVE_FORWARD])
{
amNode->setPosition(amNode->getPosition()+forward*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,180.0f-CameraAnimator->getOrientation(),0));
}
else if(KeyStates[EPA_MOVE_BACKWARD])
{
amNode->setPosition(amNode->getPosition()-forward*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,-CameraAnimator->getOrientation(),0));
}
else if(KeyStates[EPA_MOVE_LEFT])
{
amNode->setPosition(amNode->getPosition()+left*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,90.0f-CameraAnimator->getOrientation(),0));
}
else if(KeyStates[EPA_MOVE_RIGHT])
{
amNode->setPosition(amNode->getPosition()-left*MoveSpeed*timeMsFloat);
amNode->setRotation(irr::core::vector3df(0,270.0f-CameraAnimator->getOrientation(),0));

}
}


// Objects used in the Event Receiver
CSceneNodeAnimator3PCamera *camAnim;
CSceneNodeAnimator3PPlayer *sydneyAnim;

// IrrLicht input reciever class..
class MyEventReceiver : public irr::IEventReceiver
{
public:
virtual bool OnEvent(irr::SEvent event)
{
if(event.EventType == irr::EET_LOG_TEXT_EVENT)
{
return true;
}

if(camAnim->OnEvent(event)) return true;
if(sydneyAnim->OnEvent(event)) return true;

return false;
}

};

int main()
{
// Start the renderer
MyEventReceiver receiver;
irrBase = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480),16, false, false, false, &receiver);

// Create reference vars
irr::video::IVideoDriver* driver = irrBase->getVideoDriver();
irr::scene::ISceneManager* smgr = irrBase->getSceneManager();
irr::gui::IGUIEnvironment* guienv = irrBase->getGUIEnvironment();
irr::gui::ICursorControl* cursor = irrBase->getCursorControl();

irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();

// Create a Sydney
irr::scene::IAnimatedMesh *sydneyMesh = smgr->getMesh("/opt/irrlicht-0.14.0/media/sydney.md2");
irr::scene::IAnimatedMeshSceneNode *sydneyNode = smgr->addAnimatedMeshSceneNode(sydneyMesh);
sydneyNode->setMD2Animation(irr::scene::EMAT_STAND);
sydneyNode->setMaterialFlag(irr::video::EMF_LIGHTING,false);
sydneyNode->setMaterialTexture(0,driver->getTexture("/opt/irrlicht-0.14.0/media/sydney.bmp"));


// Create and position map (copied from quake map example)
irrBase->getFileSystem()->addZipFileArchive("/opt/irrlicht-0.14.0/media/map-20kdm2.pk3");
irr::scene::IAnimatedMesh* levelMesh = smgr->getMesh("/opt/irrlicht-0.14.0/media/20kdm2.bsp");
irr::scene::ISceneNode* levelNode = smgr->addOctTreeSceneNode(levelMesh->getMesh(0));
levelNode->setPosition(irr::core::vector3df(-1300,-144,-1249));

// Create triangle selector for world (copied from collision example)
irr::scene::ITriangleSelector *selector = smgr->createOctTreeTriangleSelector(levelMesh->getMesh(0), levelNode, 128);
levelNode->setTriangleSelector(selector);
selector->drop();

// Create the camera animator
camAnim = new CSceneNodeAnimator3PCamera(smgr,irrBase->getCursorControl(),sydneyNode,50,180,10,irr::core::vector3df(0,20,0));
camera->addAnimator(camAnim);
camAnim->drop();

 //Add collision response to camera (second, because order is important!)
irr::scene::ISceneNodeAnimator *cameraCollision = smgr->createCollisionResponseAnimator(
selector,
camera,
irr::core::vector3df(3,5,3),
core::vector3df(0.0f,0.0f,0.0f) ,
core::vector3df(0.0f,0.0f,0.0f), 0);
camera->addAnimator(cameraCollision);
cameraCollision->drop();

// Create the player animator
sydneyAnim = new CSceneNodeAnimator3PPlayer(smgr,camera,camAnim);
sydneyNode->addAnimator(sydneyAnim);
sydneyAnim->drop();

// Add collision response to sydney (copied from collision example)
irr::core::vector3df sydneyDimensions = sydneyMesh->getBoundingBox().MaxEdge - sydneyMesh->getBoundingBox().MinEdge;
irr::scene::ISceneNodeAnimator *sydneyCollision = smgr->createCollisionResponseAnimator(selector,sydneyNode,sydneyDimensions/2.0f);
sydneyNode->addAnimator(sydneyCollision);
sydneyCollision->drop();



// Hides the mouse
cursor->setVisible(false);

// render loop
while(irrBase->run())
{
//irr - Now we draw it all, if the window is active
if(irrBase->isWindowActive())
{
driver->beginScene(true, true, irr::video::SColor(0,100,100,100));

smgr->drawAll();
guienv->drawAll(); // not that we have GUI stuff

driver->endScene();
}
}

// Kill renderer
if(irrBase)
{
irrBase->drop();
irrBase = NULL;
}

return 0;
}
