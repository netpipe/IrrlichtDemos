
#include "World.h"

#include "IEngine.h"

#include "Physics.h"
#include "InputProfile.h"
#include "Mesh.h"
#include "Character.h"
#include "ThirdPersonCameraController.h"
#include "ProxyTransformable.h"
#include "UserCharacterController.h"
#include "NodeHandler.h"
#include "Camera.h"
#include "MotionSensor.h"
#include "SoundMotionCallback.h"
#include "SoundSource.h"
#include "ISoundSystem.h"
#include "ISound.h"
#include "IShader.h"

// Animators
#include "RotationAnimator.h"
#include "BobAnimator.h"

World::World()
{
	inputProfile = NULL;
	cameraController = NULL;
	lastCameraController = NULL;
	
	// world effects
	skyBoxNode = NULL;
	skyBoxShader = NULL;
	
	engine = GetEngine();
	
	physics = new Physics(this);
	
	camera = new Camera(engine->GetIrrlichtDevice()->getSceneManager()->getActiveCamera());
	AddTransformable(camera);
	// Do NOT drop camera yet, as we need to keep it even if it is removed from transformables list.
	// (it is only finally dropped in destructor)
	
	// Let's have mouse centring.
	engine->SetAutoMouseCentring(true,true);
}

World::~World()
{
	camera->drop();
	
	// Must set camera to NULL so it is not added back in by RemoveAllTransformables
	// (otherwise RemoveAllTransformables adds camera back to transformables after removing all).
	camera = NULL;
	
	if (inputProfile)
		inputProfile->drop();
	
	if (cameraController)
		cameraController->drop();
	
	if (skyBoxNode)
		skyBoxNode->remove();
	
	if (skyBoxShader)
		skyBoxShader->drop();
	
	RemoveAllTransformables();
	
	physics->drop();
}

IPhysics *World::GetPhysics()
{
	return physics;
}

ICamera *World::GetCamera()
{
	return camera;
}

void World::SetInputProfile(IInputProfile *profile)
{
	// If a new profile is set while we are paused, then pause it.
	if (IsPaused() && profile && profile != inputProfile)
		profile->Pause();
	
	SET_REF_COUNTED_POINTER(inputProfile, profile);
}

IInputProfile *World::GetInputProfile()
{
	return inputProfile;
}

void World::SubscribeToInput(IWantInput *wantInput)
{
	inputSubscribers.Insert(wantInput);
}

void World::UnsubscribeFromInput(IWantInput *wantInput)
{
	inputSubscribers.Remove(wantInput);
}

void World::SetCameraController(ICameraController *cameraController)
{
	SET_REF_COUNTED_POINTER(this->cameraController, cameraController)
	
	// assume camera has changed...
	lastCameraController = NULL;
}

ICameraController *World::GetCameraController()
{
	return cameraController;
}

void World::AddTransformable(ITransformable *transformable)
{
	if (!transformable)
		return;
	
	// ensure not already present
	if (transformables.Contains(transformable))
	{
		WARN << "Transformable already added.";
		return;
	}
	
	transformable->grab();
	transformables.Insert(transformable);
	
	// Enable the scene graph.
	// i.e. give the transformable a pointer to the world. So it can remove children.
	
	transformable->EnableSceneGraph(this);
	
	// Specific types
	
	if (IGraphic *graphic = dynamic_cast<IGraphic *>(transformable))
		graphics.Insert(graphic);
	
	if (ICharacter *character = dynamic_cast<ICharacter *>(transformable))
		characters.Insert(character);
	
	if (ISensor *sensor = dynamic_cast<ISensor *>(transformable))
		sensors.Insert(sensor);
	
	if (ISoundSource *soundSource = dynamic_cast<ISoundSource *>(transformable))
		soundSources.Insert(soundSource);
}

