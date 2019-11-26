#include "Player.hpp"
irr::scene::ISceneNode* outNode;
vector3df hitPos;
Player *Player::m_player = NULL;
Player Player::m_default_player_buffer;
   f32 headRotation = 0;
   f32 headPosition = 0;

void Player::SetDefaultValues()
{
  isFalling = false;
  isRunning = false;
  isDead = false;
  moveSpeed = 300.f;
}

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
  //  playerNode->setMD2Animation(EMAT_RUN);
  Player::Instance()->getPlayerNode()->setAnimationSpeed(100);
  }
  else if(!run && isRunning)
  {
    isRunning = false;
  //  playerNode->setMD2Animation("stand");
  Player::Instance()->getPlayerNode()->setAnimationSpeed(0);
  }


}

bool Player::getIsRunning(void)
{
  return isRunning;
}

void Player::moveForward()
{
  facing = vector3df(sin(( playerNode->getRotation().Y + 90.0f ) * PI/180.0f ), 0, cos((playerNode->getRotation().Y + 90.0f)*PI/180.0f ));
  facing.normalize();

  newPos = (facing * moveSpeed * LocalTimer::Instance()->GetDeltaTime()) + playerNode->getPosition();

  playerNode->setPosition( newPos );
  playerNode->updateAbsolutePosition();

  setIsRunning(true);
 #define cool
#ifdef cool
//playerNode->setLoopMode(false);
playerNode->setJointMode(EJUOR_CONTROL);
    //playerNode->setRenderFromIdentity(true);

 IBoneSceneNode* thisJoint = playerNode->getJointNode("Head");// playerNode->getJointNode("Head");


    if( thisJoint )
      {
         ISceneNode* parentJoint = thisJoint->getParent();

         if( parentJoint )
         {
         // Get the default (animated) Joint's position and rotation
            vector3df jointPos = thisJoint->getAbsolutePosition();
            vector3df jointRot = thisJoint->getAbsoluteTransformation().getRotationDegrees();

         // Get the absolute INVERSE Transformation matrix of the parent
            matrix4 iparentTransform = parentJoint->getAbsoluteTransformation();
               iparentTransform.makeInverse();

         // Set the Absolute Position or Rotation of the Joint without fear!
            vector3df newJointPos = jointPos + vector3df( 0, 1+(sin(headPosition)*2), 0 );
            vector3df newJointRot = vector3df( 0, headRotation, 0 );

         // Transform the Position by the Parent's Inverse matrix before applying it
            iparentTransform.transformVect( newJointPos );

         // APPLY
            thisJoint->setPosition( newJointPos );
            thisJoint->setRotation( newJointRot );
         }
      }


//   // funny
      headPosition += 0.01f;
      if( headPosition > 2*PI ){ headPosition -= 0; }
//
      headRotation += 0.5f;
      if( headRotation > 360 ){ headRotation -= 360; }
//thisJoint->setRotation(thisJoint->getRotation() + vector3df(0,3,0));
//playerNode->animateJoints(true);
#endif
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



void Player::jump()
{
  playerNode->setPosition(
    vector3df(
      playerNode->getPosition().X,
      playerNode->getPosition().Y+10,
      playerNode->getPosition().Z));
     // isFalling=true;
  //   playerNode->setMaterialFlag(video::EMF_LIGHTING, true);
}

void Player::CheckFalling(void)
{
 // playerNode->setPosition(     this is from the 0 gravity test
 //   vector3df(
 //     playerNode->getPosition().X,
 //     playerNode->getPosition().Y-11,
 //     playerNode->getPosition().Z));

  playerRot = playerNode->getRotation();
  playerRot.normalize();


 // irrDevice->getSceneManager()->getSceneCollisionManager()->getCollisionResultPosition(
 //   Collision::Instance()->metaSelector, playerNode->getAbsolutePosition(),
//    Collision::Instance()->getEllipsoidRadius(),
//    playerRot, tri, hitPos, isFalling, outNode, (irr::f32)0.005f, vector3df(0,-30,0));
 //   printf ("%f %f %f",hitPos.X,hitPos.Y,hitPos.Z);

    if(isFalling)
    {
     //   PlayerHelper::Instance()->DecreaseHealth(5);
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
    blobNode->setPosition(vector3df(0,5,0));
  //  blobNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

    blobNode->setMaterialFlag(video::EMF_LIGHTING, false);
    blobNode->getMaterial(0).Shininess = 0.50f;
    blobNode->getMaterial(0).DiffuseColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).EmissiveColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).AmbientColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).SpecularColor = SColor(255,255,255,255);
    blobNode->getMaterial(0).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

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
