// Copyright (C) Pazystamo
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.

//! The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
//! and mostly inspired of the original version of this project.

#include "IAtmosphereSkySceneNode.h"

namespace irr
{

//! Constructor.
IAtmosphereSkySceneNode::IAtmosphereSkySceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
        video::ITexture* sky, u16 numberOfFaces, const f32 radius)
	: scene::ISceneNode(parent, mgr, id), Buffer(0) , Radius(radius), UvX(1.f)
{
	#ifdef _DEBUG
	setDebugName("IAtmospherSkySceneNode");
	#endif

	if ( numberOfFaces > 256 )
        numberOfFaces = 256;

    if ( numberOfFaces < 3 )
        numberOfFaces = 3;

    Faces = numberOfFaces;

	setAutomaticCulling(scene::EAC_OFF);

	Buffer = new scene::SMeshBuffer();
	Buffer->Material.Lighting = false;
	Buffer->Material.ZBuffer = video::ECFN_NEVER;
	Buffer->Material.ZWriteEnable = false;
	Buffer->Material.AntiAliasing = video::EAAM_OFF;
	Buffer->Material.setTexture(0, sky);
	Buffer->Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	// regenerate the mesh
	generateMesh();
}

//! Destructor.
IAtmosphereSkySceneNode::~IAtmosphereSkySceneNode()
{
    if (Buffer)
		Buffer->drop();
}

void IAtmosphereSkySceneNode::generateMesh()
{
    Buffer->Vertices.clear();
	Buffer->Indices.clear();

	Buffer->Vertices.reallocate( Faces + 1 );
	Buffer->Indices.reallocate( Faces * 3 );

	// This is just a cone

    f32 Angle = 0.f;                //start positions
    const f32 AngleStep = 2.f * core::PI / Faces;        //angle to add

    video::S3DVertex vtx = video::S3DVertex(0.f, Radius, 0.f,
        0.f, -1.f, 0.f,
        video::SColor(255,255,255,255), 0.f, 0.1f); // If you want to change the sky color, you can do it here.

    Buffer->Vertices.push_back(vtx);

    for (u16 i = 1; i < Faces + 1; i++)
    {
        vtx.Pos.X = cosf(Angle) * Radius;   //vertice x coord
        vtx.Pos.Y = 0.f;
        vtx.Pos.Z = sinf(Angle) * Radius;   //vertice z coord
        vtx.Normal = vtx.Pos;
		vtx.Normal.normalize();
        vtx.TCoords = core::vector2df(0.f, 1.f);
        Buffer->Vertices.push_back(vtx);

        Angle += AngleStep;    // we neglish the precision of a floating summ. f32 angle = angle2 * i; This would be better
    }

    for (u16 i = 1; i < Faces; i++)
    {
        Buffer->Indices.push_back(0);
        Buffer->Indices.push_back(i);
        Buffer->Indices.push_back(i + 1);
    }

    Buffer->Indices.push_back(0);
    Buffer->Indices.push_back(Faces);
    Buffer->Indices.push_back(1);

    Buffer->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX);
}

void IAtmosphereSkySceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering( this, scene::ESNRP_SKY_BOX );
	}

	ISceneNode::OnRegisterSceneNode();
}

//! Renders the node.
void IAtmosphereSkySceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

    //update uv maping
    for(s32 i = 1; i < Faces + 1; i++)
    {
        Buffer->Vertices[i].TCoords.set(UvX, 1.f);
    }

    Buffer->Vertices[0].TCoords.set(UvX, 0.1f); // (UvX, 0.f); does a black point in the sky.

	if ( !camera->isOrthogonal() )
	{
		core::matrix4 mat(AbsoluteTransformation);
		mat.setTranslation(camera->getAbsolutePosition());

		driver->setTransform(video::ETS_WORLD, mat);

		driver->setMaterial(Buffer->Material);
		driver->drawMeshBuffer(Buffer);
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
			driver->drawMeshBufferNormals(Buffer, debugNormalLength, debugNormalColor);
		}

		// show mesh
		if ( DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY )
		{
			m.Wireframe = true;
			driver->setMaterial(m);

			driver->drawMeshBuffer(Buffer);
		}
	}
}

//! Returns the axis aligned bounding box of this node.
const core::aabbox3d<f32>& IAtmosphereSkySceneNode::getBoundingBox() const
{
	return Buffer->BoundingBox;
}

//! Changes the sky texture.
video::SMaterial& IAtmosphereSkySceneNode::setMaterial(video::ITexture* const tex)
{
    Material.setTexture(0, tex);
    return Material;
}

//! Returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& IAtmosphereSkySceneNode::getMaterial(u32 i)
{
	return Buffer->Material;
}

//! Returns amount of materials used by this scene node.
u32 IAtmosphereSkySceneNode::getMaterialCount() const
{
	return 1;
}

//! Writes attributes of the scene node.
void IAtmosphereSkySceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addInt  ("Faces",                 Faces);
	out->addFloat("Radius",               Radius);
}


//! Reads attributes of the scene node.
void IAtmosphereSkySceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
    Faces                = in->getAttributeAsInt("Faces");
	Radius               = in->getAttributeAsFloat("Radius");

	ISceneNode::deserializeAttributes(in, options);

	// regenerate the mesh
	generateMesh();
}

//! Creates a clone of this scene node and its children.
scene::ISceneNode* IAtmosphereSkySceneNode::clone(scene::ISceneNode* newParent, scene::ISceneManager* newManager)
{
	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

	IAtmosphereSkySceneNode* nb = new IAtmosphereSkySceneNode(newParent, newManager, ID, Buffer->Material.TextureLayer[0].Texture, Faces, Radius);

	nb->cloneMembers(this, newManager);

	if ( newParent )
		nb->drop();
	return nb;
}

//! Updates UV mapping X coordinate.
void IAtmosphereSkySceneNode::setUvX(f32 uvx)
{
    UvX = uvx;
}

} // namespace irr
