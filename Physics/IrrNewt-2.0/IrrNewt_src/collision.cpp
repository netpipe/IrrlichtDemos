// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/world.hpp"
#include "../include/utils.hpp"
#include "../include/collision.hpp"
#include "../include/hidden.hpp"
#include <iostream>
#include <fstream>

using namespace irr;
using namespace newton;
using namespace Hidden;

NewtonCollision* CreateCollisionTreeTerrain(
											IWorld* world,
											scene::ISceneNode* node,
											irr::s32 LOD,
											unsigned int& PolyCount) {

	NewtonWorld* nWorld = const_cast<NewtonWorld*>(world->getNewtonWorld());
	NewtonCollision* nCollision;

	//newton part
	nCollision = NewtonCreateTreeCollision(nWorld, 0);
	NewtonTreeCollisionBeginBuild(nCollision);
	u32 v1i, v2i, v3i;

	irr::f32 vArray[9]; // vertex array (3*3 floats)

	int tmpCount = 0;

#ifndef COMPILE_WITH_1_5
	scene::SMeshBufferLightMap mb2;
	((scene::ITerrainSceneNode*)node)->getMeshBufferForLOD(
		mb2,
		LOD //level of detail
		);
	mb2.Indices.set_free_when_destroyed(false);
    mb2.Vertices.set_free_when_destroyed(false);

    video::S3DVertex2TCoords* vertices = (video::S3DVertex2TCoords*)mb2.getVertices();
	u16* indices = mb2.getIndices();
	u16 index_count = mb2.getIndexCount();

#else

    scene::CDynamicMeshBuffer* mb2 = new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
    ((scene::ITerrainSceneNode*)node)->getMeshBufferForLOD(*mb2,LOD);

    scene::IVertexBuffer& vertices = mb2->getVertexBuffer();
	scene::IIndexBuffer& indices = mb2->getIndexBuffer();
	u32 index_count = indices.size();
#endif

	core::vector3df mapsize = node->getScale();

	// create collision tree faces
	for (int i = 0; i < index_count ; i += 3)
	{
	   v1i = indices[i];
	   v2i = indices[i + 1];
	   v3i = indices[i + 2];

		// build face data
		vArray[0] = vertices[v1i].Pos.X * mapsize.X * IrrToNewton;
		vArray[1] = vertices[v1i].Pos.Y * mapsize.Y * IrrToNewton;
		vArray[2] = vertices[v1i].Pos.Z * mapsize.Z * IrrToNewton;

		vArray[3] = vertices[v2i].Pos.X * mapsize.X * IrrToNewton;
		vArray[4] = vertices[v2i].Pos.Y * mapsize.Y * IrrToNewton;
		vArray[5] = vertices[v2i].Pos.Z * mapsize.Z * IrrToNewton;

		vArray[6] = vertices[v3i].Pos.X * mapsize.X * IrrToNewton;
		vArray[7] = vertices[v3i].Pos.Y * mapsize.Y * IrrToNewton;
		vArray[8] = vertices[v3i].Pos.Z * mapsize.Z * IrrToNewton;

		// make sure we do not add degenerated polygons to the tree
		core::vector3df irrvArray[3] = {
			core::vector3df(vArray[0],vArray[1],vArray[2]),
			core::vector3df(vArray[3],vArray[4],vArray[5]),
			core::vector3df(vArray[6],vArray[7],vArray[8])
		};

		core::vector3df e0 (irrvArray[1] - irrvArray[0]);
		core::vector3df e1 (irrvArray[2] - irrvArray[0]);

		// the area of the face is the cross product
		core::vector3df area (e0.crossProduct (e1));

		// skip faces with very small area
		irr::f32 mag = area.dotProduct (area);
		//if (mag > 1.0e-6f) {
			// add face to tree
			NewtonTreeCollisionAddFace(nCollision , 3, (float*)vArray, 12, 11);
			PolyCount++;
		//}// if (mag > 1.0e-6f) {

	}

	NewtonTreeCollisionEndBuild(nCollision, 1);

	//set
	#ifdef COMPILE_WITH_1_5
	mb2->drop();
	#endif

	return nCollision;

}

