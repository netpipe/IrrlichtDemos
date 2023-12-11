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

#include "ODEClass.h"


dWorldID ODEClass::odeworld;
dJointGroupID ODEClass::contactgroup;

//Constructor
ODEClass::ODEClass(IrrlichtDevice *device_)
{
	device = device_;

	dInitODE();

	//create the physics world
	odeworld = dWorldCreate();
	dWorldSetAutoDisableFlag(odeworld,1);

	contactgroup = dJointGroupCreate(0);

	//create the collision space
	odespace = dHashSpaceCreate(0);

	dWorldSetGravity(odeworld,0,-9.81*3.0f,0);
	dWorldSetCFM(odeworld,1e-5);
	dWorldSetERP(odeworld, 0.8);
	dWorldSetContactMaxCorrectingVel(odeworld, 5.9);
	dWorldSetContactSurfaceLayer(odeworld, 0.001);

	numgeoms = 0;

//	timer.startTimer();
}

//Destructor
ODEClass::~ODEClass()
{
	dJointGroupDestroy(contactgroup);
	dWorldDestroy(odeworld);
	dSpaceDestroy(odespace);
}

//Creates collision geometry
dBodyID ODEClass::setPhysics(scene::ISceneNode *node, f32 mass)
{
	core::vector3df pos = node->getPosition();
	core::vector3df rot;

	//get the scale
	core::vector3df scale = node->getScale();

	// Create a body
	dBodyID body = dBodyCreate(odeworld);
	dMass mass_;
	dMassSetBox(&mass_,scale.X*node->getBoundingBox().MaxEdge.X*2.0f,scale.Y*node->getBoundingBox().MaxEdge.Y*2.0f,scale.Z*node->getBoundingBox().MaxEdge.Z*2.0f,scale.Y*node->getBoundingBox().MaxEdge.Y*2.0f);
	dMassAdjust(&mass_,0.1f);
	dBodySetMass(body,&mass_);
	dBodySetPosition(body,pos.X,pos.Y,pos.Z);

	// Create a geom
	geom[numgeoms] = dCreateBox(odespace,scale.X*node->getBoundingBox().MaxEdge.X*2.0f, scale.Y*node->getBoundingBox().MaxEdge.Y*2.0f, scale.Z*node->getBoundingBox().MaxEdge.Z*2.0f);

	dGeomSetData(geom[numgeoms],(void*)node);

	// Bind together our previously created body and the geom
	dGeomSetBody(geom[numgeoms],body);

	numgeoms += 1;

	return body;
}

void ODEClass::createStaticGeometry(const c8 *filename)
{
//	dCreatePlane(odespace,0,1,0,0);

	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IMesh *mesh = smgr->getMesh(filename)->getMesh(0);

	//load the mesh
	scene::ISceneNode *node = smgr->addMeshSceneNode(mesh);

	int i,j,ci,cif,cv;
	u32 indexcount = 0;
	u32 vertexcount = 0;

	// count vertices and indices
	for(i=0; i< mesh->getMeshBufferCount(); i++)
	{
		scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);
		indexcount += mb->getIndexCount();
		vertexcount += mb->getVertexCount();
	}

	// build structure for ode trimesh geom
	dVector3 *vertices = new dVector3[vertexcount];
	dTriIndex *indices = new dTriIndex[indexcount];

	// fill trimesh geom
	ci = 0; // current index in the indices array
	cif = 0; // offset of the irrlicht-vertex-index in the vetices array
	cv = 0; // current index in the vertices array

	for(i = 0; i < mesh->getMeshBufferCount(); i++)
	{
		scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);

		// fill indices
		u16* mb_indices = mb->getIndices();
		for(j = 0; j < mb->getIndexCount(); j++)
		{
			// scale the indices from multiple meshbuffers to single index array
			indices[ci]=cif+mb_indices[j];
			ci++;
		}
		// update the offset for the next meshbuffer
		cif = cif + mb->getVertexCount();

		// fill vertices
		if(mb->getVertexType() == video::EVT_STANDARD)
		{
			video::S3DVertex* mb_vertices = (video::S3DVertex*)mb->getVertices();
			for(j = 0; j < mb->getVertexCount(); j++)
			{
				vertices[cv][0] = mb_vertices[j].Pos.X;
				vertices[cv][1] = mb_vertices[j].Pos.Y;
				vertices[cv][2] = mb_vertices[j].Pos.Z;
				cv++;
			}
		}else
		{
			if(mb->getVertexType() == video::EVT_2TCOORDS)
			{
				video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*)mb->getVertices();
				for(j = 0; j < mb->getVertexCount(); j++)
				{
					vertices[cv][0] = mb_vertices[j].Pos.X;
					vertices[cv][1] = mb_vertices[j].Pos.Y;
					vertices[cv][2] = mb_vertices[j].Pos.Z;
					cv++;
				}
			}
		}
	}

	irr::core::vector3df pos = node->getPosition();

	// build the trimesh data
	dTriMeshDataID data = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(data,(dReal*)vertices, vertexcount, indices, indexcount);

	// build the trimesh geom
	dGeomID geom = dCreateTriMesh(odespace,data,0,0,0);

	// set the geom position
