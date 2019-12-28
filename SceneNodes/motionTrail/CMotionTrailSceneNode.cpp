// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// created by Dean Wadsworth aka Varmint Jan 12th 2008
#include <COpenGLDriver.h>

#include <COpenGLTexture.h>


#include "CMotionTrailSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "os.h"

namespace irr
{

inline void CatmullRomCalc(core::vector3df &out, f32 t, const core::vector3df p0, const core::vector3df p1, const core::vector3df p2, const core::vector3df p3)
{
	f32 t2 = t * t;
	f32 t3 = t2 * t;

	out.X = 0.5f * ( ( 2.0f * p1.X ) +
		( -p0.X + p2.X ) * t +
		( 2.0f * p0.X - 5.0f * p1.X + 4 * p2.X - p3.X ) * t2 +
		( -p0.X + 3.0f * p1.X - 3.0f * p2.X + p3.X ) * t3 );
	out.Y = 0.5f * ( ( 2.0f * p1.Y ) +
		( -p0.Y + p2.Y ) * t +
		( 2.0f * p0.Y - 5.0f * p1.Y + 4 * p2.Y - p3.Y ) * t2 +
		( -p0.Y + 3.0f * p1.Y - 3.0f * p2.Y + p3.Y ) * t3 );
	out.Z = 0.5f * ( ( 2.0f * p1.Z ) +
		( -p0.Z + p2.Z ) * t +
		( 2.0f * p0.Z - 5.0f * p1.Z + 4 * p2.Z - p3.Z ) * t2 +
		( -p0.Z + 3.0f * p1.Z - 3.0f * p2.Z + p3.Z ) * t3 );
}

inline f32 interpolate(const f32 from, const f32 to, const f32 factor)
{
   return from + factor*(to-from);
}

namespace scene
{

//! constructor
CMotionTrailSceneNode::CMotionTrailSceneNode(ISceneNode* parent, ISceneManager* mgr,
		s32 id, IrrlichtDevice* inDevice,
		const core::vector3df& position,
		const core::vector3df& rotation, const core::vector3df& scale)
	: IMotionTrailSceneNode(parent, mgr, id, irrDevice, position, rotation, scale),
		maxMotionTrails(60), lastAddedPos(core::vector3df(0.0f, 0.0f, 0.0f)), lifetime(0.4f), curveTessel(0.05f), lastTime(0)
{
	#ifdef _DEBUG
	setDebugName("CMotionTrailSceneNode");
	#endif

	trailCount =  0;
	trailList.Head = NULL;
	trailList.Curr = NULL;

	//trailMaterial.MaterialType = video::EMT_ONETEXTURE_BLEND;
	//trailMaterial.MaterialTypeParam = pack_texureBlendFunc( video::EBF_SRC_ALPHA, video::EBF_ONE_MINUS_SRC_ALPHA, video::EMFN_MODULATE_1X );
	//trailMaterial.MaterialTypeParam = pack_texureBlendFunc( video::EBF_SRC_COLOR, video::EBF_SRC_ALPHA, video::EMFN_MODULATE_1X );

	//trailMaterial.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	//trailMaterial.MaterialTypeParam = 0.01f;
	//trailMaterial.MaterialType = video::EMT_DETAIL_MAP;

	trailMaterial.Lighting = false;
	trailMaterial.DiffuseColor = video::SColor( 51, 127, 0, 204 ); //default motiontrail colour
	//trailMaterial.ZWriteEnable = false;
	//trailMaterial.BackfaceCulling = false;
	//trailMaterial.ZBuffer = false;

	//trailMaterial.Thickness = 3.0f;

	setAutomaticCulling(scene::EAC_OFF);

	irrDevice = inDevice;

	trailCount = 0;
}

CMotionTrailSceneNode::~CMotionTrailSceneNode()
{
	motionTrailSection * dead = trailList.Head;
	while(dead) {
		motionTrailSection * next = dead->Next;
		if (dead->colour) {
			free(dead->colour);
			free(dead->texCoords);
			free(dead->verts);
			free(dead->indicies);
		}
		free(dead);
		dead = next;
	}
}

void CMotionTrailSceneNode::renderTrail(u32 currentTrail)
{
	if (trailList.Curr->verts) {
		//it's already created just render it
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, trailList.Curr->colour);
		glTexCoordPointer(2, GL_FLOAT, sizeof(texcoords_s), trailList.Curr->texCoords);
		glVertexPointer(3, GL_FLOAT, sizeof(vertpoints_s), trailList.Curr->verts);
		glDrawElements(GL_QUADS, trailList.Curr->indices_count+1, GL_UNSIGNED_SHORT, trailList.Curr->indicies);
		return;
	}

