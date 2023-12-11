// Copyright (C) 2011 tbw

// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.

// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:

//1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//3. This notice may not be removed or altered from any source distribution.

//! This Class was partly rewritten by Gawaboumga (Youri Hubaut).
//! But all rights remain at tbw.

#include "CloudSceneNode.h"

namespace irr
{

CCloudSceneNode::CCloudSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) :
	scene::ISceneNode(parent, mgr, id)
{
	#ifdef _DEBUG
	setDebugName("CCloudSceneNode");
	#endif

	// turn off automatic culling
	setAutomaticCulling(scene::EAC_OFF);

	BufferInner = new scene::SMeshBuffer();
	BufferOuter = new scene::SMeshBuffer();

	// set the default material properties
	Material.Lighting = false;
	Material.ZBuffer = video::ECFN_NEVER;
	Material.ZWriteEnable = false;
	Material.AntiAliasing = video::EAAM_OFF;

	// setup the texture wrap
	Material.TextureLayer[0].TextureWrapU = video::ETC_REPEAT;
	Material.TextureLayer[0].TextureWrapV = video::ETC_REPEAT;

	// use texture blend as default materialtype to ensure correct
	// outfading of textures with alpha channel
	Material.MaterialType = video::EMT_ONETEXTURE_BLEND;
	Material.MaterialTypeParam = video::pack_textureBlendFunc(
		video::EBF_SRC_ALPHA,
		video::EBF_ONE_MINUS_SRC_ALPHA,
		video::EMFN_MODULATE_1X,
		video::EAS_TEXTURE | video::EAS_VERTEX_COLOR);

	// initialize memeber variables
	LastUpdate = 0;

	// set the texture scale
	TextureScale = 1.0f;
	Material.getTextureMatrix(0).setTextureScale(TextureScale, TextureScale);

	// set the radius of the cloud
	InnerRadius = 1.5f;
	OuterRadius = 2.0f;

	// set the height of the cloud
	CenterHeight = 0.5f;
	InnerHeight = 0.1f;
	OuterHeight = 0.0f;

	// set the color
	CenterColor = video::SColor(220,220,220,220);
	InnerColor = video::SColor(180,220,220,220);
	OuterColor = video::SColor(0,220,220,220);

	// create the cloud with the given properties
	createCloudLayer();
}

//! Destructor.
CCloudSceneNode::~CCloudSceneNode()
{
    if (BufferInner)
		BufferInner->drop();
    if (BufferOuter)
		BufferOuter->drop();
}

void CCloudSceneNode::createCloudLayer()
{
	// setup a 2d vector which points to the edge of the ring
	core::vector2d<f32> edge(OuterRadius, 0.f);
	// calculate the factor (inner ring)
	f32 factor = InnerRadius / OuterRadius;

	BufferInner->Vertices.clear();
	BufferInner->Indices.clear();

	BufferInner->Vertices.reallocate( CLOUDSUBDIV + 1 );
	BufferInner->Indices.reallocate( CLOUDSUBDIV + 2 );

    BufferOuter->Vertices.clear();
	BufferOuter->Indices.clear();

	BufferOuter->Vertices.reallocate( 2 * CLOUDSUBDIV );
	BufferOuter->Indices.reallocate( 2 * CLOUDSUBDIV + 2 );

	// first vertex is the center of the cloud dome
	BufferInner->Vertices.push_back(video::S3DVertex(0, CenterHeight, 0, 0.f, -1.f, 0.f, CenterColor, 0, 0));

	// now calculate the vertices on the inner ring and on the outer ring
	for (u16 i = 1; i < CLOUDSUBDIV + 1; i++)
	{
		edge = edge.rotateBy(360.f/(f32)CLOUDSUBDIV, core::vector2d<f32>(0.f, 0.f));
		BufferInner->Vertices.push_back(video::S3DVertex(edge.X*factor, InnerHeight, edge.Y*factor, 0.f, -1.f, 0.f, InnerColor, edge.X*factor, edge.Y*factor));
		BufferOuter->Vertices.push_back(video::S3DVertex(edge.X*factor, InnerHeight, edge.Y*factor, 0.f, -1.f, 0.f, InnerColor, edge.X*factor, edge.Y*factor));
		BufferOuter->Vertices.push_back(video::S3DVertex(edge.X, OuterHeight, edge.Y, 0.f, -1.f, 0.f, OuterColor, edge.X, edge.Y));
	}

	// setup the indices for the inner dome
    for (u16 i = 1; i < CLOUDSUBDIV; i++)
    {
        BufferInner->Indices.push_back(0);
        BufferInner->Indices.push_back(i);
        BufferInner->Indices.push_back(i + 1);
    }

    BufferInner->Indices.push_back(0);
    BufferInner->Indices.push_back(CLOUDSUBDIV);
    BufferInner->Indices.push_back(1);

    // setup the indices for the outer dome
    for (u16 i = 0; i < CLOUDSUBDIV - 1; i++)
    {
        // Left triangles 2 --- 0
        //                  \   |
        //                    \ 1
        BufferOuter->Indices.push_back(2 * i);
        BufferOuter->Indices.push_back(2 * i + 1);
        BufferOuter->Indices.push_back(2 * i + 2);

        // Right triangles 2 \
        //                 |   \
        //                 3 --- 1

        BufferOuter->Indices.push_back(2 * i + 1);
        BufferOuter->Indices.push_back(2 * i + 3);
        BufferOuter->Indices.push_back(2 * i + 2);
    }

    // Left END triangles 0 --- 2 * CLOUDSUBDIV - 2
    //                      \   |
    //                        \ 2 * CLOUDSUBDIV - 1

    BufferOuter->Indices.push_back(2 * CLOUDSUBDIV - 2);
    BufferOuter->Indices.push_back(2 * CLOUDSUBDIV - 1);
    BufferOuter->Indices.push_back(0);

    // Right END triangles 0 \
    //                     |   \
    //                     1 --- 2 * CLOUDSUBDIV - 1

    BufferOuter->Indices.push_back(2 * CLOUDSUBDIV - 1);
    BufferOuter->Indices.push_back(1);
    BufferOuter->Indices.push_back(0);
}

