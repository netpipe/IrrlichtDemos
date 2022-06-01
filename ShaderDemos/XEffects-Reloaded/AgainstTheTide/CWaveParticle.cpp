#include "CWaveParticle.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

CWaveParticle::CWaveParticle(irr::IrrlichtDevice* irrlichtDevice,
							 irr::scene::IMeshSceneNode* meshSceneNode, irr::f32 height)
	: device(irrlichtDevice), smgr(device->getSceneManager())
{
	meshBuffer = new SMeshBuffer();
	mesh = new SMesh();

	mesh->addMeshBuffer(meshBuffer);

	IMesh* nodeMesh = meshSceneNode->getMesh();

	selector = smgr->createOctTreeTriangleSelector(nodeMesh, meshSceneNode);

	const aabbox3df& bbox = nodeMesh->getBoundingBox();
	const f32 maxPoint = bbox.getExtent().getLength();
	center = bbox.getCenter();
	center.Y = height;

	for(u32 i = 0;i < 5000;++i)
	{
		const f32 randNum = i * 0.1f;
		const vector3df randPoint(sinf(randNum) * maxPoint, height, cosf(randNum) * maxPoint);
		const line3df ray = line3df(randPoint, center);

		vector3df pOut;
		triangle3df triOut;
ISceneNode* outNode;
		if(smgr->getSceneCollisionManager()->getCollisionPoint(ray, selector, pOut, triOut,outNode))
			addQuad(pOut);
	}

	meshBuffer->setHardwareMappingHint(EHM_STATIC);
	meshBuffer->recalculateBoundingBox();
	mesh->recalculateBoundingBox();

	mSceneNode = smgr->addMeshSceneNode(mesh, meshSceneNode);

	pMat = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		"shaders/WaveV.glsl", "main", EVST_VS_2_0, "shaders/WaveP.glsl", "main",
		EPST_PS_2_0, this, EMT_TRANSPARENT_ADD_COLOR);

	mSceneNode->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)pMat;
	mSceneNode->getMaterial(0).ZWriteEnable = false;
	mSceneNode->getMaterial(0).setTexture(0, device->getVideoDriver()->getTexture("media/waveParticle.png"));
}

irr::f32 CWaveParticle::getFRand() const
{
	return ((rand() % 100) * 0.0033f) + ((rand() % 100) * 0.0033f) + ((rand() % 100) * 0.0033f);
}

void CWaveParticle::addQuad(irr::core::vector3df position)
{
	const f32 randNum = getFRand();

	meshBuffer->Vertices.push_back(
		S3DVertex(position, vector3df(randNum, 0.0f, 0.0f), SColor(0xffffffff), vector2df(0.0f, 0.0f)));
	meshBuffer->Vertices.push_back(
		S3DVertex(position, vector3df(randNum, 0.0f, 0.0f), SColor(0xffffffff), vector2df(1.0f, 0.0f)));
	meshBuffer->Vertices.push_back(
		S3DVertex(position, vector3df(randNum, 0.0f, 0.0f), SColor(0xffffffff), vector2df(0.0f, 1.0f)));
	meshBuffer->Vertices.push_back(
		S3DVertex(position, vector3df(randNum, 0.0f, 0.0f), SColor(0xffffffff), vector2df(1.0f, 1.0f)));

	const u32 startInd = meshBuffer->Vertices.size() - 4;

	meshBuffer->Indices.push_back(startInd);
	meshBuffer->Indices.push_back(startInd + 1);
	meshBuffer->Indices.push_back(startInd + 3);

	meshBuffer->Indices.push_back(startInd);
	meshBuffer->Indices.push_back(startInd + 3);
	meshBuffer->Indices.push_back(startInd + 2);
}

void CWaveParticle::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
	IVideoDriver* driver = services->getVideoDriver();

	matrix4 wvp = driver->getTransform(ETS_PROJECTION);
	wvp *= driver->getTransform(ETS_VIEW);

	services->setVertexShaderConstant("ViewProj", wvp.pointer(), 16);

	wvp = driver->getTransform(ETS_WORLD);
	services->setVertexShaderConstant("World", wvp.pointer(), 16);

	if(smgr->getActiveCamera())
	{
		const vector3df camPos = smgr->getActiveCamera()->getPosition();
		services->setVertexShaderConstant("CamPos", &camPos.X, 3);
	}

	const f32 time = device->getTimer()->getTime() / 500.0f;
	services->setVertexShaderConstant("Time", &time, 1);

	services->setVertexShaderConstant("CenterPos", &center.X, 3);
}
