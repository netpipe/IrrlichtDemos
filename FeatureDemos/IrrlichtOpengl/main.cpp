
//#include <windows.h> // Win32 Header File
#include "GL/gl.h"
#include "GL/glu.h"
//#include "GL/glaux.h"

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

void viewOrtho() // Orthogonal projection
{
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, 640, 480, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
}

void viewPerspective() // Perspective projection
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

int main()
{
  IrrlichtDevice *device = createDevice(EDT_OPENGL, core::dimension2d<s32>(640,480));
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  device->setWindowCaption(L"2D Test");
  smgr->addCameraSceneNode(0, vector3df(0,-40,0), vector3df(0,0,0));

  SMaterial material;
  material.setTexture(0, driver->getTexture("../../media/001shot.jpg"));

  while(device->run())
  {
    driver->beginScene(true, true, SColor(255,0,0,255));

    driver->setMaterial(material);

    driver->draw2DImage(material.getTexture(0), position2d<s32>(500, 400));

    viewOrtho();
    glTranslatef(200, 200, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex2f(-100,-100);
    glTexCoord2f(0,1);glVertex2f(-100, 100);
    glTexCoord2f(1,1);glVertex2f( 100, 100);
    glTexCoord2f(1,0);glVertex2f( 100,-100);
    glEnd();
    viewPerspective();

    smgr->drawAll();
    driver->endScene();
  }
  device->drop();
  return 0;
}
