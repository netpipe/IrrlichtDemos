#include <irrlicht.h>
#include <SPK.h>
#include <SPK_IRR.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace irr;
using namespace SPK;
using namespace SPK::IRR;

System* particleSystem = NULL;
IrrlichtDevice* device = NULL;

float rainRatio = 0.5f;

class MyEventReceiver : public IEventReceiver
{
    public:
        virtual bool OnEvent(const SEvent& event)
        {
            if(event.EventType==EET_KEY_INPUT_EVENT)
            {
				if(event.KeyInput.Key == KEY_ESCAPE && event.KeyInput.PressedDown==false)
				{
					device->closeDevice();
				}
                if(event.KeyInput.Key == KEY_DELETE && event.KeyInput.PressedDown==false)
                {
                    particleSystem->empty();
                    return true;
                }
				if(event.KeyInput.Key == KEY_KEY_V && event.KeyInput.PressedDown==false)
				{
					IRRBuffer::activateVBOHint(!IRRBuffer::isVBOHintActivated());
					return true;
				}
            }
            return false;
        }

        int oldMouseX,oldMouseY;
};

class ParticleManager
{
   template<class T> T param(T min,T max)
   {
      return static_cast<T>(min + rainRatio * (max - min));
   }
private:
   bool isRaining;
   float time;
   float oldtime;
   float deltaTime;

   float PI;

   float posX;
   float posZ;

   SPK::Group* rainGroup;
   SPK::Group* dropGroup;
   SPK::Group* splashGroup;
   SPK::Emitter* dropEmitter;

   Vector3D gravity;
   IRRPointRenderer* dropRenderer;
   IRRLineRenderer* rainRenderer;
   IRRQuadRenderer* splashRenderer;

   Model* rainModel;
   Model* dropModel;
   Model* splashModel;
   AABox* rainZone;
   SphericEmitter* rainEmitter;
   System* particleSystem;

//----------------------

   scene::IParticleSystemSceneNode* ps;
   scene::IParticleEmitter* em;
   scene::IParticleAffector* paf;

   core::dimension2d<s32> screensize;
   core::aabbox3d<f32> box;
   video::SMaterial material;

public:
   static ParticleManager* instance;
   ParticleManager();
   void Initialize();
   void Rain(bool);
   bool KillRain(Particle& particle,float deltaTime);
   virtual ~ParticleManager();
   virtual void updatePosition(irr::core::vector3df position);
   virtual void render();
   static ParticleManager* Instance();

};

bool KillRainF(Particle& particle,float deltaTime);

bool KillRainF(Particle& particle,float deltaTime)
{
   return ParticleManager::Instance()->KillRain(particle, deltaTime);
}

ParticleManager* ParticleManager::instance = 0;

ParticleManager* ParticleManager::Instance()
{
   return instance;
}

