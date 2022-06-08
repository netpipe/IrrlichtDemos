#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#pragma comment(lib, "irrlicht.lib")

scene::ICameraSceneNode* camera = 0;

int main()
{

IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
core::dimension2d<s32>(640,480),
32, false, true );

video::IVideoDriver* driver = device->getVideoDriver();
scene::ISceneManager* smgr = device->getSceneManager();


scene::ISceneNode* node = 0;

scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh("Hill",
core::dimension2d<f32>(20,20),
core::dimension2d<u32>(20,20), 0, 20,
core::dimension2d<f32>(2,2),
core::dimension2d<f32>(1,1));




scene::IParticleSystemSceneNode* ps = 0;
ps = smgr->addParticleSystemSceneNode(false);
ps->setPosition(core::vector3df(-160,170,80));
ps->setScale(core::vector3df(1,1,1));

ps->setParticleSize(core::dimension2d<f32>(5.0f, 5.0f));

scene::IParticleEmitter* em = ps->createBoxEmitter(
core::aabbox3d<f32>(-270,-200,-270,270,300,300),
core::vector3df(0.00f,0.00f,0.0f),
400,800,
video::SColor(0,255,255,255), video::SColor(0,255,255,255),
8000,10000);

ps->setEmitter(em);
em->drop();

scene::IParticleAffector* paf =
ps->createGravityAffector(core::vector3df(0.00f,-2.00f, 0.0f), 1000);

ps->addAffector(paf);
paf->drop();

ps->setMaterialFlag(video::EMF_LIGHTING, false);
ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
ps->setMaterialTexture(0, driver->getTexture("Raindrop.jpg"));
ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);





scene::ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
driver->getTexture("irrlicht2_up.jpg"),
driver->getTexture("irrlicht2_up.jpg"),
driver->getTexture("irrlicht2_lf.jpg"),
driver->getTexture("irrlicht2_rt.jpg"),
driver->getTexture("irrlicht2_ft.jpg"),
driver->getTexture("irrlicht2_bk.jpg"));

device->getCursorControl()->setVisible(false);

camera=smgr->addCameraSceneNodeFPS(0, 100.0f, 200.0f, -1, 0, 8);

ps->setParent(camera);
int lastFPS = -1;

core::vector3df Pos = camera->getPosition();
while(device->run())
{
driver->beginScene(true, true, 0);

smgr->drawAll();

driver->endScene();

core::stringw str = L"Rain Effects By Srini";
str += driver->getName();

}

device->drop();

return 0;
}
