#include "storm.h"


using namespace irr;


StormEffect::StormEffect(IrrlichtDevice* device)
:	Device(device),
	Smgr(Device->getSceneManager()),
	Frustrum(0),
	Bill(0),
	SCB(0),
	
	ElapsedTime(0),
	LastTime(0),

	StartTime(0),
	IsStarting(false),
	
	EndTime(0),
	IsEnding(false)

{

	// setup frustrum:
	scene::IMesh* frustrumMesh = createFrustumMesh(0.0f, 100.0f, 400.0f, 128);
	
	//core::vector2df factor(0.25f, 0.25f);
	//smgr->getMeshManipulator()->scaleTCoords(frustrumMesh, factor);
	
	Frustrum = Smgr->addMeshSceneNode(frustrumMesh);
	Frustrum->setParent(Smgr->getActiveCamera());
	Frustrum->setRotation(core::vector3df(90.0f, 0.0f, 0.0f));
	
	Frustrum->setPosition(core::vector3df(0.0f, 0.0f, -120.0f));
	
	//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	Frustrum->setMaterialTexture(0, Device->getVideoDriver()->getTexture("raindrops.png"));
	//Frustrum->setDebugDataVisible(scene::EDS_BBOX);
	//printf("Frustrum->getMaterialCount()=%i\n", Frustrum->getMaterialCount());
	
	//Frustrum->getMaterial(0).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	Frustrum->getMaterial(0).BlendOperation = video::EBO_ADD;
	Frustrum->getMaterial(0).setFlag(video::EMF_LIGHTING, false);
	Frustrum->getMaterial(0).setFlag(video::EMF_BACK_FACE_CULLING, false);
	Frustrum->getMaterial(0).setFlag(video::EMF_ZBUFFER, false);
	
	Frustrum->setVisible(false);
	
	////////////////////////////////////////////////////////////////////////////
	// setup Bill:
	Bill = Smgr->addBillboardSceneNode(Frustrum);
	Bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	Bill->setMaterialTexture(0, Device->getVideoDriver()->getTexture("explosion.png"));
	Bill->setMaterialFlag(video::EMF_ZBUFFER, false);
	Bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
	Bill->setPosition(core::vector3df(0.0f, 395.0f, 0.0f));
	
	Bill->setVisible(false);

	// setup shader cb:
	video::IGPUProgrammingServices* gpu = Device->getVideoDriver()->getGPUProgrammingServices();
	
	std::string pixelShader = "frustrum.frag";
	std::string vertexShader = "common.vert";
	SCB = new ShaderCallBack(device);
	
	s32 shaderMaterial = gpu->addHighLevelShaderMaterialFromFiles
	(
		vertexShader.c_str(), "main", video::EVST_VS_2_0,
		pixelShader.c_str(), "main", video::EPST_PS_2_0,
		SCB,
		video::EMT_TRANSPARENT_ALPHA_CHANNEL
	);
	Frustrum->setMaterialType((video::E_MATERIAL_TYPE)shaderMaterial);
	
	pixelShader = "bill.frag";
	s32 billShaderMaterial = gpu->addHighLevelShaderMaterialFromFiles
	(
		vertexShader.c_str(), "main", video::EVST_VS_2_0,
		pixelShader.c_str(), "main", video::EPST_PS_2_0,
		SCB,
		video::EMT_TRANSPARENT_ALPHA_CHANNEL
	);
	Bill->setMaterialType((video::E_MATERIAL_TYPE)billShaderMaterial);
	Bill->setMaterialFlag(video::EMF_LIGHTING, false);

	
}


StormEffect::~StormEffect()
{
	if (SCB) { delete SCB; SCB = 0;}
}


f32 StormEffect::sigmoid
(
	f32 x1, f32 x2,
	u32 totalTime,
	u32 time,
	u32 nctime
)
{
	if (totalTime == 0)
		return 0.0f;
	
	f32 nct = f32(nctime);
	
	f32 t = f32(time);
	f32 T = f32(totalTime);
	t = t*nct*2.0f/T-nct;
	
	f32 out = 1.0f/(1.0f+exp((-1.0f)*t));
	return (x2-x1)*out+x1;
}


