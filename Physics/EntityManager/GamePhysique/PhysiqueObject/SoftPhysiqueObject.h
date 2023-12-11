#ifndef _SOFT_PHYSIQUE_OBJECT_H_
#define _SOFT_PHYSIQUE_OBJECT_H_

#include <map>
#include <list>
#include "../Outils.h"
#include "PhysiqueObject.h"
#include <iostream>
#include <vector>
//! the licence is on EntityManager.h

namespace GamePhysique
{
    class SoftPhysiqueObject : public PhysiqueObjet
    {
        public:
            SoftPhysiqueObject(Physique*phy,irr::scene::IMeshSceneNode*mh,float Mass = 0.1f,OBJECT_FORME fm = OFM_CUBE,OBJECT_TYPE tp = OTP_NORMAL);
            virtual ~SoftPhysiqueObject();
            virtual void drawNode();
        protected:
            SoftPhysiqueObject(){}
            void creatSpecialTrimesh();
        private:
            btSoftBody *SoftNode;
            irr::scene::IAnimatedMesh *Node;
            struct MeshData
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
            MeshData *SoftMesh;
    };
}
#endif // _SOFT_PHYSIQUE_OBJECT_H_