void CCloudSceneNode::setTranslation(const core::vector2d<f32>& translation)
{
	// set the translation
	Translation = translation;
}

void CCloudSceneNode::setTextureScale(f32 textureScale)
{
	// set the texture scale
	TextureScale = textureScale;

	// apply the new scale to the texture
	Material.getTextureMatrix(0).setTextureScale(TextureScale, TextureScale);
}

void CCloudSceneNode::setCloudRadius(f32 innerRadius, f32 outerRadius)
{
	// set the new radius for the inner and the outer ring
	InnerRadius = innerRadius;
	OuterRadius = outerRadius;

	// recreate the vertices
	createCloudLayer();
}

void CCloudSceneNode::setCloudHeight(f32 centerHeight, f32 innerHeight, f32 outerHeight)
{
	// set the new cloud height
	CenterHeight = centerHeight;
	InnerHeight = innerHeight;
	OuterHeight = outerHeight;

	// update the vertices
	// we don't need to call createCloudLayer here
	// because only the y-coord of the vertices has to be changed
	BufferInner->Vertices[0].Pos.Y = CenterHeight;

	for (u16 i = 0; i < CLOUDSUBDIV; i++)
	{
		BufferInner->Vertices[i+1].Pos.Y = InnerHeight;
		BufferOuter->Vertices[2*i].Pos.Y = InnerHeight;
		BufferOuter->Vertices[2*i+1].Pos.Y = OuterHeight;
	}
}

void CCloudSceneNode::setCloudColor(const video::SColor& centerColor, const video::SColor& innerColor, const video::SColor& outerColor)
{
	// set the new cloud color
	CenterColor = centerColor;
	InnerColor = innerColor;
	OuterColor = outerColor;

	// update the vertices
	// we don't need to call createCloudLayer here
	// because only the color property of the vertices has to be changed
	BufferInner->Vertices[0].Color = CenterColor;

	for (u16 i = 0; i < CLOUDSUBDIV; i++)
	{
		BufferInner->Vertices[i+1].Color = InnerColor;
		BufferOuter->Vertices[2*i].Color = InnerColor;
		BufferOuter->Vertices[2*i+1].Color = OuterColor;
	}
}

void CCloudSceneNode::OnRegisterSceneNode()
{
	// if node is visible register it for the ESNRP_SKY_BOX pass
	if (IsVisible)
		SceneManager->registerNodeForRendering(this, scene::ESNRP_SKY_BOX);

	// call base OnRegisterSceneNode
	ISceneNode::OnRegisterSceneNode();
}

void CCloudSceneNode::OnAnimate(u32 timeMs)
{
	// call base OnAnimate
	ISceneNode::OnAnimate(timeMs);

	// calculate the elapsed time
	u32 time = timeMs-LastUpdate;

	// calculate the new translation of the texture
	CurrentTranslation += Translation*(f32)time*0.001f;
	// apply the new translation to the cloud texture
	Material.getTextureMatrix(0).setTextureTranslate(CurrentTranslation.X, CurrentTranslation.Y);

	// store the last update time
	LastUpdate = timeMs;
}

