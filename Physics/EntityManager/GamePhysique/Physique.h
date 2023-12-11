#ifndef _GAME_PHYSIQUE_H_
#define _GAME_PHYSIQUE_H_

#include "PhysiqueObject/CollisionForm.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    class Physique
    {
        public:
            Physique(irr::IrrlichtDevice*);//Init the Physics engine
            virtual ~Physique();//clear physique

            btDiscreteDynamicsWorld*getWorldPhysic(){ return dynamicsWorld; }
            irr::IrrlichtDevice*getDevice()         { return device; }

            enum RenderTiming { RTG_LOW,RTG_MEDIUM,RTG_FAST,RTG_ULTRA_FAST };

            //PhysiqueObjet* checkCollide();
            void drawAll(float,int precision = 1);
            void drawAll(RenderTiming rd = RTG_MEDIUM)
            {
                if(rd == RTG_LOW)        drawAll(1.f/27.f);
                if(rd == RTG_MEDIUM)     drawAll(1.f/38.f);
                if(rd == RTG_FAST)       drawAll(1.f/49.f);
                if(rd == RTG_ULTRA_FAST) drawAll(1.f/60.f);
            }
        protected:
            Physique(){}
            irr::u32 TimeStamp;
            irr::IrrlichtDevice *device;
            btSoftRigidDynamicsWorld *dynamicsWorld;
        private:
            btDefaultCollisionConfiguration *CollisionConfiguration;
            btCollisionDispatcher *dispatcher;
            btBroadphaseInterface *broadphase;
            btConstraintSolver *solver;
    };
}
#endif // GAMEPHYSIQUE_H
