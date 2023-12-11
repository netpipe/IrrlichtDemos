// Copyright (C) Pazystamo
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.

//! The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
//! and mostly inspired of the original version of this project.

#include "IAtmosphereStarSceneNode.h"

namespace irr
{

//! Constructor.
IAtmosphereStarSceneNode::IAtmosphereStarSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
    video::ITexture* sun, video::ITexture* moon, const core::dimension2df& size)
	: scene::ISceneNode(parent, mgr, id)
{
    #ifdef _DEBUG
	setDebugName("IAtmospherStarSceneNode");
	#endif

    setSize(size);

	setAutomaticCulling(scene::EAC_OFF);

	BufferSun = new scene::SMeshBuffer();
	BufferSun->Material.Lighting = false;
	BufferSun->Material.ZBuffer = video::ECFN_NEVER;
	BufferSun->Material.ZWriteEnable = false;
	BufferSun->Material.AntiAliasing = video::EAAM_OFF;
	BufferSun->Material.setTexture(0, sun);
    BufferSun->Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

    BufferMoon = new scene::SMeshBuffer();
    BufferMoon->Material.Lighting = false;
    BufferMoon->Material.ZBuffer = video::ECFN_NEVER;
	BufferMoon->Material.ZWriteEnable = false;
	BufferMoon->Material.AntiAliasing = video::EAAM_OFF;
    BufferMoon->Material.setTexture(0, moon);
    BufferMoon->Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

    generateMesh();
}

//! Destructor.
IAtmosphereStarSceneNode::~IAtmosphereStarSceneNode()
{
    if (BufferSun)
		BufferSun->drop();
    if (BufferMoon)
		BufferMoon->drop();
}

void IAtmosphereStarSceneNode::generateMesh()
{
    BufferSun->Vertices.clear();
	BufferSun->Indices.clear();

    BufferMoon->Vertices.clear();
	BufferMoon->Indices.clear();

	BufferSun->Vertices.reallocate( 4 );
	BufferSun->Indices.reallocate( 6 );

	BufferMoon->Vertices.reallocate( 4 );
	BufferMoon->Indices.reallocate( 6 );

    BufferSun->Indices.push_back(0);
    BufferSun->Indices.push_back(2);
    BufferSun->Indices.push_back(1);
    BufferSun->Indices.push_back(0);
    BufferSun->Indices.push_back(3);
    BufferSun->Indices.push_back(2);

    BufferMoon->Indices.push_back(0);
    BufferMoon->Indices.push_back(2);
    BufferMoon->Indices.push_back(1);
    BufferMoon->Indices.push_back(0);
    BufferMoon->Indices.push_back(3);
    BufferMoon->Indices.push_back(2);

    video::S3DVertex vtx = video::S3DVertex(0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        video::SColor(255,255,255,255), 0.f, 0.f);

    for (u16 i = 0; i < 4; i++)
    {
        BufferSun->Vertices.push_back(vtx);
        BufferMoon->Vertices.push_back(vtx);
    }

    BufferSun->Vertices[0].TCoords.set(1.0f, 1.0f);
    BufferSun->Vertices[1].TCoords.set(1.0f, 0.0f);
    BufferSun->Vertices[2].TCoords.set(0.0f, 0.0f);
    BufferSun->Vertices[3].TCoords.set(0.0f, 1.0f);

    BufferMoon->Vertices[0].TCoords.set(1.0f, 1.0f);
    BufferMoon->Vertices[1].TCoords.set(1.0f, 0.0f);
    BufferMoon->Vertices[2].TCoords.set(0.0f, 0.0f);
    BufferMoon->Vertices[3].TCoords.set(0.0f, 1.0f);

    BufferSun->setHardwareMappingHint(scene::EHM_STREAM, scene::EBT_VERTEX);
    BufferMoon->setHardwareMappingHint(scene::EHM_STREAM, scene::EBT_VERTEX);
}

void IAtmosphereStarSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
    {
        SceneManager->registerNodeForRendering( this, scene::ESNRP_SKY_BOX );
        ///ISceneNode::updateAbsolutePosition(); //!Can help if the sun pos lag
    }
    ISceneNode::OnRegisterSceneNode();
}

