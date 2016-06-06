#ifndef SCENE_INTERFACE_H
#define SCENE_INTERFACE_H

// Wrapper class for Squirrel
// Coded by Evo
// Version 0.1 Date: 18-september-2006
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the author be held liable for any damages
// arising from the use of this software.
//
// This code is placed in the public domain

#include "definitions.h"

using namespace irr;
using namespace SqPlus;

class vector3	// a basic vector 3df class to interface with Squirrel
{
public:
	f32	X, Y, Z;
	vector3() : X(0), Y(0), Z(0) {};
	vector3(f32 x, f32 y, f32 z) : X(x), Y(y), Z(z) {};
	vector3(core::vector3df irrvector3) : X(irrvector3.X), Y(irrvector3.Y), Z(irrvector3.Z) {};
   ~vector3() {};

	vector3 operator+(vector3 &v)
	{
		return vector3(X + v.X, Y + v.Y, Z + v.Z);
	};

	vector3 operator-(vector3 &v)
	{
		return vector3(X - v.X, Y - v.Y, Z - v.Z);
	};

	vector3 operator*(vector3 &v)
	{
		return vector3(X * v.X, Y * v.Y, Z * v.Z);
	};

	f32 getLength() { return sqrt(X*X + Y*Y + Z*Z); };

	vector3 &normalize()
	{
		f32 l = getLength();
		if (l == 0)
			return *this;

		l = 1.0f / l;
		X *= l;
		Y *= l;
		Z *= l;
		return *this;
	}

	vector3 getHorizontalAngle()
	{
		core::vector3df angle = core::vector3df(X, Y, Z);
		return angle.getHorizontalAngle();
	}

	// custom constructor code
	static int construct(HSQUIRRELVM v)
	{
		StackHandler sa(v);
		u32 paramCount = sa.GetParamCount();
		if (paramCount == 1)
			return PostConstruct<vector3>(SquirrelVM::GetVMPtr(), new vector3(), release);
		if (paramCount == 4)
			return PostConstruct<vector3>(SquirrelVM::GetVMPtr(), new vector3(sa.GetFloat(2),sa.GetFloat(3),sa.GetFloat(4) ), release);
		return sq_throwerror(v,_T("Invalid Constructor arguments"));
	}
	SQ_DECLARE_RELEASE(vector3)
};
DECLARE_INSTANCE_TYPE(vector3);

class Scene_Interface	// Class for interfacing with Irrlicht function that you may need
						// Feel free to add to these functions. Please post your additions on the irrlicht forum
{
public:
	 Scene_Interface(void);
	~Scene_Interface(void);

	// functions bound to Squirrel+
	bool		setCurrentNode(const u32 node);					// following functions from ISceneNode
	int			getCurrentNode();

	vector3		getPosition();
	vector3		getRotation();
	vector3		getScale();

	bool		setPosition(vector3 *value);
	bool		setRotation(vector3 *value);
	bool		setScale   (vector3 *value);

	bool		setFrameLoop(const s32 begin, const s32 end);	// from IAnimatedMeshSceneNode
	bool		setFrameType(const u32 type);
	s32			getFrameNr();

	vector3		getCameraPosition();
	vector3		getCameraRotation();
	vector3		getCameraTarget();
	bool		setCameraTarget(vector3 *value);

	// functions NOT bound to Squirrel+
	void		setSceneManager(scene::ISceneManager *mgr);
	void		setCamera(scene::ICameraSceneNode *cam);

	// public variables: define and bind to Squirrel as needed
	int			CurrentTick;
	int			LastTick;
	vector3		SydVec;		// position of first Sydney

private:
	scene::ISceneManager		*smgr;
	scene::ISceneNode			*CurrentNode;
	scene::ICameraSceneNode		*camera;
};
DECLARE_INSTANCE_TYPE(Scene_Interface);

#endif	// SCENE_INTERFACE_H
