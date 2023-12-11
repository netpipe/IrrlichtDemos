#include <sstream>

#include "vehicle_scene_node.h"


using namespace irr;

void VehicleSceneNodeParams::print()
{
	char* buf = new char[100];
	
	wcstombs(buf, BodyMeshFile.c_str(), 100);
	printf("BodyMeshFile = %s\n", buf);
	
	wcstombs(buf, TireMeshFile.c_str(), 100);
	printf("TireMeshFile = %s\n", buf);
	
	printf("BodyMeshRot = (%.2f,%.2f,%.2f)\n", BodyMeshRot.X, BodyMeshRot.Y, BodyMeshRot.Z);
	
	printf("FrontRightTireRot = (%.2f,%.2f,%.2f)\n", FrontRightTireRot.X, FrontRightTireRot.Y, FrontRightTireRot.Z);
	printf("FrontLeftTireRot = (%.2f,%.2f,%.2f)\n", FrontLeftTireRot.X, FrontLeftTireRot.Y, FrontLeftTireRot.Z);
	printf("RearRightTireRot = (%.2f,%.2f,%.2f)\n", RearRightTireRot.X, RearRightTireRot.Y, RearRightTireRot.Z);
	printf("RearLeftTireRot = (%.2f,%.2f,%.2f)\n", RearLeftTireRot.X, RearLeftTireRot.Y, RearLeftTireRot.Z);

	printf("TireScale = %.2f\n", TireScale);
	printf("TireOffsetDx = %.2f\n", TireOffsetDx);
	printf("TireOffsetDy = %.2f\n", TireOffsetDy);
	printf("TireOffsetDz1 = %.2f\n", TireOffsetDz1);
	printf("TireOffsetDz2 = %.2f\n", TireOffsetDz2);
	
	delete[] buf;
}

VehicleSceneNode::VehicleSceneNode
(
	scene::ISceneNode* parent,
	scene::ISceneManager* smgr,
	s32 id,
	const core::vector3df& position,
	const core::vector3df& rotation,
	const core::vector3df& scale
)
:	scene::IMeshSceneNode(parent ? parent : smgr->getRootSceneNode(), smgr, id, position, rotation, scale),
	Smgr(smgr),
	Body(0),
	Interior(0),
	FrontRightTire(0),
	FrontLeftTire(0),
	RearRightTire(0),
	RearLeftTire(0),

	RootEmpty(0),
	BodyPitchEmpty(0),
	BodyRollEmpty(0),
	FrontRightTireEmpty(0),
	FrontLeftTireEmpty(0),
	RearRightTireEmpty(0),
	RearLeftTireEmpty(0),
	
	//BodyRotation(0.0f, 180.0f, 0.0f),
	BodyRotation(0.0f, 0.0f, 0.0f),
	InteriorRotation(0.0f, 0.0f, 0.0f),
	FrontRightTireRotation(0.0f, 0.0f, 0.0f),
	FrontLeftTireRotation(0.0f, 0.0f, 0.0f),
	RearRightTireRotation(0.0f, 0.0f, 0.0f),
	RearLeftTireRotation(0.0f, 0.0f, 0.0f),

	BodyMesh(0),
	InteriorMesh(0),
	TireMesh(0),
	
	//TireScale(1.0f),
	TireScale(0.65f),
	SteerAngleDegs(0.0f),
	RevolutionPerSec(0.0f),
	
	ResetPitchElapsedTime(0),
	OldPitchAngleDegs(0.0f),
	IsResettingPitch(false)

