// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef RAFFAELE_MANCUSO_PHYSICS_IRRLICHT_LIB_HIDDEN_HPP_541684AGDVASDV684684EGDAGEGHHSJSDFCZAAWREGY545618
#define RAFFAELE_MANCUSO_PHYSICS_IRRLICHT_LIB_HIDDEN_HPP_541684AGDVASDV684684EGDAGEGHHSJSDFCZAAWREGY545618
#pragma once

#include <irrlicht.h>
#include <Newton.h>
#include "irrnewt_base.hpp"
#include "dll_exports.hpp"

namespace irr{
	namespace newton{

		class IBody;

		//!This namesapce contain internally functions that user would not need
		namespace Hidden{

			enum ERROR_CODE{
				NO_ERRORS,
				FILE_NOT_FOUND
			};

			void SetDevice(irr::IrrlichtDevice* device);
			irr::IrrlichtDevice* GetDevice();

			void NewtonDebugCollision(void* body, int vertexCount, const float* faceVertec, int id);
			void NewtonDebugBody(const NewtonBody* body);
			void NewtonSerialize (void* serializeHandle, const void* buffer, size_t size);
			void NewtonDeserialize (void* serializeHandle, void* buffer, size_t size);

			ERROR_CODE GetLastError();
			void SetLastError(ERROR_CODE error);

			//fill an array of 3 floats with a vector3df
			inline void Vector3dfFillArray(irr::core::vector3df vector, irr::f32* array);

			//inverse of the function above
			//create a vector3df object from an array of 3 floats
			inline irr::core::vector3df Vector3dfFromArray(irr::f32* array);

			//to destruc correct dynamic objects
			//abnd list opertaions
			IRRNEWT_API void AddObjectToDestroy(
				irr::newton::IrrNewt_Base* obj);
			void DestroyAll();
			void DestroyAllOnExit();
			void EraseFromDestroyList(irr::newton::IrrNewt_Base* obj);
			irr::core::array<irr::newton::IrrNewt_Base*>* GetObjsList();
			void ClearObjsList();

			//chronometer functions
			void StartChronometer();
			irr::core::stringc EndChronometer();

			//camera related
			inline core::vector3df correctCameraTarget(irr::scene::ICameraSceneNode* camera);

			//dynamic array operations
			template<class T> void PushArray(core::array<T>& A, core::array<T>& B) {

				for(irr::u32 i=0; i<B.size(); i++)
					A.push_back(B[i]);

			}

			//matrix operations
			irr::core::matrix4 getIrrOffsetMatrix(
				irr::scene::ISceneNode* parent,
				irr::scene::ISceneNode* child);

			inline irr::core::matrix4 AddMatrixNoScale(
				const irr::core::matrix4& matrix1,
				const irr::core::matrix4& matrix2);

			inline irr::f32* GetMatrixPointer(const core::matrix4& mat) {

				   //see irrtonewt.hpp file to select your irrlicht version

				   #ifdef COMPILE_WITH_1_3

				   //irrlicht 1.3 and mayor version implementation
				    return const_cast<f32*>(mat.pointer());

				    #else

				   //irrlicht 1.2 and less version implementation
				   return const_cast<f32*>(&mat.M[0]);

				   #endif

			}

			//material collision callbacks

			int NewtonMaterialCollisionContactBegin(
				const NewtonMaterial* material,
				const NewtonBody* body0,
				const NewtonBody* body1);

			int NewtonMaterialCollisionContactProcess(
				const NewtonMaterial* material,
				const NewtonContactsProcess* contact);

			void NewtonMaterialCollisionContactEnd(
				const NewtonMaterial* material);

			//body callbacks
			void defBodyLeaveWorldEvent(const NewtonBody* n_body, int);
			void defBodyForceAndTorqueCallback (const NewtonBody* n_body, float, int);
			void defBodyForceAndTorqueCallback(newton::IBody* body, const NewtonBody* n_body, int);
			void defBodyTransformCallback(const NewtonBody* body, const float* matrix, int);

			//character control callbacks
			void defCharacterControllerForceAndTorqueCallback(const NewtonBody* body, float, int);
			void defCharacterControllerTransformCallback(const NewtonBody* body, const float* matrix, int);

			//camera callbacks
      void defCameraBodyTransformCallback(const NewtonBody* body, const float* matrix, int);
			void defCameraCharacterControllerTransformCallback(const NewtonBody* body, const float* matrix, int);

			//vehicle callbacks
			void VehicleTireCallBack(const NewtonJoint* vehicle);

			//ragdoll callbacks
			//void defRagDollTransformCallback(const NewtonRagDollBone* bone);
			//void defRagDollForceCallback(const NewtonBody* body);

			//joints callbacks
			irr::u32 defJointHingeCallback(const NewtonJoint* hinge, NewtonHingeSliderUpdateDesc* desc);
			irr::u32 defJointSliderCallback(const NewtonJoint* slider, NewtonHingeSliderUpdateDesc* desc);

		}//hidden
	}//newton
}//irr


//---------------INLINE FUNCTIONS------------------------------

irr::core::vector3df irr::newton::Hidden::correctCameraTarget(irr::scene::ICameraSceneNode* camera) {

	camera->updateAbsolutePosition();

	irr::core::vector3df direction = core::vector3df(0,0,1);

	irr::core::matrix4 matrix;
	matrix.setRotationDegrees(core::vector3df(
		camera->getRotation().X,
		camera->getRotation().Y,
		0));

	//rotatevect
	matrix.rotateVect(direction);

	//direction.normalize();

	direction += camera->getPosition();
	camera->setTarget(direction);

	camera->updateAbsolutePosition();

	return direction;
}

void irr::newton::Hidden::Vector3dfFillArray(
		irr::core::vector3df vector,
		irr::f32* array) {

	array[0]=vector.X;
	array[1]=vector.Y;
	array[2]=vector.Z;
}

irr::core::vector3df irr::newton::Hidden::Vector3dfFromArray(
	 irr::f32* array) {

	return core::vector3df(
		array[0],
		array[1],
		array[2]
	);

}

//vec32<->vec64
inline irr::core::vector3d<irr::f64> Vec64FromVec32(irr::core::vector3df v) {
	return irr::core::vector3d<irr::f64>(v.X,v.Y,v.Z);
}

inline irr::core::vector3df Vec32FromVec64(irr::core::vector3d<irr::f64> v) {
	return irr::core::vector3df(v.X,v.Y,v.Z);
}
//end

inline irr::core::matrix4 irr::newton::Hidden::AddMatrixNoScale(
									const irr::core::matrix4& matrix1,
									const irr::core::matrix4& matrix2) {

	irr::core::matrix4 returned;

	returned.setTranslation( matrix1.getTranslation() + matrix2.getTranslation());

	core::vector3df rot1 =  matrix1.getRotationDegrees();
	core::vector3df rot2 =  matrix2.getRotationDegrees();

	returned.setRotationDegrees( matrix1.getRotationDegrees() + matrix2.getRotationDegrees() );

	core::vector3df rot_fin = returned.getRotationDegrees();

	return returned;

}

#endif
