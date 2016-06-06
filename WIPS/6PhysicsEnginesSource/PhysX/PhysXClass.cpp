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

#include "PhysXClass.h"


//Constructor
PhysXClass::PhysXClass(IrrlichtDevice *device_)
{
	device = device_;

	physxworld = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);

	scenedesc.gravity = NxVec3(0.0f, -9.81f * 3.0f, 0.0f);
	scene = physxworld->createScene(scenedesc);

	defaultmat = scene->getMaterialFromIndex(0);
	defaultmat->setRestitution(0.0f);
	defaultmat->setStaticFriction(0.5f);
	defaultmat->setDynamicFriction(0.5f);

	cooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	cooking->NxInitCooking();

	timer.startTimer();
}

//Destructor
PhysXClass::~PhysXClass()
{
	physxworld->releaseScene(*scene);
	NxReleasePhysicsSDK(physxworld);
}

//Creates collision geometry
NxActor *PhysXClass::setPhysics(scene::ISceneNode *node, f32 mass)
{
	core::vector3df scale = node->getScale();

	// Create body
	NxBodyDesc bodydesc;
	bodydesc.angularDamping	= 0.5f;

	NxBoxShapeDesc boxdesc;
	boxdesc.dimensions = NxVec3(scale.X*node->getBoundingBox().MaxEdge.X, scale.Y*node->getBoundingBox().MaxEdge.Y, scale.Z*node->getBoundingBox().MaxEdge.Z);

	NxActorDesc actordesc;
	actordesc.shapes.pushBack(&boxdesc);
	actordesc.body = &bodydesc;
	actordesc.density = mass;
	actordesc.globalPose.t = NxVec3(node->getPosition().X,node->getPosition().Y,node->getPosition().Z);
	NxActor *actor = scene->createActor(actordesc);
	actor->userData = (void*)node;

	return actor;
}

void PhysXClass::createStaticGeometry(const c8 *filename)
{
	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IMesh *mesh = smgr->getMesh(filename)->getMesh(0);

	//load the mesh
	smgr->addMeshSceneNode(mesh);

	// retrieve how many meshes there are
	s32 meshBufferCount = mesh->getMeshBufferCount();

	core::array<NxVec3> vertices; // used for allocating vertices
	core::array<NxU32> indices; // used for allocating indices

	NxU32 tempIndexCount = 0; // used for offsetting indices

	u32 i;
	u32 j;
	for(i = 0; i < meshBufferCount; ++i)
	{
		// pointer to the map's mesh buffer
		scene::IMeshBuffer *mb = mesh->getMeshBuffer(i); // for every mesh buffer

		s32 numVertices = mb->getVertexCount(); // get vertex num every mesh buffer
		s32 numIndices = mb->getIndexCount(); // get index num every mesh buffer

		video::S3DVertex2TCoords *mbVertices = (video::S3DVertex2TCoords*)mb->getVertices(); // get pointer to vertices in the mesh buffer
		u16 *mbIndices = mb->getIndices(); // get pointer to indices in the mesh buffer

		for(j = 0; j < numVertices; ++j) // push vertices into an array
			vertices.push_back(NxVec3(mbVertices[j].Pos.X, mbVertices[j].Pos.Y, mbVertices[j].Pos.Z));

		for(j = 0; j < numIndices; ++j) // push indices into an array
			indices.push_back(NxU32(mbIndices[j]) + tempIndexCount);

		tempIndexCount += numIndices;
	}

	NxPMap q3mapPMap;
	q3mapPMap.dataSize    = 0;
	q3mapPMap.data        = NULL;
	NxTriangleMeshDesc mapMeshDesc; //  mesh description
	// Build physical model
	mapMeshDesc.numVertices = vertices.size();
	mapMeshDesc.numTriangles = indices.size() / 3;
	mapMeshDesc.pointStrideBytes = sizeof(NxVec3);
	mapMeshDesc.triangleStrideBytes = 3*sizeof(NxU32);
	mapMeshDesc.points = vertices.const_pointer();
	mapMeshDesc.triangles = indices.const_pointer();
	mapMeshDesc.flags = 0;
	std::cout << std::endl << mapMeshDesc.numVertices << " vertices.\n";
	std::cout << indices.size() << " indices.\n";
	std::cout << mapMeshDesc.numTriangles << " triangles.\n";

	if(!mapMeshDesc.isValid())
	{
		std::cout << "fuck\n";
		return;
	}

	cooking->NxInitCooking();
	if(!cooking->NxCookTriangleMesh(mapMeshDesc, PhysXUserStream("geometrie.shape", false)))
	{
		std::cout << "fuck\n";
		return;
	}
	NxTriangleMesh *q3mapTriangleMesh = physxworld->createTriangleMesh(PhysXUserStream("geometrie.shape", true));
	NxTriangleMeshShapeDesc mapMeshShapeDesc;
	mapMeshShapeDesc.meshData = q3mapTriangleMesh;
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&mapMeshShapeDesc);
//	actorDesc.globalPose.t = NxVec3(pos.X,pos.Y,pos.Z);
	indices.clear();
	vertices.clear();

	scene->createActor(actorDesc);
}

void PhysXClass::update()
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

	scene->simulate(timestep);

	updateEntitiesAfterPhysics();
}

void PhysXClass::updateEntitiesAfterPhysics()
{
	// Render all actors
	core::vector3df rot;
	int nbActors = scene->getNbActors();
	NxActor** actors = scene->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		if(!actor->userData) continue;

		scene::ISceneNode *node = (scene::ISceneNode *)actor->userData;

		//set position
		node->setPosition(core::vector3df(actor->getGlobalPosition().x,actor->getGlobalPosition().y,actor->getGlobalPosition().z));

		//get the rotation
		NxQuat result = actor->getGlobalOrientationQuat();

		// convert it to eulerangles
		core::quaternion quat(result.x,result.y,result.z,result.w);
		quat.toEuler(rot);
		rot.set(rot.X*180/core::PI,rot.Y*180/core::PI,rot.Z*180/core::PI);

		// set the rotation
		node->setRotation(rot);
	}

	// Fetch simulation results
	scene->flushStream();
	while(!scene->fetchResults(NX_RIGID_BODY_FINISHED, false));
}
