#ifndef _VEHICLE_SCENE_NODE_H_
#define _VEHICLE_SCENE_NODE_H_

#include <irrlicht.h>

using namespace irr;
using namespace scene;
//#include "ISceneNode.h"


#ifndef VEHICLE_SCENE_NODE_PROPERTIES
#define VEHICLE_SCENE_NODE_PROPERTIES
#define MAX_STEER_ANGLE_DEGS 40.0f
#define MAX_REVOLUTION_PER_SEC 10.0f
#define RATE_CONSTANT 5.0f
#define STOPPING_INTERPOLATE 0.9f
#endif


struct VehicleSceneNodeParams
{
VehicleSceneNodeParams()
:	BodyMeshFile(L""),
	TireMeshFile(L""),

	BodyMeshRot(0.0f),
	FrontRightTireRot(0.0f),
	FrontLeftTireRot(0.0f),
	RearRightTireRot(0.0f),
	RearLeftTireRot(0.0f),

	TireScale(1.0f),
	TireOffsetDx(0.5f),
	TireOffsetDy(0.5f),
	TireOffsetDz1(-0.5f),
	TireOffsetDz2(0.5f)
{}

core::stringw BodyMeshFile;
core::stringw TireMeshFile;

core::vector3df BodyMeshRot;
core::vector3df FrontRightTireRot;
core::vector3df FrontLeftTireRot;
core::vector3df RearRightTireRot;
core::vector3df RearLeftTireRot;

f32 TireScale;
f32 TireOffsetDx;
f32 TireOffsetDy;
f32 TireOffsetDz1;
f32 TireOffsetDz2;

void print();


};


class VehicleSceneNode: public scene::IMeshSceneNode
{

	public:

