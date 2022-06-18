#include "Camera.hpp"

Camera *Camera::m_camera = NULL;
Camera Camera::m_default_camera_buffer;

Camera *Camera::Instance()
{
  if(!m_camera)
    m_camera = &m_default_camera_buffer;

  return m_camera;
}

void Camera::createCamera(void)
{
  irrDevice->getCursorControl()->setPosition( 0.5f, 0.5f );
  camera = irrDevice->getSceneManager()->addCameraSceneNode(0, vector3df(0.0f,0.0f,0.0f) , vector3df(0.0f,0.0f,0.0f), -1);
  camera->setNearValue(0.01f);
  camera->setFarValue(10000.0f);
  camera->setFOV(PI/2.0f);
}

void Camera::moveCameraControl()
{
  cursorPos = irrDevice->getCursorControl()->getRelativePosition();
  camera = irrDevice->getSceneManager()->getActiveCamera();

  change_x = ( cursorPos.X - 0.5 ) * 256.0f;
  change_y = ( cursorPos.Y - 0.5 ) * 256.0f;

  direction += change_x;
  zdirection -= change_y;

  if( zdirection <- 90 )
     zdirection = -90;
  else
  if( zdirection > 90 )
     zdirection = 90;

  irrDevice->getCursorControl()->setPosition( 0.5f, 0.5f );

  playerPos = Player::Instance()->getPlayerNode()->getPosition();

  xf = playerPos.X - cos( direction * PI / 180.0f ) * 64.0f;
  yf = playerPos.Y - sin( zdirection * PI / 180.0f ) * 128.0f;
  zf = playerPos.Z + sin( direction * PI / 180.0f ) * 64.0f;

  camera->setPosition( core::vector3df( xf, yf, zf ) );
  camera->updateAbsolutePosition();
  camera->setTarget( core::vector3df( playerPos.X, playerPos.Y+25.0f, playerPos.Z ) );
  Player::Instance()->getPlayerNode()->setRotation( core::vector3df( 0, direction, 0 ) );
}