//	dGeomSetPosition(geom,pos.X,pos.Y,pos.Z);

	// lets have a pointer to our bounceable
	// we could need this in the collision callback
	dGeomSetData(geom,(void*)node);

	// in our application we don't want geoms
	// converted from meshes to have a body
	dGeomSetBody(geom,0);
}

void ODEClass::update()
{
	//get the time passed during the last frame
//	timer.stopTimer();
	timestep = device->getTimer()->getTime();
//	timer.startTimer();

/*	if(timestep > 0.01f)
	{
		timestep = 0.01f;
	}
	if(timestep < 0.005f)
	{
		timestep = 0.005f;
	}*/

	// Find the corresponding number of steps that must be taken
//	u32 nbStepsToPerform = (u32)(timestep*1000.0f);
//	u32 i;

	// Make these steps to advance world time
//	for (i = 0; i < nbStepsToPerform; i++)
//	{
		// build the collision joints for all objects in 'theSpace'
		dSpaceCollide(odespace,0,&nearCollisionCallback);

		// Step world
		dWorldQuickStep(odeworld,timestep);// 0.001f);

		// make a simulation step
//		dWorldStep(odeworld,0.01f);

		// remove all contact joints
		dJointGroupEmpty(contactgroup);
//	}

	updateEntitiesAfterPhysics();

//	timer.startTimer();
}

void ODEClass::nearCollisionCallback(void* data, dGeomID o1, dGeomID o2)
{
	u32 i = 0;
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if(b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact))return;
	dContact contact[10];

	for(i = 0; i < 10; i++)
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 1e-5f;
		contact[i].surface.bounce_vel = 1e-9f;
		contact[i].surface.soft_cfm = 1e-6f;
	}
	int numc = dCollide(o1,o2,10,&contact[0].geom,sizeof(dContact));
	if(numc > 0)
	{
		for(i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(odeworld,contactgroup,&contact[i]);
			dJointAttach(c,b1,b2);
		}
	}
}

void ODEClass::updateEntitiesAfterPhysics()
{
	core::vector3df pos;
	core::vector3df rot;
	scene::ISceneNode *node;
	u32 i;

	for(i = 0; i < numgeoms; i++)
	{
		node = (scene::ISceneNode*)dGeomGetData(geom[i]);

		// get the new position of the ODE geometry
		dReal* ode_pos = (dReal*)dGeomGetPosition(geom[i]);

		// set the position at the scenenode
		pos.set((f32)ode_pos[0],(f32)ode_pos[1],(f32)ode_pos[2]);
		node->setPosition(pos);

		// get the rotation quaternion
		dQuaternion result;
		dGeomGetQuaternion(geom[i], result);

		// convert it to eulerangles
		core::quaternion quat(result[1],result[2],result[3],result[0]);
		quat.toEuler(rot);
		rot.set(rot.X*180/core::PI,rot.Y*180/core::PI,rot.Z*180/core::PI);

		// set the rotation
		node->setRotation(rot);
	}
}