ParticleManager::ParticleManager()
{
   instance = this;
}
void ParticleManager::Initialize()
{
   //Rain
   PI = 3.14159265358979323846f;

   time = (f32)device->getTimer()->getTime() / 1000.0f;

   posX = 0;
   posZ = 0;

   gravity = Vector3D(0.0f,-10.0f,0.0f);

   // point renderer
   dropRenderer = IRRPointRenderer::create(device);
   dropRenderer->setType(POINT_CIRCLE);
   dropRenderer->setSize(2.0f * 0.7f);
   dropRenderer->setBlending(BLENDING_ADD);
   //dropRenderer->enableRenderingHint(DEPTH_WRITE,false);

   // line renderer
   rainRenderer = IRRLineRenderer::create(device);
   rainRenderer->setLength(-0.1f);
   rainRenderer->setBlending(BLENDING_ADD);
   //rainRenderer->enableRenderingHint(DEPTH_WRITE,false);

   // quad renderer
   splashRenderer = IRRQuadRenderer::create(device);
   splashRenderer->setScale(0.25f,0.25f);
   splashRenderer->setTexture(device->getVideoDriver()->getTexture("waterdrops.bmp"));
   splashRenderer->setTexturingMode(TEXTURE_2D);
   splashRenderer->setBlending(BLENDING_ADD);

   // Models
   // rain model
   rainModel = Model::create(FLAG_GREEN | FLAG_RED | FLAG_BLUE | FLAG_ALPHA | FLAG_MASS,0,   FLAG_MASS);
   rainModel->setParam(PARAM_ALPHA,0.2f);
   rainModel->setImmortal(true);

   // drop model
   dropModel = Model::create(FLAG_GREEN | FLAG_RED | FLAG_BLUE | FLAG_ALPHA | FLAG_MASS,0,FLAG_MASS);
   dropModel->setParam(PARAM_ALPHA,0.6f);

   // splash model
   splashModel = Model::create(FLAG_GREEN | FLAG_RED | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE,   FLAG_SIZE | FLAG_ALPHA,   FLAG_SIZE | FLAG_ANGLE);
   splashModel->setParam(PARAM_ANGLE,0.0f,2.0f * PI);
   splashModel->setParam(PARAM_ALPHA,1.0f,0.0f);

   // rain emitter
   rainZone = AABox::create(Vector3D(0.0f,5.0f,0.0f));
   rainEmitter = SphericEmitter::create(Vector3D(0.0f,-1.0f,0.0f),0.0f,0.03f * PI);
   rainEmitter->setZone(rainZone);

   // drop emitter
   dropEmitter = SphericEmitter::create(Vector3D(0.0f,1.0f,0.0f),0.0f,0.2f * PI);

   // Groups
   // rain group
   rainGroup = Group::create(rainModel,10000);
   rainGroup->setCustomUpdate(&KillRainF);
   rainGroup->setRenderer(rainRenderer);
   rainGroup->addEmitter(rainEmitter);
   rainGroup->setFriction(0.7f);
   rainGroup->setGravity(gravity);
   rainGroup->enableAABBComputing(true);

   // drop group
   dropGroup = Group::create(dropModel,22000);
   dropGroup->setRenderer(dropRenderer);
   dropGroup->setFriction(0.7f);
   dropGroup->setGravity(gravity);
   dropGroup->enableAABBComputing(true);

   // splash group
   splashGroup = Group::create(splashModel,3000);
   splashGroup->setRenderer(splashRenderer);
   splashGroup->enableAABBComputing(true);

   // System
   particleSystem = IRRSystem::create(device->getSceneManager()->getRootSceneNode(),device->getSceneManager());
   particleSystem->addGroup(splashGroup);
   particleSystem->addGroup(dropGroup);
   particleSystem->addGroup(rainGroup);
   particleSystem->enableAABBComputing(true);

   SPKFactory::getInstance().traceAll();
}
void ParticleManager::Rain(bool raining)
{
   //TODO: enable - disable rain state
   rainModel->setParam(PARAM_RED,param(1.0f,0.40f));
   rainModel->setParam(PARAM_GREEN,param(1.0f,0.40f));
   rainModel->setParam(PARAM_BLUE,param(1.0f,0.42f));
   rainModel->setParam(PARAM_MASS,param(0.4f,0.8f),param(0.8f,1.6f));

   dropModel->setParam(PARAM_RED,param(1.0f,0.40f));
   dropModel->setParam(PARAM_GREEN,param(1.0f,0.40f));
   dropModel->setParam(PARAM_BLUE,param(1.0f,0.42f));
   dropModel->setParam(PARAM_MASS,param(0.4f,0.8f),param(3.0f,4.0f));
   dropModel->setLifeTime(param(0.05f,0.3f),param(0.1f,0.5f));

   splashModel->setParam(PARAM_RED,param(1.0f,0.40f));
   splashModel->setParam(PARAM_GREEN,param(1.0f,0.40f));
   splashModel->setParam(PARAM_BLUE,param(1.0f,0.42f));
   splashModel->setParam(PARAM_SIZE,0.0f,0.0f,param(0.375f,2.25f),param(0.75f,3.78f));
   splashModel->setLifeTime(param(0.2f,0.3f),param(0.4f,0.5f));

   rainEmitter->setFlow(param(0.0f,6000.0f));
   rainEmitter->setForce(param(3.0f,5.0f),param(6.0f,10.0f));
   rainZone->setDimension(Vector3D(param(100.0f,10.0f),0.0f,param(100.0f,10.0f)));

   dropEmitter->setForce(param(0.1f,1.0f),param(0.2f,2.0f));

   dropRenderer->setSize(param(1.0f,3.0f) * 0.7f);
   rainRenderer->setWidth(param(1.0f,4.0f) * 0.7f);
}

ParticleManager::~ParticleManager()
{
   SPKFactory::getInstance().traceAll();
   SPKFactory::getInstance().destroyAll();
   SPKFactory::getInstance().traceAll();
}
void ParticleManager::updatePosition(irr::core::vector3df position)
{
	rainZone->setPosition(Vector3D(position.X,20.0f,position.Z));
}
void ParticleManager::render()
{
   oldtime = time;
   time = (f32)device->getTimer()->getTime() / 1000.0f;
        deltaTime = time - oldtime;
   particleSystem->update(deltaTime);
}
bool ParticleManager::KillRain(Particle& particle,float deltaTime)
{
   if (particle.position().y <= 0.0f)
   {
      particle.position().set(particle.position().x,0.01f,particle.position().z);
      splashGroup->addParticles(1,particle.position(),Vector3D());
      dropGroup->addParticles(param(2,8),particle.position(),dropEmitter);
      return true;
   }
   return false;
}

