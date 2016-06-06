#ifndef _TERRAIN_PHYSIQUE_H_
#define _TERRAIN_PHYSIQUE_H_

#include "../Outils.h"
#include "../Physique.h"
#include "../Constraint/Constraint.h"
//#include "../../GameEngine/TerrainEngine/TerrainEngine.h"

namespace GamePhysique
{
    class TerrainPhysique
    {
        public:
            TerrainPhysique(Physique*phy,void*data,int height, int width);
            virtual ~TerrainPhysique();

            void setScale(irr::core::vector3df sc);
            //void setPosition(irr::core::vector3df ps){ setTransform(ps,getRotation()); }
            //void setRotation(irr::core::vector3df rt){ setTransform(getPosition(),rt); }
            void setDeActivationTime(int time){ node->setDeactivationTime((btScalar)time); }
            void setFriction(float fq){ node->setFriction((btScalar)fq); }

            void addContraint(Constraint*kst){ Kst = kst; node->addConstraintRef(kst->getBulletConstraint()); }
            void removeContraint(Constraint*kst){ Kst = NULL; node->removeConstraintRef(kst->getBulletConstraint()); }
            Constraint*getConstraint(){ return Kst; }

            OBJECT_TYPE getType()   { return GamePhysique::OTP_STATIC;    }
            OBJECT_FORME getForme() { return GamePhysique::OFM_HEIGHTMAP; }

            irr::core::vector3df getScale   (){ return Outils::phyToIrr(node->getCollisionShape()->getLocalScaling()); }
            irr::core::vector3df getPosition(){ return irr::core::vector3df((irr::f32)node->getCenterOfMassPosition()[0],(irr::f32)node->getCenterOfMassPosition()[1],(irr::f32)node->getCenterOfMassPosition()[2]); }
            irr::core::vector3df getRotation(){ return Outils::QuaternionToEuler(node->getOrientation()); }

            float getFriction()                  { return node->getFriction(); }
            float getDeActivationTime()          { return (float)node->getDeactivationTime(); }
            //bool checkCollide(PhysiqueObjet*obj) { return node->checkCollideWithOverride(obj->node); }

            void drawNode();
        protected:
            Constraint *Kst;
            TerrainPhysique(){}
            Physique *phy;
            PhyNode *node;
            CollisionForm *formCollide;
            irr::scene::IMesh *mesh;
        private:
            friend class Constraint;
            irr::scene::ISceneNode *Node;
    };
}
#endif // _TERRAIN_PHYSIQUE_H_