	//create the trail
	if (trailCount < 2)
		return;

	core::vector3df startPoints[4], endPoints[4];

	if(currentTrail < 1)
		startPoints[0] = trailList.Head->start;
	else
		startPoints[0] = trailList.Curr->Prev->start;
	startPoints[1] = trailList.Curr->start;
	startPoints[2] = trailList.Curr->Next->start;
	if (trailCount-1 == 1)
		startPoints[3] = trailList.Head->start;
	else if(currentTrail+2 > trailCount-1)
		startPoints[3] = trailList.Curr->Prev->start;
	else
		startPoints[3] = trailList.Curr->Next->Next->start;

	if(currentTrail < 1)
		endPoints[0] = trailList.Head->end;
	else
		endPoints[0] = trailList.Curr->Prev->end;
	endPoints[1] = trailList.Curr->end;
	endPoints[2] = trailList.Curr->Next->end;
	if (trailCount-1 == 1)
		endPoints[3] = trailList.Head->end;
	else if(currentTrail+2 > trailCount-1)
		endPoints[3] = trailList.Curr->Prev->end;
	else
		endPoints[3] = trailList.Curr->Next->Next->end;

	motionTrailSection quad1 = *trailList.Curr;
	motionTrailSection quad2 = *trailList.Curr->Next;
	float	fPercentage1 = (float)currentTrail/(float)(trailCount-1);
	float	fPercentage2 = (float)(currentTrail+1)/(float)(trailCount-1);
	video::SColor clr = trailMaterial.DiffuseColor;

	core::vector3df start, end;
	float texCoordX;

	f32 fDist = core::vector3df(quad1.start - quad2.start).getLength();
	f32 fInc = curveTessel/fDist;
// -----------------------------------------------------------------------
	s32 size = (s32)((1.0f / fInc)+2.0f)*4;  //*4 for quads

	trailList.Curr->colour		= (u8 *)malloc(sizeof(u8)*size*4); //*4 four colours r,g,b,a
	memset(trailList.Curr->colour, '\0', (sizeof(u8)*size*4));
	trailList.Curr->texCoords	= (texcoords_s *)malloc(sizeof(texcoords_s)*size);
	memset(trailList.Curr->texCoords, '\0', (sizeof(texcoords_s)*size));
	trailList.Curr->verts		= (vertpoints_s *)malloc(sizeof(vertpoints_s)*size);
	memset(trailList.Curr->verts, '\0', (sizeof(vertpoints_s)*size));
	trailList.Curr->indicies	= (u16 *)malloc(sizeof(u16)*size);
	memset(trailList.Curr->indicies, '\0', (sizeof(u16)*size));

	trailList.Curr->indices_count = -1;

	s32 count = 0;
	s32 clrCount = 0;
// -----------------------------------------------------------------------

