// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/utils.hpp"
#include "../include/structures.hpp"
#include "../include/irrtonewt.hpp"
#include "../include/body.hpp"
#include "../include/world.hpp"
#include "../include/intersection_point.hpp"
#include "../include/hidden.hpp"
#include "../include/body_controller.hpp"
#include "../include/IrrBox.h"

#include "rand.hpp"

using namespace irr;

irr::newton::IUtils::IUtils(irr::newton::IWorld* f_world) {
	this->world=f_world;
	this->class_type.Type=ECT_UTILS;
}

irr::newton::IUtils::IUtils() {
	this->class_type.Type=ECT_UTILS;
}


/*

irr::newton::IUtils::IUtils(NewtonWorld* nnworld) {
	this->n_world         = nnworld;
	this->class_type.Type = ECT_UTILS;
}

  */

irr::f32 irr::newton::IUtils::calculateTireWidth(scene::ISceneNode* tire_node) {
	core::aabbox3d<irr::f32> box=tire_node->getBoundingBox();

	core::matrix4 mat;
	mat.setScale(tire_node->getScale());
	mat.setTranslation(tire_node->getPosition());
	mat.setRotationDegrees(tire_node->getRotation());
	mat.transformBoxEx(box);

	core::vector3df box_size = box.getExtent()*tire_node->getScale();

	//width
	irr::f32 width=(box_size.Z*2)*IrrToNewton;
	return width;
}

irr::f32 irr::newton::IUtils::calculateTireRadius(scene::ISceneNode* tire_node) {
	core::aabbox3d<irr::f32> box=tire_node->getBoundingBox();

	core::matrix4 mat;
	mat.setScale(tire_node->getScale());
	mat.setTranslation(tire_node->getPosition());
	mat.setRotationDegrees(tire_node->getRotation());
	mat.transformBoxEx(box);

	core::vector3df box_size = box.getExtent()*tire_node->getScale();

	//radius
	irr::f32 radius=sqrt((box_size.X*box_size.X)+(box_size.Z*box_size.Z));
	radius/=2;
	radius*=IrrToNewton;

	return radius;
}



void irr::newton::IUtils::calculateCarTiresPosition(SCar car,
irr::core::vector3df position_offset_front, irr::core::vector3df position_offset_back) {

	position_offset_front.X = -position_offset_front.X;
	position_offset_front.Z = -position_offset_front.Z;
	position_offset_back.X = -position_offset_back.X;
	position_offset_back.Z = -position_offset_back.Z;

	core::vector3df tire_pos_front = car.ChassisBody->getNode()->getPosition()-position_offset_front;
	core::vector3df tire_pos_back = car.ChassisBody->getNode()->getPosition()-position_offset_back;

	//positiong front right tire
	car.TireNodeFR->setPosition(tire_pos_front);

	//positiong front left tire
	tire_pos_front.Z = tire_pos_front.Z + (position_offset_front.Z*2);
	car.TireNodeFL->setPosition(tire_pos_front);

	//positiong back left tire
	tire_pos_back.Z = tire_pos_back.Z + (position_offset_back.Z*2);
	tire_pos_back.X = tire_pos_back.X + (position_offset_back.X*2);
	car.TireNodeBL->setPosition(tire_pos_back);

	//positiong back right tire
	tire_pos_back.Z = tire_pos_back.Z - (position_offset_back.Z*2);
	car.TireNodeBR->setPosition(tire_pos_back);
}



//random number generator
unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
static MTRand_int32 irand(init, length); // 32-bit int generator
static MTRand drand(init,length); // 32-bit int generator
static MTRand_open drand_open(init,length); // 32-bit int generator
static MTRand_closed drand_closed(init,length); // 32-bit int generator

irr::s32 irr::newton::IUtils::randomS32() {
	static bool first_time=true;
	if(first_time) {
		first_time=false;
		irand.seed(irr::newton::Hidden::GetDevice()->getTimer()->getRealTime());
	}
	return irand();
}

irr::f64 irr::newton::IUtils::randomF64HalfOpen() {
	static bool first_time=true;
	if(first_time) {
		first_time=false;
		drand.seed(irr::newton::Hidden::GetDevice()->getTimer()->getRealTime());
	}
	return drand();
}


