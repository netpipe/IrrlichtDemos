#include "PhysiqueObject.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    PhysiqueObjet::PhysiqueObjet(Physique *world,irr::scene::IMeshSceneNode*mh,float Mass,OBJECT_FORME form,OBJECT_TYPE tp) : phy(world), mesh(mh->getMesh()), Forme(form), Type(tp)
    {
        Node = mh;
        btTransform Transform;
            Transform.setIdentity();
            Transform.setOrigin(btVector3(0,0,0));
        btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
        formCollide = new CollisionForm(world->getDevice());
            formCollide->makeRigid(mh,Mass,Forme);
        node = new btRigidBody(Mass,MotionState,formCollide->getShape(),formCollide->getLocalInertia());
            node->setUserPointer((void*)(this));
        setType(tp);
        phy->getWorldPhysic()->addRigidBody(node);
        Kst = NULL;
    }
    PhysiqueObjet::~PhysiqueObjet()
    {
        phy->getWorldPhysic()->removeRigidBody(node);
        delete formCollide;
		delete node->getMotionState();
		delete node;
    }

    bool PhysiqueObjet::isVisible()
    {
        //if() return true;
        return false;
    }

    void PhysiqueObjet::drawNode()
    {
        if(Node->isVisible())
        {
            Node->setPosition(getPosition());
            Node->setRotation(getRotation());
        }
    }

    void PhysiqueObjet::setMass(float mass)
    {
        btVector3 LocalInertia(0,0,0);
        node->getCollisionShape()->calculateLocalInertia(mass,LocalInertia);
        node->setMassProps(mass,LocalInertia);
    }
    void PhysiqueObjet::setTransform(irr::core::vector3df ps,irr::core::vector3df rt)
    {
        btQuaternion QuaternionRotation(rt.Y,rt.X,rt.Z);
		btTransform Transform;
            Transform.setIdentity();
            Transform.setRotation(QuaternionRotation);
            Transform.setOrigin(Outils::irrToPhy(ps));
            node->setWorldTransform(Transform);
        Node->setPosition(ps);
        Node->setRotation(rt);
    }
    void PhysiqueObjet::setScale(irr::core::vector3df sc)
    {
        node->getCollisionShape()->setLocalScaling(Outils::irrToPhy(sc));
        node->setCollisionShape(node->getCollisionShape());
        Node->setScale(sc);
    }
    void PhysiqueObjet::setType(OBJECT_TYPE tp)
    {
        Type = tp;
        if(tp == OTP_NORMAL)
            node->setCollisionFlags( btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
        else if(tp == OTP_STATIC)
            node->setCollisionFlags( btCollisionObject::CF_STATIC_OBJECT );
        else if(tp == OTP_CAMERA)
            node->setCollisionFlags( (btCollisionObject::CollisionFlags)0 );
        else
            node->setCollisionFlags( btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
    }
}
