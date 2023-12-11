// Copyright (C) 2011 Jaromir Veber

#ifndef __I_WSKIN_MESH_BUFFER_H_INCLUDED__
#define __I_WSKIN_MESH_BUFFER_H_INCLUDED__


#include "SSkinMeshBuffer.h"

namespace irr
{
namespace scene
{

//! WeightSSkinMeshBuffer  An extension of SSkinMeshBuffer supporting storage of Weights
struct WSSkinMeshBuffer : public SSkinMeshBuffer
{
	WSSkinMeshBuffer(video::E_VERTEX_TYPE vt=video::EVT_STANDARD) : SSkinMeshBuffer(vt)
	{
		#ifdef _DEBUG
		setDebugName("WSSkinMeshBuffer");
		#endif
	}

	//! A vertex weight
	struct SWeight
	{
		//! Weight Strength/Percentage (0-1)
		f32 strength;
		//! Index of Joint
		void* Joint;
	};

	//! Skin weights (2D array - 1st dimesion is vertex position; 2nd dimesion is weight position)
	core::array< core::array<SWeight> > weight_array;

	//! Initial positions in mesh bufer
	core::array< core::vector3df > InitialPositions;
	//! Initial normals in mesh bufer
	core::array< core::vector3df > InitialNormals;
	//TODO if someone need it I could not test it so I decided to skip tangents
	// there's almost same alogo as for normals but it does not need normalisation
	//core::array< core::vector3df > InitialTangents;
};



} // end namespace scene
} // end namespace irr
#endif //#ifndef __I_WSKIN_MESH_BUFFER_H_INCLUDED__