{
	// setup hierarchy of nodes:
	RootEmpty = Smgr->addEmptySceneNode(Parent);
	BodyPitchEmpty = Smgr->addEmptySceneNode(RootEmpty);
	BodyRollEmpty = Smgr->addEmptySceneNode(BodyPitchEmpty);
	
	Body = Smgr->addMeshSceneNode(BodyMesh, BodyRollEmpty, -1, core::vector3df(0.0f), core::vector3df(0.0f), core::vector3df(1.0f), true);
	
	
	//Interior = Smgr->addMeshSceneNode(InteriorMesh, RootEmpty);
	Interior = Smgr->addMeshSceneNode
	(
		0, RootEmpty, -1,
		core::vector3df(0.0f), core::vector3df(0.0f), core::vector3df(1.0f),
		true
	);
	
	FrontRightTireEmpty = Smgr->addEmptySceneNode(RootEmpty);
	FrontLeftTireEmpty = Smgr->addEmptySceneNode(RootEmpty);

	RearRightTireEmpty = Smgr->addEmptySceneNode(RootEmpty);
	RearLeftTireEmpty = Smgr->addEmptySceneNode(RootEmpty);

	
	FrontRightTire = Smgr->addMeshSceneNode(TireMesh, FrontRightTireEmpty, -1, core::vector3df(0.0f), core::vector3df(0.0f), core::vector3df(1.0f), true);
	FrontLeftTire = Smgr->addMeshSceneNode(TireMesh, FrontLeftTireEmpty, -1, core::vector3df(0.0f), core::vector3df(0.0f), core::vector3df(1.0f), true);
	RearRightTire = Smgr->addMeshSceneNode(TireMesh, RearRightTireEmpty, -1, core::vector3df(0.0f), core::vector3df(0.0f), core::vector3df(1.0f), true);
	RearLeftTire = Smgr->addMeshSceneNode(TireMesh, RearLeftTireEmpty, -1, core::vector3df(0.0f), core::vector3df(0.0f), core::vector3df(1.0f), true);
	
	
	
	
	this->setTireScale(TireScale);
	//FrontRightTire->setDebugDataVisible(scene::EDS_BBOX);
	//FrontLeftTire->setDebugDataVisible(scene::EDS_BBOX);
	
	this->setTireOffsets(1.0f, 1.0f, 1.0f, -1.0f);
	
}



VehicleSceneNode::~VehicleSceneNode()
{
}	

void VehicleSceneNode::setup(IrrlichtDevice* device, const VehicleSceneNodeParams& params)
{
	this->setBodyMeshFile(device, params.BodyMeshFile);
	this->setTireMeshFile(device, params.TireMeshFile);
	
	
	core::vector3df rot0 = params.BodyMeshRot;
	core::vector3df rot1 = params.FrontRightTireRot;
	core::vector3df rot2 = params.FrontLeftTireRot;
	core::vector3df rot3 = params.RearRightTireRot;
	core::vector3df rot4 = params.RearLeftTireRot;
	
	this->setBodyMeshRotation(rot0);
	this->setFrontRightTireRotation(rot1);
	this->setFrontLeftTireRotation(rot2);
	this->setRearRightTireRotation(rot3);
	this->setRearLeftTireRotation(rot4);

	this->setTireOffsets(params.TireOffsetDx, params.TireOffsetDy, params.TireOffsetDz1, params.TireOffsetDz2);
	this->setTireScale(params.TireScale);

}

void VehicleSceneNode::stop() { RevolutionPerSec = 0.0f; }
	
void VehicleSceneNode::speedUp()
{
	RevolutionPerSec += 0.1f;
	if (RevolutionPerSec > MAX_REVOLUTION_PER_SEC)
	{
		RevolutionPerSec = MAX_REVOLUTION_PER_SEC;
		return;
	}
	this->pitchBody(0.1f);
}
	
void VehicleSceneNode::slowDown()
{
	RevolutionPerSec -= 0.1f;
	if (RevolutionPerSec < 0.0f)
	{
		RevolutionPerSec = 0.0f;
		return;
	}
	this->pitchBody(-0.1f);
}

void VehicleSceneNode::setPosition(const core::vector3df& newpos)
{
	RootEmpty->setPosition(newpos);
}

core::vector3df VehicleSceneNode::getAbsolutePosition() const
{
	return RootEmpty->getAbsolutePosition();

}

