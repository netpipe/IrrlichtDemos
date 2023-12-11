#include "3DP.h"
    CScene3PCamera::CScene3PCamera(
    irr::scene::ISceneManager *manager,
    irr::gui::ICursorControl *cursor,
    irr::scene::ISceneNode *player,
    irr::f32 Distanciaancia,irr::f32 initAngleY,
    irr::f32 initAngleZ,
	bool RotatePlayer,
    irr::core::vector3df targetOffset,
    irr::f32 minDistancia,
    irr::f32 maxDistancia,
    irr::f32 minAngle,
    irr::f32 maxAngle,
    irr::f32 boxSize,
    irr::f32 rotationSpeed)
    : Active(true), Manager(manager), Cursor(cursor), Player(player),
    Distancia(Distanciaancia),AngleY(initAngleY),AngleZ(initAngleZ),XRotatePlayer(RotatePlayer),
    TargetOffset(targetOffset),
    MinDistancia(minDistancia), MaxDistancia(maxDistancia), MinAngle(-maxAngle), MaxAngle(-minAngle),
    BoxSize(boxSize), RotationSpeed(rotationSpeed)
    {
 

    Cursor->setPosition(0.5f,0.5f);

    // Ensure Distanciaance is bounded correctly
    if(Distancia < MinDistancia) Distancia = MinDistancia;
    else if(Distancia > MaxDistancia) Distancia = MaxDistancia;

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
    CScene3PCamera::~CScene3PCamera(void)
    {
    }

    //! Puts the cursor back in the box
    void CScene3PCamera::updateCursorPosition()
    {
    irr::core::position2d<irr::f32> pos = Cursor->getRelativePosition();

    if(pos.X < (0.5f-BoxSize)) pos.X = (0.5f-BoxSize);
    if(pos.X > (0.5f+BoxSize)) pos.X = (0.5f+BoxSize);

    if(pos.Y < (0.5f-BoxSize)) pos.Y = (0.5f-BoxSize);
    if(pos.Y > (0.5f+BoxSize)) pos.Y = (0.5f+BoxSize);

    Cursor->setPosition(pos);
    }

    //! Process an input event
  bool CScene3PCamera::OnEvent(const irr::SEvent& event)
    {
    if(!Active)
    {
    return false;
    }

    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
    {

    }

    return false;
    }

    //! Get/set active status
    int CScene3PCamera::isActive()
    {
    return Active;
    }

    void CScene3PCamera::setActive(bool status)
    {
    // reset the cursor only if we are switching back to active from inactive
    if(!Active && status)
    {
    updateCursorPosition();
    }

    Active = status;
    }

    //! Get/set box size
    irr::f32 CScene3PCamera::getBoxSize()
    {
    return BoxSize;
    }

    void CScene3PCamera::setBoxSize(irr::f32 newSize)
    {
    BoxSize = newSize;
    updateCursorPosition();
    }

 

    //! Access the camera's current orientation
    irr::f32 CScene3PCamera::getOrientation()
    {
    return AngleY;
    }


    //! animates the scene node
    void CScene3PCamera::animateNode(irr::scene::ISceneNode *node, irr::u32 timeMs)
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
	if (XRotatePlayer == true){
	Player->setRotation(irr::core::vector3df(0,-(AngleY+180),0));
	}


    updateCursorPosition();
    }

    // Create translation vector
    irr::core::vector3df translation(Distancia,0,0);
    translation.rotateXYBy(-AngleZ,irr::core::vector3df(0,0,0));
    translation.rotateXZBy(AngleY,irr::core::vector3df(0,0,0));

    // Assumes the camera is *not* a child of the player node
    camera->setTarget(Player->getPosition()+TargetOffset);
    camera->setPosition(Player->getPosition()+translation+TargetOffset);
    }


   