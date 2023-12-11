#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include "../Outils.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    class Physique;
    class PhysiqueObjet;
    class Constraint
    {
        public:
            Constraint(Physique*phy);
            ~Constraint();

            void creatMotor(PhysiqueObjet *obj, irr::core::vector3df axis, irr::core::vector3df pivot = irr::core::vector3df(0,0,0), float velocity = 1.f, float maxVelocity = 1.f, bool UserNoModif = true, bool userStop = false);

            PhysiqueObjet *getObjet1(){ return Obj1; }
            PhysiqueObjet *getObjet2(){ return Obj2; }

            OBJECT_TYPE getType(){ return type; }
            btTypedConstraint *getBulletConstraint(){ return kst; }
            btTypedConstraintType getConstraintType(){ return bulletType; }
        protected:
            OBJECT_TYPE type;
            btTypedConstraint *kst;
            btTypedConstraintType bulletType;
            PhysiqueObjet *truc;
        private:
            Physique *phy;
            PhysiqueObjet *Obj1, *Obj2;
    };
}
#endif // _CONSTRAINT_H_
