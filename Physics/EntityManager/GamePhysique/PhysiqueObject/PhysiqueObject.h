#ifndef _PHYSIQUE_OBJECT_H_
#define _PHYSIQUE_OBJECT_H_

#include "../Physique.h"
#include "../Constraint/Constraint.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    class PhysiqueObjet
    {
        public:
            PhysiqueObjet(Physique*phy,irr::scene::IMeshSceneNode*mh,float Mass = 0.1f,OBJECT_FORME fm = OFM_CUBE,OBJECT_TYPE tp = OTP_NORMAL);
            virtual ~PhysiqueObjet();

            void setTransform(irr::core::vector3df ps,irr::core::vector3df rt=irr::core::vector3df(0,0,0));

            void setType(OBJECT_TYPE tp = OTP_NORMAL);
            void setMass(float mass);
            void setScale(irr::core::vector3df sc);
            void setPosition(irr::core::vector3df ps){ setTransform(ps,getRotation()); }
            void setRotation(irr::core::vector3df rt){ setTransform(getPosition(),rt); }
            void setDeActivationTime(int time){ node->setDeactivationTime((btScalar)time); }

            bool isVisible();

            void setFriction(float fq){ node->setFriction((btScalar)fq); }
            void setRestitution(float rst)                   { node->setRestitution(((btScalar)rst)); }
            void setOrigine(irr::core::vector3df org)        { formCollide->setOrigine(node->getInvMass(),org); }
            void setAngularVelocity (irr::core::vector3df vl){ node->setAngularVelocity(Outils::irrToPhy(vl));  }
            void setLinearVelocity  (irr::core::vector3df vl){ node->setLinearVelocity (Outils::irrToPhy(vl));  }
            void applyCentralImpulse(irr::core::vector3df im){ node->applyCentralImpulse(Outils::irrToPhy(im)); }

            void addContraint(Constraint*kst){ Kst = kst; node->addConstraintRef(kst->getBulletConstraint()); }
            void removeContraint(Constraint*kst){ Kst = NULL; node->removeConstraintRef(kst->getBulletConstraint()); }
            Constraint*getConstraint(){ return Kst; }

            OBJECT_TYPE getType() { return Type; }
            float        getMass() { return node->getInvMass(); }
            irr::core::vector3df getScale   (){ return Outils::phyToIrr(node->getCollisionShape()->getLocalScaling()); }
            irr::core::vector3df getPosition(){ return irr::core::vector3df((irr::f32)node->getCenterOfMassPosition()[0],(irr::f32)node->getCenterOfMassPosition()[1],(irr::f32)node->getCenterOfMassPosition()[2]); }
            irr::core::vector3df getRotation(){ return Outils::QuaternionToEuler(node->getOrientation()); }

            float getRestitution                   (){ return node->getRestitution(); }
            float getFriction                      (){ return node->getFriction(); }
            irr::core::vector3df getAngularVelocity(){ return Outils::phyToIrr(node->getAngularVelocity()); }
            irr::core::vector3df getLinearVelocity (){ return Outils::phyToIrr(node->getLinearVelocity ()); }
            float getDeActivationTime              (){ return (float)node->getDeactivationTime(); }

            bool checkCollide(PhysiqueObjet*obj){ return node->checkCollideWithOverride(obj->node); }

            virtual irr::scene::ISceneNode*getSceneNode() { return Node; }

            virtual void drawNode();
        protected:
            Constraint *Kst;
            PhysiqueObjet(){}
            Physique *phy;
            PhyNode *node;
            CollisionForm *formCollide;
            irr::scene::IMesh *mesh;
        private:
            friend class Constraint;
            OBJECT_FORME Forme;
            OBJECT_TYPE Type;
            irr::scene::ISceneNode *Node;
    };
}
#endif