void World::RemoveTransformable(ITransformable *transformable)
{
	if (!transformables.Contains(transformable))
	{
		WARN << "Specified transformable was not found.";
		return;
	}
	
	// Pointer is found and therefore is valid, can now check for parent.
	
	if (transformable->GetParent())
	{
		// if it has a parent, then we must remove from the parent first.
		// (parent->RemoveChild will then call RemoveTransformable)
		transformable->GetParent()->RemoveChild(transformable);
		return;
	}
	
	// Ok, transformable doesn't have a parent, so we can remove it right away.
	// Pointer is valid, so now remove specific types.
	
	if (IGraphic *graphic = dynamic_cast<IGraphic *>(transformable))
		graphics.Remove(graphic);
	
	if (ICharacter *character = dynamic_cast<ICharacter *>(transformable))
		characters.Remove(character);
	
	if (ISensor *sensor = dynamic_cast<ISensor *>(transformable))
		sensors.Remove(sensor);
	
	if (ISoundSource *soundSource = dynamic_cast<ISoundSource *>(transformable))
		soundSources.Remove(soundSource);
	
	// Remove from main transformables list
	// Must erase *then* drop as transformable's destructor might
	// want to call this method.
	transformables.Remove(transformable);
	transformable->drop();
}

void World::QueueForRemoval(ITransformable *transformable)
{
	removalQueue.push_back(transformable);
}

void World::RemoveAllTransformables()
{
	// Erase each entity individually rather than looping through
	// This allows for the case where a transformable might remove another
	// when it destructs
	
	while (transformables.size())
		RemoveTransformable(transformables.GetAnyForRemoval());
	
	// The recreate camera if not NULL
	// (will be NULL if this is called from destructor)
	if (camera)
		AddTransformable(camera);
}

IMesh *World::AddMesh(const c8 *meshName)
{
	IMesh *mesh = new Mesh(
			engine->GetIrrlichtDevice()->getSceneManager(),
			meshName
			);
	AddTransformable(mesh);
	mesh->drop();
	return mesh;
}

INodeHandler *World::AddIrrNodeHandler(scene::ISceneNode *irrNode, bool removeOnDestruct)
{
	INodeHandler *handler = new NodeHandler(irrNode,removeOnDestruct);
	AddTransformable(handler);
	handler->drop();
	return handler;
}

ITransformable *World::AddTransform()
{
	ITransformable *transformable = new ITransformable();
	AddTransformable(transformable);
	transformable->drop();
	return transformable;
}

ICharacter *World::AddCharacter(IMesh *mesh, s32 idAnimIdle, s32 idAnimWalk, f32 height)
{
	ICharacter *character = new Character(this, physics, mesh, idAnimIdle, idAnimWalk, height);
	AddTransformable(character);
	character->drop();
	return character;
}

IProxyTransformable *World::AddProxyTransformable(ITransformable *other)
{
	IProxyTransformable *pt = new ProxyTransformable(this,other);
	AddTransformable(pt);
	pt->drop();
	return pt;
}

ISoundSource *World::AddSoundSource()
{
	ISoundSource *ss = new SoundSource(engine->GetSoundSystem());
	AddTransformable(ss);
	ss->drop();
	
	// Maybe start it paused
	if (IsPaused())
		ss->Pause();
	
	return ss;
}

IMotionSensor *World::AddMotionSensor()
{
	IMotionSensor *ms = new MotionSensor();
	AddTransformable(ms);
	ms->drop();
	return ms;
}

IMotionSensor *World::AddSoundMotionSensor(const c8 *startSound, const c8 *stopSound, const c8 *motionLoopSound)
{
	// A motion sensor with a sound source as a child.
	IMotionSensor *ms = AddMotionSensor();
	ISoundSource *ss = AddSoundSource();
	ms->AddChild(ss);
	
	// motion sensor callback has a pointer to the sound source
	SoundMotionCallback *callback = new SoundMotionCallback(
			ss,
			startSound, stopSound, motionLoopSound
			);
	ms->SetAnyMotionCallback(callback);
	callback->drop();
	
	// return the motion sensor
	return ms;
}

