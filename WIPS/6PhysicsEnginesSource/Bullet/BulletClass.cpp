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

#include "BulletClass.h"


//Constructor
BulletClass::BulletClass(IrrlichtDevice *device_)
{
	device = device_;

/*	u32 i;
	for(i = 1000-1; i >= 0; i--)
	{
		userdata[i] = NULL;
	}*/

//	collisionconfiguration = new btDefaultCollisionConfiguration();
	collisionconfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionconfiguration);

	broadphase = new btAxisSweep3(btVector3(-1000,-1000,-1000),btVector3(1000,1000,1000),32766);

	solver = new btSequentialImpulseConstraintSolver();

//	bulletworld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionconfiguration);
//	bulletworld = new btContinuousDynamicsWorld(dispatcher,broadphase,solver,collisionconfiguration);
	bulletworld = new btSoftRigidDynamicsWorld(dispatcher,broadphase,solver,collisionconfiguration);


	bulletworld->setGravity(btVector3(0,-9.81*3.0f,0));

	worldinfo.m_gravity.setValue(0,-9.81*3.0f,0);
	worldinfo.m_broadphase = broadphase;
	worldinfo.m_sparsesdf.Initialize();

	timer.startTimer();
}

//Destructor
BulletClass::~BulletClass()
{
	//remove the rigidbodies from the dynamics world and delete them
	u32 i = bulletworld->getNumCollisionObjects();
	while(i > 0)
	{
		i--;
		btCollisionObject* obj = bulletworld->getCollisionObjectArray()[i];

		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
				delete body->getMotionState();
		}

		bulletworld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (i = 0; i < collisionshapes.size(); i++)
	{
		btCollisionShape* shape = collisionshapes[i];
		delete shape;
	}

	delete bulletworld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionconfiguration;
}