irr::f64 irr::newton::IUtils::randomF64Open() {
	static bool first_time=true;
	if(first_time) {
		first_time=false;
		drand_open.seed(irr::newton::Hidden::GetDevice()->getTimer()->getRealTime());
	}
	return drand_open();
}

irr::f64 irr::newton::IUtils::randomF64Closed() {
	static bool first_time=true;
	if(first_time) {
		first_time=false;
		drand_closed.seed(irr::newton::Hidden::GetDevice()->getTimer()->getRealTime());
	}
	return drand_closed();
}

irr::core::vector3df irr::newton::IUtils::getMiddlePoint(irr::scene::ISceneNode* node1,irr::scene::ISceneNode* node2) {
	irr::core::line3d<irr::f32> line(node1->getPosition(),node2->getPosition());
	return line.getMiddle();
}

void irr::newton::IUtils::updateCameraFPS(
										  irr::newton::IBody* cameraBody,
										  irr::f32 mouseSensitivity,
										  irr::f32 moveSpeed,
										  irr::newton::SDirection direction,
										  bool verticalMovement,
										  bool only_y,
										  bool stop_if_not_moving
										  ) {

	//if the camera is not the active camera return
	if(cameraBody->getNode() != this->world->getIrrlichtDevice()->getSceneManager()->getActiveCamera())
		return;

	static irr::IrrlichtDevice* device=irr::newton::Hidden::GetDevice();
	static bool firstUpdate=true;
	static irr::gui::ICursorControl* cursorControl=device->getCursorControl();


	//STEP 1: ROTATION FROM MOUSE

	core::position2d<f32> centerCursor;

	cameraBody->getNode()->updateAbsolutePosition();

	if (firstUpdate)
	{
		if (cursorControl)
			cursorControl->setPosition(0.5f, 0.5f);

		centerCursor = cursorControl->getRelativePosition();

		firstUpdate = false;
		return;
	}

	core::vector3df rotation;
	core::vector3df movedir, target, strafevect, movevect;

	target.set(0,0,1);

	rotation = cameraBody->getNode()->getRotation();

	rotation.X *= -1.0f;
	rotation.Y *= -1.0f;

	core::position2d<f32> cursorpos = cursorControl->getRelativePosition();

	//temp
	centerCursor.X=0.5f;
	centerCursor.Y=0.5f;
	//end of temp

	if (!core::equals(cursorpos.X, centerCursor.X) ||
		!core::equals(cursorpos.Y, centerCursor.Y))
	{
		rotation.Y += (0.5f - cursorpos.X) * mouseSensitivity /** this->world->getTimeElapsed()*/;
		rotation.X += (0.5f - cursorpos.Y) * mouseSensitivity /** this->world->getTimeElapsed()*/;

		//center the cursor
		cursorControl->setPosition(0.5f, 0.5f);

		centerCursor = cursorControl->getRelativePosition();

		if (rotation.X > 89.0f)
			rotation.X = 89.0f;
		if (rotation.X < -89.0f)
			rotation.X = -89.0f;
	}

	//update target
	core::matrix4 mat;
	mat.setRotationDegrees(core::vector3df(-rotation.X,- rotation.Y, 0));
	mat.transformVect(target);
	//end

	//apply rotation

	rotation.X *= -1.0f;
	rotation.Y *= -1.0f;

	cameraBody->getNode()->setRotation(rotation);
	core::vector3df movedir_target;

	if(!only_y)
		movedir_target = irr::newton::Hidden::correctCameraTarget((scene::ICameraSceneNode*)cameraBody->getNode());

	rotation.X *= 1.0f;
	rotation.Y *= 1.0f;


	//STEP 2: MOVING FROM KEYBOARD
	if(direction.isMoving()) {

	if(only_y) {

		core::vector3df camRotation = cameraBody->getNode()->getRotation();

		//recalculate camera target
		movedir_target.set(0,0,1);

		core::matrix4 m;
		m.setRotationDegrees(core::vector3df(0,camRotation.Y,0));
		//m.makeInverse();
		m.rotateVect(movedir_target);

		movedir_target += cameraBody->getNode()->getPosition();
	}

	movedir = movedir_target - cameraBody->getNode()->getPosition();

	movedir.normalize();

	if(direction.Up)
		movevect += movedir;

	if(direction.Down)
		movevect -= movedir;

	strafevect = target;
	strafevect = strafevect.crossProduct(core::vector3df(0,1,0));
	strafevect.normalize();

	if(direction.Left)
		movevect += strafevect;

	if(direction.Right)
		movevect -= strafevect;

	//movevect.normalize();

	movevect *= moveSpeed;

	//set velocity
	core::vector3df vel = cameraBody->getVelocity() ;

	vel.X = movevect.X;

	if(verticalMovement && !only_y) {

		//if(vel.Y < movevect.Y)
			vel.Y = movevect.Y;

	}

	vel.Z = movevect.Z;

	cameraBody->setVelocity(vel);

	//cameraBody->addForce(vel);

	}//if(direction.isMoving) {

	else { //is not moving

		if(stop_if_not_moving) {

			core::vector3df vel = cameraBody->getVelocity() ;
			vel.X = 0;

			if(verticalMovement && !only_y)
				vel.Y = 0;

			vel.Z = 0;
			cameraBody->setVelocity(vel);

		}//if(stop_if_not_moving) {


	}//else is not moving


	//set target
	//target+=cameraBody->getNode()->getPosition();
	//((scene::ICameraSceneNode*)cameraBody->getNode())->setTarget(target);
	//irr::newton::Hidden::correctCameraTarget((scene::ICameraSceneNode*)cameraBody->getNode());

	cameraBody->getNode()->updateAbsolutePosition();
}

