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

#include "TrueAxisClass.h"


//Constructor
TrueAxisClass::TrueAxisClass(IrrlichtDevice *device_)
{
	device = device_;

    aabb.Initialise(
        TA::Vec3(0.0f, 0.0f, 0.0f), // Center.
        TA::Vec3(1000,1000,1000));  // Size/2
    TA::Physics::CreateInstance(
        aabb,                                   // World bounding box.
        TA::Vec3(0.0f, -9.81f * 3.0f, 0.0f),           // Gravity.
        TA::Physics::FLAG_XZ_COLLISION_GRID);   // Flags.

    // Make a reference to the physics instance.
    // Would normaly just use TA::Physics::GetInstance() directly
    // but the automatic documentation generator doesn't recognise it.
    physics = &TA::Physics::GetInstance();

	numbodies = 0;
	timer.startTimer();
}

//Destructor
TrueAxisClass::~TrueAxisClass()
{
	TA::Physics::DestroyInstance();

	u32 i;
	for(i = 0; i < numbodies; i++)
	{
		bodies[i]->Release();
	}
}

//Creates collision geometry
TA::DynamicObject *TrueAxisClass::setPhysics(scene::ISceneNode *node, f32 mass)
{
	core::vector3df scale = node->getScale();

	TA::DynamicObject* body = TA::DynamicObject::CreateNew();
    aabb.Initialise(                       // Make a 1 by 1 by 1 box.
        TA::Vec3(0.0f, 0.0f, 0.0f),        // Center.
        TA::Vec3(0.5f, 0.5f, 0.5f));       // Extent
    body->InitialiseAsABox(aabb);

    // Place the dynamic object
    body->SetPosition(TA::Vec3(node->getPosition().X, node->getPosition().Y, node->getPosition().Z));

    body->SetMass(mass);
    body->SetUserData((void *)node);

    // Add the dynamic object to the simulation
    physics->AddDynamicObject(body);
	bodies[numbodies] = body;
	numbodies += 1;

	return body;
}

void TrueAxisClass::createStaticGeometry(const c8 *filename)
{
/*	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IMesh *mesh = smgr->getMesh(filename)->getMesh(0);

	//load the mesh
	smgr->addMeshSceneNode(mesh);

	// retrieve how many meshes there are
	irr::s32 meshBufferCount = mesh->getMeshBufferCount();
	scene::IMeshBuffer *mb;

	u32 numVertices = 0;
	u32 numIndices = 0;

	u32 i;
	u32 j;
	for (i = 0; i < meshBufferCount; ++i)
	{
		mb = mesh->getMeshBuffer(i);

		numVertices += mb->getVertexCount();
		numIndices += mb->getIndexCount();
	}

    // Create a collision object to add to the static object.
    TA::CollisionObjectAABBMesh * pStaticCollisionObject = TA::CollisionObjectAABBMesh::CreateNew();
    pStaticCollisionObject->Initialise(
        numVertices,    // Num vertices.
        numIndices/3,   // Num polygons.
        numIndices);    // Num polygon indices.

	std::cout << numIndices/3 << std::endl;

	u32 tempIndexCount = 0; // used for offsetting indices
	core::array<s32> indices; // used for allocating indices
	for (i = 0; i < meshBufferCount; ++i)
	{
		// pointer to the map's mesh buffer
		mb = mesh->getMeshBuffer(i); // for every mesh buffer

		numVertices = mb->getVertexCount(); // get vertex num every mesh buffer
		numIndices = mb->getIndexCount(); // get index num every mesh buffer

		video::S3DVertex2TCoords *mbVertices = (irr::video::S3DVertex2TCoords*)mb->getVertices(); // get pointer to vertices in the mesh buffer
		irr::u16 *mbIndices = mb->getIndices(); // get pointer to indices in the mesh buffer

		for(j = 0; j < numVertices; ++j) // push vertices into an array
			pStaticCollisionObject->AddVertex(TA::Vec3(mbVertices[j].Pos.X, mbVertices[j].Pos.Y, mbVertices[j].Pos.Z));

		for(j = 0; j < numIndices; ++j) // push indices into an array
			indices.push_back(mbIndices[j]+tempIndexCount);

		tempIndexCount += numIndices;
	}

	core::array<s32> indicesflipped;
	for(j = indices.size()-1; j > 0; j--)
	{
		indicesflipped.push_back(indices.const_pointer()[j]);
	}

	j = 0;
	for(i = 0; i < indicesflipped.size(); i += 3)
	{
		pStaticCollisionObject->AddPolygon(3,&indicesflipped.const_pointer()[i]);
		j++;
	}
	std::cout << j << std::endl;*/
/////////////////////////////////////
	TA::CollisionObjectAABBMesh * pStaticCollisionObject = TA::CollisionObjectAABBMesh::CreateNew();
	pStaticCollisionObject->Initialise(
        4,              // Num vertices.
        1,              // Num polygons.
        4);             // Num polygon indices.
    pStaticCollisionObject->AddVertex(TA::Vec3(1000, 0.0f, 1000));
    pStaticCollisionObject->AddVertex(TA::Vec3(-1000, 0.0f, 1000));
    pStaticCollisionObject->AddVertex(TA::Vec3(-1000, 0.0f, -1000));
    pStaticCollisionObject->AddVertex(TA::Vec3(1000, 0.0f, -1000));


    int pnPolygonIndexList[4] = { 0, 1, 2, 3 };
    pStaticCollisionObject->AddPolygon(4, pnPolygonIndexList);
////////////////////////////////////
    pStaticCollisionObject->FinishedAddingGeometry();

    // Initialise the static object with the collision object.
    TA::StaticObject* pStaticObject = TA::StaticObject::CreateNew();
    pStaticObject->Initialise(pStaticCollisionObject);
    pStaticCollisionObject->Release(); // We no long need the reference.
    pStaticCollisionObject = 0;

    // Add the static object to the simulation.
    physics->AddStaticObject(pStaticObject);
    pStaticObject->Release(); // We no long need the reference.
}

void TrueAxisClass::update()
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

	physics->Update(timestep);

	updateEntitiesAfterPhysics();
}

void TrueAxisClass::updateEntitiesAfterPhysics()
{
	scene::ISceneNode *node;
	core::CMatrix4<f32> rotation;
	f32 mat44[16];
	u32 i;
	for(i = 0; i < numbodies; i++)
	{
		node = (scene::ISceneNode *)bodies[i]->GetUserData();
		const TA::MFrame *frame = &bodies[i]->GetFrame();

		//update the position
		node->setPosition(core::vector3df(frame->v3Translation.x,frame->v3Translation.y,frame->v3Translation.z));

		//update the rotation
		mat44[0] = frame->m33Rotation.v3X.x;
		mat44[1] = frame->m33Rotation.v3X.y;
		mat44[2] = frame->m33Rotation.v3X.z;
		mat44[3] = 0;
		mat44[4] = frame->m33Rotation.v3Y.x;
		mat44[5] = frame->m33Rotation.v3Y.y;
		mat44[6] = frame->m33Rotation.v3Y.z;
		mat44[7] = 0;
		mat44[8] = frame->m33Rotation.v3Z.x;
		mat44[9] = frame->m33Rotation.v3Z.y;
		mat44[10] = frame->m33Rotation.v3Z.z;
		mat44[11] = 0;
		mat44[12] = 0;
		mat44[13] = 0;
		mat44[14] = 0;
		mat44[15] = 1.0f;
		rotation.setM(mat44);
		node->setRotation(rotation.getRotationDegrees());
	}
}
