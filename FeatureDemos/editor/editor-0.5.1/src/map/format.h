// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

// OSRPG MAP FORMAT

#ifndef OSRPG_EDITOR_MAP_FORMAT_H
#define OSRPG_EDITOR_MAP_FORMAT_H

#include <vector>

// Current format version

const unsigned char VERSION_MAJOR = 0;
const unsigned char VERSION_MINOR = 5;

// Helpful structures

typedef struct {
	float a,
		  b,
		  c;
} OMFVector3;

typedef struct {
	float a,
		  b,
		  c,
		  d;
} OMFVector4;

// File structures

typedef struct {
	unsigned char major,
				  minor;
} OMFVersion;

typedef struct {
	char name[32];
	char script_name[64];
	OMFVector3 gravity;
	bool skydome; // if false, use skybox
	unsigned short mesh_num,
				   light_num,
				   zone_num,
				   place_tag_num,
				   joint_ball_socket_num,
				   joint_hinge_num,
				   joint_slider_num,
				   joint_universal_num,
				   joint_hinge2_num,
				   joint_fixed_num;
} OMFHeader;

typedef struct {
	char top[64],
		 bottom[64],
		 left[64],
		 right[64],
		 front[64],
		 back[64];
} OMFSkyBox;

typedef struct {
	char texture[64];
	unsigned int horiz_res,
				 vert_res;
	double tpercentage,
		   spercentage;

} OMFSkyDome;

typedef struct {
	char name[32];
	char script_name[64];
	char model_file[64];
	char collision_model_file[64]; // if blank, use model_file
	OMFVector3 pos,
			   rot;
	char maggun_effect;
	float mass;
	bool collidable,
		 optimize;
} OMFMesh;

typedef struct {
	char name[32];
	OMFVector3 pos;
	float radius;
	OMFVector4 color;
} OMFLight;

typedef struct {
	char name[32];
	char model_file[64];
	char script_name[64];
	OMFVector3 pos,
			   rot;
} OMFZone;

typedef struct {
	char name[32];
	OMFVector3 pos;
} OMFPlaceTag;

typedef struct {
	// this is kinda slow, but ids don't fit here
	char mesh1_name[32],
		 mesh2_name[32];
	OMFVector3 anchor;
	bool constrain_x,
		 constrain_y,
		 constrain_z;
	short x_low,
		  x_high,
		  y_low,
		  y_high,
		  z_low,
		  z_high;
} OMFJointBallSocket;

typedef struct {
	char mesh1_name[32],
		 mesh2_name[32];
	OMFVector3 anchor,
			   axis;
	bool constrain;
	short angle_low,
		  angle_high;
} OMFJointHinge;

// Previous supported versions

// OMF 0.4

namespace OMF_0_4 {

	typedef struct {
		char name[32];
		char script_name[64];
		float gravity;
		unsigned short mesh_num,
					   light_num,
					   zone_num,
					   place_tag_num;
		bool skydome; // if false, use skybox
	} OMFHeader_0_4;

	typedef struct {
		char top[64],
			 bottom[64],
			 left[64],
			 right[64],
			 front[64],
			 back[64];
	} OMFSkyBox_0_4;

	typedef struct {
		char texture[64];
		unsigned int horiz_res,
					 vert_res;
		double tpercentage,
			   spercentage;
	} OMFSkyDome_0_4;

	typedef struct {
		char name[32];
		char model_file[64];
		OMFVector3 pos,
				   rot;
		char maggun_effect;
		float mass;
		bool collidable,
			 optimize;
	} OMFMesh_0_4;

	typedef struct {
		char name[32];
		OMFVector3 pos;
		float radius;
		OMFVector4 color;
	} OMFLight_0_4;

	typedef struct {
		char name[32];
		char model_file[64];
		char script_name[64];
		OMFVector3 pos,
				   rot;
	} OMFZone_0_4;

	typedef struct {
		char name[32];
		OMFVector3 pos;
	} OMFPlaceTag_0_4;

} // namespace OMF_0_4

// OMF 0.3

namespace OMF_0_3 {

	typedef struct {
		char name[32];
		char script_name[64];
		float gravity;
		unsigned short mesh_num,
					   light_num,
					   zone_num,
					   place_tag_num;
	} OMFHeader_0_3;

	typedef struct {
		char name[32];
		char model_file[64];
		OMFVector3 pos,
				   rot;
		char maggun_effect;
		float mass;
		bool optimize;
	} OMFMesh_0_3;

	typedef struct {
		char name[32];
		OMFVector3 pos;
		float radius;
		OMFVector4 color;
	} OMFLight_0_3;

	typedef struct {
		char name[32];
		char model_file[64];
		char script_name[64];
		OMFVector3 pos,
				   rot;
	} OMFZone_0_3;

	typedef struct {
		char name[32];
		OMFVector3 pos;
	} OMFPlaceTag_0_3;

} // namespace OMF_0_3

#endif // OSRPG_EDITOR_MAP_FORMAT_H