NewtonCollision* CreateCollisionTree(
									 IWorld* world,
									 scene::IMesh* mesh,
									 core::vector3df scale_value,
									 unsigned int& SkippedFace,
									 unsigned int& PolyCount
									 ) {

		NewtonCollision* nCollision;

		nCollision = NewtonCreateTreeCollision(world->getNewtonWorld(), 0);
		NewtonTreeCollisionBeginBuild(nCollision);

		core::array< core::triangle3d<f32> > polys =
			world->getUtils()->getPolysNewton(

			mesh,
			scale_value,
			world->getInternalBehaviour()->treeTriangleMinArea,
			SkippedFace

			);


		for(irr::u32 i=0; i<polys.size(); i++) {

			f32 vertices[] = {
				polys[i].pointA.X,
				polys[i].pointA.Y,
				polys[i].pointA.Z,

				polys[i].pointB.X,
				polys[i].pointB.Y,
				polys[i].pointB.Z,

				polys[i].pointC.X,
				polys[i].pointC.Y,
				polys[i].pointC.Z
			};

			NewtonTreeCollisionAddFace(
				nCollision, //collision
				3,  //vertex count, triangle 3
				vertices, //vertices pointer
				sizeof (core::vector3df), //size of each vertex in bytes.
				0 //face attribute
				);

			PolyCount++;

		}

		// we definilte want to optimize the mesh (improve performace a lot)
		//so we set the second parameter to 1
		//set to 0 to don't optimize
		NewtonTreeCollisionEndBuild(nCollision, 1);

		return nCollision;
}

//create convex from mesh
NewtonCollision* createConvexHullCollision(
										   newton::IWorld* world,
										   scene::ISceneNode* node,
										   scene::IMesh* mesh,
										   irr::core::matrix4 offset) {

	NewtonCollision* nCollision;

	core::vector3df scale = node->getScale();
	irr::u32 skippedFaceNumber;

	core::array<f32> polys =
		world->getUtils()->getAsFloats(
		world->getUtils()->getAllVertices(
		world->getUtils()->getPolysNewton(
		mesh,
		scale,
		1.0e-7f,
		skippedFaceNumber
		)));


   nCollision = NewtonCreateConvexHull(
	   world->getNewtonWorld(),
	   (polys.size()/3),
	   &polys[0],
	   (sizeof(irr::f32)*3),
	   0.003f,
	   0,
	   GetMatrixPointer(offset));

   	return nCollision;
}


