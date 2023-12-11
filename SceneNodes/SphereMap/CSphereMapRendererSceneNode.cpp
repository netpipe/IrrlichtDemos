#include "CSphereMapRendererSceneNode.h"

#include "ISceneManager.h"
#include "IVideoDriver.h"

#include "ICameraSceneNode.h"

#include "IMeshCache.h"
#include "SAnimatedMesh.h"
#include "SMesh.h"
#include "SMeshBuffer.h"


using namespace irr;
using namespace core;

namespace irr
{
namespace scene
{

CSphereMapRendererSceneNode::CSphereMapRendererSceneNode(ISceneNode* parent,
	ISceneManager* mgr, s32 id, u32 gridSize, bool createRenderTargets, u32 wtSize, u32 otSize)
	: ISceneNode(parent, mgr, id),
	Box(-5.f,-5.f,-5.f, 5.f, 5.f, 5.f), MeshBuffer(0), Camera(0), NeedsUpdate(true),
	Magnify(1.0f)
{
	Material.Wireframe = true;
	Material.Lighting = false;

	// create/grab mesh buffer
	MeshBuffer = createMeshBuffer(gridSize, true);

	// create render targets
	RenderTargets.set_used(7);

	for (u32 i=0; i<6; ++i)
		RenderTargets[i] = createRenderTargets ? mgr->getVideoDriver()->addRenderTargetTexture(core::dimension2du(wtSize,wtSize)): 0;

	RenderTargets[6] = createRenderTargets ? mgr->getVideoDriver()->addRenderTargetTexture(core::dimension2du(otSize,otSize)) : 0;

	// create debug billboard vertices
	billvertices[0].Color = video::SColor(255,255,255,255);
	billvertices[0].TCoords.set(1,1);
	billvertices[1].Color = video::SColor(255,255,255,255);
	billvertices[1].TCoords.set(1,0);
	billvertices[2].Color = video::SColor(255,255,255,255);
	billvertices[2].TCoords.set(0,0);
	billvertices[3].Color = video::SColor(255,255,255,255);
	billvertices[3].TCoords.set(0,1);

	// add the camera
	ICameraSceneNode* cam = SceneManager->getActiveCamera();
	Camera = SceneManager->addCameraSceneNode(this);
	Camera->setName("SphereMapRendererCam");
	Camera->setIsDebugObject(true);
	Camera->grab();
	Camera->setParent(SceneManager->getRootSceneNode());
	// square aspect
	Camera->setAspectRatio(1.0f);
	Camera->setFOV(core::PI / 2.0f);
	// remember old camera
	SceneManager->setActiveCamera(cam);

	// add the skybox
	SkyBox = SceneManager->addSkyBoxSceneNode(
		RenderTargets[(u32)ESMRT_UP],
		RenderTargets[(u32)ESMRT_DOWN],
		RenderTargets[(u32)ESMRT_LEFT],
		RenderTargets[(u32)ESMRT_RIGHT],
		RenderTargets[(u32)ESMRT_FRONT],
		RenderTargets[(u32)ESMRT_BACK]);
	Camera->setName("SphereMapRendererSkyBox");
	SkyBox->setIsDebugObject(true);
	u32 i = SkyBox->getMaterialCount();

	// flip the floor (disabled due to bug)
	//core::matrix4 flipMatrix;
	//flipMatrix.buildTextureTransform(0,vector2df(0.0f,0.0f),vector2df(1,1), vector2df(-1,-1));
	//SkyBox->getMaterial(5).setTextureMatrix(0, flipMatrix);

	SkyBox->grab();
	SkyBox->setVisible(false);

	// temp bg
	BackColour = video::SColor(0,100,101,140);
}

CSphereMapRendererSceneNode::~CSphereMapRendererSceneNode()
{
	if (MeshBuffer)
		MeshBuffer->drop();

	for (u32 i=0; i<7; ++i)
		if (RenderTargets[i])
			RenderTargets[i]->drop();

	if (Camera)
		Camera->drop();

	if (SkyBox)
		SkyBox->drop();
}

IMeshBuffer* CSphereMapRendererSceneNode::createMeshBuffer(u32 size, bool inside)
{

	core::stringc name(inside ? "HexahedralSphereFaceInner" : "HexahedralSphereFaceOuter");
	name += size;
	name += ".irrMesh";

	IAnimatedMesh* amsh = SceneManager->getMeshCache()->getMeshByFilename(name.c_str());

	if (amsh)
	{
		IMesh* msh = amsh->getMesh(0);

		if (msh)
		{
			msh->getMeshBuffer(0)->grab();
			return msh->getMeshBuffer(0);
		}
	}

	SMeshBuffer* buffer = new SMeshBuffer();

	// create mesh

	int p=0;

	buffer->Vertices.set_used((size+1)*(size+1));

	f32 diff = 1.0f / f32(size);

	for (f32 y=-0.5f; y <= 0.5f; y+=diff)
		for (f32 x=-0.5f; x <= 0.5f; x+=diff)
		{
			core::vector3df v(x,y,-0.5f);
			core::vector3df n = v.normalize();
			core::vector2df t(-x+0.5f,-y+0.5f); // tcoords
			v.setLength(0.5f);
			buffer->Vertices[p++] = video::S3DVertex(v.X,v.Y,v.Z, n.X,n.Y,n.Z, video::SColor(255,255,255,255), t.X, t.Y);
		}

	buffer->Indices.set_used(size*size*6);
	p=0;
	for (u32 y=0; y<size; ++y)
		for (u32 x=0; x<size; ++x)
		{
			u16 a = y*(size+1) + x,
				b = y*(size+1) + x+1;
			buffer->Indices[p++] = inside ? a : b;
			buffer->Indices[p++] = inside ? b : a;
			buffer->Indices[p++] = (y+1)*(size+1) + x;

			a = (y+1)*(size+1) + x;
			b = (y+1)*(size+1) + x+1;

			buffer->Indices[p++] = inside ? a : b;
			buffer->Indices[p++] = y*(size+1) + x+1;
			buffer->Indices[p++] = inside ? b : a;
		}

	buffer->recalculateBoundingBox();

	// does not change, so good candidate for a hardware mesh buffer
	buffer->setHardwareMappingHint(scene::EHM_STATIC);

	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	mesh->setBoundingBox(buffer->getBoundingBox());
	SAnimatedMesh *animMesh = new SAnimatedMesh();
	animMesh->addMesh(mesh);
	animMesh->setBoundingBox(buffer->getBoundingBox());

	SceneManager->getMeshCache()->addMesh(name.c_str(), animMesh);

	// drop the mesh because it now belongs to the animated mesh
	mesh->drop();

	// drop the animated mesh because it now belongs to the mesh cache
	animMesh->drop();

	// don't drop the mesh buffer because this is a create function
	return buffer;

}

void CSphereMapRendererSceneNode::update(bool UpdateAll, bool UpdateSphereMesh)
{
	bool updateAll = UpdateAll | NeedsUpdate;

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	// save old camera
	ICameraSceneNode* cam = SceneManager->getActiveCamera();
	cam->updateAbsolutePosition();

	// save old visible state
	bool visState = IsVisible;

	vector3df pos = getAbsolutePosition();

	// render scene to target
	SceneManager->setActiveCamera(Camera);

	setVisible(false);

	if (updateAll)
	{
		Camera->setPosition(pos);
		Camera->setFarValue(10000.0f);
		Camera->setNearValue(1.0f);
		Camera->setUpVector(vector3df(0,1,0));
		Camera->updateAbsolutePosition();

		driver->setRenderTarget(RenderTargets[(u32)ESMRT_FRONT], true, true, BackColour);
		Camera->setTarget(pos + vector3df(0,0,-100));
		SceneManager->drawAll();

		driver->setRenderTarget(RenderTargets[(u32)ESMRT_BACK], true, true, BackColour);
		Camera->setTarget(pos + vector3df(0,0,100));
		SceneManager->drawAll();

		driver->setRenderTarget(RenderTargets[(u32)ESMRT_RIGHT], true, true, BackColour);
		Camera->setTarget(pos + vector3df(-100,0,0));
		SceneManager->drawAll();

		driver->setRenderTarget(RenderTargets[(u32)ESMRT_LEFT], true, true, BackColour);
		Camera->setTarget(pos + vector3df(100,0,0));
		SceneManager->drawAll();

		Camera->setUpVector(vector3df(-1,0,0));

		driver->setRenderTarget(RenderTargets[(u32)ESMRT_UP], true, true, BackColour);
		Camera->setTarget(pos + vector3df(0,100,0));
		SceneManager->drawAll();

		driver->setRenderTarget(RenderTargets[(u32)ESMRT_DOWN], true, true, BackColour);
		Camera->setTarget(pos + vector3df(0,-100,0));
		SceneManager->drawAll();

		NeedsUpdate = false;
	}

	// render to output texture
	driver->setRenderTarget(RenderTargets[(u32)ESMRT_OUTPUT], true, true, BackColour);

	// work out new up vector
	core::vector3df UpVector = cam->getUpVector();

	core::vector3df view(cam->getTarget() - cam->getAbsolutePosition());
	view.normalize();
	core::vector3df horizontal = UpVector.crossProduct(view);
	if (horizontal.getLengthSQ() == 0)
		horizontal = UpVector;

	horizontal.normalize();
	core::vector3df vertical = horizontal.crossProduct(-view);
	vertical.normalize();

	// draw background
	Camera->setPosition(cam->getAbsolutePosition());
	Camera->setTarget(pos);
	Camera->setUpVector(vertical);
	Camera->updateAbsolutePosition();
	Camera->render();
	SkyBox->render();

	if (UpdateSphereMesh)
	{
		// draw mesh
		Camera->setPosition((cam->getAbsolutePosition()-pos).normalize());
		Camera->setTarget(vector3df(0,0,0));
		Camera->updateAbsolutePosition();
		Camera->render();

		f32 influence = 2.0f * Magnify;
		core::matrix4 m;
		m.setScale( core::vector3df(influence, influence, influence));
		renderOutput(m, true);
	}

	// set old render target
	driver->setRenderTarget(0, false, false, BackColour);

	// reset old camera
	SceneManager->setActiveCamera(cam);

	// set old visible state
	setVisible(visState);
}

void CSphereMapRendererSceneNode::setMagnify(f32 zoom)
{
	Magnify = zoom;
}

void CSphereMapRendererSceneNode::renderOutput(const core::matrix4& trans, bool mirror)
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	video::SMaterial mat;