irr::newton::IBody* irr::newton::IUtils::getBodyFromNodeID(irr::s32 id) {

	irr::core::array<irr::newton::IBody*> bodies = this->world->getAllBodies();

	for(int i=0; i<bodies.size(); i++) {
		if(bodies[i]->getNode()->getID()==id) return bodies[i];
	}//for

	return NULL;
}

irr::newton::IBody* irr::newton::IUtils::getBodyFromNode(scene::ISceneNode* node) {

	irr::core::array<irr::newton::IBody*> bodies = this->world->getAllBodies();

	for(int i=0; i<bodies.size(); i++) {
		if(bodies[i]->getNode()==node) return bodies[i];
	}//for

	return NULL;
}


void irr::newton::IUtils::avoidRotationOnAllAxes(irr::newton::IBody* body) {
	irr::newton::SJointUpVector jointData;
	jointData.ParentBody=body;
	jointData.PinDir.set(1,0,1);
	irr::newton::IJointUpVector* joint=this->world->createJoint(jointData);
	jointData.PinDir.set(0,1,0);
	joint=this->world->createJoint(jointData);
}

//-----------------------------------------------------------------------------------------
irr::core::array<irr::newton::SIntersectionPoint> irr::newton::IUtils::sortPointsByDistance(
								irr::core::array<SIntersectionPoint>& points,
								core::vector3df start_point) {


	irr::f32 last_distance=9999999;
	irr::s32 last_point=-1;

	irr::core::array<irr::newton::SIntersectionPoint> returned;

	while(true) {

	for(irr::u32 i=0;i<points.size();i++) {
		irr::f32 distance=points[i].point.getDistanceFrom(start_point);

		if(distance<last_distance) {
			last_distance=distance;
			last_point=i;
		}
	}//	for(int i=0;i<points.size();i++) {


	//not found another point
	if(last_point==-1) {
		return returned;
	}

	//
	else {
		//push point to end
		returned.push_back(points[last_point]);
		points.erase(last_point);

		//reinit
		last_distance=9999999;
		last_point=-1;

	}

	}//while

	return returned;
}

irr::newton::SIntersectionPoint irr::newton::IUtils::getClosestPoint(
	irr::core::array<irr::newton::SIntersectionPoint>& points,
	core::vector3df start_point) {

	irr::f32 last_distance=9999999;
	irr::s32 last_point=-1;

	irr::newton::SIntersectionPoint returned;
	if(points.size()==0) return returned;


	for(irr::u32 i=0;i<points.size();i++) {
		irr::f32 distance=points[i].point.getDistanceFrom(start_point);
		if(distance<last_distance) {
			last_distance=distance;
			last_point=i;
		}
	}//	for(int i=0;i<points.size();i++) {

	//not found another point
	if(last_point==(irr::s32)-1) {
		return returned;
	}

	else {
		return points[last_point];
	}

}