IInputProfile *World::CreateInputProfile(int buttonCount, int axesCount)
{
	return new InputProfile(buttonCount, axesCount, engine);
}

ICharacterController *World::CreateUserCharacterController(
			int idButtonWalkForwards,int idButtonWalkBackwards, int idButtonWalkLeft, int idButtonWalkRight,
			ITransformable *moveRelativeTo)
{
	return new UserCharacterController(this,
			idButtonWalkForwards, idButtonWalkBackwards, idButtonWalkLeft, idButtonWalkRight,
			moveRelativeTo
			);
}

IThirdPersonCameraController *World::CreateThirdPersonCameraController(f32 distance, f32 heightOffset,
		f32 minPitch, f32 maxPitch, f32 turnSpeed)
{
	return new ThirdPersonCameraController( this, engine->GetIrrlichtDevice()->getSceneManager(),
			distance, heightOffset, minPitch, maxPitch, turnSpeed );
}

IMotionAnimator *World::CreateRotationAnimator(core::vector3df speed)
{
	return new RotationAnimator(speed);
}

IBobAnimator *World::CreateBobAnimator(f32 amplitude, f32 speed)
{
	return new BobAnimator(amplitude,speed);
}

void World::SetSkyBox(const core::stringc &top, const core::stringc &bottom, const core::stringc &left,
		const core::stringc &right, const core::stringc &front, const core::stringc &back)
{
	scene::ISceneManager *smgr = engine->GetIrrlichtDevice()->getSceneManager();
	video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
	
	if (skyBoxNode)
		skyBoxNode->remove();
	
	skyBoxNode = smgr->addSkyBoxSceneNode(
			driver->getTexture( top.c_str() ),
			driver->getTexture( bottom.c_str() ),
			driver->getTexture( left.c_str() ),
			driver->getTexture( right.c_str() ),
			driver->getTexture( front.c_str() ),
			driver->getTexture( back.c_str() )
			);
	
	SetSkyBoxShader(skyBoxShader);
}

void World::SetSkyBoxShader(IShader *shader)
{
	if (skyBoxNode)
	{
		if (shader)
		{
			for (u32 i = 0; i < skyBoxNode->getMaterialCount(); i ++)
				shader->ApplyToIrrMaterial( skyBoxNode->getMaterial(i) );
		}
		else
		{
			if (skyBoxShader)
			{
				for (u32 i = 0; i < skyBoxNode->getMaterialCount(); i ++)
					skyBoxShader->ClearFromIrrMaterial( skyBoxNode->getMaterial(i), video::EMT_SOLID );
			}
		}
		
		SET_REF_COUNTED_POINTER(skyBoxShader, shader);
	}
}

video::SMaterial &World::GetSkyBoxMaterial(u32 i)
{
	ASSERT( i < 6 );
	ASSERT( skyBoxNode );
	
	return skyBoxNode->getMaterial(i);
}

void World::ClearSkyBox()
{
	if (skyBoxNode)
	{
		skyBoxNode->remove();
		skyBoxNode = NULL;
	}
}

void World::OnPause()
{
	IUpdatable::OnPause();
	
	// Need to pause all in-world sounds, but *NOT* the entire sound manager.
	for (u32 i = 0; i < soundSources.size(); i ++)
		soundSources[i]->Pause();
	
	// Stop all logic-graphics interpolation
	// This will cause a millisecond pause when resuming, but that is acceptable.
	
	for (u32 i = 0; i < transformables.size(); i ++)
		transformables[i]->CacheInterpolatableState();
	
	// Pause animation and other graphics by stopping Irrlicht's virtual timer
	engine->GetIrrlichtDevice()->getTimer()->stop();
	
	// Pause input
	if (inputProfile)
		inputProfile->Pause();
	
	// Little need for centring if world is paused.
	engine->SetAutoMouseCentring(false,false);
}