void CCloudSceneNode::render()
{
	// get the videodriver and the active camera
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

	// return if we don't have a valid driver or a valid camera
	if (!camera || !driver)
		return;

	// render only if the camera is not orthogonal
	if (!camera->isOrthogonal())
	{
		// get the correct transformation
		core::matrix4 translate(AbsoluteTransformation);
		translate.setTranslation(camera->getAbsolutePosition());

		// draw the clouds between the near and far clip plane
		const f32 viewDistance = (camera->getNearValue() + camera->getFarValue()) * 0.5f;
		core::matrix4 scale;
		scale.setScale(core::vector3df(viewDistance, viewDistance, viewDistance));

		// set the new world transformation
		driver->setTransform(video::ETS_WORLD, translate * scale);

		// set the material
		driver->setMaterial(Material);

		// render the vertices
		driver->drawMeshBuffer(BufferInner);
		driver->drawMeshBuffer(BufferOuter);

		// show mesh if desired
		if (DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY)
		{
			// setup a new material with wireframe property set to true
			video::SMaterial m;
			m.Wireframe = true;
			m.Lighting = false;
			m.AntiAliasing = 0;
			driver->setMaterial(m);

			// render the vertices as above
            driver->drawMeshBuffer(BufferInner);
            driver->drawMeshBuffer(BufferOuter);
		}
	}
}

const core::aabbox3d<f32>& CCloudSceneNode::getBoundingBox() const
{
	// return the bounding box
	return BufferInner->BoundingBox;
}

scene::ESCENE_NODE_TYPE CCloudSceneNode::getType() const
{
	// return the scene node type
	// (important when using with a custom scene node factory)
	return scene::ESNT_UNKNOWN;
	//return (ESCENE_NODE_TYPE)ECSNT_CLOUD;
}

u32 CCloudSceneNode::getMaterialCount() const
{
	// return the material count (always one in our case)
	return 1;
}

video::SMaterial& CCloudSceneNode::getMaterial(u32 i)
{
	// return the material
	return Material;
}

void CCloudSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	// write out the attributes of the scene node.
	ISceneNode::serializeAttributes(out, options);

	// radius of the cloud
	out->addFloat("InnerRadius", InnerRadius);
	out->addFloat("OuterRadius", OuterRadius);

	// height of the cloud
	out->addFloat("CenterHeight", CenterHeight);
	out->addFloat("InnerHeight", InnerHeight);
	out->addFloat("OuterHeight", OuterHeight);

	// color of the cloud
	out->addColor("CenterColor", CenterColor);
	out->addColor("InnerColor", InnerColor);
	out->addColor("OuterColor", OuterColor);

	// translation of the cloud
	out->addFloat("TranslationX", Translation.X);
	out->addFloat("TranslationY", Translation.Y);

	// current translation of the cloud
	out->addFloat("CurrentTranslationX", CurrentTranslation.X);
	out->addFloat("CurrentTranslationY", CurrentTranslation.Y);

	// and the scale of the cloud texture
	out->addFloat("TextureScale", TextureScale);
}

void CCloudSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	// read the attributes of the scene node.
	ISceneNode::deserializeAttributes(in, options);

	// radius of the cloud
	InnerRadius = in->getAttributeAsFloat("InnerRadius");
	OuterRadius = in->getAttributeAsFloat("OuterRadius");

	// height of the cloud
	CenterHeight = in->getAttributeAsFloat("CenterHeight");
	InnerHeight = in->getAttributeAsFloat("InnerHeight");
	OuterHeight = in->getAttributeAsFloat("OuterHeight");

	// color of the cloud
	CenterColor = in->getAttributeAsColor("CenterColor");
	InnerColor = in->getAttributeAsColor("InnerColor");
	OuterColor = in->getAttributeAsColor("OuterColor");

	// translation of the cloud
	Translation.X = in->getAttributeAsFloat("TranslationX");
	Translation.Y = in->getAttributeAsFloat("TranslationY");

	// current translation of the cloud
	CurrentTranslation.X = in->getAttributeAsFloat("CurrentTranslationX");
	CurrentTranslation.Y = in->getAttributeAsFloat("CurrentTranslationY");

	// and the scale of the cloud texture
	TextureScale = in->getAttributeAsFloat("TextureScale");

	// finally create the vertices
	createCloudLayer();
}

scene::ISceneNode* CCloudSceneNode::clone(scene::ISceneNode* newParent, scene::ISceneManager* newManager)
{
	// set the parent
	if (!newParent)
		newParent = Parent;
	// and set the scene manager
	if (!newManager)
		newManager = SceneManager;

	// create a new cloud scene node
	CCloudSceneNode* nb = new CCloudSceneNode(newParent, newManager, ID);

	// clone the members
	nb->cloneMembers(this, newManager);

	// clone the material
	nb->Material = Material;

	// set the current translation
	nb->CurrentTranslation = CurrentTranslation;

	// set the translatioon
	nb->setTranslation(Translation);

	// set the scale of the cloud texture
	nb->setTextureScale(TextureScale);

	// set the cloud color
	nb->setCloudColor(CenterColor, InnerColor, OuterColor);

	// set the cloud height
	nb->setCloudHeight(CenterHeight, InnerHeight, OuterHeight);

	// set the raduis of the cloud
	nb->setCloudRadius(InnerRadius, OuterRadius);

	// drop the created node
	nb->drop();

	// return the created node
	return nb;
}

} // namespace irr
