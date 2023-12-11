#include "Constraint.h"
#include "../PhysiqueObject/PhysiqueObject.h"
#include "../Physique.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    Constraint::Constraint(Physique*ph) : phy(ph), type(GamePhysique::OPT_NULL){}
    Constraint::~Constraint(){ phy->getWorldPhysic()->removeConstraint(kst); delete kst; }

    void Constraint::creatMotor(PhysiqueObjet *obj, irr::core::vector3df axis, irr::core::vector3df pivot, float velocity, float maxVelocity, bool UserNoModif, bool userStop)
    {
        Obj1 = obj;        Obj2 = NULL;
            btVector3 axe = Outils::irrToPhy(axis);
            btHingeConstraint* hinge  = new btHingeConstraint(*Obj1->node,Outils::irrToPhy(pivot),axe);
            kst = hinge;
            hinge->enableAngularMotor(UserNoModif,velocity,maxVelocity);
        phy->getWorldPhysic()->addConstraint(kst,userStop);
    }
}
