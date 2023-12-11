
#ifndef I_WORLD_H
#define I_WORLD_H

#include "litha_internal.h"
#include "IUpdatable.h"

class IPhysics;
class IInputProfile;
class IMesh;
class IBody;
class IWantInput;

class ITransformable;
class ICharacter;
class IProxyTransformable;
class INodeHandler;
class ICamera;
class ISoundSource;
class IShader;

// Character controller
class ICharacterController;

// Camera controller
class ICameraController;
class IThirdPersonCameraController;

// Animators
class IMotionAnimator;
class IBobAnimator;

// Sensors
class IMotionSensor;

class IWorld : public virtual IReferenceCounted, public IUpdatable
{
public:
	virtual ~IWorld(){}
	
	// Single instance things
	virtual IPhysics *GetPhysics() = 0;
	virtual ICamera *GetCamera() = 0;
	
	// Input
	virtual void SetInputProfile(IInputProfile *profile) = 0;
	virtual IInputProfile *GetInputProfile() = 0;
	
	// Note: Input is cached and processed during every logic update.
	virtual void SubscribeToInput(IWantInput *wantInput) = 0;
	virtual void UnsubscribeFromInput(IWantInput *wantInput) = 0;
	
	// Camera
	virtual void SetCameraController(ICameraController *cameraController) = 0;
	virtual ICameraController *GetCameraController() = 0;
	
	// Add a user implemented transformable. (enables things like animators and scene graph).
	virtual void AddTransformable(ITransformable *transformable) = 0;
	
	// Remove a transformable. This includes graphics, characters...
	virtual void RemoveTransformable(ITransformable *transformable) = 0;
	
	// Remove a transformable, but not right now.
	// Useful if currently doing operations within the transformable.
	// (e.g. an Animator)
	virtual void QueueForRemoval(ITransformable *transformable) = 0;
	
	// Removes all *except* the camera, which is a special case ITransformable with only ever one instance.
	virtual void RemoveAllTransformables() = 0;
	
	// Note: IMesh is not an Irrlicht irr::scene::IMesh
	virtual IMesh *AddMesh(const c8 *meshName) = 0;
	
	// Provides smooth interpolated motion for an Irrlicht scene node.
	// (so position etc can be changed at fixed FPS of logic task)
	// removeOnDestruct - if true, will remove the Irrlicht scene node when it is itself removed.
	virtual INodeHandler *AddIrrNodeHandler(scene::ISceneNode *irrNode, bool removeOnDestruct = true) = 0;
	
	// A dummy node that does nothing except provide a transform.
	virtual ITransformable *AddTransform() = 0;
	
	// If height is not given then the character body geometry will be determined from the
	// mesh bounding box.
	virtual ICharacter *AddCharacter(IMesh *mesh, s32 idAnimIdle, s32 idAnimWalk, f32 height = 0.0) = 0;
	
	// A proxy transformable. Generally not all that useful.
	// Provides an offset when callling Apply/Set position etc.
	// When removed also removes the other entity.
	virtual IProxyTransformable *AddProxyTransformable(ITransformable *other) = 0;
	
	// A thing for playing sound at a point in the world.
	virtual ISoundSource *AddSoundSource() = 0;
	
	virtual IMotionSensor *AddMotionSensor() = 0;
	
	// Creates a motion sensor with a callback implementation that runs a sound when in motion.
	// startSound, stopSound - sound played once on start/stop
	// motionLoop - sound played looped while in motion.
	virtual IMotionSensor *AddSoundMotionSensor(const c8 *startSound, const c8 *stopSound, const c8 *motionLoopSound) = 0;
	
	virtual IInputProfile *CreateInputProfile(int buttonCount, int axesCount) = 0;//maybe move to IEngine?
	
	// button list, and the moveRelativeTo Entity (e.g. a third person camera entity). Uses the
	// Entity's angle.
	// Without a moveRelativeTo entity given, the character moves only along the X,Z axes.
	virtual ICharacterController *CreateUserCharacterController(
			int idButtonWalkForwards,int idButtonWalkBackwards, int idButtonWalkLeft, int idButtonWalkRight,
			ITransformable *moveRelativeTo = NULL) = 0;
	
	virtual IThirdPersonCameraController *CreateThirdPersonCameraController(f32 distance = 8.0, f32 heightOffset = 0.0,
			f32 minPitch = -60.0, f32 maxPitch = 60.0, f32 turnSpeed = 120.0) = 0;
	
	// Speed is in degrees / second
	// Currently speed is limited to 360 degrees/second. (is that a bug?)
	virtual IMotionAnimator *CreateRotationAnimator(core::vector3df speed) = 0;
	
	// Bob up and down vertically (Y axis) using a sine wave.
	virtual IBobAnimator *CreateBobAnimator(f32 amplitude, f32 speed) = 0;
	
	// Some world effects
	virtual void SetSkyBox(const core::stringc &top, const core::stringc &bottom, const core::stringc &left,
			const core::stringc &right, const core::stringc &front, const core::stringc &back) = 0;
	
	// Assuming a sky box has already been set, this allows you to set a shader for it.
	// Sets for all materials.
	virtual void SetSkyBoxShader(IShader *shader) = 0;
	
	// Get sky box material. Indices 0-5 inclusive. (6 in total)
	// Will fail if no sky box is set.
	virtual video::SMaterial &GetSkyBoxMaterial(u32 i) = 0;
	
	// Remove the sky box.
	virtual void ClearSkyBox() = 0;
};

#endif

