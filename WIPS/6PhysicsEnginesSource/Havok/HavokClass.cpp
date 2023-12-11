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

#include "HavokClass.h"


static void errorReportFunction(const char* str, void* errorOutputObject)
{
    std::cout << str << std::endl;
}

//Constructor
HavokClass::HavokClass(IrrlichtDevice *device_)
{
	device = device_;

//	memorymanager = new hkPoolMemory();
//	hkBaseSystem::init(memorymanager, HK_NULL, errorReportFunction);

	hkMemory *memorymanager = new hkPoolMemory();
    threadmemory = new hkThreadMemory(memorymanager, 16);
    hkBaseSystem::init(memorymanager, threadmemory, errorReportFunction);
    memorymanager->removeReference();

    //init the thread stack
    int stack_size = 1024 * 1000;
	void *thread_stack = hkAllocate<char>( stack_size, HK_MEMORY_CLASS_BASE);
    hkThreadMemory::getInstance().setStackArea( thread_stack, stack_size);

	hkpWorldCinfo info;
	info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
	info.m_gravity.set(0,-9.81f*3.0f,0);
	info.m_collisionTolerance = 0.1f;
	info.setBroadPhaseWorldSize(150.0f);
	info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);

	havokworld = new hkpWorld(info);

	hkpAgentRegisterUtil::registerAllAgents(havokworld->getCollisionDispatcher());

	timer.startTimer();
}

//Destructor
HavokClass::~HavokClass()
{
	delete havokworld;

	hkBaseSystem::quit();

	delete threadmemory;
}

//Creates collision geometry
HavokRigidBody *HavokClass::setPhysics(scene::ISceneNode *node, f32 mass)
{
	hkpBoxShape *shape = new hkpBoxShape(hkVector4(0.4,0.4,0.4));

	hkpRigidBodyCinfo bodyinfo;

	bodyinfo.m_shape = shape;

	// Compute the shapes inertia tensor
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(shape, mass, bodyinfo);

	bodyinfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	bodyinfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;

	bodyinfo.m_position = hkVector4(node->getPosition().X,node->getPosition().Y,node->getPosition().Z);

	HavokRigidBody* body = new HavokRigidBody(bodyinfo);
	body->userdata = node;

	havokworld->addEntity(body);
//	body->removeReference();
	shape->removeReference();

	return body;
}

void HavokClass::createStaticGeometry(const c8 *filename)
{
/*	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IMesh *mesh = smgr->getMesh(filename)->getMesh(0);
	scene::IMeshBuffer *buffer;

	//load the mesh
	smgr->addMeshSceneNode(mesh);

	// retrieve how many meshes there are
	irr::s32 meshBufferCount = mesh->getMeshBufferCount();

	core::array<f32> vertices; // used for allocating vertices
	core::array<u16> indices; // used for allocating indices

	u32 tempIndexCount = 0; // used for offsetting indices
	u32 i;
	u32 j;

	for (i = 0; i < meshBufferCount; ++i)
	{
		// pointer to the map's mesh buffer
		scene::IMeshBuffer *mb = mesh->getMeshBuffer(i); // for every mesh buffer

		u32 numVertices = mb->getVertexCount(); // get vertex num every mesh buffer
		u32 numIndices = mb->getIndexCount(); // get index num every mesh buffer

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

	hkpExtendedMeshShape *m_mesh = new hkpExtendedMeshShape;

	// it is common to have a landscape with 0 convex radius (for each triangle)
	// and all moving objects with non zero radius.
	m_mesh->setRadius( 0.0f );

	hkpExtendedMeshShape::TrianglesSubpart part;

	part.m_vertexBase = vertices.const_pointer();
	part.m_vertexStriding = sizeof(f32)*3;
	part.m_numVertices = vertices.size()/3;

	part.m_indexBase = (void *)indices.const_pointer();
	part.m_indexStriding = sizeof(u16);
	part.m_numTriangleShapes = indices.size()/3;
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;
	part.m_flipAlternateTriangles = 0;

	m_mesh->addTrianglesSubpart(part);

	hkpMoppCompilerInput mci;
	hkpMoppCode *m_code = hkpMoppUtility::buildCode(m_mesh, mci);*/

	//create the shape
//	hkpMoppBvTreeShape* shape = new hkpMoppBvTreeShape(m_mesh, m_code);
	hkpBoxShape *shape = new hkpBoxShape(hkVector4(100,0.4,100));

	hkpRigidBodyCinfo bodyinfo;

	bodyinfo.m_shape = shape;

	bodyinfo.m_motionType = hkpMotion::MOTION_FIXED;
	bodyinfo.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

	bodyinfo.m_position = hkVector4(0,0,0);

	HavokRigidBody* body = new HavokRigidBody(bodyinfo);

	havokworld->addEntity(body);
	body->removeReference();
	shape->removeReference();
}

void HavokClass::update()
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

	havokworld->stepDeltaTime(timestep);

	updateEntitiesAfterPhysics();
}

void HavokClass::updateEntitiesAfterPhysics()
{
	core::vector3df rot;
	const hkArray<hkpSimulationIsland*>& islands = havokworld->getActiveSimulationIslands();
	for (int i = 0; i < islands.getSize(); i++ )
	{
		hkpSimulationIsland* island = islands[i];
		for (int b = 0; b < island->getEntities().getSize(); b++ )
		{
			HavokRigidBody* body =  static_cast<HavokRigidBody*>( island->getEntities()[b]);

			scene::ISceneNode *node = body->userdata;
			if(node == NULL)
				continue;

			hkVector4 pos = body->getPosition();

			f32 position[3];
			pos.store3(position);

			node->setPosition(core::vector3df(position[0],position[1],position[2]));

			// get the rotation quaternion
			hkQuaternion result = body->getRotation();

			hkVector4 rota;
			rota = result.getImag();

			f32 rotation[3];
			rota.store3(rotation);

			// convert it to eulerangles
			core::quaternion quat(rotation[0],rotation[1],rotation[2],result.getReal());
			quat.toEuler(rot);
			rot.set(rot.X*180/core::PI,rot.Y*180/core::PI,rot.Z*180/core::PI);

			// set the rotation
			node->setRotation(rot);
		}
	}
}
