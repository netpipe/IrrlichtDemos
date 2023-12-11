
#include "IWorld.h"
#include <deque>

class IEngine;
class Physics;
class PhysicsTask;
class LogicTask;
class RenderTask;
class IGraphic;
class ISensor;

class World : public IWorld
{
	IEngine *engine;
	Physics *physics;
	
	Set<IWantInput *> inputSubscribers;
	
	IInputProfile *inputProfile;
	ICameraController *cameraController;
	ICameraController *lastCameraController;
	ICamera *camera;
	
	// Transformables
	Set<ITransformable *> transformables;
	// All those in these sub lists are also present in the main transformables list.
	Set<IGraphic *> graphics;
	Set<ICharacter *> characters;
	Set<ISensor *> sensors;
	Set<ISoundSource *> soundSources;
	
	// Waiting for removal
	std::deque<ITransformable *> removalQueue;
	
	// Some world effects
	scene::ISceneNode *skyBoxNode;
	IShader *skyBoxShader;
	
protected:
	void OnPause();
	void OnResume();
	
public:
	World();
	~World();
	
	// Used by render task.
	const Set<IGraphic *> &GetAllGraphics() { return graphics; }
	
	IPhysics *GetPhysics();
	ICamera *GetCamera();
	
	void SetInputProfile(IInputProfile *profile);
	IInputProfile *GetInputProfile();
	
	void SubscribeToInput(IWantInput *wantInput);
	void UnsubscribeFromInput(IWantInput *wantInput);
	
	void SetCameraController(ICameraController *cameraController);
	ICameraController *GetCameraController();
	
	void AddTransformable(ITransformable *transformable);
	void RemoveTransformable(ITransformable *transformable);
	void QueueForRemoval(ITransformable *transformable);
	void RemoveAllTransformables();
	
	IMesh *AddMesh(const c8 *meshName);
	
	INodeHandler *AddIrrNodeHandler(scene::ISceneNode *irrNode, bool removeOnDestruct);
	
	ITransformable *AddTransform();
	
	ICharacter *AddCharacter(IMesh *mesh, s32 idAnimIdle, s32 idAnimWalk, f32 height);
	
	IProxyTransformable *AddProxyTransformable(ITransformable *other);
	
	ISoundSource *AddSoundSource();
	
	IMotionSensor *AddMotionSensor();
	
	IMotionSensor *AddSoundMotionSensor(const c8 *startSound, const c8 *stopSound, const c8 *motionLoopSound);
	
	IInputProfile *CreateInputProfile(int buttonCount, int axesCount);
	
	ICharacterController *CreateUserCharacterController(
			int idButtonWalkForwards,int idButtonWalkBackwards, int idButtonWalkLeft, int idButtonWalkRight,
			ITransformable *moveRelativeTo);
	
	IThirdPersonCameraController *CreateThirdPersonCameraController(f32 distance, f32 heightOffset,
			f32 minPitch, f32 maxPitch, f32 turnSpeed);
	
	IMotionAnimator *CreateRotationAnimator(core::vector3df speed);
	IBobAnimator *CreateBobAnimator(f32 amplitude, f32 speed);
	
	void SetSkyBox(const core::stringc &top, const core::stringc &bottom, const core::stringc &left,
			const core::stringc &right, const core::stringc &front, const core::stringc &back);
	
	void SetSkyBoxShader(IShader *shader);
	video::SMaterial &GetSkyBoxMaterial(u32 i);
	
	void ClearSkyBox();
	
	void Update(f32 dt);
};