NewtonCollision* CreateCollision(
								 newton::IWorld* world,
								 scene::ISceneNode* node,
								 irr::core::vector3df& box_size,
								 core::matrix4 offset,
								 irr::scene::IMesh* mesh_for_convex,
								 newton::E_BODY_TYPE& type,
								 s32 LOD,
								 unsigned& SkippedFace,
								 unsigned& PolyCount) {

	//make 0 the counters
	SkippedFace = 0; PolyCount = 0;

	scene::ESCENE_NODE_TYPE nodeType = node->getType();

	NewtonCollision* nCollision;

	irr::f32 radius, height;
	core::vector3df scale;

	//switch among different body types

	switch(type) {

	case EBT_AUTODETECT:

	if( nodeType == scene::ESNT_MESH||
		nodeType == scene::ESNT_ANIMATED_MESH)

		type = irr::newton::EBT_PRIMITIVE_BOX;


	else if(nodeType == scene::ESNT_CAMERA||
		    nodeType == scene::ESNT_CAMERA_MAYA||
		    nodeType == scene::ESNT_CAMERA_FPS)

		type = irr::newton::EBT_PRIMITIVE_BOX;

	else if(nodeType == scene::ESNT_OCTREE || nodeType == scene::ESNT_UNKNOWN)

		type = irr::newton::EBT_TREE;

	else if(nodeType == scene::ESNT_TERRAIN)

		type = irr::newton::EBT_TREE_TERRAIN;

	else if(nodeType == scene::ESNT_CUBE)

		type = irr::newton::EBT_PRIMITIVE_BOX;

	else if(nodeType == scene::ESNT_SPHERE)

		type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

	nCollision = CreateCollision(world,node,box_size,offset,mesh_for_convex,type, LOD, SkippedFace, PolyCount);

	break;

	//tree
	case EBT_TREE:

        scale = node->getScale();
        if(node->getParent() != NULL)
            scale = scale * node->getParent()->getScale();

		nCollision = CreateCollisionTree( world, mesh_for_convex, scale,
			SkippedFace, PolyCount);

		break;

	//tree terrain
	case EBT_TREE_TERRAIN:

		nCollision = CreateCollisionTreeTerrain(
			world, node, LOD, PolyCount);

		break;


	//box
	case EBT_PRIMITIVE_BOX:

	nCollision = NewtonCreateBox(
		world->getNewtonWorld(),
		box_size.X,
		box_size.Y,
		box_size.Z,
		0,
		GetMatrixPointer(offset));
		break;


	//ellipsoid
	case EBT_PRIMITIVE_ELLIPSOID:

	nCollision = NewtonCreateSphere(
		world->getNewtonWorld(),
		box_size.X/2.0f,
		box_size.Y/2.0f,
		box_size.Z/2.0f,
		0,
		GetMatrixPointer(offset));
		break;

	//cylinder
	case EBT_PRIMITIVE_CYLINDER:

		offset.setRotationDegrees(offset.getRotationDegrees()+core::vector3df(0,0,-90));

		//get radius
		if(box_size.Z >= box_size.X) radius = box_size.Z;
		else radius = box_size.X;
		radius /= 2.0f;
		//end

	nCollision = NewtonCreateCylinder(
		world->getNewtonWorld(),
		radius,
		box_size.Y,//height
		0,
		GetMatrixPointer(offset));
		break;

	//chamfer cylinder
	case EBT_PRIMITIVE_CHAMFER_CYLINDER:

		offset.setRotationDegrees(offset.getRotationDegrees()+core::vector3df(0,0,-90));

		//get radius
		if(box_size.Z>=box_size.X) radius=box_size.Z;
		else radius=box_size.X;
		radius/=2.0f;
		//end

	nCollision = NewtonCreateChamferCylinder(
		world->getNewtonWorld(),
		radius,
		box_size.Y,//height
		0,
		GetMatrixPointer(offset));
		break;

	//cone
	case EBT_PRIMITIVE_CONE:

		offset.setRotationDegrees(offset.getRotationDegrees()+core::vector3df(-90,0,90));

		//get radius
		if(box_size.X>=box_size.Z) radius = box_size.X;
		else radius = box_size.Z;

		//radius/=2.0f;
		//radius*=IrrToNewton;
		//end

		height = box_size.Y;

		nCollision = NewtonCreateCone(
			world->getNewtonWorld(),
			radius,
			height,//height /**1.33f*/
			0,
			GetMatrixPointer(offset));
		break;

	//capsule
	case EBT_PRIMITIVE_CAPSULE:

		offset.setRotationDegrees(offset.getRotationDegrees()+core::vector3df(-90,0,90));

		//get radius
		irr::f32 radius;
		if(box_size.Z>=box_size.X) radius=box_size.Z;
		else radius=box_size.X;
		radius/=2.0f;
		//end

		nCollision = NewtonCreateCapsule(
			world->getNewtonWorld(),
			radius, //radius
			box_size.Y,//height
			0,
			GetMatrixPointer(offset));
		break;

	//null
	case EBT_NULL:

		nCollision = NewtonCreateNull(world->getNewtonWorld());
		break;

	//convex hull
	case EBT_CONVEX_HULL:

		nCollision = createConvexHullCollision(world, node, mesh_for_convex,offset);
		break;

	case EBT_CONVEX_HULL_MODIFIER:

		nCollision = createConvexHullCollision(world, node, mesh_for_convex,offset);
		NewtonCreateConvexHullModifier(world->getNewtonWorld(), nCollision, 0);
		break;

	default:
		world->getIrrlichtDevice()->getLogger()->log("Error in IBody::CreateCollision. "
			"Invalid collision type",irr::ELL_ERROR);

		nCollision = NULL;
		return nCollision;
		break;
	}//switch
	//END OF CREATING COLLISION

	return nCollision;
}

core::vector3df CalcBoxSize(newton::SBodyFromNode data) {

	core::aabbox3d<f32> box;

	//get the aabbox
	if( data.CalculateSizeMode == ECS_MESH && data.Mesh == NULL ) data.CalculateSizeMode = ECS_NODE;


	if( data.CalculateSizeMode == ECS_MESH ) {
		box = data.Mesh->getBoundingBox();
	}

	else if( data.CalculateSizeMode == ECS_NODE ){
		box = data.Node->getBoundingBox();
	}

	scene::ESCENE_NODE_TYPE type = data.Node->getType();

	if(type==scene::ESNT_CAMERA||
		type==scene::ESNT_CAMERA_MAYA||
		type==scene::ESNT_CAMERA_FPS) {

		box.MaxEdge.set(+2,+2,+2);
		box.MinEdge.set(-2,-2,-2);
	}

	//size the box in Newton units

	/*
	core::matrix4 mat;
	mat.setScale(node->getScale());
	mat.setTranslation(node->getPosition()*IrrToNewton);
	mat.setRotationDegrees(node->getRotation());
	mat.transformBoxEx(box);
	*/

	core::vector3df box_size = (box.getExtent() * IrrToNewton);

	//already in matrix transformation
	core::vector3df scale = data.Node->getScale();
	box_size *= (data.Node->getScale());

	box_size *= data.BodyScaleOffsetFromNode;

	return box_size;

	//end of calculate box size
}