	mat.Lighting=false;
	mat.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
	//mat.Wireframe = true;
	//mat.MaterialType=video::EMT_TRANSPARENT_ALPHA_CHANNEL; // wow!

	//core::matrix4 flipMatrix;
	//if (mirror)
	//	flipMatrix.buildTextureTransform(0,vector2df(0,0),vector2df(1,0), vector2df(-1,1));

	core::matrix4 m;

	//mat.setTextureMatrix(0, flipMatrix);

	mat.setTexture(0, RenderTargets[(u32)(ESMRT_FRONT)]);
	driver->setMaterial(mat);
	driver->setTransform(video::ETS_WORLD, trans);
	driver->drawMeshBuffer(MeshBuffer);

	mat.setTexture(0, RenderTargets[(u32)(ESMRT_BACK)]);
	driver->setMaterial(mat);
	m.setRotationDegrees(core::vector3df(0,180.0f,0));
	driver->setTransform(video::ETS_WORLD, trans*m);
	driver->drawMeshBuffer(MeshBuffer);

	mat.setTexture(0, RenderTargets[(u32)(ESMRT_RIGHT)]);
	driver->setMaterial(mat);
	m.setRotationDegrees(core::vector3df(0,90.0f,0));
	driver->setTransform(video::ETS_WORLD, trans*m);
	driver->drawMeshBuffer(MeshBuffer);

