#include "IEntitySoft.h"

namespace GameEntity
{
    IEntitySoft::IEntitySoft(irr::IrrlichtDevice*Device, btDiscreteDynamicsWorld*World, bool gdb)
    {
        debug = gdb;
        if(debug)
            printf("add Soft Entity\n");
        device = Device;
        dynamicsWorld = World;
        softWorld = (btSoftRigidDynamicsWorld*)dynamicsWorld;
        smgr = device->getSceneManager();

        mesh = smgr->addCubeSceneNode(1.0f)->getMesh();
        Node = smgr->addCubeSceneNode(1.0f);
        Node->setScale(scale);
        Node->setMaterialFlag(irr::video::EMF_LIGHTING, 1);
        Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
        Node->setMaterialTexture(0,Device->getVideoDriver()->getTexture("static.jpg"));
        physique();
    }
    IEntitySoft::~IEntitySoft()
    {
        if(debug)
            printf("remove Soft Entity\n");
        Node->remove();
        clearAttribute();
        softWorld->removeSoftBody(TObject);
		delete TObject->getCollisionShape();
		delete TObject;
    }
    void IEntitySoft::physique()
    {
        Node->setMaterialFlag(irr::video::EMF_LIGHTING,1);
        ///////////////////////////////////////////////////////////////
        creatShape();

		const btScalar	s=8;
        TObject = btSoftBodyHelpers::CreatePatch(Info,btVector3(-s,0,-s),btVector3(+s,0,-s),btVector3(-s,0,+s),btVector3(+s,0,+s),15,15,1+2+4+8,true);
        TObject->setCollisionShape(Shape);
        TObject->setUserPointer((void *)(Node));
        switch(NOD)
        {
            case ENT_STATIC :
                TObject->setCollisionFlags( TObject->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK  );
            break;
            case ENT_MOVE :
                TObject->setCollisionFlags( TObject->getCollisionFlags() );
            break;
            case ENT_HUMAN :
                TObject->setCollisionFlags( TObject->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK  );
            break;
        }
        softWorld->addSoftBody(TObject);
        setPosition(irr::core::vector3df(0,0,0));
    }
    void IEntitySoft::setScale(irr::core::vector3df sc)
    {
        scale = sc;
        Node->setScale(sc);
		btTransform Transform;
        Transform.setIdentity();
        TObject->setWorldTransform(Transform);
    }
    void IEntitySoft::setPosition(irr::core::vector3df ps)
    {
        Node->setPosition(ps);
		btTransform Transform;
        Transform.setIdentity();
        Transform.setOrigin(btVector3(ps.X,ps.Y,ps.Z));
        TObject->setWorldTransform(Transform);
    }
    void IEntitySoft::setRotation(irr::core::vector3df rt)
    {
        Node->setRotation(rt);
		btTransform Transform;
        btQuaternion QuaternionRotation(rt.Y,rt.X,rt.Z);
        Transform.setIdentity();
        Transform.setRotation(QuaternionRotation);
        TObject->setWorldTransform(Transform);
    }
    void IEntitySoft::creatShape()
    {
        irr::core::vector3df ht = Node->getScale()*4.8;
        switch(FOM)
        {
            case ETF_SPHERE :
                Shape = new btSphereShape(btScalar((ht.X+ht.Y+ht.Z)/3));
            break;
            case ETF_CUBE :
                Shape = new btBoxShape(btVector3(ht.X,ht.Y,ht.Z));
            break;
            default :
                btTriangleMesh *CollisionMesh = new btTriangleMesh();
                meshToPhysic(mesh,CollisionMesh,ht);
                Shape = new btBvhTriangleMeshShape(CollisionMesh, true);
            break;
        }
    }
    void IEntitySoft::draw()
    {
//        Node->setPosition(irr::core::vector3df((irr::f32)TObject->getCenterOfMassPosition()[0],(irr::f32)TObject->getCenterOfMassPosition()[1],(irr::f32)TObject->getCenterOfMassPosition()[2]));
//        btVector3 EulerRotation = QuaternionToEuler(TObject->getOrientation());
//        Node->setRotation(irr::core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
    }
}