void World::OnResume()
{
	IUpdatable::OnResume();
	
	// Resume sounds
	for (u32 i = 0; i < soundSources.size(); i ++)
		soundSources[i]->Resume();
	
	// Resume Irrlicht graphics
	engine->GetIrrlichtDevice()->getTimer()->start();
	
	// Resume input
	if (inputProfile)
		inputProfile->Resume();
	
	// Probably going to have a mouse controlled camera.
	engine->SetAutoMouseCentring(true,true);
	
	// Reset mouse delta
	engine->CentreMouse();
}

void World::Update(f32 dt)
{
	if (IsPaused())
		return;
	
	// Send input events to subscribers.
	
	if (inputProfile)
		inputProfile->ProcessAndNotify(inputSubscribers);
	
	// Cache transforms (and other interpolated state) at this point in time.
	// (for interpolation in render task)
	// We now cache *all* transformable's transforms (not just IGraphics)
	// This is because a child in the scene node may rely on its parent's interpolated transform,
	// yet parent does not have to be a graphic.
	// So all nodes must be able to be interpolated if necessary.
	
	for (u32 i = 0; i < transformables.size(); i ++)
	{
		// Calculate velocity *before* caching the new position etc.
		// So velocity has a lag of one frame.
		// This is required since the final new position is not known yet
		// (and the final position may change at various points during this Update)
		transformables[i]->CalculateVelocities(dt);
		
		transformables[i]->CacheInterpolatableState();
	}
	
	// Step physics! (updates bodies)
	
	physics->Step(dt);
	
	// Update characters and character controllers.
	
	for (u32 i = 0; i < characters.size(); i ++)
	{
		if (characters[i]->GetController())
			characters[i]->GetController()->Update(characters[i]);
		
		characters[i]->Update(dt);
	}
	
	// Update sensors
	
	for (u32 i = 0; i < sensors.size(); i ++)
		sensors[i]->Update(dt);
	
	
	// Update other updatables after all ITransformables but before camera in case any Updatables
	// change the positions of any characters etc..
	// (camera will want to watch these new positions)
	IUpdatable::Update(dt);
	
	
	// Update camera controller?
	
	if (cameraController)
	{
		cameraController->Update(camera, dt);
		
		// If this is the first update with a newly set camera controller,
		// we need to ApplyTransformNow so that there is no huge velocity
		// as camera jumps to new location.
		if (cameraController != lastCameraController)
		{
			lastCameraController = cameraController;
			camera->ApplyTransformNow();
		}
	}
	
	// Update animators on every transformable
	
	for (u32 i = 0; i < transformables.size(); i ++)
	{
		const std::vector<IMotionAnimator *> &animators = transformables[i]->GetAnimators();
		
		for (u32 j = 0; j < animators.size(); j ++)
			animators[j]->Animate(transformables[i], dt);
	}
	
	// Update sound listener to be at camera position.
	// Doesn't need interpolation, no human will notice.
	if (ITransformable *listener = camera)
	{
		ISoundSystem *soundSystem = engine->GetSoundSystem();
		
		soundSystem->SetListenerPosition( listener->GetAbsolutePosition() );
		
		soundSystem->SetListenerOrientation(
				// Look vector
				listener->GetAbsoluteRotation().rotationToDirection(core::vector3df(0,0,1)),
				// Up vector
				listener->GetAbsoluteRotation().rotationToDirection(core::vector3df(0,1,0))
				);
		
		soundSystem->SetListenerVelocity( listener->GetAbsoluteLinearVelocity() );
	}
	
	// Update sound sources.
	
	for (u32 i = 0; i < soundSources.size(); i ++)
		soundSources[i]->Update(dt);
	
	
	// Remove anything queued for removal.
	while (removalQueue.size())
	{
		RemoveTransformable(removalQueue.front());
		removalQueue.pop_front();
	}
}