    for(f32 f=0.0; f<1.0; f+=fInc)
    {
		// Clamp to 1.0f due to floating point errors
		f32	nextf = core::min_(f+fInc, 1.0f);

		// Compute alpha for first side of quad
		clr.setAlpha(u32(interpolate(quad1.startAlpha, quad2.startAlpha, f)*255.0f));

		// Compute texture coordinate for first side of quad
		texCoordX = interpolate(fPercentage1, fPercentage2, f);

		// Compute interpolated coordinate of first side (top)
		CatmullRomCalc(start, f, startPoints[0], startPoints[1], startPoints[2], startPoints[3]);

		// First quad point
		trailList.Curr->indices_count++;
		clr.toOpenGLColor(&trailList.Curr->colour[clrCount]);
		trailList.Curr->indicies[count] = trailList.Curr->indices_count;
		trailList.Curr->verts[count].x = start.X;
		trailList.Curr->verts[count].y = start.Y;
		trailList.Curr->verts[count].z = start.Z;
		trailList.Curr->texCoords[count].x = texCoordX;
		trailList.Curr->texCoords[count].y = 0.0f;
		count++;
		clrCount+=4;

		// Compute interpolated coordinate of first side (bottom)
		CatmullRomCalc(end, f, endPoints[0], endPoints[1], endPoints[2], endPoints[3]);

		// Second quad point
		trailList.Curr->indices_count++;
		clr.toOpenGLColor(&trailList.Curr->colour[clrCount]);
		trailList.Curr->indicies[count] = trailList.Curr->indices_count;
		trailList.Curr->verts[count].x = end.X;
		trailList.Curr->verts[count].y = end.Y;
		trailList.Curr->verts[count].z = end.Z;
		trailList.Curr->texCoords[count].x = texCoordX;
		trailList.Curr->texCoords[count].y = 1.0f;
		count++;
		clrCount+=4;

		// Compute alpha for second side of quad
		clr.setAlpha(u32(interpolate(quad1.startAlpha, quad2.startAlpha, nextf)*255.0f));

		// Compute texture coordinate for second side of quad
		texCoordX = interpolate(fPercentage1, fPercentage2, nextf);

		// Compute interpolated coordinate of second side (bottom)
		CatmullRomCalc(end, nextf, endPoints[0], endPoints[1], endPoints[2], endPoints[3]);

		// Third quad point
		trailList.Curr->indices_count++;
		clr.toOpenGLColor(&trailList.Curr->colour[clrCount]);
		trailList.Curr->indicies[count] = trailList.Curr->indices_count;
		trailList.Curr->verts[count].x = end.X;
		trailList.Curr->verts[count].y = end.Y;
		trailList.Curr->verts[count].z = end.Z;
		trailList.Curr->texCoords[count].x = texCoordX;
		trailList.Curr->texCoords[count].y = 1.0f;
		count++;
		clrCount+=4;

		// Compute interpolated coordinate of first side (top)
		CatmullRomCalc(start, nextf, startPoints[0], startPoints[1], startPoints[2], startPoints[3]);

		// Forth quad point
		trailList.Curr->indices_count++;
		clr.toOpenGLColor(&trailList.Curr->colour[clrCount]);
		trailList.Curr->indicies[count] = trailList.Curr->indices_count;
		trailList.Curr->verts[count].x = start.X;
		trailList.Curr->verts[count].y = start.Y;
		trailList.Curr->verts[count].z = start.Z;
		trailList.Curr->texCoords[count].x = texCoordX;
		trailList.Curr->texCoords[count].y = 0.0f;
		count++;
		clrCount+=4;

		if ((count+4) >= size)
			break;
	}

	//if (size != count)
	//	printf("count: %i size: %i\n", count, size);
	if (trailList.Curr->verts) {
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, trailList.Curr->colour);
		glTexCoordPointer(2, GL_FLOAT, sizeof(texcoords_s), trailList.Curr->texCoords);
		glVertexPointer(3, GL_FLOAT, sizeof(vertpoints_s), trailList.Curr->verts);
		glDrawElements(GL_QUADS, trailList.Curr->indices_count+1, GL_UNSIGNED_SHORT, trailList.Curr->indicies);
	}
}

