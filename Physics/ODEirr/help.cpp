// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "help.h"
#include "irrlicht.h"
//using namespace irr;
float GRAD_PI = 57.32f;
float GRAD_PI2 = 0.174532925f;

void PrintVector (const dReal *vec) {
	cout << "vec[0] = " << vec[0] << endl;
	cout << "vec[1] = " << vec[1] << endl;
	cout << "vec[2] = " << vec[2] << endl;
	cout << "_________\n";
}

void PrintVector (const core::vector3df vec) {
	cout << "vec[0] = " << vec.X << endl;
	cout << "vec[1] = " << vec.Y << endl;
	cout << "vec[2] = " << vec.Z << endl;
	cout << "_________\n";
}

void QuaternionToEuler (const dQuaternion q, core::vector3df &euler) {
	// This code is taken from Bounceable example (shows how to use
	// ODE with Irrlicht)
	dReal w, x, y, z;
	w = q[0];
	x = q[1];
	y = q[2];
	z = q[3];
	double sqw = w * w;
	double sqx = x * x;
	double sqy = y * y;
	double sqz = z * z;
	euler.Z = (f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))*GRAD_PI);
	euler.X = (f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))*GRAD_PI);
	euler.Y = (f32) (asin(-2.0 * (x*z - y*w))*GRAD_PI);
}

void EulerToQuaternion (core::vector3df euler, dQuaternion quaternion) {
	// This code is taken from Bounceable example (shows how to use
	// ODE with Irrlicht)
	double _heading=euler.Z*GRAD_PI2/2.0;
	double _attitude=euler.Y*GRAD_PI2/2.0;
	double _bank=euler.X*GRAD_PI2/2.0;
	double c1 = cos(_heading);
	double s1 = sin(_heading);
	double c2 = cos(_attitude);
	double s2 = sin(_attitude);
	double c3 = cos(_bank);
	double s3 = sin(_bank);
	double c1c2 = c1*c2;
	double s1s2 = s1*s2;
    //w
	quaternion[0]=(dReal) (c1c2*c3 + s1s2*s3);
	//x
	quaternion[1]=(dReal) (c1c2*s3 - s1s2*c3);
	//y
	quaternion[2]=(dReal) (c1*s2*c3 + s1*c2*s3);
	//z
	quaternion[3]=(dReal) (s1*c2*c3 - c1*s2*s3);
}

dTriMeshDataID GetGeomFromMesh (IMesh *mesh) {
	// This code is taken from Bounceable example (shows how to use
	// ODE with Irrlicht)
	if (mesh == NULL) {
		cout << "\nGetGeomFromMesh F A I L E D ! ! !\n";
		// TODO: May be return "-1" in case of failure?
		// i.e., don't quit here
		exit (1);
	}
	int index_count = 0,
		vertex_count = 0;
	for (int i = 0; i < mesh->getMeshBufferCount (); i++) {
		IMeshBuffer *mb = mesh->getMeshBuffer (i);
		index_count += mb->getIndexCount ();
		vertex_count += mb->getVertexCount ();
	}
	dVector3 *vertices = new dVector3[vertex_count];
	int *indices = new int[index_count];

	int ic = 0,
		vc = 0,
		v2c = 0;
	// what the hell is v2c for?
	for (int i = 0; i < mesh->getMeshBufferCount (); i++) {
		IMeshBuffer *mb = mesh->getMeshBuffer (i);
		u16 *mb_indices = mb->getIndices ();
		for (int j = 0; j < mb->getIndexCount (); j++) {
			indices[ic] = v2c + mb_indices[j]; // what's the use of it?
			ic++;
		}
		v2c += mb->getVertexCount ();
		if (mb->getVertexType () == irr::video::EVT_STANDARD) {
			S3DVertex *mb_vertices = (S3DVertex*)mb->getVertices ();
			for (int j = 0; j < mb->getVertexCount (); j++) {
				vertices[vc][0] = mb_vertices[j].Pos.X;
				vertices[vc][1] = mb_vertices[j].Pos.Y;
				vertices[vc][2] = mb_vertices[j].Pos.Z;
				vc++;
			}
		}
		else
			if (mb->getVertexType () == EVT_2TCOORDS) {
				S3DVertex2TCoords *mb_vertices = (S3DVertex2TCoords*)mb->getVertices ();
				for (int j = 0; j < mb->getVertexCount (); j++) {
					vertices[vc][0] = mb_vertices[j].Pos.X;
					vertices[vc][1] = mb_vertices[j].Pos.Y;
					vertices[vc][2] = mb_vertices[j].Pos.Z;
					vc++;
				}
			}
	}
	dTriMeshDataID data = dGeomTriMeshDataCreate ();
	// If this is not always desireable to build simple one,
	// then we can add int parameter to identify necessary build scheme
	dGeomTriMeshDataBuildSimple (data, (dReal*)vertices, vertex_count,
		(const dTriIndex*)	indices, index_count);
	return data;
}

float RadianToDegree (float radian) {
	return 180 * radian / M_PI;
}

float DegreeToRadian (float degree) {
	return M_PI * degree / 180;
}

bool CheckForDisabling (dBodyID b, float lin_vel, float ang_vel) {
	bool disable = true;
	const dReal *lvel = dBodyGetLinearVel (b);
	// I took this formulas from ODE tests
	dReal linear_speed = lvel[0] * lvel[0] + lvel[1] * lvel[1] +
		lvel[2] * lvel[2];
	if (linear_speed > lin_vel)
		disable = false;
	const dReal *avel = dBodyGetAngularVel (b);
	dReal angular_speed = avel[0] * avel[0] + avel[1] * avel[1] +
		avel[2] * avel[2];
	if (angular_speed > ang_vel)
		disable = false;
	return disable;
}

/*
float FRandom () {
	int val = (int)pow (10.0, 4);
	return (rand () % val) / (float) val;
}
*/

void UpdateObject (dGeomID g, ISceneNode *n) {
	// Set position
	const dReal *pos = dGeomGetPosition (g);
	n->setPosition (core::vector3df (pos[0], pos[1], pos[2]));
	// Set rotation
	dQuaternion q;
	dGeomGetQuaternion (g, q);
	core::vector3df rot;
	QuaternionToEuler (q, rot);
	n->setRotation (rot);
}