//Creates collision geometry
btRigidBody *BulletClass::setPhysics(scene::ISceneNode *node, f32 mass)
{
	core::vector3df scale = node->getScale();

	btCollisionShape* colshape = new btBoxShape(btVector3(scale.X*node->getBoundingBox().MaxEdge.X, scale.Y*node->getBoundingBox().MaxEdge.Y, scale.Z*node->getBoundingBox().MaxEdge.Z));

	collisionshapes.push_back(colshape);

	/// Create Dynamic Objects
	btTransform starttransform;
	starttransform.setIdentity();

	btScalar mass_(1.0f);

	btVector3 localinertia(1.0,1.0,1.0);
	if (mass != 0.0f)
		colshape->calculateLocalInertia(mass,localinertia);

	starttransform.setOrigin(btVector3(node->getPosition().X,node->getPosition().Y,node->getPosition().Z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* mymotionstate = new btDefaultMotionState(starttransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,mymotionstate,colshape,localinertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	bulletworld->addRigidBody(body);

	userdata[bulletworld->getNumCollisionObjects()-1] = node;

	return body;
}

void BulletClass::createStaticGeometry(const c8 *filename)
{
	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IMesh *mesh = smgr->getMesh(filename)->getMesh(0);
	scene::IMeshBuffer *buffer;

	//load the mesh
	smgr->addMeshSceneNode(mesh);

	btTriangleMesh *meshinterface = new btTriangleMesh();

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

			//add the face to the collision geometry
			meshinterface->addTriangle(btVector3(vArray[0],vArray[1],vArray[2]),btVector3(vArray[3],vArray[4],vArray[5]),btVector3(vArray[6],vArray[7],vArray[8]));
		}
	}

	btCollisionShape* groundshape = new btBvhTriangleMeshShape(meshinterface,true);

	collisionshapes.push_back(groundshape);

	btTransform groundtransform;
	groundtransform.setIdentity();
	groundtransform.setOrigin(btVector3(0,0,0));

	btScalar mass(0.0f);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* mymotionstate = new btDefaultMotionState(groundtransform);
	btRigidBody::btRigidBodyConstructionInfo rbinfo(mass,mymotionstate,groundshape,btVector3(0,0,0));
	btRigidBody* body = new btRigidBody(rbinfo);

	//add the body to the dynamics world
	bulletworld->addRigidBody(body);

	userdata[bulletworld->getNumCollisionObjects()-1] = NULL;
}

void BulletClass::setSoftBody(scene::ISceneNode *node, const c8 *filename, f32 mass)
{
	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IMesh *mesh = smgr->getMesh(filename)->getMesh(0);
	scene::IMeshBuffer *buffer;

	s32 meshBufferCount = mesh->getMeshBufferCount();

	core::array<f32> vertices; // used for allocating vertices
	core::array<s32> indices; // used for allocating indices

	u32 tempIndexCount = 0; // used for offsetting indices

	u32 i;
	u32 j;
	for (i = 0; i < meshBufferCount; ++i)
	{
		// pointer to the map's mesh buffer
		scene::IMeshBuffer *mb = mesh->getMeshBuffer(i); // for every mesh buffer

		s32 numVertices = mb->getVertexCount(); // get vertex num every mesh buffer
		s32 numIndices = mb->getIndexCount(); // get index num every mesh buffer

		video::S3DVertex2TCoords *mbVertices = (video::S3DVertex2TCoords*)mb->getVertices(); // get pointer to vertices in the mesh buffer
		u16 *mbIndices = mb->getIndices(); // get pointer to indices in the mesh buffer

		for (j = 0; j < numVertices; ++j) // push vertices into an array
		{
			vertices.push_back(mbVertices[j].Pos.X);
			vertices.push_back(mbVertices[j].Pos.Y);
			vertices.push_back(mbVertices[j].Pos.Z);
		}

		for (j = 0; j < numIndices; ++j) // push indices into an array
			indices.push_back(mbIndices[j] + tempIndexCount);

		tempIndexCount += numIndices;
	}

	std::cout << indices.size()/3 << std::endl;

	btSoftBody *body = btSoftBodyHelpers::CreateEllipsoid(worldinfo,btVector3(0,0,0),btVector3(1,1,1),100);
	//btSoftBody *body = btSoftBodyHelpers::CreateFromTriMesh(worldinfo,vertices.const_pointer(),indices.const_pointer(),indices.size()/3);
	btSoftBody::Material *mat = body->appendMaterial();
	mat->m_kLST = 0.5;
	body->generateBendingConstraints(2,mat);
	body->m_cfg.piterations = 2;
	body->m_cfg.kDF = 0.5;
	body->randomizeConstraints();
	body->scale(btVector3(1,1,1));
	body->setTotalMass(mass,true);
	bulletworld->addSoftBody(body);



/*	const btVector3	h = btVector3(0.5,0.5,0.5);
	const btVector3	c[]={
		h*btVector3(-1,-1,-1),
		h*btVector3(+1,-1,-1),
		h*btVector3(-1,+1,-1),
		h*btVector3(+1,+1,-1),
		h*btVector3(-1,-1,+1),
		h*btVector3(+1,-1,+1),
		h*btVector3(-1,+1,+1),
		h*btVector3(+1,+1,+1)};
	btSoftBody* psb = btSoftBodyHelpers::CreateFromConvexHull(worldinfo,c,8);
	psb->generateBendingConstraints(2);
	bulletworld->addSoftBody(psb);*/

	userdata[bulletworld->getNumCollisionObjects()-1] = NULL;
}

void BulletClass::update()
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

	bulletworld->stepSimulation(timestep);

	updateEntitiesAfterPhysics();
}

void BulletClass::updateEntitiesAfterPhysics()
{
	core::vector3df rot;
	u32 i = bulletworld->getNumCollisionObjects();
	while(i > 0)
	{
		i--;
		btCollisionObject* obj = bulletworld->getCollisionObjectArray()[i];

		btRigidBody* body = btRigidBody::upcast(obj);
		if(userdata[i] == NULL)
		{
			if(body->getInvMass() != 0.0f)
			{

			}else
				continue;
		}else
		{
			if(body->getInvMass() != 0.0f)
			{
				scene::ISceneNode *node = userdata[i];
				btVector3 pos = body->getCenterOfMassPosition();

				node->setPosition(core::vector3df(pos.x(),pos.y(),pos.z()));

				// get the rotation quaternion
				btQuaternion result = body->getOrientation();

				// convert it to eulerangles
				core::quaternion quat(result.x(),result.y(),result.z(),result.w());
				quat.toEuler(rot);
				rot.set(rot.X*180/core::PI,rot.Y*180/core::PI,rot.Z*180/core::PI);

				// set the rotation
				node->setRotation(rot);
			}
		}
	}
}