//! renders the node.
void CMotionTrailSceneNode::render()
{
	if (trailCount < 2)
		return;

	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	core::matrix4 mat;
	driver->setTransform(video::ETS_WORLD, mat);
	//driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	//driver->setMaterial(trailMaterial);

	//store settings to restore after rendering done
	GLboolean isBlendOn		= glIsEnabled(GL_BLEND);
	GLboolean isCullFace	= glIsEnabled(GL_CULL_FACE);
	GLboolean isDepthTest	= glIsEnabled(GL_DEPTH_TEST);
	GLboolean isTexture2D	= glIsEnabled(GL_TEXTURE_2D);
	GLboolean isAlphaOn		= glIsEnabled(GL_ALPHA_TEST);
	GLboolean isLightingOn	= glIsEnabled(GL_LIGHTING);
	GLboolean isDepthWritable = false;
	GLint blendSrcAlpha = 0, blendDstAlpha = 0;
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);
	glGetBooleanv(GL_DEPTH_WRITEMASK, &isDepthWritable);
	GLint textureModeEnv = 0;
	glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &textureModeEnv);
	GLint alphaTestFunc = 0;
	GLfloat alphaRef = 0.0f;
	glGetIntegerv(GL_ALPHA_TEST_FUNC, &alphaTestFunc);
	glGetFloatv(GL_ALPHA_TEST_REF, &alphaRef);
	//store settings to restore after rendering done

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.01f);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (!isBlendOn)
		glEnable(GL_BLEND);
	if (isLightingOn)
		glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	const video::ITexture* texture = trailMaterial.getTexture(0);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, static_cast<const video::COpenGLTexture*>(texture)->getOpenGLTextureName());
	}

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	motionTrailSection * curTrail = trailList.Curr;
	motionTrailSection * mptr = trailList.Head;
	for (u32 i = 0; i < (trailCount-1); i++) {
		if (mptr->ElapsedTime < lifetime) {
			trailList.Curr = mptr;
			renderTrail(i);
		}
		mptr = mptr->Next;
		if (!mptr)
			break;
	}

	trailList.Curr = curTrail;

	//restore env
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureModeEnv);
	glBlendFunc(blendSrcAlpha, blendDstAlpha);
	glAlphaFunc(alphaTestFunc, alphaRef);
	if (isDepthWritable)
		glDepthMask(GL_TRUE);
	if (!isDepthTest)
		glDisable(GL_DEPTH_TEST);
	if (isCullFace)
		glEnable(GL_CULL_FACE);
	if (!isBlendOn)
		glDisable(GL_BLEND);
	if (!isTexture2D)
		glDisable(GL_TEXTURE_2D);
	if (!isAlphaOn)
		glDisable(GL_ALPHA_TEST);
	if (isLightingOn)
		glEnable(GL_LIGHTING);
}

void CMotionTrailSceneNode::updateTrail(core::vector3df inStart, core::vector3df inEnd, bool addMoreTrails)
{
	if (addMoreTrails && trailCount < maxMotionTrails)
		addTrail(inStart, inEnd);

	u32 curTime = irrDevice->getTimer()->getTime();
	if (trailCount > 0) {
		motionTrailSection * dead = trailList.Head;
		while(dead) {
			motionTrailSection * next = dead->Next;
			dead->ElapsedTime += (f32)(curTime - lastTime) / 1000.0f;
			if (dead->ElapsedTime >= lifetime) {
				if (dead->Prev)
					dead->Prev->Next = next;
				if (next)
					next->Prev = dead->Prev;
				if (trailList.Head == dead) {
					trailList.Head = next;
					if (next)
						setPosition((next->start + next->end) * 0.5f);
				}
				if (trailList.Curr == dead) {
					if (next)
						trailList.Curr = next;
					else if (dead->Prev)
						trailList.Curr = dead->Prev;
					if (!trailList.Curr)
						trailList.Curr = trailList.Head;
				}
				if (dead->colour) {
					free(dead->colour);
					free(dead->texCoords);
					free(dead->verts);
					free(dead->indicies);
				}
				free(dead);
				trailCount--;
			}
			dead = next;
		}
		if (trailCount < 0)
			trailCount = 0;
		if (trailCount == 0) {
			trailList.Head = NULL;
			trailList.Curr = NULL;
		}
	}

	lastTime = curTime;
}


void CMotionTrailSceneNode::updateBoundingBox()
{
   if(1 == trailCount)
   {
      // Reset object box
      boundingBox.reset(0,0,0);
	  boundingBox.addInternalPoint(trailList.Head->start);
	  boundingBox.addInternalPoint(trailList.Head->end);
      // Update the world space position
   } else {
      // Calculate new object box
      core::vector3df vecDist1 = trailList.Curr->start - getAbsolutePosition();
      core::vector3df vecDist2 = trailList.Curr->end - getAbsolutePosition();

      // Has vecDist1 new MinEdge values?
      if(boundingBox.MinEdge.X > vecDist1.X)
         boundingBox.MinEdge.X = vecDist1.X;
      if(boundingBox.MinEdge.Y > vecDist1.Y)
         boundingBox.MinEdge.Y = vecDist1.Y;
      if(boundingBox.MinEdge.Z > vecDist1.Z)
         boundingBox.MinEdge.Z = vecDist1.Z;
      // Has start new MaxEdge values?
      if(boundingBox.MaxEdge.X < vecDist1.X)
         boundingBox.MaxEdge.X = vecDist1.X;
      if(boundingBox.MaxEdge.Y < vecDist1.Y)
         boundingBox.MaxEdge.Y = vecDist1.Y;
      if(boundingBox.MaxEdge.Z < vecDist1.Z)
         boundingBox.MaxEdge.Z = vecDist1.Z;
      // Has end new MinEdge values?
      if(boundingBox.MinEdge.X > vecDist2.X)
         boundingBox.MinEdge.X = vecDist2.X;
      if(boundingBox.MinEdge.Y > vecDist2.Y)
         boundingBox.MinEdge.Y = vecDist2.Y;
      if(boundingBox.MinEdge.Z > vecDist2.Z)
         boundingBox.MinEdge.Z = vecDist2.Z;
      // Has end new MaxEdge values?
      if(boundingBox.MaxEdge.X < vecDist2.X)
         boundingBox.MaxEdge.X = vecDist2.X;
      if(boundingBox.MaxEdge.Y < vecDist2.Y)
         boundingBox.MaxEdge.Y = vecDist2.Y;
      if(boundingBox.MaxEdge.Z < vecDist2.Z)
         boundingBox.MaxEdge.Z = vecDist2.Z;
   }
}