//! Renders the node.
void IAtmosphereStarSceneNode::render()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

    if (!camera || !driver)
        return;

    // make billboard look to camera
    core::vector3df campos = camera->getAbsolutePosition();
    core::vector3df target = camera->getTarget();
    core::vector3df up = camera->getUpVector();
    core::vector3df view = target - campos;
    view.normalize();

    core::vector3df horizontal = up.crossProduct(view);
    horizontal.normalize();

    core::vector3df vertical = horizontal.crossProduct(view);
    vertical.normalize();

    core::dimension2d<f32> oldSize = Size;

    horizontal *= (oldSize.Width / 16);
    vertical *= (oldSize.Height / 16);

    // Draw the sun

    BufferSun->Vertices[0].Pos.set(SunPosition + horizontal + vertical);
    BufferSun->Vertices[1].Pos.set(SunPosition + horizontal - vertical);
    BufferSun->Vertices[2].Pos.set(SunPosition - horizontal - vertical);
    BufferSun->Vertices[3].Pos.set(SunPosition - horizontal + vertical);

    view *= -1.0f;

    for (s32 i=0; i<4; ++i)
        BufferSun->Vertices[i].Normal.set(view);

    // Draw the moon

    BufferMoon->Vertices[0].Pos.set(MoonPosition + horizontal + vertical);
    BufferMoon->Vertices[1].Pos.set(MoonPosition + horizontal - vertical);
    BufferMoon->Vertices[2].Pos.set(MoonPosition - horizontal - vertical);
    BufferMoon->Vertices[3].Pos.set(MoonPosition - horizontal + vertical);

    view *= -1.0f;

    for (s32 i=0; i<4; ++i)
        BufferMoon->Vertices[i].Normal.set(view);

    if ( !camera->isOrthogonal() )
	{
		core::matrix4 mat(AbsoluteTransformation);
		mat.setTranslation(camera->getAbsolutePosition());
		driver->setTransform(video::ETS_WORLD, mat);

		driver->setMaterial(BufferSun->Material);
		driver->drawMeshBuffer(BufferSun);
        driver->setMaterial(BufferMoon->Material);
		driver->drawMeshBuffer(BufferMoon);
	}

	// for debug purposes only:
	if ( DebugDataVisible )
	{
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);

		if ( DebugDataVisible & scene::EDS_NORMALS )
		{
			// draw normals
			const f32 debugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(scene::DEBUG_NORMAL_LENGTH);
			const video::SColor debugNormalColor = SceneManager->getParameters()->getAttributeAsColor(scene::DEBUG_NORMAL_COLOR);
			driver->drawMeshBufferNormals(BufferSun, debugNormalLength, debugNormalColor);
			driver->drawMeshBufferNormals(BufferMoon, debugNormalLength, debugNormalColor);
		}

		// show mesh
		if ( DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY )
		{
			m.Wireframe = true;
			driver->setMaterial(m);

			driver->drawMeshBuffer(BufferSun);
			driver->drawMeshBuffer(BufferMoon);
		}
	}
}

//! Returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& IAtmosphereStarSceneNode::getBoundingBox() const
{
    return BufferSun->BoundingBox;
}

//! Changes the sun texture.
void IAtmosphereStarSceneNode::setSunTexture(video::ITexture* const newSunTex)
{
    BufferSun->Material.setTexture(0, newSunTex);
}

//! Changes the moon texture.
void IAtmosphereStarSceneNode::setMoonTexture(video::ITexture* const newMoonTex)
{
    BufferMoon->Material.setTexture(0, newMoonTex);
}

//! Returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& IAtmosphereStarSceneNode::getMaterial(u32 i)
{
    return BufferSun->Material;
}

video::SMaterial& IAtmosphereStarSceneNode::getMoonMaterial(u32 i)
{
    return BufferMoon->Material;
}

//! Returns amount of materials used by this scene node.
u32 IAtmosphereStarSceneNode::getMaterialCount() const
{
      return 1;
}

//! Creates a clone of this scene node and its children.
scene::ISceneNode* IAtmosphereStarSceneNode::clone(scene::ISceneNode* newParent, scene::ISceneManager* newManager)
{
	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

	IAtmosphereStarSceneNode* nb = new IAtmosphereStarSceneNode(newParent, newManager, ID, BufferSun->Material.getTexture(0), BufferMoon->Material.getTexture(0), Size);

	nb->cloneMembers(this, newManager);

	if ( newParent )
		nb->drop();
	return nb;
}

//! Sets the size of the billboard.
void IAtmosphereStarSceneNode::setSize(const core::dimension2d<f32>& size)
{
    Size = size;

    if (Size.Width < 0.1f)
        Size.Width = 1.0f;

    if (Size.Height < 0.1f )
        Size.Height = 1.0f;
}

//! Returns the size of the sun & the moon.
const core::dimension2d<f32>& IAtmosphereStarSceneNode::getSize()
{
    return Size;
}

//! Sets the sun postion.
void IAtmosphereStarSceneNode::setSunPosition(core::vector3df& newSunPos)
{
    SunPosition = newSunPos;
}

//! Sets the moon postion.
void IAtmosphereStarSceneNode::setMoonPosition(core::vector3df& newMoonPos)
{
    MoonPosition = newMoonPos;
}

} // namespace irr
