#ifndef CSTATE_H
#define CSTATE_H

#include <irrlicht.h>

class CStateManager;

class CState
{
    public:
        /** Default constructor */
        CState(CStateManager* manager);
        /** Default destructor */
        virtual ~CState();

        virtual void update(irr::f32 timediff){}

        void remove(void);
    protected:
        CStateManager* Manager;
        friend class CStateManager;
        virtual void becomeMainState(void){}
        virtual void becomeSlaveState(void){}
    private:
};

#endif // CSTATE_H