void CMotionTrailSceneNode::addTrail(core::vector3df inStart, core::vector3df inEnd)
{
	// Check, if the new pos is somewhat different from the last one.
	if(core::vector3df(lastAddedPos - inStart).getLength() < 0.1f)
		return;

	lastAddedPos = inStart;

	motionTrailSection * newTrail;

	newTrail = (motionTrailSection *)malloc(sizeof(motionTrailSection));
	memset(newTrail, '\0', sizeof(motionTrailSection));

	newTrail->start = inStart;
	newTrail->end   = inEnd;
	newTrail->ElapsedTime = 0.0f;
	newTrail->startAlpha = 1.0f;

	if (trailCount == 0) {
		trailList.Head = newTrail;
	}

	if (trailList.Curr) {
		newTrail->Prev = trailList.Curr;
		trailList.Curr->Next = newTrail;
		//need to free current & prev trail for smooth blending with next trail
		if (trailList.Curr->colour) {
			free(trailList.Curr->colour);
			trailList.Curr->colour = NULL;
			free(trailList.Curr->texCoords);
			trailList.Curr->texCoords = NULL;
			free(trailList.Curr->verts);
			trailList.Curr->verts = NULL;
			free(trailList.Curr->indicies);
			trailList.Curr->indicies = NULL;
		}
		if (trailList.Curr->Prev && trailList.Curr->Prev->colour) {
			free(trailList.Curr->Prev->colour);
			trailList.Curr->Prev->colour = NULL;
			free(trailList.Curr->Prev->texCoords);
			trailList.Curr->Prev->texCoords = NULL;
			free(trailList.Curr->Prev->verts);
			trailList.Curr->Prev->verts = NULL;
			free(trailList.Curr->Prev->indicies);
			trailList.Curr->Prev->indicies = NULL;
		}
	}
	trailList.Curr = newTrail;

	trailCount++;

	//boundingBox.reset(0,0,0);

	//boundingBox.addInternalPoint(trailList.Head->start);
	//boundingBox.addInternalPoint(trail[trailCount-1].end);

	if (trailCount == 1) {
		setPosition((inStart + inEnd) * 0.5f);
		//updateAbsolutePosition();
		//updateBoundingBox();
	}
}

//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CMotionTrailSceneNode::getBoundingBox() const
{
	return boundingBox;
}


void CMotionTrailSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
	}
	ISceneNode::OnRegisterSceneNode();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CMotionTrailSceneNode::getMaterial(u32 i)
{
	return trailMaterial;
}


//! returns amount of materials used by this scene node.
u32 CMotionTrailSceneNode::getMaterialCount() const
{
	return 1;
}


//! Writes attributes of the scene node.
void CMotionTrailSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

}


//! Reads attributes of the scene node.
void CMotionTrailSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{

	ISceneNode::deserializeAttributes(in, options);
}


//! Creates a clone of this scene node and its children.
ISceneNode* CMotionTrailSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

	CMotionTrailSceneNode* nb = new CMotionTrailSceneNode(newParent,
		newManager, ID, irrDevice);
	nb->cloneMembers(this, newManager);

	nb->drop();
	return nb;
}


} // end namespace scene
} // end namespace irr

