// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include <stddef.h>
#include "../include/collision_manager.hpp"
#include "../include/intersection_point.hpp"
#include "../include/body.hpp"
#include "../include/world.hpp"
#include "../include/hidden.hpp"
//#include "../include/utils.hpp"

using namespace irr::newton::Hidden;

#pragma warning(4:4800) //forcing value int to be true or false


irr::newton::ICollisionManager::ICollisionManager() {
	this->class_type.Type=ECT_COLLISION_MANAGER;
}


irr::newton::ICollisionManager::ICollisionManager(NewtonWorld* newton_world) {
	this->n_world=newton_world;
	this->class_type.Type=ECT_COLLISION_MANAGER;
}

//------------------------------------------------------
bool irr::newton::ICollisionManager::getCollisionPoint(
						 irr::newton::IBody* body,
						core::line3d<f32> line,
						irr::newton::SIntersectionPoint& out) {

	//line.start.normalize();
   //line.end.normalize();

   //moltiplico per l'inversa della matrice
   //prendo la matrice
   core::matrix4 node_matrix=
	   body->getNode()->getAbsoluteTransformation();
   //NewtonBodyGetMatrix(body->getNewtonBody(),&node_matrix.M[0]);

   //la inverto
   core::matrix4 inverse_matrix;
   node_matrix.getInverse(inverse_matrix);

   //moltiplico i due punti
   inverse_matrix.transformVect(line.start);
   inverse_matrix.transformVect(line.end);

   //li preparo per essere importati in newton
   line.start*=IrrToNewton;
   line.end*=IrrToNewton;

   int face_id;

   float line_start_array[3];
   irr::newton::Hidden::Vector3dfFillArray(line.start,line_start_array);
   float line_end_array[3];
   irr::newton::Hidden::Vector3dfFillArray(line.end,line_end_array);
   float normals_array[3];

   float parametric_value=NewtonCollisionRayCast(
	  body->getNewtonCollision(),
	  line_start_array,
      line_end_array,
      normals_array,
      &face_id);

   if(parametric_value>1) return false;

   //setto il giusto valore a out
   core::vector3df out_point=line.start + parametric_value*(line.end - line.start);
   out_point*=NewtonToIrr;

   node_matrix.transformVect(out_point);

	out.point=out_point;
	out.normals=core::vector3df(
		normals_array[0],
		normals_array[1],
		normals_array[2]);

	out.parametric_value=parametric_value;
	out.body=body;
	out.face_id=face_id;

	return true;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

irr::core::array<irr::newton::SIntersectionPoint>* internal_intersect_points;
irr::core::line3d<irr::f32> internal_line;

int time=0;
dFloat IrrNewt_NewtonWorldRayFilterCallback (
	const NewtonBody* body,
	const dFloat* hitNormal,
	int collisionID,
	void* userData,
	dFloat intersetParam) {
	time++;

	irr::newton::IBody* this_body=(irr::newton::IBody*)NewtonBodyGetUserData(body);
	irr::core::vector3df out=internal_line.start + intersetParam*(internal_line.end - internal_line.start);
	out *= NewtonToIrr;

	irr::newton::SIntersectionPoint this_point;

	this_point.parametric_value = intersetParam;
	this_point.body    = this_body;
	this_point.point   = out;
	this_point.normals = irr::newton::Hidden::Vector3dfFromArray( const_cast<irr::f32*>(hitNormal) );

	internal_intersect_points->push_back(this_point);
	return 1.0;
}

unsigned IrrNewt_NewtonWorldRayPrefilterCallback(
	const NewtonBody* body,
	const NewtonCollision* collision,
	void* userData) {

	return 1;
}

void irr::newton::ICollisionManager::getCollisionPoints(
		irr::core::line3d<irr::f32> line,
		irr::core::array<SIntersectionPoint>* intersect_points) {

	line.start*=IrrToNewton;
	line.end*=IrrToNewton;

	internal_intersect_points=intersect_points;
	internal_line=line;

	int user_data=1;

	float line_start_array[3],line_end_array[3];
	irr::newton::Hidden::Vector3dfFillArray(line.start,line_start_array);
	irr::newton::Hidden::Vector3dfFillArray(line.end,line_end_array);

	NewtonWorldRayCast(
		this->n_world,
		line_start_array,
		line_end_array,
		IrrNewt_NewtonWorldRayFilterCallback,
		(void*)&user_data,//user data
		IrrNewt_NewtonWorldRayPrefilterCallback//prefilter
		);

	return;
}

//----------------------------------------------------------------------
//-----------------------------------------------------------------------

irr::newton::SIntersectionPoint* getfirstpoint_internal_intersect_point;
irr::core::line3d<irr::f32> getfirstpoint_internal_line;

dFloat IrrNewt_NewtonFirstCollisionPointWorldRayFilterCallback (
	const NewtonBody* body,
	const dFloat* hitNormal,
	int collisionID,
	void* userData,
	dFloat intersetParam) {

	irr::newton::IBody* this_body=(irr::newton::IBody*)NewtonBodyGetUserData(body);
	irr::core::vector3df out=getfirstpoint_internal_line.start + intersetParam*(getfirstpoint_internal_line.end - getfirstpoint_internal_line.start);
	out*=NewtonToIrr;

	getfirstpoint_internal_intersect_point->parametric_value = intersetParam;
	getfirstpoint_internal_intersect_point->body             = this_body;
	getfirstpoint_internal_intersect_point->point            = out;
	getfirstpoint_internal_intersect_point->normals = irr::newton::Hidden::Vector3dfFromArray( const_cast<irr::f32*>(hitNormal) );

	return 0;
}

unsigned IrrNewtFirstCollisionPoint_NewtonWorldRayPrefilterCallback(
	const NewtonBody* body,
	const NewtonCollision* collision,
	void* userData) {
	return 1;
}

irr::newton::SIntersectionPoint irr::newton::ICollisionManager::getCollisionFirstPoint(
		irr::core::line3d<irr::f32> line) {

	irr::newton::SIntersectionPoint intersection_point;
	intersection_point.body=NULL;
	intersection_point.parametric_value=0.0f;

	line.start *= IrrToNewton;
	line.end   *= IrrToNewton;

	getfirstpoint_internal_intersect_point=&intersection_point;
	getfirstpoint_internal_line=line;

	int user_data=1;

	float line_start_array[3],line_end_array[3];
	irr::newton::Hidden::Vector3dfFillArray(line.start,line_start_array);
	irr::newton::Hidden::Vector3dfFillArray(line.end,line_end_array);

	NewtonWorldRayCast(
		this->n_world,
		line_start_array,
		line_end_array,
		IrrNewt_NewtonFirstCollisionPointWorldRayFilterCallback,
		(void*)&user_data,//user data
		IrrNewtFirstCollisionPoint_NewtonWorldRayPrefilterCallback//prefilter
		);

	return intersection_point;
}

//--------getFirstCollisionPointEx

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

static irr::newton::SIntersectionPoint intersect_point_ex;
static int times = 0;

dFloat IrrNewt_FirstCollisionPointExFilterCallback (
	const NewtonBody* body,
	const dFloat* hitNormal,
	int collisionID,
	void* userData,
	dFloat intersetParam) {

	times++;

	if( intersetParam <= intersect_point_ex.parametric_value ) {

		irr::newton::IBody* this_body = (irr::newton::IBody*)NewtonBodyGetUserData(body);

		intersect_point_ex.parametric_value = intersetParam;
		intersect_point_ex.body             = this_body;
		intersect_point_ex.normals          = irr::newton::Hidden::Vector3dfFromArray( const_cast<irr::f32*>(hitNormal) );

	}//	if(intersetParam<internal_intersect_point.parametric_value) {

	return intersetParam;
}

unsigned IrrNewt_FirstCollisionPointExPrefilterCallback(
	const NewtonBody* body,
	const NewtonCollision* collision,
	void* userData) {

	return 1;
}

irr::newton::SIntersectionPoint irr::newton::ICollisionManager::getCollisionFirstPointEx(
		 irr::core::line3d<irr::f32> line) {

	line.start *= IrrToNewton;
	line.end   *= IrrToNewton;

	intersect_point_ex.parametric_value = 1.0f;
	intersect_point_ex.body  = NULL;
	intersect_point_ex.point = core::vector3df();

	int user_data=1;

	float line_start_array[3],line_end_array[3];
	irr::newton::Hidden::Vector3dfFillArray( line.start, line_start_array);
	irr::newton::Hidden::Vector3dfFillArray( line.end  , line_end_array);

	NewtonWorldRayCast(
		this->n_world,
		line_start_array,
		line_end_array,
		IrrNewt_FirstCollisionPointExFilterCallback,
		NULL,  //user data
		IrrNewt_FirstCollisionPointExPrefilterCallback  //prefilter
		);

	//calculate out point
	if(intersect_point_ex.body != NULL) {
		irr::core::vector3df out = line.start + intersect_point_ex.parametric_value * (line.end - line.start);
		out *= NewtonToIrr;
		intersect_point_ex.point = out;
	}

	return intersect_point_ex;
}








//--------------------getCollisionFirstPointEx with exluded
irr::newton::SIntersectionPoint internal_intersect_point_exclusion;
irr::core::array<NewtonBody*> excluded_body;

dFloat IrrNewt_FirstCollisionPointNewtonWorldRayFilterCallback_WithExclusion (
	const NewtonBody* body,
	const dFloat* hitNormal,
	int collisionID,
	void* userData,
	dFloat intersetParam) {

	if(intersetParam<internal_intersect_point_exclusion.parametric_value) {

		//if the body is not excluded
		if(excluded_body.binary_search(const_cast<NewtonBody*>(body))==-1) {

			irr::newton::IBody* this_body=(irr::newton::IBody*)NewtonBodyGetUserData(body);

			internal_intersect_point_exclusion.parametric_value = intersetParam;
			internal_intersect_point_exclusion.body             = this_body;
			internal_intersect_point_exclusion.normals          = irr::newton::Hidden::Vector3dfFromArray( const_cast<irr::f32*>(hitNormal) );

		}

	}//	if(intersetParam<internal_intersect_point.parametric_value) {

	return intersetParam;
}

unsigned IrrNewt_FirstCollisionPointNewtonWorldRayPrefilterCallback_WithExclusion(
	const NewtonBody* body,
	const NewtonCollision* collision,
	void* userData) {

	return 1;
}

irr::newton::SIntersectionPoint irr::newton::ICollisionManager::getCollisionFirstPointEx(
														 irr::core::line3d<irr::f32> line,
														 irr::core::array<NewtonBody*> excluded) {

	line.start*=IrrToNewton;
	line.end*=IrrToNewton;

	internal_intersect_point_exclusion.parametric_value = 1.0f;
	internal_intersect_point_exclusion.body = NULL;

	int user_data=1;

	float line_start_array[3],line_end_array[3];
	irr::newton::Hidden::Vector3dfFillArray(line.start,line_start_array);
	irr::newton::Hidden::Vector3dfFillArray(line.end,line_end_array);

	//build excluded body array
	//for(int i=0;i<excluded.size();i++)
		//excluded_body.push_back(excluded[i]->getNewtonBody());

    excluded_body = excluded;


	NewtonWorldRayCast(
		this->n_world,
		line_start_array,
		line_end_array,
		IrrNewt_FirstCollisionPointNewtonWorldRayFilterCallback_WithExclusion,
		(void*)&user_data,//user data
		IrrNewt_FirstCollisionPointNewtonWorldRayPrefilterCallback_WithExclusion//prefilter
		);

	//calculate out point
	if(internal_intersect_point_exclusion.body!=NULL) {
		irr::core::vector3df out=line.start + internal_intersect_point_exclusion.parametric_value*(line.end - line.start);

    //internal_intersect_point_exclusion.body->getNewtonMatrix().transformVect(out);

		out*=NewtonToIrr;

		internal_intersect_point_exclusion.point=out;


	}

	return internal_intersect_point_exclusion;
}








//-------------------------------------------------------------
//----------------------------CLOSEST POINT-------------------------------------

bool irr::newton::ICollisionManager::getClosestPoint(
													 irr::newton::IBody* body_a,
													 irr::newton::IBody* body_b,
													 core::vector3df& contactA,
													 core::vector3df& contactB,
													 core::vector3df& normal) {

	irr::f32 contactA_arr[3];
	irr::f32 contactB_arr[3];
	irr::f32 normal_arr[3];


	bool value=(bool)NewtonCollisionClosestPoint(
		this->n_world, //world
		body_a->getNewtonCollision(), //first collision
		GetMatrixPointer(body_a->getMatrixNewton()), //first offset matrix
		body_b->getNewtonCollision(), //second collision
		GetMatrixPointer(body_b->getMatrixNewton()), //second offset matrix
		contactA_arr, //cantact a
		contactB_arr, //contact b
		normal_arr, //normal,
		0
		);

	//return
	contactA=irr::newton::Hidden::Vector3dfFromArray(contactA_arr);
	contactB=irr::newton::Hidden::Vector3dfFromArray(contactB_arr);
	normal=irr::newton::Hidden::Vector3dfFromArray(normal_arr);

	//convert in irrlicht unit
	contactA*=NewtonToIrr;
	contactB*=NewtonToIrr;
	normal*=NewtonToIrr;

	return value;
}

bool irr::newton::ICollisionManager::getClosestPoint(
	irr::newton::IBody* body,
	irr::core::vector3df point,
	irr::core::vector3df& contact,
	irr::core::vector3df& normal) {

	irr::f32 point_arr[3];
	irr::f32 contact_arr[3];
	irr::f32 normal_arr[3];

	point*=IrrToNewton;
	irr::newton::Hidden::Vector3dfFillArray(point,point_arr);

	bool value=static_cast<bool>(
		NewtonCollisionPointDistance(
		this->n_world,
		point_arr,
		body->getNewtonCollision(),
		GetMatrixPointer(body->getMatrixNewton()),
		contact_arr,
		normal_arr,
		0)
	);

	contact=irr::newton::Hidden::Vector3dfFromArray(contact_arr);
	normal=irr::newton::Hidden::Vector3dfFromArray(normal_arr);

	contact*=NewtonToIrr;
	normal*=NewtonToIrr;

	return value;
}

bool irr::newton::ICollisionManager::collide(
											 irr::newton::IBody* body_a,
											 irr::newton::IBody* body_b,
											 irr::u32 maxSize,
											 irr::core::array<irr::core::vector3df>& contacts,
											 irr::core::array<irr::core::vector3df>& normals,
											 irr::core::array<irr::f32>& penetrations
											 ) {

	float* contacts_arr=new float[maxSize*3];
	float* normals_arr=new float[maxSize*3];
	float* penetrations_arr=new float[maxSize];

	int num=NewtonCollisionCollide(
		this->n_world,
		maxSize,
		body_a->getNewtonCollision(),
		GetMatrixPointer(body_a->getMatrixNewton()),
		body_b->getNewtonCollision(),
		GetMatrixPointer(body_b->getMatrixNewton()),
		contacts_arr,
		normals_arr,
		penetrations_arr,
		0
		);

	//clear the vectors
	contacts.clear();
	normals.clear();
	penetrations.clear();

	if(num==0) return false;

	//put objects in arrays
	irr::u32 i;
	for(i=0;i<num;i+=3) {

		//contacts
		core::vector3df temp;
		temp.X=contacts_arr[i+0];
		temp.Y=contacts_arr[i+1];
		temp.Z=contacts_arr[i+2];
		temp*=NewtonToIrr;
		contacts.push_back(temp);

		//normals
		temp.X=normals_arr[i+0];
		temp.Y=normals_arr[i+1];
		temp.Z=normals_arr[i+2];
		temp*=NewtonToIrr;
		normals.push_back(temp);

	}

	//penetrations
	for(i=0;i<num;i++) {

		//penetrations
		penetrations.push_back(penetrations_arr[i]);

	}

	//delete all
	delete [] contacts_arr;
	delete [] normals_arr;
	delete [] penetrations_arr;

	//return
	return true;
}