void VehicleSceneNode::setMesh(scene::IMesh* mesh)
{

}


scene::IMesh* VehicleSceneNode::getMesh()
{
	return BodyMesh;
}


void VehicleSceneNode::setReadOnlyMaterials(bool readonly)
{

}


bool VehicleSceneNode::isReadOnlyMaterials() const
{
	return true;
}


void VehicleSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
		SceneManager->registerNodeForRendering(Body);
		SceneManager->registerNodeForRendering(Interior);
		SceneManager->registerNodeForRendering(FrontRightTire);
		SceneManager->registerNodeForRendering(FrontLeftTire);
		SceneManager->registerNodeForRendering(RearRightTire);
		SceneManager->registerNodeForRendering(RearLeftTire);
	}
	scene::IMeshSceneNode::OnRegisterSceneNode();
}



void VehicleSceneNode::render()
{
}


const core::aabbox3d<f32>& VehicleSceneNode::getBoundingBox() const
{
	return Box;
}


u32 VehicleSceneNode::getMaterialCount() const
{
	return 3;
}


video::SMaterial& VehicleSceneNode::getMaterial(u32 i)
{
	if (i == 0)
		return BodyMaterial;
	else if (i == 1)
		return InteriorMaterial;
	else if (i == 2)
		return TireMaterial;
	return BodyMaterial;
}


void VehicleSceneNode::OnAnimate(u32 timeMs)
{
}


void VehicleSceneNode::update(u32 deltaTime)
{
	if (IsResettingPitch)
	{
		f32 t = ResetPitchElapsedTime * 0.001f;
		f32 interpolate = 1.0f - exp(- RATE_CONSTANT * t);
		
		f32 pitchAngleDegs = (1.0f - interpolate) * OldPitchAngleDegs;
		BodyPitchEmpty->setRotation(core::vector3df(pitchAngleDegs, 0.0f, 0.0f));
		
		if (interpolate > STOPPING_INTERPOLATE)
		{
			BodyPitchEmpty->setRotation(core::vector3df(0.0f));
			IsResettingPitch = false;
		}
		
		ResetPitchElapsedTime += deltaTime;
		//printf("ResetPitchElapsedTime=%i\n", ResetPitchElapsedTime);
		
	}
	
	if (core::iszero(RevolutionPerSec))
		return;
	
	f32 dt = deltaTime * 0.001f;
	f32 degs = RevolutionPerSec * dt * 2 * core::PI * core::RADTODEG;
	
	core::vector3df rot;
	
	rot = FrontRightTireEmpty->getRotation();
	rot.X += degs;
	FrontRightTireEmpty->setRotation(rot);

	rot = FrontLeftTireEmpty->getRotation();
	rot.X += degs;
	FrontLeftTireEmpty->setRotation(rot);
	
	rot = RearRightTireEmpty->getRotation();
	rot.X += degs;
	RearRightTireEmpty->setRotation(rot);

	rot = RearLeftTireEmpty->getRotation();
	rot.X += degs;
	RearLeftTireEmpty->setRotation(rot);	
	

}


void VehicleSceneNode::steerFrontWheels(f32 degs)
{
	if (fabs(SteerAngleDegs + degs) > MAX_STEER_ANGLE_DEGS)
		return;
	
	//IsResettingSteer = false;
	
	SteerAngleDegs += degs;
	
	core::vector3df rot;
	
	
	rot = FrontRightTireEmpty->getRotation();
	rot.Y = -SteerAngleDegs;
	FrontRightTireEmpty->setRotation(rot);

	rot = FrontLeftTireEmpty->getRotation();
	rot.Y = -SteerAngleDegs;
	FrontLeftTireEmpty->setRotation(rot);
	
	
	//resetSteer();
	
	// roll body:
	f32 rollDegs = - 0.2f * degs / 5.0f;
	this->rollBody(rollDegs);
}