		 IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh=0,
		s32 id=-1, bool zfailmethod=true, f32 infinity=1000.0f){};

	static
	VehicleSceneNode* loadFromFile(IrrlichtDevice* device, const core::stringc& filename);

	static
	void saveToFile(IrrlichtDevice* device, VehicleSceneNode* vehicle, const core::stringc& filename);

	static
	bool loadParamsFromFile
	(
		IrrlichtDevice* device,
		const core::stringc& filename,
		VehicleSceneNodeParams& params
	);

	static
	bool saveParamsToFile
	(
		IrrlichtDevice* device,
		const core::stringc& filename,
		const VehicleSceneNodeParams& params
	);

	static
	core::stringw stringFromF32(f32 val);

	VehicleSceneNode
	(
		scene::ISceneNode* parent,
		scene::ISceneManager* smgr,
		s32 id = -1,
		const core::vector3df& position = core::vector3df(0.0f),
		const core::vector3df& rotation = core::vector3df(0.0f),
		const core::vector3df& scale = core::vector3df(1.0f)
	);


	virtual ~VehicleSceneNode();
	virtual void setup(IrrlichtDevice* device, const VehicleSceneNodeParams& params);

	virtual void stop();
	virtual void speedUp();
	virtual void slowDown();

	virtual void setPosition(const core::vector3df& newpos);
	virtual core::vector3df getAbsolutePosition() const;
	virtual void setMesh(scene::IMesh* mesh);
	virtual scene::IMesh* getMesh(void);
	virtual void setReadOnlyMaterials(bool readonly);
	virtual bool isReadOnlyMaterials() const;

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);
	virtual void OnAnimate(u32 timeMs);
	void update(u32 deltaTime);

	void steerFrontWheels(f32 degs);
	void pitchBody(f32 degs);
	void rollBody(f32 degs);
	void rotate(scene::ISceneNode* node, const core::vector3df& rot);
	void resetPitch();
	f32 clampAngleDegs(f32 degs);

	//bool isResettingPitch() const { return IsResettingPitch; }

	bool isVisible() const { return true; }

	void setBodyMeshFile(IrrlichtDevice* device, const core::stringc& filename);
	void setTireMeshFile(IrrlichtDevice* device, const core::stringc& filename);

	void setBodyMeshRotation(const core::vector3df& rot);
	void setTireOffsets(f32 dx, f32 dy, f32 dz1, f32 dz2);
	void setTireScale(f32 scale);

	void setFrontRightTireRotation(const core::vector3df& rot);
	void setFrontLeftTireRotation(const core::vector3df& rot);
	void setRearRightTireRotation(const core::vector3df& rot);
	void setRearLeftTireRotation(const core::vector3df& rot);

	// setters + getters:
	void setBody(scene::IMeshSceneNode* node) { Body = node; }
	void setInterior(scene::IMeshSceneNode* node) { Interior = node; }
	void setFrontRightTire(scene::IMeshSceneNode* node) { FrontRightTire = node; }
	void setFrontLeftTire(scene::IMeshSceneNode* node) { FrontLeftTire = node; }
	void setRearRightTire(scene::IMeshSceneNode* node) { RearRightTire = node; }
	void setRearLeftTire(scene::IMeshSceneNode* node) { RearLeftTire = node; }
	void setRootEmpty(scene::ISceneNode* node) { RootEmpty = node; }
	void setFrontRightTireEmpty(scene::ISceneNode* node) { FrontRightTireEmpty = node; }
	void setFrontLeftTireEmpty(scene::ISceneNode* node) { FrontLeftTireEmpty = node; }
	void setRearRightTireEmpty(scene::ISceneNode* node) { RearRightTireEmpty = node; }
	void setRearLeftTireEmpty(scene::ISceneNode* node) { RearLeftTireEmpty = node; }



	scene::IMeshSceneNode* getBody() const { return Body; }
	scene::IMeshSceneNode* getInterior() const { return Interior; }
	scene::IMeshSceneNode* getFrontRightTire() const { return FrontRightTire; }
	scene::IMeshSceneNode* getFrontLeftTire() const { return FrontLeftTire; }
	scene::IMeshSceneNode* getRearRightTire() const { return RearRightTire; }
	scene::IMeshSceneNode* getRearLeftTire() const { return RearLeftTire; }
	scene::ISceneNode* getRootEmpty() const { return RootEmpty; }
	scene::ISceneNode* getBodyPitchEmpty() const { return BodyPitchEmpty; }
	scene::ISceneNode* getBodyRollEmpty() const { return BodyRollEmpty; }
	scene::ISceneNode* getFrontRightTireEmpty() const { return FrontRightTireEmpty; }
	scene::ISceneNode* getFrontLeftTireEmpty() const { return FrontLeftTireEmpty; }
	scene::ISceneNode* getRearRightTireEmpty() const { return RearRightTireEmpty; }
	scene::ISceneNode* getRearLeftTireEmpty() const { return RearLeftTireEmpty; }


	void setTireOffsetDx(f32 offset);
	void setTireOffsetDy(f32 offset);
	void setTireOffsetDz1(f32 offset);
	void setTireOffsetDz2(f32 offset);

	f32 getTireOffsetDx() const { return TireOffsetDx; }
	f32 getTireOffsetDy() const { return TireOffsetDy; }
	f32 getTireOffsetDz1() const { return TireOffsetDz1; }
	f32 getTireOffsetDz2() const { return TireOffsetDz2; }

	f32 getTireScale() const { return TireScale; }

	protected:


	private:
	scene::ISceneManager* Smgr;

	scene::IMeshSceneNode* Body;
	scene::IMeshSceneNode* Interior;
	scene::IMeshSceneNode* FrontRightTire;
	scene::IMeshSceneNode* FrontLeftTire;
	scene::IMeshSceneNode* RearRightTire;
	scene::IMeshSceneNode* RearLeftTire;

	scene::ISceneNode* RootEmpty;
	scene::ISceneNode* BodyPitchEmpty;
	scene::ISceneNode* BodyRollEmpty;
	scene::ISceneNode* FrontRightTireEmpty;
	scene::ISceneNode* FrontLeftTireEmpty;
	scene::ISceneNode* RearRightTireEmpty;
	scene::ISceneNode* RearLeftTireEmpty;

	// rotations
	core::vector3df BodyRotation;
	core::vector3df InteriorRotation;
	core::vector3df FrontRightTireRotation;
	core::vector3df FrontLeftTireRotation;
	core::vector3df RearRightTireRotation;
	core::vector3df RearLeftTireRotation;

	scene::IMesh* BodyMesh;
	scene::IMesh* InteriorMesh;
	scene::IMesh* TireMesh;

	video::SMaterial BodyMaterial;
	video::SMaterial InteriorMaterial;
	video::SMaterial TireMaterial;

	f32 TireOffsetDx;
	f32 TireOffsetDy;
	f32 TireOffsetDz1;
	f32 TireOffsetDz2;

	f32 TireScale;

	core::aabbox3d<f32> Box;
	f32 SteerAngleDegs;
	f32 RevolutionPerSec;

	u32 ResetPitchElapsedTime;
	f32 OldPitchAngleDegs;
	bool IsResettingPitch;

	void setupMaterial(scene::ISceneNode* node);
};







#endif // _VEHICLE_SCENE_NODE_H_