//---------------------------------------------------------------------------

core::array<newton::SPolygon> irr::newton::IUtils::getTriangles(newton::SPolygon poly) {

	core::array<newton::SPolygon> array;

	if(poly.Vertices.size()==3) {
		array.push_back(poly);
		return array;
	}

	//quad

	/*

  3  2
  +--+
  |  |
  |  |
  +--+
  0   1

	*/

	if(poly.Vertices.size()==3) {
		newton::SPolygon A,B;

		A.Vertices.push_back(poly.Vertices[0]);
		A.Vertices.push_back(poly.Vertices[1]);
		A.Vertices.push_back(poly.Vertices[2]);

		B.Vertices.push_back(poly.Vertices[0]);
		B.Vertices.push_back(poly.Vertices[1]);
		B.Vertices.push_back(poly.Vertices[3]);

		array.push_back(A);
		array.push_back(B);
		return array;
	}

	return array;

}

//--------------------------------------------------------------------------------------
core::array<core::vector3df> irr::newton::IUtils::getAllVertices(
												core::array<newton::SPolygon> poly) {

	core::array<core::vector3df> vertices;

	for(int i=0;i<poly.size();i++) {

		irr::newton::Hidden::PushArray(
			vertices,
			poly[i].Vertices);
	}//for

	return vertices;
}

core::array<core::vector3df> irr::newton::IUtils::getAllVertices(core::array<core::triangle3df> poly)  {

	core::array<core::vector3df> vertices;

	for(int i=0; i<poly.size(); i++) {

		vertices.push_back(poly[i].pointA);
		vertices.push_back(poly[i].pointB);
		vertices.push_back(poly[i].pointC);

	}//for

	return vertices;

}


core::array<f32> irr::newton::IUtils::getAsFloats(core::array<core::vector3df> poly) {

	core::array<f32> vertices;

	for(int i=0;i<poly.size();i++) {

		vertices.push_back(poly[i].X);
		vertices.push_back(poly[i].Y);
		vertices.push_back(poly[i].Z);

	}

	return vertices;
}

int irr::newton::IUtils::getMeshInfoFromMesh(scene::IMesh* mesh) {

	for(int i=0; i<this->meshInfos.MeshInfos.size(); i++) {
		if(this->meshInfos.MeshInfos[i].Mesh == mesh) return i;
	}
	return -1;

}

irr::f32 irr::newton::IUtils::getDistanceFromFloor(newton::IBody* body, irr::newton::SIntersectionPoint& i_point) {

	core::aabbox3d<f32> box = body->calculateAABB();

	core::line3d<f32> line(box.MinEdge,core::vector3df());
	line.end = line.start;
	line.end.Y -= 999999.9f;

	i_point = this->world->getCollisionManager()->getCollisionFirstPointEx(line);

	if( i_point.body == NULL ) return -1;

	return i_point.point.getDistanceFrom(line.start);

}