void VehicleSceneNode::pitchBody(f32 degs)
{
	if (IsResettingPitch)
		IsResettingPitch = false;
	
	//FIXME: not tested!
	f32 z = BodyRollEmpty->getRotation().Z;
	if ((z>90.0f) && (z<270.0f))
		rotate(BodyPitchEmpty, core::vector3df(degs, 0.0f, 0.0f));
	else
		rotate(BodyPitchEmpty, core::vector3df(-degs, 0.0f, 0.0f));
}


void VehicleSceneNode::rollBody(f32 degs)
{
	rotate(BodyRollEmpty, core::vector3df(0.0f, 0.0f, degs));
}


void VehicleSceneNode::rotate(scene::ISceneNode* node, const core::vector3df& rot)
{
	core::matrix4 m;
	m.setRotationDegrees(node->getRotation());
	
	core::matrix4 n;
	n.setRotationDegrees(rot);
	m *= n;
	
	node->setRotation(m.getRotationDegrees());
}

void VehicleSceneNode::resetPitch()
{
	//printf("VehicleSceneNode::resetPitch()\n");
	IsResettingPitch = true;
	ResetPitchElapsedTime = 0;
	OldPitchAngleDegs = clampAngleDegs(BodyPitchEmpty->getRotation().X);
}

f32 VehicleSceneNode::clampAngleDegs(f32 degs)
{
	if ((degs >= -180.0f) && (degs <= 180.0f))
		return degs;
	
	while (degs > 180.0f)
		degs -= 360.0f;
	
	while (degs < -180.0f)
		degs += 360.0f;
	
	return degs;
}

void VehicleSceneNode::setBodyMeshFile(IrrlichtDevice* device, const core::stringc& filename)
{
	io::IFileSystem* fs = device->getFileSystem();
	core::stringc path = fs->getFileDir(filename);
	if (path != ".")
		fs->addFileArchive(path);
	//printf("path=%s\n", path.c_str());
	
	scene::IMesh* mesh = Smgr->getMesh(filename);
	if (!mesh)
		return;
	
	if (BodyMesh == mesh)
	{
		printf("BodyMesh == mesh\n");
		return;
	}
	BodyMesh = mesh;
	Body->setMesh(mesh);
	Body->setVisible(true);
	Body->setDebugDataVisible(scene::EDS_BBOX);
	
	
	BodyMaterial = Body->getMaterial(0);
	
	this->setupMaterial(Body);
	this->setBodyMeshRotation(core::vector3df(0.0f));
	
	Box.reset(0.0f, 0.0f, 0.0f);
	Box.addInternalBox(Body->getBoundingBox());
}


void VehicleSceneNode::setTireMeshFile(IrrlichtDevice* device, const core::stringc& filename)
{
	io::IFileSystem* fs = device->getFileSystem();
	core::stringc path = fs->getFileDir(filename);
	if (path != ".")
		fs->addFileArchive(path);
	
	scene::IMesh* mesh = Smgr->getMesh(filename);
	if (!mesh)
		return;
	
	if (TireMesh == mesh)
		return;
	
	TireMesh = mesh;
	FrontRightTire->setMesh(mesh);
	FrontLeftTire->setMesh(mesh);
	RearRightTire->setMesh(mesh);
	RearLeftTire->setMesh(mesh);
	
	TireMaterial = FrontRightTire->getMaterial(0);
	
	this->setupMaterial(FrontRightTire);
	this->setupMaterial(FrontLeftTire);
	this->setupMaterial(RearRightTire);
	this->setupMaterial(RearLeftTire);
	
	this->setTireScale(1.0f);

	this->setFrontRightTireRotation(core::vector3df(0.0f));
	this->setFrontLeftTireRotation(core::vector3df(0.0f));
	this->setRearRightTireRotation(core::vector3df(0.0f));
	this->setRearLeftTireRotation(core::vector3df(0.0f));

	Box.addInternalBox(FrontRightTire->getBoundingBox());
	Box.addInternalBox(FrontLeftTire->getBoundingBox());
	Box.addInternalBox(RearRightTire->getBoundingBox());
	Box.addInternalBox(RearLeftTire->getBoundingBox());

}


