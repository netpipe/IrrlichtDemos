#include "player.h"
#include "../Physics/Collision.hpp"

Player::Player() {
  isFalling = false;
  isRunning = false;
  isDead = false;
  moveSpeed = 300.f;
}

Player::~Player() {
	delete m_cWeapon;
}

void Player::registerIrrlichtDevice(IrrlichtDevice &device) {
	m_irrDevice = &device;
}

void Player::initialize(void) {
	m_cWeapon = new Rifle();
	m_cWeapon->registerIrrlichtDevice(*m_irrDevice);
	m_cWeapon->initialize();
	m_cWeapon->setPosition(vector3df(0.5f,-1.0f,0.5f));
	m_cWeapon->setScale(vector3df(0.02f,0.02f,0.02f));
}

void Player::setParent(ISceneNode *parent) {
	m_cWeapon->setParent((ICameraSceneNode*)parent);
}

//below is powercell code
//#include "Player.hpp"

Player *Player::m_player = NULL;
Player Player::m_default_player_buffer;

Player *Player::Instance()
{
  if(!m_player)
    m_player = &m_default_player_buffer;

  return m_player;
}

void Player::setIsRunning(bool run)
{
  if(run && !isRunning)
  {
    isRunning = true;
    playerNode->setMD2Animation(EMAT_RUN);
  }
  else if(!run && isRunning)
  {
    isRunning = false;
    playerNode->setMD2Animation("stand");
  }
}

bool Player::getIsRunning(void)
{
  return isRunning;
}
/*
void Player::moveForward()
{
  facing = vector3df(sin(( playerNode->getRotation().Y + 90.0f ) * PI/180.0f ), 0, cos((playerNode->getRotation().Y + 90.0f)*PI/180.0f ));
  facing.normalize();

  newPos = (facing * moveSpeed * LocalTimer::Instance()->GetDeltaTime()) + playerNode->getPosition();

  playerNode->setPosition( newPos );
  playerNode->updateAbsolutePosition();

  setIsRunning(true);
}

void Player::strafeLeft()
{
  facing = vector3df(sin(
    ( playerNode->getRotation().Y) * PI/180.0f ),
    0,
    cos((playerNode->getRotation().Y)*PI/180.0f ));
  facing.normalize();
  newPos = (facing*(moveSpeed/2)*LocalTimer::Instance()->GetDeltaTime()) + playerNode->getPosition();
  playerNode->setPosition( newPos );
  playerNode->updateAbsolutePosition();

  setIsRunning(true);
}

void Player::strafeRight()
{
  facing = vector3df(sin(
    ( playerNode->getRotation().Y + 180.0f) * PI/180.0f ),
    0,
    cos((playerNode->getRotation().Y + 180.0f)*PI/180.0f ));
  facing.normalize();
  newPos = (facing*(moveSpeed/2)*LocalTimer::Instance()->GetDeltaTime()) + playerNode->getPosition();
  playerNode->setPosition( newPos );
  playerNode->updateAbsolutePosition();

  setIsRunning(true);
}

void Player::moveBackwards()
{
  facing = vector3df(sin(
    ( playerNode->getRotation().Y + 90.0f) * PI/180.0f ),
    0,
    cos((playerNode->getRotation().Y + 90.0f)*PI/180.0f ));
  facing.normalize();
  newPos = (-facing*moveSpeed*LocalTimer::Instance()->GetDeltaTime()) + playerNode->getPosition();
  playerNode->setPosition( newPos );
  playerNode->updateAbsolutePosition();

  setIsRunning(true);
}
*/
void Player::jump()
{
  playerNode->setPosition(
    vector3df(
      playerNode->getPosition().X,
      playerNode->getPosition().Y+110,
      playerNode->getPosition().Z));
     // isFalling=true;
}

void Player::CheckFalling(void)
{
 // playerNode->setPosition(   //  this is from the 0 gravity test
 //   vector3df(
 //     playerNode->getPosition().X,
 //     playerNode->getPosition().Y-11,
 //     playerNode->getPosition().Z));

 // playerRot = playerNode->getRotation();
 // playerRot.normalize();
    printf ("checking for the fall");
/*
  irrDevice->getSceneManager()->getSceneCollisionManager()->getCollisionResultPosition(
    Collision::Instance()->metaSelector, playerNode->getAbsolutePosition(),
    Collision::Instance()->getEllipsoidRadius(),
    vector3df(0,-30,0), //playerRot
    tri, hitPos,isFalling,outNode);
*/


    if(isFalling)
    {
//        PlayerHelper::Instance()->DecreaseHealth(5);
        printf ("%f %f %f",hitPos.X,hitPos.Y,hitPos.Z);
    }
}

void Player::createShadow(void)
{
  f32 size = 60;
  u32 tiles = 1;
  f32 rep = 1;

  IAnimatedMesh *blobMesh = irrDevice->getSceneManager()->addHillPlaneMesh("blobShadow",
    dimension2d<f32>(size,size),
    dimension2d<u32>(tiles,tiles), 0, 0,
    dimension2d<f32>(0,0),
    dimension2d<f32>(rep,rep));

  if (blobMesh)
  {
    IMeshSceneNode* blobNode = irrDevice->getSceneManager()->addMeshSceneNode(blobMesh->getMesh(0));
    blobNode->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("PlayerModel/blob.tga"));
    blobNode->setParent(playerNode);
    blobNode->setPosition(vector3df(0,-10,0));
    blobNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    /*
    blobNode->setMaterialFlag(video::EMF_LIGHTING, false);
    blobNode->getMaterial(0).Shininess = 100.0f;
    blobNode->getMaterial(0).DiffuseColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).EmissiveColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).AmbientColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).SpecularColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).MaterialType = EMT_SOLID;
    */
    blobNode->getMaterial(0).MaterialTypeParam = 0.01;
  }
}

bool Player::getIsDead(void)
{
  return isDead;
}

void Player::setIsDead(bool val)
{
  isDead = val;
}
void Player::setSpeed(float val)
{
  moveSpeed = val;
}