core::array<core::triangle3df> irr::newton::IUtils::getPolys(
										scene::IMesh* iMeshBase,
										core::vector3df scaleValue,
										float excludeAreaLess, /*=1.0e-6f*/
										irr::u32& skippedFace
										) {


	skippedFace = 0;
	core::array<core::triangle3df> triangles;

	int posInVertexInfoArray = this->getMeshInfoFromMesh(iMeshBase);
	bool buildVertexInfoData = posInVertexInfoArray == -1 ? true : false;

	SMeshInfo currMeshInfo;


//------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//
// Create the newton collision tree from the map mesh
///////////////////////////////////////////////////////////////////////////

	int cMeshBuffer, j;
	int v1i, v2i, v3i;
	scene::IMeshBuffer *mb;
	video::E_VERTEX_TYPE vertex_type;

	core::vector3df vArray[3]; // vertex array (3*3 floats)

	int tmpCount = 0;

	for (cMeshBuffer=0; cMeshBuffer<iMeshBase->getMeshBufferCount(); cMeshBuffer++)
	{
		mb = iMeshBase->getMeshBuffer(cMeshBuffer);

		//get the vertex type
		vertex_type = mb->getVertexType();

//-------------------------STANDARD VERTEX----------------------------------------

		//based on the vertex type
		if(vertex_type == video::EVT_STANDARD) {

			irr::video::S3DVertex* mb_vertices =
				(irr::video::S3DVertex*)mb->getVertices();

			u16* mb_indices  = mb->getIndices();

			// add each triangle from the mesh
			for (j=0; j<mb->getIndexCount(); j+=3)
			{
				v1i = mb_indices[j];
				v2i = mb_indices[j+1];
				v3i = mb_indices[j+2];

				// to make thing easier, her we can use engine data type
				vArray[0] = mb_vertices[v1i].Pos * scaleValue;
				vArray[1] = mb_vertices[v2i].Pos * scaleValue;
				vArray[2] = mb_vertices[v3i].Pos * scaleValue;

				// make sure we do not add degenerated polygons to the tree
				core::vector3df e0 (vArray[1] - vArray[0]);
				core::vector3df e1 (vArray[2] - vArray[0]);

				// the area of the face is the cross product
				core::vector3df area (e0.crossProduct (e1));

				// skip faces with very small area
				float mag = area.dotProduct (area);

				if (mag > excludeAreaLess) {

					core::triangle3d<f32> thisTriangle;
					thisTriangle.set( vArray[0], vArray[1], vArray[2] );
					triangles.push_back(thisTriangle);

					//store vertex information
					if(buildVertexInfoData) {
						newton::STriangleInfoData currVertexInfo;

						currVertexInfo.TCoords[0] = &mb_vertices[v1i].TCoords;
						currVertexInfo.TCoords[1] = &mb_vertices[v2i].TCoords;
						currVertexInfo.TCoords[2] = &mb_vertices[v3i].TCoords;

						currVertexInfo.Triangle = thisTriangle;

						currMeshInfo.Triangles.push_back(currVertexInfo);
					}

				}// if (mag > 1.0e-6f) {
				else ++skippedFace;

			}//for (j=0; j<mb->getIndexCount(); j+=3)

		}//if(vertex_type==video::EVT_STANDARD) {

//-------------------------Vertex with two texture coordinates----------------------------------------------

		else if(vertex_type == video::EVT_2TCOORDS) {

			irr::video::S3DVertex2TCoords* mb_vertices =
				(irr::video::S3DVertex2TCoords*)mb->getVertices();

			u16* mb_indices  = mb->getIndices();

			// add each triangle from the mesh
			for (j=0; j<mb->getIndexCount(); j+=3)
			{
				v1i = mb_indices[j];
				v2i = mb_indices[j+1];
				v3i = mb_indices[j+2];

				// to make thing easier, her we can use engine data type
				vArray[0] = mb_vertices[v1i].Pos * scaleValue;
				vArray[1] = mb_vertices[v2i].Pos * scaleValue;
				vArray[2] = mb_vertices[v3i].Pos * scaleValue;

				// make sure we do not add degenerated polygons to the tree
				core::vector3df e0 (vArray[1] - vArray[0]);
				core::vector3df e1 (vArray[2] - vArray[0]);

				// the area of the face is the cross product
				core::vector3df area (e0.crossProduct (e1));

				// skip faces with very small area
				float mag = area.dotProduct (area);

				if (mag > excludeAreaLess) {

					core::triangle3df thisTriangle;
					thisTriangle.set( vArray[0], vArray[1], vArray[2] );
					triangles.push_back(thisTriangle);

					//store vertex information
					if(buildVertexInfoData) {
						newton::STriangleInfoData currVertexInfo;

						currVertexInfo.TCoords[0] = &mb_vertices[v1i].TCoords;
						currVertexInfo.TCoords[1] = &mb_vertices[v2i].TCoords;
						currVertexInfo.TCoords[2] = &mb_vertices[v3i].TCoords;

						currVertexInfo.Triangle = thisTriangle;

						currMeshInfo.Triangles.push_back(currVertexInfo);
					}

				}// if (mag > 1.0e-6f) {

				else ++skippedFace;

			}//for (j=0; j<mb->getIndexCount(); j+=3)

		}//if(vertex_type==video::EVT_STANDARD) {

//--------------------------Vertex with a tangent and binormal vector--------------------------------------------

		else if(vertex_type==video::EVT_TANGENTS) {
			irr::video::S3DVertexTangents* mb_vertices =
				(irr::video::S3DVertexTangents*)mb->getVertices();

			u16* mb_indices  = mb->getIndices();

			// add each triangle from the mesh
			for (j=0; j<mb->getIndexCount(); j+=3)
			{
				v1i = mb_indices[j];
				v2i = mb_indices[j+1];
				v3i = mb_indices[j+2];

				// to make thing easier, her we can use engine data type
				vArray[0] = mb_vertices[v1i].Pos * scaleValue;
				vArray[1] = mb_vertices[v2i].Pos * scaleValue;
				vArray[2] = mb_vertices[v3i].Pos * scaleValue;

				// make sure we do not add degenerated polygons to the tree
				core::vector3df e0 (vArray[1] - vArray[0]);
				core::vector3df e1 (vArray[2] - vArray[0]);

				// the area of the face is the cross product
				core::vector3df area (e0.crossProduct (e1));

				// skip faces with very small area
				float mag = area.dotProduct (area);

				if (mag > excludeAreaLess) {

					core::triangle3df thisTriangle;
					thisTriangle.set( vArray[0], vArray[1], vArray[2] );
					triangles.push_back(thisTriangle);

					//store vertex information
					if(buildVertexInfoData) {

						newton::STriangleInfoData currVertexInfo;

						currVertexInfo.TCoords[0] = &mb_vertices[v1i].TCoords;
						currVertexInfo.TCoords[1] = &mb_vertices[v2i].TCoords;
						currVertexInfo.TCoords[2] = &mb_vertices[v3i].TCoords;

						currVertexInfo.Triangle = thisTriangle;

						currMeshInfo.Triangles.push_back(currVertexInfo);

					}

				}// if (mag > 1.0e-6f) {

				else ++skippedFace;

			}//for (j=0; j<mb->getIndexCount(); j+=3)

		}//if(vertex_type==video::EVT_STANDARD) {

//-------------------------------------------------------------------------

	}//	for (cMeshBuffer=0; cMeshBuffer<iMeshBase->getMesh(0)->getMeshBufferCount(); cMeshBuffer++)

	//put in mesh info array
	if(buildVertexInfoData) {
		currMeshInfo.Mesh = iMeshBase;
		this->meshInfos.MeshInfos.push_back(currMeshInfo);
	}

	//return
	return triangles;

}// function end