core::matrix4 CalcOffset(newton::SBodyFromNode data) {

	data.Node->updateAbsolutePosition();

	core::matrix4 offset;

//	offset.setScale(core::vector3df(1,1,1));

	//translation offset
	core::vector3df trans_offset;

	//automatic calculation of translation offset
	if (data.CalculateOffsetMode == ECO_MESH && data.Mesh == NULL) data.CalculateOffsetMode = ECO_NODE;
	if (data.CalculateOffsetMode == ECO_NODE && data.Node == NULL) data.CalculateOffsetMode = ECO_NONE;


	if( data.Type  != newton::EBT_CONVEX_HULL          &&
		data.Type  != newton::EBT_CONVEX_HULL_MODIFIER &&
		data.CalculateOffsetMode != ECO_NONE) {

		//if mesh is NULL try to calculate the offset from the node (not so good)

		if( data.CalculateOffsetMode == ECO_NODE ) {

			//calculate translation offset
			core::aabbox3df transf_box  = data.Node->getTransformedBoundingBox();
			core::aabbox3df untransf_box = data.Node->getBoundingBox();

			core::vector3df t_center = transf_box.getCenter();
			core::vector3df ut_center = data.Node->getBoundingBox().getCenter();

			core::vector3df position = data.Node->getPosition();
			core::vector3df abs_position = data.Node->getAbsolutePosition();

			trans_offset = transf_box.getCenter() - abs_position;

		}

		//calculate the offset from the mesh (better?)
		//this->calcOffsetMode is ECO_MESH
		else {
			trans_offset = data.Mesh->getBoundingBox().getCenter();
		}


		//convert in newton unit
		trans_offset *= IrrToNewton;

		//set translation offset
		offset.setTranslation(trans_offset);


  	}//if(this->shape_type!=newton::EST_CONVEX_HULL&&

	//convert user defined translation in newton unit
	data.BodyOffsetFromNode.setTranslation( data.BodyOffsetFromNode.getTranslation() * IrrToNewton);

	//add user offset with automatic calculated offset
	offset = irr::newton::Hidden::AddMatrixNoScale(offset, data.BodyOffsetFromNode);

	return offset;

}


void ICollision::create(core::array<irr::newton::ICollision*> collisions)
{
	core::array<NewtonCollision*> nCollisionList;

	for(int i=0; i<collisions.size(); i++)
		nCollisionList.push_back(collisions[i]->getNewtonCollision());


	this->n_collision = NewtonCreateCompoundCollision(
		this->world->getNewtonWorld(),
		nCollisionList.size(),
		&nCollisionList[0],
		0);

	this->creationData.Type = newton::EBT_COMPOUND_COLLISION;

}


void ICollision::create(newton::SBodyFromNode data)
{
	this->size = CalcBoxSize(data);

	//add to box_size the user specified size
	this->size += data.Size;

	core::matrix4 offset = CalcOffset(data);

	NewtonCollision* nCollision = CreateCollision(
		this->world,
		data.Node,
		this->size,
		offset,
		data.Mesh,
		data.Type,
		data.TerrainLOD,
		this->SkippedFace,
		this->PolyCount);

	this->n_collision = nCollision;
	this->creationData = data;

}

void ICollision::release() {
	NewtonReleaseCollision(this->world->getNewtonWorld(), this->n_collision);
	this->isReleased = true;
}

f32 ICollision::calculateVolume() {
	return NewtonConvexCollisionCalculateVolume(this->n_collision);
}

void ICollision::setUserID(u32 ID) {
	NewtonCollisionSetUserID(this->n_collision,ID);
}

u32 ICollision::getUserID() {
	return NewtonCollisionGetUserID(this->n_collision);
}

void ICollision::calculateInertialMatrix(
	core::vector3df& inertia,
	core::vector3df& origin) {

	f32 inertia_array[3], origin_array[3];

	NewtonConvexCollisionCalculateInertialMatrix(this->n_collision, inertia_array, origin_array);

	inertia = Vector3dfFromArray(inertia_array);
	origin = Vector3dfFromArray(origin_array);
}
