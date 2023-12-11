#ifndef CIrrBP_CONVERT_H_
#define CIrrBP_CONVERT_H_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>

#include "actions/wheel.h"
namespace bullet
{
	inline static btVector3 irrVectorToBulletVector(const irr::core::vector3df & toConvert)
	{
		return btVector3(toConvert.X,toConvert.Y,toConvert.Z);
	}
	inline static irr::core::vector3df bulletVectorToIrrVector(const btVector3 & toConvert)
	{
		return irr::core::vector3df(toConvert.x(),toConvert.y(),toConvert.z());
	}
	inline static void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler)
	{
	   btScalar W = TQuat.getW();
	   btScalar X = TQuat.getX();
	   btScalar Y = TQuat.getY();
	   btScalar Z = TQuat.getZ();
	   float WSquared = W * W;
	   float XSquared = X * X;
	   float YSquared = Y * Y;
	   float ZSquared = Z * Z;
	   TEuler.setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
	   TEuler.setY(asinf(-2.0f * (X * Z - Y * W)));
	   TEuler.setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
	   TEuler *= irr::core::RADTODEG;
	}

	static irr::core::vector3df QuaternionToIrrEuler(const btQuaternion &TQuat)
	{
		btVector3 bulletEuler;
		QuaternionToEuler(TQuat,bulletEuler);
		return bullet::bulletVectorToIrrVector(bulletEuler);
	}

	inline static irr::core::vector3df bulletTransformToIrrRotation(const btTransform & tr)
	{
		irr::core::matrix4 mat;
		irr::f32 * ptr;
		ptr = mat.pointer();
		tr.getOpenGLMatrix(ptr);
		return mat.getRotationDegrees();
	}
	inline static btTransform irrRotationToBulletTransform(const irr::core::vector3df & rotation)
	{
		irr::core::matrix4 mat;
		mat.setRotationDegrees(rotation);
		btTransform tr;
		tr.setFromOpenGLMatrix(mat.pointer());

		return tr;
	}

	static btTransform getTransformFromIrrlichtNode(const irr::scene::ISceneNode * irrNode)
	{
		btTransform transf;
		transf.setIdentity();
		transf.setOrigin(bullet::irrVectorToBulletVector(irrNode->getPosition()));
		transf.getBasis().setEulerZYX(irrNode->getRotation().X * irr::core::DEGTORAD,irrNode->getRotation().Y* irr::core::DEGTORAD,irrNode->getRotation().Z* irr::core::DEGTORAD);
		return transf;
	}
	static btWheelInfoConstructionInfo & irrBPWheelICtoBulletIC(const irrBPWheelInfoConstructionInfo & tc)
	{
		btWheelInfoConstructionInfo ci;
		ci.m_chassisConnectionCS = bullet::irrVectorToBulletVector(tc.m_chassisConnectionCS);
		ci.m_wheelDirectionCS = bullet::irrVectorToBulletVector(tc.m_wheelDirectionCS);
		ci.m_wheelAxleCS = bullet::irrVectorToBulletVector(tc.m_wheelAxleCS);
		ci.m_suspensionRestLength = tc.m_suspensionRestLength;
		ci.m_maxSuspensionTravelCm = tc.m_maxSuspensionTravelCm;
		ci.m_wheelRadius = tc.m_wheelRadius;
		ci.m_suspensionStiffness = tc.m_suspensionStiffness;
		ci.m_wheelsDampingCompression = tc.m_wheelsDampingCompression;
		ci.m_wheelsDampingRelaxation = tc.m_wheelsDampingRelaxation;
		ci.m_frictionSlip = tc.m_frictionSlip;
		ci.m_maxSuspensionForce = tc.m_maxSuspensionForce;
		ci.m_bIsFrontWheel = tc.m_bIsFrontWheel;
		return ci;
		
	}	
	static btWheelInfo::RaycastInfo & irrBPWheelRCItoBulletWheelRCI(const irrBPWheelInfo::RaycastInfo & rci)
	{
		btWheelInfo::RaycastInfo ri;
		ri.m_contactNormalWS = bullet::irrVectorToBulletVector(rci.m_contactNormalWS);
		ri.m_contactPointWS = bullet::irrVectorToBulletVector(rci.m_contactPointWS);
		
		ri.m_suspensionLength = rci.m_suspensionLength;
		
		ri.m_hardPointWS = bullet::irrVectorToBulletVector(rci.m_hardPointWS);
		ri.m_wheelDirectionWS = bullet::irrVectorToBulletVector(rci.m_wheelDirectionWS);  
		ri.m_wheelAxleWS = bullet::irrVectorToBulletVector(rci.m_wheelAxleWS); 
		ri.m_isInContact = rci.m_isInContact;
		return ri;
	}
	
	static btWheelInfo & irrBPWheelInfotoBulletWheelInfo(const irrBPWheelInfo & info, const irrBPWheelInfoConstructionInfo & tc)
	{
		btWheelInfo wi(irrBPWheelICtoBulletIC(tc));
		wi.m_clippedInvContactDotSuspension = info.m_clippedInvContactDotSuspension;
		wi.m_suspensionRelativeVelocity= info.m_suspensionRelativeVelocity;
		wi.m_wheelsSuspensionForce = info.m_wheelsSuspensionForce;
		wi.m_skidInfo = info.m_skidInfo;
		return wi;
	}
	static void irrBPWheelInfotoBulletWheelInfo(const irrBPWheelInfo & info, btWheelInfo & btinfo)
	{
		btinfo.m_chassisConnectionPointCS = bullet::irrVectorToBulletVector(info.m_chassisConnectionPointCS);
		btinfo.m_wheelDirectionCS = bullet::irrVectorToBulletVector(info.m_wheelDirectionCS);
		btinfo.m_wheelAxleCS = bullet::irrVectorToBulletVector(info.m_wheelAxleCS);
		btinfo.m_suspensionRestLength1 = info.m_suspensionRestLength1;
		btinfo.m_maxSuspensionTravelCm = info.m_maxSuspensionTravelCm;
		btinfo.m_wheelsRadius = info.m_wheelsRadius;
		btinfo.m_suspensionStiffness = info.m_suspensionStiffness;
		btinfo.m_wheelsDampingCompression = info.m_wheelsDampingCompression;
		btinfo.m_wheelsDampingRelaxation = info.m_wheelsDampingRelaxation;
		btinfo.m_frictionSlip = info.m_frictionSlip;
		btinfo.m_maxSuspensionForce = info.m_maxSuspensionForce;
		btinfo.m_bIsFrontWheel = info.m_bIsFrontWheel;
		btinfo.m_clippedInvContactDotSuspension = info.m_clippedInvContactDotSuspension;
		btinfo.m_suspensionRelativeVelocity= info.m_suspensionRelativeVelocity;
		btinfo.m_wheelsSuspensionForce = info.m_wheelsSuspensionForce;
		btinfo.m_skidInfo = info.m_skidInfo;
	}
	static void bulletWheelInfotoirrBPWheelInfo(const btWheelInfo & btinfo, irrBPWheelInfo & info )
	{
		info.m_chassisConnectionPointCS = bullet::bulletVectorToIrrVector(btinfo.m_chassisConnectionPointCS);
		info.m_wheelDirectionCS = bullet::bulletVectorToIrrVector(btinfo.m_wheelDirectionCS);
		info.m_wheelAxleCS = bullet::bulletVectorToIrrVector(btinfo.m_wheelAxleCS);
		info.m_suspensionRestLength1 = btinfo.m_suspensionRestLength1;
		info.m_maxSuspensionTravelCm = btinfo.m_maxSuspensionTravelCm;
		info.m_wheelsRadius = btinfo.m_wheelsRadius;
		info.m_suspensionStiffness = btinfo.m_suspensionStiffness;
		info.m_wheelsDampingCompression = btinfo.m_wheelsDampingCompression;
		info.m_wheelsDampingRelaxation = btinfo.m_wheelsDampingRelaxation;
		info.m_frictionSlip = btinfo.m_frictionSlip;
		info.m_maxSuspensionForce = btinfo.m_maxSuspensionForce;
		info.m_bIsFrontWheel = btinfo.m_bIsFrontWheel;
		info.m_clippedInvContactDotSuspension = btinfo.m_clippedInvContactDotSuspension;
		info.m_suspensionRelativeVelocity= btinfo.m_suspensionRelativeVelocity;
		info.m_wheelsSuspensionForce = btinfo.m_wheelsSuspensionForce;
		info.m_skidInfo = btinfo.m_skidInfo;
	}

	static btTriangleMesh* irrMeshToBulletTriangleMesh(irr::scene::IMesh* pMesh,const irr::core::vector3df& scaling)
	{
	  btVector3 vertices[3];
	  irr::u32 i,j,k,index,numVertices,numIndices;
	  irr::u16* mb_indices;
	  btTriangleMesh *pTriMesh = new btTriangleMesh();
	  for (i=0; i<pMesh->getMeshBufferCount(); i++)
	  {
		irr::scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);
		if(mb->getVertexType()==irr::video::EVT_STANDARD)
		{
		  irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
		  mb_indices = mb->getIndices();
		  numVertices = mb->getVertexCount();
		  numIndices = mb->getIndexCount();
		  for(j=0;j<numIndices;j+=3)
		  {
			for (k=0;k<3;k++)
			{
			  index = mb_indices[j+k];
			  vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X, mb_vertices[index].Pos.Y*scaling.Y, mb_vertices[index].Pos.Z*scaling.Z);
			}

			pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
		  }
		}
		else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
		  irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
		  mb_indices = mb->getIndices();
		  numVertices = mb->getVertexCount();
		  numIndices = mb->getIndexCount();
		  for(j=0;j<numIndices;j+=3)
		  {
			for (k=0;k<3;k++)
			{
			  index = mb_indices[j+k];
			  vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X, mb_vertices[index].Pos.Y*scaling.Y, mb_vertices[index].Pos.Z*scaling.Z);
			}
			pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
		  }
		}
	  }
	  return pTriMesh;
	}



};

#endif