//---------------------------------------
//get polys in newton units

core::array<core::triangle3df> irr::newton::IUtils::getPolysNewton(
									scene::IMesh* iMeshBase,
									core::vector3df scaleValue,
									float excludeAreaLess, /*=1.0e-6f*/
									irr::u32& skippedFace
									) {

	core::array<core::triangle3df> tri = this->getPolys(
		iMeshBase,
		scaleValue,
		excludeAreaLess,
		skippedFace
	);

	for(irr::u32 i=0; i<tri.size(); i++) {
		tri[i].pointA *= IrrToNewton;
		tri[i].pointB *= IrrToNewton;
		tri[i].pointC *= IrrToNewton;
	}

	return tri;

}
//-----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

newton::IBody* irr::newton::IUtils::launchCube(irr::f32 size) {

	video::IVideoDriver* driver = this->world->getIrrlichtDevice()->getVideoDriver();
	scene::ISceneManager* smgr = this->world->getIrrlichtDevice()->getSceneManager();
	scene::ICameraSceneNode* camera = smgr->getActiveCamera();


	scene::ISceneNode* cube = smgr->addCubeSceneNode(size);

	cube->setPosition(camera->getPosition());
	cube->setMaterialFlag(video::EMF_LIGHTING,false);
	cube->setMaterialTexture(
		0,
		driver->getTexture("data/2d/terminal/buy_humvee.png"));



	irr::newton::ICharacterController* cube_body =
		this->world->createCharacterController(
		this->world->createBody(cube));

	core::vector3df rot;
	rot.Y = camera->getRotation().Y;
	rot.X = camera->getRotation().X;
	cube_body->setRotation(rot);

	core::vector3df force = cube_body->FRIgetDirectionPositionXY(
		core::vector3df(0, 0, 150));

	cube_body->addForce(force);

	cube_body->addForceContinuous(core::vector3df(0,-2.0,0));

	return cube_body;

}