	mat.setTexture(0, RenderTargets[(u32)(ESMRT_LEFT)]);
	driver->setMaterial(mat);
	m.setRotationDegrees(core::vector3df(0,-90.0f,0));
	driver->setTransform(video::ETS_WORLD, trans*m);
	driver->drawMeshBuffer(MeshBuffer);

	mat.setTexture(0, RenderTargets[(u32)ESMRT_UP]);
	driver->setMaterial(mat);
	m.setRotationDegrees(core::vector3df(90,-90.0f,0));
	driver->setTransform(video::ETS_WORLD, trans*m);
	driver->drawMeshBuffer(MeshBuffer);

	mat.setTexture(0, RenderTargets[(u32)ESMRT_DOWN]);
	driver->setMaterial(mat);
	m.setRotationDegrees(core::vector3df(-90,90.0f,0));
	driver->setTransform(video::ETS_WORLD, trans*m);
	driver->drawMeshBuffer(MeshBuffer);


}

//
video::ITexture* CSphereMapRendererSceneNode::getTexture(ESPHERE_MAP_RENDER_TARGET which)
{
	return RenderTargets[(u32)which];
}


// node functions

const aabbox3df& CSphereMapRendererSceneNode::getBoundingBox() const
{
	return Box;
}


void CSphereMapRendererSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		if (NeedsUpdate)
			update(true);

