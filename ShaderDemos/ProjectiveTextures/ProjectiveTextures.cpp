#include "ProjectiveTextures.h"

/* CONSTRUCTOR */
CProjectiveTextures::CProjectiveTextures(ISceneNode* parent, ISceneManager* mgr, s32 id,
	const core::vector3df& position, const core::vector3df& lookat)
	: ISceneNode(parent, mgr, id, position)
{
	// set default projection
	Fovy = core::PI / 2.5f;	// Field of view, in radians.

	Target = lookat;

	recalculateProjectionMatrix();
	recalculateViewArea();

	// Load the shaders.
	projTex = pGPU->addShaderMaterialFromFiles("shader/projector.vs",
											   "shader/projector.fs",
											   this,video::EMT_TRANSPARENT_ADD_COLOR,0);
}

/* DESTRUCTOR */
CProjectiveTextures::~CProjectiveTextures()
{
	// Remove the texture used.
	pVideo->removeTexture(texture);
}

//! sets the look at target of the projector
void CProjectiveTextures::setTarget(const vector3df& pos)
{
	Target = pos;
	const vector3df toTarget = Target - getAbsolutePosition();
}

//! Gets the current look at target of the projector
const vector3df& CProjectiveTextures::getTarget() const
{
	return Target;
}

void CProjectiveTextures::setFOV(f32 f)
{
	Fovy = f;
	recalculateProjectionMatrix();
}

f32 CProjectiveTextures::getFOV()
{
	return Fovy;
}

//! returns the axis aligned bounding box of this node
const aabbox3d<f32>& CProjectiveTextures::getBoundingBox() const
{
	return ViewArea.getBoundingBox();
}

/* UPDATE */
void CProjectiveTextures::render()
{
	vector3df pos = getAbsolutePosition();
	vector3df tgtv = Target - pos;
	tgtv.normalize();

	// if upvector and vector to the target are the same, we have a
	// problem. so solve this problem:
	vector3df up(0.0f,0.0f,1.0f);

	ViewArea.getTransform(ETS_VIEW).buildCameraLookAtMatrixLH(pos,Target,up);

	recalculateViewArea();

	for(u32 i=0; i<nodeArray.size(); ++i)
	{
		projMat.MaterialType = (E_MATERIAL_TYPE)projTex;
		projMat.TextureLayer[0].Texture = texture;
//		projMat.TextureLayer[0].TextureWrap = ETC_CLAMP_TO_BORDER;

		pVideo->setTransform(ETS_WORLD,nodeArray[i]->getAbsoluteTransformation());
		pVideo->setMaterial(projMat);

		for(u32 j=0; j<nodeArray[i]->getMaterialCount(); ++j)
			pVideo->drawMeshBuffer(nodeArray[i]->getMesh()->getMeshBuffer(j));
	}
}

void CProjectiveTextures::recalculateProjectionMatrix()
{
	ViewArea.getTransform(ETS_PROJECTION ).buildProjectionMatrixPerspectiveFovLH(Fovy,1.0f,1.0f,3000.0f);
}

void CProjectiveTextures::recalculateViewArea()
{
	ViewArea.cameraPosition = getAbsolutePosition();

	matrix4 m(matrix4::EM4CONST_NOTHING);
	m.setbyproduct_nocheck(ViewArea.getTransform(ETS_PROJECTION),ViewArea.getTransform(ETS_VIEW));
	ViewArea.setFrom(m);
}

void CProjectiveTextures::OnSetConstants(IMaterialRendererServices* services,s32 userData)
{
	if(userData == 0)
	{
		matrix4 bias;
		bias.makeIdentity();
		bias.setTranslation(vector3df(0.5f,0.5f,0));
		bias.setScale(vector3df(0.5f,-0.5f,1.0f));

		matrix4 worldViewProj;
		worldViewProj = bias * ViewArea.getTransform(ETS_PROJECTION);
		worldViewProj *= ViewArea.getTransform(ETS_VIEW);
		worldViewProj *= pVideo->getTransform(ETS_WORLD);
		services->setVertexShaderConstant(reinterpret_cast<const f32*>(worldViewProj.pointer()),5,4);
	}
}
