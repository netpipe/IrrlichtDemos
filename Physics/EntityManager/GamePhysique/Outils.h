#ifndef _GAME_PHYSIQUE_OUTILS_
#define _GAME_PHYSIQUE_OUTILS_

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBody.h"

#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpa2.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btConvexHull.h"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>//heigt map collision algo

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067
#define Gravity -9.41
//! the licence is on EntityManager.h

struct OctTree
{
    irr::scene::IMesh *mesh;
    irr::scene::ISceneNode *Node;
};
namespace GamePhysique
{
    enum OBJECT_FORME   { OFM_CUBE,   OFM_SPHERE, OFM_TRIMESH, OFM_HEIGHTMAP };
    enum OBJECT_TYPE    { OTP_STATIC, OTP_NORMAL, OTP_HUMAN, OTP_CAMERA, OPT_NULL };
    enum OBJECT_MATERIAL{ OMT_NORMAL, OMT_SOFT,   OMT_GHOST };
    struct OBJECT
    {
        OBJECT(){ Forme = OFM_CUBE; Type = OTP_NORMAL;  Material = OMT_NORMAL; }
            OBJECT(OBJECT_FORME forme){ Forme = forme; Type = OTP_NORMAL;  Material = OMT_NORMAL; }
            OBJECT(OBJECT_TYPE type){ Forme = OFM_CUBE; Type = type;  Material = OMT_NORMAL; }
            OBJECT(OBJECT_MATERIAL mat){ Forme = OFM_CUBE; Type = OTP_NORMAL;  Material = mat; }
                OBJECT(OBJECT_FORME forme,OBJECT_TYPE type){ Forme = forme; Type = type;  Material = OMT_NORMAL; }
                OBJECT(OBJECT_TYPE type,OBJECT_MATERIAL mat){ Forme = OFM_CUBE; Type = type;  Material = mat; }
                OBJECT(OBJECT_MATERIAL mat,OBJECT_FORME forme){ Forme = forme; Type = OTP_NORMAL;  Material = mat; }
                    OBJECT(OBJECT_MATERIAL mat,OBJECT_FORME forme,OBJECT_TYPE type){ Forme = forme; Type = type;  Material = mat; }
        bool operator==(OBJECT obj){  if( obj.Forme == Forme && obj.Type == Type && obj.Material == Material) return true;     return false;   }
        void operator=(OBJECT obj) {  Forme = obj.Forme;   Type = obj.Type;    Material = obj.Material;    }
        OBJECT_FORME Forme;
        OBJECT_TYPE Type;
        OBJECT_MATERIAL Material;
    };
    enum OBJECT_CONSTRAINT_TYPE{ OCT_NONE, OCT_POINT, OCT_2POINT, OCT_SLIDER, OCT_RAYCAST, OCT_HINGE };
    namespace Outils
    {
        struct MeshData// soft body !!
        {
            irr::u16              *irr_indices;
            irr::video::S3DVertex *irr_vertices;
            int                   *tmp_indices;
            btScalar              *tmp_vertices;
            int                   *soft_indices;
            btSoftBody::Node     **soft_nodes;
            int                    indexCount;
            int                    irr_vertexCount;
            int                    temp_vertexCount;
            int                    soft_nodeCount;
        };
        inline irr::core::vector3df phyToIrr(btVector3 vec){ return irr::core::vector3df(vec.getX(),vec.getY(),vec.getZ()); }
        inline btVector3 irrToPhy(irr::core::vector3df vec){ return btVector3(vec.X,vec.Y,vec.Z); }
        inline irr::core::vector3df vectorToDegre(irr::core::vector3df vec)
        {
            irr::core::matrix4 mat;
            mat.setRotationDegrees(vec);
            return irr::core::vector3df(mat[8],mat[9],mat[10]);
        }
        inline void MatrixToEuler(const btScalar mat[16], btScalar euler[3])
        {
            if (-mat[6] <= -1.0f)    euler[1] = -1.570796f; // -pi/2
            else if (-mat[6] >= 1.0) euler[1] = 1.570796f; // pi/2
            else                     euler[1] = asin(-mat[6]);
            if (-mat[6] > 0.9999f){  euler[2] = 0.0f;                    euler[0] = atan2(mat[2], mat[10]);  }
            else             {  euler[0] = atan2(mat[2], mat[10]);  euler[2] = atan2(mat[4], mat[5]);   }
        }
        inline irr::core::vector3df QuaternionToEuler(const btQuaternion &TQuat)
        {
            irr::core::vector3df result;
                result.X = (atan2f(2.0f*(TQuat.getY()*TQuat.getZ()+TQuat.getX()*TQuat.getW()),-(TQuat.getX()*TQuat.getX())-(TQuat.getY()*TQuat.getY())+(TQuat.getZ()*TQuat.getZ())+(TQuat.getW()*TQuat.getW())))*irr::core::RADTODEG;
                result.Y = (asinf(-2.0f*(TQuat.getX()*TQuat.getZ()-TQuat.getY()*TQuat.getW())))*irr::core::RADTODEG;
                result.Z = (atan2f(2.0f*(TQuat.getX()*TQuat.getY()+TQuat.getZ()*TQuat.getW()), (TQuat.getX()*TQuat.getX())-(TQuat.getY()*TQuat.getY())-(TQuat.getZ()*TQuat.getZ())+(TQuat.getW()*TQuat.getW())))*irr::core::RADTODEG;
            return result;
        }
    }
}
#endif