void VehicleSceneNode::setBodyMeshRotation(const core::vector3df& rot)
{
	if (!BodyMesh)
		return;
	
	core::vector3df oldBodyRotation = BodyRotation;
	BodyRotation = rot;
	
	core::matrix4 oldMatrix;
	oldMatrix.setRotationDegrees(oldBodyRotation);
	
	core::matrix4 matrix;
	if (oldMatrix.getInverse(matrix))
		Smgr->getMeshManipulator()->transform(BodyMesh, matrix);
	
	matrix.setRotationDegrees(BodyRotation);
	Smgr->getMeshManipulator()->transform(BodyMesh, matrix);
}


void VehicleSceneNode::setTireOffsets(f32 dx, f32 dy, f32 dz1, f32 dz2)
{
	TireOffsetDx = dx;
	TireOffsetDy = dy;
	TireOffsetDz1 = dz1;
	TireOffsetDz2 = dz2;
	
	core::vector3df frontRightTireOffset(dx, dy, dz1);
	core::vector3df frontLeftTireOffset(-dx, dy, dz1);
	core::vector3df rearRightTireOffset(dx, dy, dz2);
	core::vector3df rearLeftTireOffset(-dx, dy, dz2);
	
	FrontRightTireEmpty->setPosition(frontRightTireOffset);
	FrontLeftTireEmpty->setPosition(frontLeftTireOffset);
	RearRightTireEmpty->setPosition(rearRightTireOffset);
	RearLeftTireEmpty->setPosition(rearLeftTireOffset);

}


void VehicleSceneNode::setTireScale(f32 scale)
{
	TireScale = scale;
	core::vector3df tireScale(scale);
	FrontRightTire->setScale(tireScale);
	FrontLeftTire->setScale(tireScale);
	RearRightTire->setScale(tireScale);
	RearLeftTire->setScale(tireScale);
}

void VehicleSceneNode::setFrontRightTireRotation(const core::vector3df& rot)
{
	FrontRightTireRotation = rot;
	FrontRightTire->setRotation(rot);
}


void VehicleSceneNode::setFrontLeftTireRotation(const core::vector3df& rot)
{
	FrontLeftTireRotation = rot;
	FrontLeftTire->setRotation(rot);
}


void VehicleSceneNode::setRearRightTireRotation(const core::vector3df& rot)
{
	RearRightTireRotation = rot;
	RearRightTire->setRotation(rot);
}


void VehicleSceneNode::setRearLeftTireRotation(const core::vector3df& rot)
{
	RearLeftTireRotation = rot;
	RearLeftTire->setRotation(rot);
}



void VehicleSceneNode::setTireOffsetDx(f32 offset)
{
	TireOffsetDx = offset;
	this->setTireOffsets(TireOffsetDx, TireOffsetDy, TireOffsetDz1, TireOffsetDz2);
}

void VehicleSceneNode::setTireOffsetDy(f32 offset)
{
	TireOffsetDy = offset;
	this->setTireOffsets(TireOffsetDx, TireOffsetDy, TireOffsetDz1, TireOffsetDz2);
}


void VehicleSceneNode::setTireOffsetDz1(f32 offset)
{
	TireOffsetDz1 = offset;
	this->setTireOffsets(TireOffsetDx, TireOffsetDy, TireOffsetDz1, TireOffsetDz2);
}


void VehicleSceneNode::setTireOffsetDz2(f32 offset)
{
	TireOffsetDz2 = offset;
	this->setTireOffsets(TireOffsetDx, TireOffsetDy, TireOffsetDz1, TireOffsetDz2);
}



void VehicleSceneNode::setupMaterial(scene::ISceneNode* node)
{
	for (u32 n=0; n<node->getMaterialCount(); ++n)
	{
		node->getMaterial(n).setFlag(video::EMF_LIGHTING, false);
		node->getMaterial(n).NormalizeNormals = true;
	}
}

