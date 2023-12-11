#include "SoftPhysiqueObject.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    SoftPhysiqueObject::SoftPhysiqueObject(Physique*phy,irr::scene::IMeshSceneNode*mh,float Mass,OBJECT_FORME fm,OBJECT_TYPE tp)
    {
    }

    SoftPhysiqueObject::~SoftPhysiqueObject()
    {
    }
    void SoftPhysiqueObject::drawNode()
    {
//        for (int i=0; i<testMesh.indexCount; i++)
//        {
//            int irr_index = testMesh.irr_indices[i];
//            int soft_index = testMesh.soft_indices[i];
//            btSoftBody::Node* node = SoftMesh.soft_nodes[soft_index];
//            SoftMesh.irr_vertices[irr_index].Pos.X =  node->m_x.x();
//            SoftMesh.irr_vertices[irr_index].Pos.Y =  node->m_x.y();
//            SoftMesh.irr_vertices[irr_index].Pos.Z = -node->m_x.z();
//            SoftMesh.irr_vertices[irr_index].Normal.X =  node->m_n.x();
//            SoftMesh.irr_vertices[irr_index].Normal.Y =  node->m_n.y();
//            SoftMesh.irr_vertices[irr_index].Normal.Z = -node->m_n.z();
//        }
    }
}
