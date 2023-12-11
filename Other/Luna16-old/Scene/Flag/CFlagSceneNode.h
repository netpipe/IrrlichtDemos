#ifdef _WIN32
#pragma once
#endif
//********************************************************************************************
// File:	CFlagSceneNode.h
// Purpose:	Realtime flag vertex animation. Based on Flagsim by
//			created by Patrick Engel and Philipp Crocoll February, 2003. Change the way
//			normal are created to a more robust method.
// Started: 23rd October 2011
// Last ed:
// History:
//********************************************************************************************

#ifndef __CFLAGSCENENODE_H__
#define __CFLAGSCENENODE_H__

#include <math.h>
#include <irrlicht.h>
#include "CWindGen.h"


using namespace irr;
using namespace core;
using namespace video;
using namespace scene;


class CFlagSceneNode : public ISceneNode
{
protected:

	SMesh*		Mesh;				// the vertex animated flag mesh
	CWindGen*	windgen;			// wind generator
	int			Xsubdivision;		// x dim subdivisions
	int			Ysubdivision;		// y dim subdivisions
	float		spring_const;		// springyness
	float		damping;			// bounce reducer
	vector3df	gravity;			// the tug
	float		directdistance;		// horizontal dist
	float		aslantdistance;		// diagonal
	float		deltatime;			// sampling rate
	vector3df*	velocity;			// array of velocities
	vector3df*	accel;				// array of accelerations

public:

	CFlagSceneNode(ISceneNode* parent, ISceneManager* mgr,s32 id, CWindGen* wgen,
		float size = 10.0f, u32 xsub = 10, u32 ysub = 10, float spring = 50.0f, float dmping = 0.997,
		float sampling = 0.05,
		const vector3df& grav = vector3df(0.0f,-0.098f,0.000f),
        IImage*	vmap = 0,
		const vector3df& position = vector3df(0,0,0),
		const vector3df& rotation = vector3df(0,0,0),
		const vector3df& scale = vector3df(1.0f, 1.0f, 1.0f))
		:	ISceneNode(parent, mgr, id, position, rotation, scale),
			Mesh(0),
			velocity(0),
			accel(0),
			windgen(wgen),
			deltatime(sampling),
			gravity(grav),
			damping(dmping),
			spring_const(spring)
	{
		Initialize(xsub, ysub, size, vmap);
		getMaterial().setFlag(EMF_BACK_FACE_CULLING,false);
		getMaterial().setFlag(EMF_LIGHTING,false);
	}
	~CFlagSceneNode()
	{
		if(Mesh)
			Mesh->drop();
		if(velocity)
			delete[] velocity;
		if(accel)
			delete[] accel;
	}
	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
		{
			SceneManager->registerNodeForRendering(this, ESNRP_SOLID);
			ISceneNode::OnRegisterSceneNode();
		}
	}
	virtual void render();
	virtual void OnAnimate(u32 timeMs)
	{
		if (IsVisible){
			UpdateVertices((float)timeMs * 0.01f);
		ISceneNode::OnAnimate(timeMs);}
	}
	virtual SMaterial& getMaterial(u32 i = 0)				{ return Mesh->getMeshBuffer(0)->getMaterial(); }
	virtual u32 getMaterialCount() const					{ return 1; }
	virtual const aabbox3df& getBoundingBox() const			{ return Mesh->getMeshBuffer(0)->getBoundingBox(); }
	virtual ESCENE_NODE_TYPE getType() const				{ return ESNT_MESH; }

private:

	void Initialize(int xsubdiv, int ysubdiv, float FlagSize, IImage* vmap);
	void UpdateVertices(float timeOffset);
	void GetForce(int xVertex, int yVertex, vector3df& OtherVertexPos, float StandardDistance , vector3df& result);
	void UpdateNormals();
};

//********************************************************************************************
// callback class for the flag shader

class FlagShaderCallBack : public IShaderConstantSetCallBack
{
	int			light;
	SColorf		ambient;
	SColorf		diffuse;
	SColorf		translucency;
	int			sampler0;

public:

	virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		services->setVertexShaderConstant("gDiffuse", (float*)(&diffuse), 4);
		services->setVertexShaderConstant("gAmbient", (float*)(&ambient), 4);
		services->setVertexShaderConstant("gTranslucency", (float*)(&translucency), 4);
		services->setVertexShaderConstant("glight", (float*)(&light), 1);
		services->setPixelShaderConstant("diffuseMapTexture",(float*)(&sampler0),1);
	}
	void setLight(const int l)				{ light = l; }
	void setAmbient(const SColorf& c)		{ ambient = c; }
	void setDiffuse(const SColorf& c)		{ diffuse = c; }
	void setTranslucency(const SColorf& c)	{ translucency = c; }
	void setSampler(const int i)			{ sampler0 = i; }
};


//********************************************************************************************

#endif