bool VehicleSceneNode::loadParamsFromFile
(
	IrrlichtDevice* device,
	const core::stringc& filename,
	VehicleSceneNodeParams& params
)
{
	if (!device)
		device = createDevice(video::EDT_NULL);
	
	if (!device)
		return false;
	
	io::IXMLReader* xmlReader = device->getFileSystem()->createXMLReader(filename);
	if (!xmlReader)
		return false;
	
	// example:
	/***
	<VehicleSceneNodeParams>
	<BodyMeshFile>/home/smso/ex/irrlicht/torcs/cars/car_Scene.obj</BodyMeshFile>
	<TireMeshFile>/home/smso/ex/irrlicht/torcs/cars/wheel3-src.obj</TireMeshFile>
	<BodyMeshRot X="0.0" Y="90.0" Z="0.0" />
	<FrontRightTireRot X="0.0" Y="-90.0" Z="0.0" />
	<FrontLeftTireRot X="0.0" Y="90.0" Z="0.0" />
	<RearRightTireRot X="0.0" Y="-90.0" Z="0.0" />
	<RearLeftTireRot X="0.0" Y="90.0" Z="0.0" />
	<TireScale Value="0.6" />
	<TireOffsetDx Value="0.6" />
	<TireOffsetDy Value="0.2" />
	<TireOffsetDz1 Value="1.3" />
	<TireOffsetDz2 Value="-1.3" />
	...
	***/

	const core::stringw rootTag(L"VehicleSceneNodeParams");
	
	const core::stringw bodyMeshFileTag(L"BodyMeshFile");
	const core::stringw tireMeshFileTag(L"TireMeshFile");
	
	const core::stringw bodyMeshRotTag(L"BodyMeshRot");
	const core::stringw frontRightTireRotTag(L"FrontRightTireRot");
	const core::stringw frontLeftTireRotTag(L"FrontLeftTireRot");
	const core::stringw rearRightTireRotTag(L"RearRightTireRot");
	const core::stringw rearLeftTireRotTag(L"RearLeftTireRot");
	
	const core::stringw tireScaleTag(L"TireScale");
	const core::stringw tireOffsetDxTag(L"TireOffsetDx");
	const core::stringw tireOffsetDyTag(L"TireOffsetDy");
	const core::stringw tireOffsetDz1Tag(L"TireOffsetDz1");
	const core::stringw tireOffsetDz2Tag(L"TireOffsetDz2");
	
	core::stringw currentSection; //keep track of our currentsection

	////////////////////////////////////////////////////////////////////////////
	while (xmlReader->read())
	{
		switch (xmlReader->getNodeType())
		{
			case io::EXN_ELEMENT:
			{
				if (rootTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					currentSection = rootTag;
				}

				else if (bodyMeshFileTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					currentSection = bodyMeshFileTag;
				}
				else if (tireMeshFileTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					currentSection = tireMeshFileTag;
				}
				else if (bodyMeshRotTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 x, y, z;
					x = xmlReader->getAttributeValueAsFloat(L"X");
					y = xmlReader->getAttributeValueAsFloat(L"Y");
					z = xmlReader->getAttributeValueAsFloat(L"Z");
					
					params.BodyMeshRot = core::vector3df(x, y, z);
					
					currentSection = bodyMeshRotTag;
				}

				else if (frontRightTireRotTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 x, y, z;
					x = xmlReader->getAttributeValueAsFloat(L"X");
					y = xmlReader->getAttributeValueAsFloat(L"Y");
					z = xmlReader->getAttributeValueAsFloat(L"Z");
					
					params.FrontRightTireRot = core::vector3df(x, y, z);
					
					currentSection = frontRightTireRotTag;
				}
				else if (frontLeftTireRotTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 x, y, z;
					x = xmlReader->getAttributeValueAsFloat(L"X");
					y = xmlReader->getAttributeValueAsFloat(L"Y");
					z = xmlReader->getAttributeValueAsFloat(L"Z");
					
					params.FrontLeftTireRot = core::vector3df(x, y, z);
					
					currentSection = frontLeftTireRotTag;
				}
				else if (rearRightTireRotTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 x, y, z;
					x = xmlReader->getAttributeValueAsFloat(L"X");
					y = xmlReader->getAttributeValueAsFloat(L"Y");
					z = xmlReader->getAttributeValueAsFloat(L"Z");
					
					params.RearRightTireRot = core::vector3df(x, y, z);
					
					currentSection = rearRightTireRotTag;
				}
				else if (rearLeftTireRotTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 x, y, z;
					x = xmlReader->getAttributeValueAsFloat(L"X");
					y = xmlReader->getAttributeValueAsFloat(L"Y");
					z = xmlReader->getAttributeValueAsFloat(L"Z");
					
					params.RearLeftTireRot = core::vector3df(x, y, z);
					
					currentSection = rearLeftTireRotTag;
				}
				
				else if (tireScaleTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 value = xmlReader->getAttributeValueAsFloat(L"Value");
					params.TireScale = value;
					
					currentSection = tireScaleTag;
				}
				else if (tireOffsetDxTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 value = xmlReader->getAttributeValueAsFloat(L"Value");
					params.TireOffsetDx = value;
					
					currentSection = tireOffsetDxTag;
				}
				else if (tireOffsetDyTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 value = xmlReader->getAttributeValueAsFloat(L"Value");
					params.TireOffsetDy = value;
					
					currentSection = tireOffsetDyTag;
				}
				else if (tireOffsetDz1Tag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 value = xmlReader->getAttributeValueAsFloat(L"Value");
					params.TireOffsetDz1 = value;
					
					currentSection = tireOffsetDz1Tag;
				}
				else if (tireOffsetDz2Tag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 value = xmlReader->getAttributeValueAsFloat(L"Value");
					params.TireOffsetDz2 = value;
					
					currentSection = tireOffsetDz2Tag;
				}

			}
			break;
			
			case io::EXN_TEXT:
			{
				const wchar_t* data = xmlReader->getNodeData();
				if (data)
				{
					if (currentSection == bodyMeshFileTag)
					{
						params.BodyMeshFile = core::stringw(data);
					}
					else if (currentSection == tireMeshFileTag)
					{
						params.TireMeshFile = core::stringw(data);
					}
				}
			}
			break;
			
			case io::EXN_ELEMENT_END:
			{
				if (rootTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					currentSection = L"";
				}
			}
			break;
			
			default:
			break;
		}
	}
	
	xmlReader->drop();

	return true;
}