scene::IMesh* StormEffect::createFrustumMesh
(
	f32 topRadius,
	f32 bottomRadius,
	f32 height,
	u32 tesselation
)
{
	scene::SMeshBuffer* buffer = new scene::SMeshBuffer();
	
	video::S3DVertex v;
	f32 radius;
	f32 angle;
	f32 x, y, z;
	f32 s, t;
	
	for (u32 m = 0; m <= tesselation; ++m) // height step:
	{
		y = height * f32(m) / f32(tesselation);
		radius = ((height - y) * (bottomRadius - topRadius) / height) + topRadius;
		//radius = 15.0f;
		t = 1.0f - f32(m) / f32(tesselation);
		
		for (u32 n = 0; n <= tesselation; ++n) // subdivide circle:
		{
			angle = core::PI * 2.0f * f32(n) / f32(tesselation);
			x = radius * cosf(angle);
			z = radius * sinf(angle);
			s = f32(n) / f32(tesselation);
			
			
			v.Pos.X = x;
			v.Pos.Y = y;
			v.Pos.Z = z;
			
			v.Normal = v.Pos;
			v.Normal.normalize();
			
			v.Color = 0xFFFFFFFF;
			
			//v.TCoords.X = s;
			//v.TCoords.Y = t;
			v.TCoords = core::vector2df(s, t);
			
			buffer->Vertices.push_back(v);
			
			//printf("(s,t)=%f,%f\n", s, t);
		}
	}
	
	u32 index00, index10, index11, index01;
	for (u32 m = 0; m < tesselation; ++m)
	{
		for (u32 n = 0; n < tesselation; ++n)
		{
			index00 = m * (tesselation + 1) + n;
			index10 = (m + 1) * (tesselation + 1) + n;
			index11 = (m + 1) * (tesselation + 1) + n + 1;
			index01 = m * (tesselation + 1) + n + 1;
			
			buffer->Indices.push_back(index00);
			buffer->Indices.push_back(index10);
			buffer->Indices.push_back(index11);
			
			buffer->Indices.push_back(index00);
			buffer->Indices.push_back(index11);
			buffer->Indices.push_back(index01);
		}
	}
	
	buffer->recalculateBoundingBox();
	scene::SMesh* mesh = new scene::SMesh();
	mesh->addMeshBuffer(buffer);
	buffer->drop();

	mesh->setHardwareMappingHint(scene::EHM_STATIC);
	mesh->recalculateBoundingBox();
	return mesh;
}


////////////////////////////////////////////////////////////////////////////////
void StormEffect::update()
{
	u32 time = Device->getTimer()->getTime();
	ElapsedTime += time;
	
	if (IsStarting)
	{
		u32 t = time - StartTime;
		if (t > START_DURATION)
		{
			IsStarting = false;
			return;
		}
		
		f32 alpha = sigmoid(0.0f, 0.25f, START_DURATION, t);
		SCB->setAlpha(alpha);
		
		f32 z = sigmoid(-120.0f, -150.0f, START_DURATION, t);
		core::vector3df pos = Frustrum->getPosition();
		pos.Z = z;
		Frustrum->setPosition(pos);
	}
	
	if (IsEnding)
	{
		u32 t = time - EndTime;
		if (t > END_DURATION)
		{
			Frustrum->setVisible(false);
			Bill->setVisible(false);
			IsEnding = false;
			return;
		}
		
		f32 alpha = sigmoid(0.25f, 0.0f, END_DURATION, t);
		SCB->setAlpha(alpha);
		
		f32 z = sigmoid(-150.0f, -120.0f, END_DURATION, t);
		core::vector3df pos = Frustrum->getPosition();
		pos.Z = z;
		Frustrum->setPosition(pos);
	}

	SCB->setTime(f32(ElapsedTime));
	
	LastTime = time;
}

void StormEffect::start()
{
	if (IsStarting || IsEnding)
		return;
	
	StartTime = Device->getTimer()->getTime();
	IsStarting = true;

	Frustrum->setVisible(true);
	Bill->setVisible(true);
}

void StormEffect::stop()
{
	if (IsStarting || IsEnding)
		return;
	
	EndTime = Device->getTimer()->getTime();
	IsEnding = true;
}

////////////////////////////////////////////////////////////////////////////////
/***
void StormEffect::()
{
	
}
***/



