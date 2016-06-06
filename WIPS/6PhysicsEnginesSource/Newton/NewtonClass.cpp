/*
Copyright (c) 2008 Nils Daumann

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "NewtonClass.h"

namespace _newton
{
	static NewtonWorld *nworld;

	// memory allocation for Newton
	void *PhysicsAlloc(int sizeinbytes)
	{
		return malloc (sizeinbytes);
	}

	// memory de-allocation for Newton
	void PhysicsFree(void *ptr, int sizeinbytes)
	{
		free (ptr);
	}

	// rigid body destructor
	void  PhysicsBodyDestructor(const NewtonBody* body)
	{
		scene::ISceneNode* node;

		// get the graphic object from the rigid body
		node = (scene::ISceneNode*)NewtonBodyGetUserData(body);

		// destroy the graphic object
		node->remove();
	}

	// set the transformation of a rigid body
	void  PhysicsSetTransform(const NewtonBody* body, const f32* matrix)
	{
		scene::ISceneNode* node;

		// get the graphic object from the rigid body
		node = (scene::ISceneNode*)NewtonBodyGetUserData(body);

		// set the transformation matrix for this rigid body
		f32 TempRot[3] = {0,0,0};
		NewtonGetEulerAngle(matrix, TempRot);
		node->setRotation(core::vector3df(TempRot[0]*core::RADTODEG,TempRot[1]*core::RADTODEG,TempRot[2]*core::RADTODEG));
		core::matrix4 mat;
		mat.setM(matrix);
		node->setPosition(mat.getTranslation());
	}

	// add force and torque to rigid body
	void  PhysicsApplyForceAndTorque(const NewtonBody* body)
	{
		f32 Ixx;
		f32 Iyy;
		f32 Izz;
		f32 mass;

		NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
		const f32 force[3] = {0.0f, -mass * 9.81f * 3.0f, 0.0f};
		NewtonBodySetForce(body, force);

/*		f32 omega[3] = {1.0f, 1.0f, 1.0f};
		NewtonBodySetTorque(body,omega);*/
	}
}


//Constructor
NewtonClass::NewtonClass(IrrlichtDevice *device_)
{
	device = device_;

	//create the newton world
	_newton::nworld = NewtonCreate(_newton::PhysicsAlloc, _newton::PhysicsFree);

	f32 minbnd[3] = {-1000,-50,-1000};
	f32 maxbnd[3] = {1000,100000,1000};
	NewtonSetWorldSize(_newton::nworld,minbnd,maxbnd);

	timer.startTimer();
}

//Destructor
NewtonClass::~NewtonClass()
{
	// destroy the Newton world
	NewtonDestroy(_newton::nworld);
}

//Creates collision geometry
NewtonBody *NewtonClass::setPhysics(scene::ISceneNode *node, f32 mass)
{
	//get the scale
	core::vector3df scale = node->getScale();

	//create the collision geometry
	collision = NewtonCreateBox(_newton::nworld, scale.X*node->getBoundingBox().MaxEdge.X*2.0f, scale.Y*node->getBoundingBox().MaxEdge.Y*2.0f, scale.Z*node->getBoundingBox().MaxEdge.Z*2.0f, NULL);

	// Create the rigid body
	NewtonBody* boxBody = NewtonCreateBody(_newton::nworld, collision);

	//drop the collision geometry
	NewtonReleaseCollision(_newton::nworld, collision);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(boxBody, node);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(boxBody, _newton::PhysicsBodyDestructor);

	// set the transform call back function
	NewtonBodySetTransformCallback(boxBody, _newton::PhysicsSetTransform);

	// set the force and torque call back function
	NewtonBodySetForceAndTorqueCallback(boxBody, _newton::PhysicsApplyForceAndTorque);

	if(mass != 0)
	{
		//make the body dynamic through setting its mass
		NewtonBodySetMassMatrix(boxBody, mass, 1.0f, 1.0f, 1.0f);
		NewtonBodySetFreezeTreshold(boxBody, 1.0f, 1.0f, 10);
	}

	// Set the transformation matrix
	core::matrix4 mat;
	mat.setTranslation(node->getPosition());
	NewtonBodySetMatrix(boxBody, &mat[0]);

	NewtonBodySetLinearDamping(boxBody,0.01);

//	f32 omega[3] = {1.0f, 1.0f, 1.0f};
//	NewtonBodySetOmega(boxBody, omega);

	return boxBody;
}

void NewtonClass::createStaticGeometry(const c8 *filename)
{
	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IMesh *mesh = smgr->getMesh(filename)->getMesh(0);
	scene::IMeshBuffer *buffer;

	//load the mesh
	smgr->addMeshSceneNode(mesh);

	//create the new collision object TODO: Callback!!!
	collision = NewtonCreateTreeCollision(_newton::nworld, NULL);

	// prepare to create collsion geometry
	NewtonTreeCollisionBeginBuild(collision);

	u32 buffercount;
	u32 j;
	u32 v1i, v2i, v3i;
	f32 vArray[9]; // vertex array (3*3 floats)

	for (buffercount = 0; buffercount < mesh->getMeshBufferCount(); buffercount++)
	{
		buffer = mesh->getMeshBuffer(buffercount);
		video::S3DVertex2TCoords* buffer_vertices = (irr::video::S3DVertex2TCoords*)buffer->getVertices();
		u16* buffer_indices  = buffer->getIndices();

		// add each triangle from the mesh
		for (j = 0; j < buffer->getIndexCount(); j += 3)
		{
			v1i = buffer_indices[j];
			v2i = buffer_indices[j+1];
			v3i = buffer_indices[j+2];

			vArray[0] = buffer_vertices[v1i].Pos.X;
			vArray[1] = buffer_vertices[v1i].Pos.Y;
			vArray[2] = buffer_vertices[v1i].Pos.Z;
			vArray[3] = buffer_vertices[v2i].Pos.X;
			vArray[4] = buffer_vertices[v2i].Pos.Y;
			vArray[5] = buffer_vertices[v2i].Pos.Z;
			vArray[6] = buffer_vertices[v3i].Pos.X;
			vArray[7] = buffer_vertices[v3i].Pos.Y;
			vArray[8] = buffer_vertices[v3i].Pos.Z;

			//add the face to the collision geometry TODO: Face Data!!!
			NewtonTreeCollisionAddFace(collision, 3, vArray, 12, 1);
		}
	}

	//compile the collision geometry
	NewtonTreeCollisionEndBuild(collision, 1);

	//create the static physics body
	NewtonBody* body = NewtonCreateBody(_newton::nworld, collision);

	// release the collision tree
	NewtonReleaseCollision(_newton::nworld, collision);
}

void NewtonClass::update()
{
	//get the time passed during the last frame
	timer.stopTimer();
	timestep = timer.getElapsedTime();
	timer.startTimer();

/*	if(timestep > 0.01f)
	{
		timestep = 0.01f;
	}
	if(timestep < 0.005f)
	{
		timestep = 0.005f;
	}*/

	//update the Newton physics world
	NewtonUpdate(_newton::nworld, (f32)timestep);
}