// ************************************************************************************
//
// I can not get the mesh x file to work, so I will make a rad doll with boxes
//
// **************************************************************************************
using namespace scene;
using namespace core;

scene::ISceneNode* irr::newton::IUtils::buildDefaultRagdoll(
															core::vector3df loc,
															video::ITexture* texture) {

    scene::ISceneManager* smgr = this->world->getIrrlichtDevice()->getSceneManager();

	float headSize;
	ISceneNode *head_Bone;
	ISceneNode *spine0_Bone;
	ISceneNode *spine1_Bone;
	ISceneNode *rUpperArm_Bone;
	ISceneNode *rLowerArm_Bone;
	ISceneNode *lUpperArm_Bone;
	ISceneNode *lLowerArm_Bone;

	ISceneNode *rUpperThigh_Bone;
	ISceneNode *rLowerThigh_Bone;
	ISceneNode *lUpperThigh_Bone;
	ISceneNode *lLowerThigh_Bone;

	// create a fake model made out of boxes
	// all dimension proportional to the head
	headSize = 20.0f;

	// set the lower spine bone (this is the root of the rag doll)
	vector3df spineSize (headSize * 2.0f, headSize, headSize * 2.0f);
	spine0_Bone = (IAnimatedMeshSceneNode*)new IrrBox(smgr->getRootSceneNode(), smgr, 666, spineSize.X, spineSize.Y, spineSize.Z);
	if(texture != NULL)
	{
		spine0_Bone->setMaterialTexture(0, texture);
		spine0_Bone->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	spine0_Bone->setPosition (loc);
	spine0_Bone->setRotation (vector3df (90.0f, 0.0f, 90.0f));
	spine0_Bone->setID (0);

	// set the upper spine bone
	spine1_Bone = (ISceneNode*)new IrrBox(spine0_Bone, smgr, 666, spineSize.X, spineSize.Y, spineSize.Z);
	if(texture != NULL)
	spine1_Bone->setMaterialTexture(0, texture);
	spine1_Bone->setPosition (vector3df (spineSize.X, 0.0f, 0.0f));
	spine1_Bone->setRotation (vector3df (0.0f, 0.0f, 0.0f));
	spine1_Bone->setID (1);

	// add the head
	vector3df headSize1 (headSize, headSize, headSize);
	head_Bone = (ISceneNode*)new IrrBox(spine1_Bone, smgr, 666, headSize1.X, headSize1.Y, headSize1.Z);
	if(texture != NULL)
	head_Bone->setMaterialTexture(0, texture);
	head_Bone->setPosition (vector3df (spineSize.X, 0.0f, 0.0f));
	head_Bone->setRotation (vector3df (0.0f, 0.0f, 0.0f));
	head_Bone->setName("head_bone");
	head_Bone->setID (2);


	// Right Upper arm
	vector3df armSize (headSize * 1.5f, headSize * 0.5f, headSize * 0.5f);
	rUpperArm_Bone = (ISceneNode*)new IrrBox(spine1_Bone, smgr, 666, armSize.X, armSize.Y, armSize.Z);
	if(texture != NULL)
	rUpperArm_Bone->setMaterialTexture(0, texture);
	rUpperArm_Bone->setPosition (vector3df (spineSize.X, 0.0f, spineSize.X * 0.55f));
	rUpperArm_Bone->setRotation (vector3df (0.0f, -90.0f, 90.0f));
	rUpperArm_Bone->setID (3);

	// Right Lower arm
	rLowerArm_Bone = (ISceneNode*)new IrrBox(rUpperArm_Bone, smgr, 666, armSize.X, armSize.Y, armSize.Z);
	if(texture != NULL)
	rLowerArm_Bone->setMaterialTexture(0, texture);
	rLowerArm_Bone->setPosition (vector3df (armSize.X, 0.0f, 0.0f));
	rLowerArm_Bone->setRotation (vector3df (0.0f, 0.0f, 0.0f));
	rLowerArm_Bone->setID (4);

	// Left Upper leg
	lUpperArm_Bone = (ISceneNode*)new IrrBox(spine1_Bone, smgr, 666, armSize.X, armSize.Y, armSize.Z);
	if(texture != NULL)
	lUpperArm_Bone->setMaterialTexture(0, texture);
	lUpperArm_Bone->setPosition (vector3df (spineSize.X, 0.0f, -spineSize.X * 0.55f));
	lUpperArm_Bone->setRotation (vector3df (0.0f, 90.0f, 90.0f));
	lUpperArm_Bone->setID (5);

	// Left Lower leg
	lLowerArm_Bone = (ISceneNode*)new IrrBox(lUpperArm_Bone, smgr, 666, armSize.X, armSize.Y, armSize.Z);
	if(texture != NULL)
	lLowerArm_Bone->setMaterialTexture(0, texture);
	lLowerArm_Bone->setPosition (vector3df (armSize.X, 0.0f, 0.0f));
	lLowerArm_Bone->setRotation (vector3df (0.0f, 0.0f, 0.0f));
	lLowerArm_Bone->setID (6);

	// Right Upper arm
	vector3df thighSize (headSize * 2.0f, headSize * 0.75f, headSize * 0.75f);
	rUpperThigh_Bone = (ISceneNode*)new IrrBox(spine0_Bone, smgr, 666, thighSize.X, thighSize.Y, thighSize.Z);
	if(texture != NULL)
	rUpperThigh_Bone->setMaterialTexture(0, texture);
	rUpperThigh_Bone->setPosition (vector3df (0.0, 0.0f, spineSize.X * 0.35f));
	rUpperThigh_Bone->setRotation (vector3df (0.0f, 0.0f, 180.0f));
	rUpperThigh_Bone->setID (7);

	// Right Lower arm
	vector3df calfSize (headSize * 2.0f, headSize * 0.65f, headSize * 0.65f);
	rLowerThigh_Bone = (ISceneNode*)new IrrBox(rUpperThigh_Bone, smgr, 666, calfSize.X, calfSize.Y, calfSize.Z);
	if(texture != NULL)
	rLowerThigh_Bone->setMaterialTexture(0, texture);
	rLowerThigh_Bone->setPosition (vector3df (thighSize.X, 0.0f, 0.0f));
	rLowerThigh_Bone->setRotation (vector3df (0.0f, 0.0f, 0.0f));
	rLowerThigh_Bone->setID (8);

	// Left Upper leg
	//vector3df thighSize (headSize * 2.0f, headSize * 0.75f, headSize * 0.75f);
	lUpperThigh_Bone = (ISceneNode*)new IrrBox(spine0_Bone, smgr, 666, thighSize.X, thighSize.Y, thighSize.Z);
	if(texture != NULL)
	lUpperThigh_Bone->setMaterialTexture(0, texture);
	lUpperThigh_Bone->setPosition (vector3df (0.0, 0.0f, -spineSize.X * 0.35f));
	lUpperThigh_Bone->setRotation (vector3df (0.0f, 0.0f, 180.0f));
	lUpperThigh_Bone->setID (9);

	// Left Lower leg
	//vector3df calfSize (headSize * 2.0f, headSize * 0.65f, headSize * 0.65f);
	lLowerThigh_Bone = (ISceneNode*)new IrrBox(lUpperThigh_Bone, smgr, 666, calfSize.X, calfSize.Y, calfSize.Z);
    if(texture != NULL)
	lLowerThigh_Bone->setMaterialTexture(0, texture);
	lLowerThigh_Bone->setPosition (vector3df (thighSize.X, 0.0f, 0.0f));
	lLowerThigh_Bone->setRotation (vector3df (0.0f, 0.0f, 0.0f));
	lLowerThigh_Bone->setID (10);

	return spine0_Bone;
}