// example of usage:
/***
			VehicleSceneNodeParams params;
			params.BodyMeshFile = L"/home/smso/ex/irrlicht/torcs/cars/car_Scene.obj";
			params.TireMeshFile = L"/home/smso/ex/irrlicht/torcs/cars/wheel3-src.obj";

			params.BodyMeshRot = core::vector3df(0.0f, 90.0f, 0.0f);
			params.FrontRightTireRot = core::vector3df(0.0f, -90.0f, 0.0f);
			params.FrontLeftTireRot = core::vector3df(0.0f, 90.0f, 0.0f);
			params.RearRightTireRot = core::vector3df(0.0f, -90.0f, 0.0f);
			params.RearLeftTireRot = core::vector3df(0.0f, 90.0f, 0.0f);

			params.TireScale = 0.65f;
			params.TireOffsetDx = 0.65f;
			params.TireOffsetDy = 0.15f;
			params.TireOffsetDz1 = 1.33f;
			params.TireOffsetDz2 = -1.27f;

			VehicleSceneNode::saveParamsToFile(device, "/home/smso/ex/irrlicht/string/vehicle_scene_node_params.xml", params);
***/
bool VehicleSceneNode::saveParamsToFile
(
	IrrlichtDevice* device,
	const core::stringc& filename,
	const VehicleSceneNodeParams& params
)
{
	if (!device)
		device = createDevice(video::EDT_NULL);
	if (!device)
		return false;
	
	
	io::IXMLWriter* xmlWriter = device->getFileSystem()->createXMLWriter(filename);
	if (!xmlWriter)
		return false;
	
	// save some typing
	core::vector3df rot0 = params.BodyMeshRot;
	core::vector3df rot1 = params.FrontRightTireRot;
	core::vector3df rot2 = params.FrontLeftTireRot;
	core::vector3df rot3 = params.RearRightTireRot;
	core::vector3df rot4 = params.RearLeftTireRot;

	xmlWriter->writeXMLHeader();

	xmlWriter->writeElement(L"VehicleSceneNodeParams");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"BodyMeshFile");
	xmlWriter->writeText(params.BodyMeshFile.c_str());
	xmlWriter->writeClosingTag(L"BodyMeshFile");
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement(L"TireMeshFile");
	xmlWriter->writeText(params.TireMeshFile.c_str());
	xmlWriter->writeClosingTag(L"TireMeshFile");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement
	(
		L"BodyMeshRot", true,
		L"X", stringFromF32(rot0.X).c_str(),
		L"Y", stringFromF32(rot0.Y).c_str(),
		L"Z", stringFromF32(rot0.Z).c_str()
	);
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement
	(
		L"FrontRightTireRot", true,
		L"X", stringFromF32(rot1.X).c_str(),
		L"Y", stringFromF32(rot1.Y).c_str(),
		L"Z", stringFromF32(rot1.Z).c_str()
	);
	xmlWriter->writeLineBreak();
	
	
	xmlWriter->writeElement
	(
		L"FrontLeftTireRot", true,
		L"X", stringFromF32(rot2.X).c_str(),
		L"Y", stringFromF32(rot2.Y).c_str(),
		L"Z", stringFromF32(rot2.Z).c_str()
	);
	xmlWriter->writeLineBreak();
	
	
	xmlWriter->writeElement
	(
		L"RearRightTireRot", true,
		L"X", stringFromF32(rot3.X).c_str(),
		L"Y", stringFromF32(rot3.Y).c_str(),
		L"Z", stringFromF32(rot3.Z).c_str()
	);
	xmlWriter->writeLineBreak();
	
	
	xmlWriter->writeElement
	(
		L"RearLeftTireRot", true,
		L"X", stringFromF32(rot4.X).c_str(),
		L"Y", stringFromF32(rot4.Y).c_str(),
		L"Z", stringFromF32(rot4.Z).c_str()
	);
	xmlWriter->writeLineBreak();
	
	
	xmlWriter->writeElement
	(
		L"TireScale", true,
		L"Value", stringFromF32(params.TireScale).c_str()
	);
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement
	(
		L"TireOffsetDx", true,
		L"Value", stringFromF32(params.TireOffsetDx).c_str()
	);
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement
	(
		L"TireOffsetDy", true,
		L"Value", stringFromF32(params.TireOffsetDy).c_str()
	);
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement
	(
		L"TireOffsetDz1", true,
		L"Value", stringFromF32(params.TireOffsetDz1).c_str()
	);
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement
	(
		L"TireOffsetDz2", true,
		L"Value", stringFromF32(params.TireOffsetDz2).c_str()
	);
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeClosingTag(L"VehicleSceneNodeParams");
	xmlWriter->writeLineBreak();
	
	////////////////////////////////////////////////////////////////////////////
	xmlWriter->drop();
	
	return true;
}

core::stringw VehicleSceneNode::stringFromF32(f32 val)
{
	std::wstringstream ss;
	ss.setf(std::ios::fixed);
	ss.precision(2);
	
	ss << val;
	return core::stringw(std::wstring(ss.str()).c_str());
}

/***
void VehicleSceneNode::()
{
	
}
***/



