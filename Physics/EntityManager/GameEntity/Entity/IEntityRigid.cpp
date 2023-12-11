#include "IEntityRigid.h"
//! the licence is on EntityManager.h

namespace GameEntity
{
    IEntityRigid::IEntityRigid(irr::IrrlichtDevice*Device,GamePhysique::Physique*World,GamePhysique::OBJECT Sobj,irr::scene::IMeshSceneNode*mesh,float Mass,int id,bool gdb)
                 : mass(Mass), Node(mesh)
    {
        setID(id);       debug = gdb;               Obj = Sobj;
        device = Device; dynamicsWorld = World;     smgr = device->getSceneManager();
        Node->setAutomaticCulling(irr::scene::EAC_BOX);
        obj = new GamePhysique::PhysiqueObjet(dynamicsWorld,Node,mass,Sobj.Forme,Sobj.Type);
    }
    IEntityRigid::IEntityRigid(irr::IrrlichtDevice*Device,GamePhysique::Physique*World,GamePhysique::OBJECT Sobj,irr::scene::IMesh*mesh,float Mass,int id,bool gdb)
                 : mass(Mass)
    {
        setID(id);       debug = gdb;               Obj = Sobj;
        device = Device; dynamicsWorld = World;     smgr = device->getSceneManager();
        Node->setAutomaticCulling(irr::scene::EAC_BOX);
        obj = new GamePhysique::PhysiqueObjet(dynamicsWorld,Node,mass,Sobj.Forme,Sobj.Type);
    }
    IEntityRigid::IEntityRigid(irr::IrrlichtDevice*Device,GamePhysique::Physique*World,GamePhysique::OBJECT Sobj,float Mass,int id,bool gdb) : mass(Mass)
    {
        setID(id);          debug = gdb;               Obj = Sobj;
        device = Device;    dynamicsWorld = World;     smgr = device->getSceneManager();
        if(Sobj.Forme == GamePhysique::OFM_CUBE)       Node = smgr->addCubeSceneNode();
        else if(Sobj.Forme == GamePhysique::OFM_SPHERE)Node = smgr->addSphereSceneNode();
        Node->setAutomaticCulling(irr::scene::EAC_BOX);
        obj = new GamePhysique::PhysiqueObjet(dynamicsWorld,Node,mass,Sobj.Forme,Sobj.Type);
    }
    IEntityRigid::~IEntityRigid()
    {
        clearAttribute();
    }
    void IEntityRigid::draw()
    {
        //obj->drawNode();
    }
}
