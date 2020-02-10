// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __E_SCENE_NODE_TYPES_H_INCLUDED__
#define __E_SCENE_NODE_TYPES_H_INCLUDED__

namespace irr
{
namespace scene  
{
	//! An enumeration for all types of built-in scene nodes 
	enum ESCENE_NODE_TYPE
	{
		//! simple cube scene node
		ESNT_CUBE = 0,

		//! Sphere scene node
		ESNT_SPHERE,

		//! Text Scene Node
		ESNT_TEXT,

		//! Water Surface Scene Node
		ESNT_WATER_SURFACE,

		//! Terrain Scene Node
		ESNT_TERRAIN,

		//! Sky Box Scene Node
		ESNT_SKY_BOX,

		//! Shadow Volume Scene Node
		ESNT_SHADOW_VOLUME,

		//! OctTree Scene Node
		ESNT_OCT_TREE,

		//! Mesh Scene Node
		ESNT_MESH,

		//! Light Scene Node
		ESNT_LIGHT,

		//! Empty Scene Node
		ESNT_EMPTY,

		//! Dummy Transformation Scene Node
		ESNT_DUMMY_TRANSFORMATION,

		//! Camera Scene Node
		ESNT_CAMERA,

		//! Maya Camera Scene Node
		ESNT_CAMERA_MAYA,

		//! First Person Shooter style Camera
		ESNT_CAMERA_FPS,

		//! Billboard Scene Node
		ESNT_BILLBOARD,

		//! Animated Mesh Scene Node
		ESNT_ANIMATED_MESH,

		//! Particle System Scene Node
		ESNT_PARTICLE_SYSTEM,

		//! Amount of build in Scene Nodes
		ESNT_COUNT,

		//! Unknown scene node
		ESNT_UNKNOWN,

		//! This enum is never used, it only forces the compiler to 
		//! compile these enumeration values to 32 bit.
		ESNT_FORCE_32_BIT = 0x7fffffff
	};

} // end namespace scene
} // end namespace irr


#endif