template<class T>
T param(T min,T max)
{
	return static_cast<T>(min + rainRatio * (max - min));
}

// Main function
int main(int argc, char *argv[])
{
	//!IRRLICHT
    video::E_DRIVER_TYPE chosenDriver = video::EDT_OPENGL;

    //!IRRLICHT
	device = createDevice(chosenDriver,
		core::dimension2d<u32>(1024,768),
		32,
		false,
		false,
		false,
		new MyEventReceiver);

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    device->setWindowCaption(L"SPARK Basic Demo using Irrlicht");
	device->getCursorControl()->setVisible(false);

	smgr->setAmbientLight(video::SColorf(0.3f,0.3f,0.3f,1));
	driver->setFog(video::SColor(0,138,125,81), video::EFT_FOG_LINEAR, 0.1f, 100.0f, 1.0f, true, false);

	video::SMaterial mat;
    mat.MaterialType = video::EMT_SOLID;
    mat.setTexture(0, driver->getTexture("paving.bmp"));
    mat.Lighting  = true;
    mat.FogEnable = true;
    mat.Shininess = 50.0f;
	mat.EmissiveColor = irr::video::SColor(255, 240,200,150);
    mat.SpecularColor = irr::video::SColor(255, 223,231,100);

    irr::scene::IMesh *mesh = smgr->getGeometryCreator()->createPlaneMesh (
                                    irr::core::dimension2d<irr::f32> (10,10), //tileSize,
                                    irr::core::dimension2d<irr::u32> (10,10),              //tileCount,
                                    &mat,                                //video::SMaterial *,
                                    irr::core::dimension2d<irr::f32>(16,16) );          //textureRepeatCount

    irr::scene::IMeshSceneNode *node = smgr->addMeshSceneNode(mesh);

	scene::IMeshSceneNode* q3node = 0;
    q3node = smgr->addOctreeSceneNode(mesh,0,-1);
	scene::ITriangleSelector* selector = 0;

    if (q3node)
    {
        q3node->setPosition(core::vector3df(0,0,0));

        selector = smgr->createOctreeTriangleSelector(
                q3node->getMesh(), q3node, 128);
        q3node->setTriangleSelector(selector);
        // We're not done with this selector yet, so don't drop it.
    }
	// Set a jump speed of 3 units per second, which gives a fairly realistic jump
    // when used with the gravity of (0, -10, 0) in the collision response animator.
    scene::ICameraSceneNode* cam =
        smgr->addCameraSceneNodeFPS(0, 100.0f, 0.01f, -1, 0, 0, true, 0.03f);
	cam->bindTargetAndRotation(false);
    cam->setPosition(core::vector3df(0.0f,4.0f,2.0f));
	cam->setRotation(irr::core::vector3df(90.0f,0.0f,0.0f));
    //cam->setTarget(core::vector3df(-70,30,-60));
	cam->setNearValue(0.05f);

    if (selector)
    {
        scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
            selector, cam, core::vector3df(1,3,1),
            core::vector3df(0,-0.1f,0), core::vector3df(0,1,0));
        selector->drop(); // As soon as we're done with the selector, drop it.
        cam->addAnimator(anim);
        anim->drop();  // And likewise, drop the animator when we're done referring to it.
    }

	mesh->drop();

	//-------------------------------SPARK BEGIN
	SPK::randomSeed = device->getTimer()->getRealTime();
	ParticleManager *particleMngr = new ParticleManager;
	particleMngr->Initialize();
    particleMngr->Rain(true);
	//-------------------------------SPARK END

	while(device->run())
	{
		particleMngr->updatePosition(cam->getAbsolutePosition());
		particleMngr->render();

		driver->beginScene(true, true, irr::video::SColor(0,0,0,0));

		smgr->drawAll();

		/*irr::core::stringw infos; infos+="FPS: "; infos+=driver->getFPS(); infos+=" - Nb Particles: "; infos+=particleSystem->getNbParticles();
		infos+=" - VBO: "; infos+=(IRRBuffer::isVBOHintActivated() ? "true" : "false");
        guienv->getBuiltInFont()->draw(infos.c_str(),irr::core::rect<irr::s32>(0,0,170,20),irr::video::SColor(255,255,255,255));*/

		driver->endScene();
	}

	return 0;
}