		if (DebugDataVisible)
			SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);

	}

	ISceneNode::OnRegisterSceneNode();
}

void CSphereMapRendererSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	video::SMaterial mat;
	mat.Lighting=false;
	mat.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
	//mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	if (DebugDataVisible & EDS_SPHEREMAP_BILLBOARD)
	{
		// draw billboard with final render target

		f32 scale = getScale().X + getScale().Y + getScale().Z;
		scale *= 10.f;

		const u16 indices[] = {0,2,1, 0,3,2};

		core::vector3df pos = getAbsolutePosition();

		core::vector3df campos = camera->getAbsolutePosition();
		core::vector3df target = camera->getTarget();
		core::vector3df up = camera->getUpVector();
		core::vector3df view = target - campos;
		view.normalize();

		core::vector3df horizontal = up.crossProduct(view);
		if (horizontal.getLength() == 0)
			horizontal.set(up.Y,up.X,up.Z);

		horizontal.normalize();
		horizontal *= 0.5f * scale;

		core::vector3df vertical = horizontal.crossProduct(view);
		vertical.normalize();
		vertical *= 0.5f * scale;

		view *= -1.0f;

		billvertices[0].Pos = pos + horizontal + vertical;
		billvertices[1].Pos = pos + horizontal - vertical;
		billvertices[2].Pos = pos - horizontal - vertical;
		billvertices[3].Pos = pos - horizontal + vertical;

		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

		mat.setTexture(0, RenderTargets[(u32)ESMRT_OUTPUT]);

		driver->setMaterial(mat);

		driver->drawIndexedTriangleList(billvertices, 4, indices, 2);
	}

	if (DebugDataVisible & EDS_SPHEREMAP_MESH)
	{
		AbsoluteTransformation;
		core::matrix4 m;
		m.setScale(core::vector3df(10,10,10));
		m = AbsoluteTransformation*m;
		renderOutput(m, false);
	}

	if (DebugDataVisible & EDS_BBOX)
	{
		// draw bounding box
		mat.setTexture(0,0);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->setMaterial(mat);
		driver->draw3DBox(Box, video::SColor(128,255,0,0));
	}
}


} // scene
} // irr
