#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

#include <irrlicht.h>
#include <CEventDispatcher.h>

class CStateFactory;
class CState;
class CInput_catcher;
class CStateManager : public CEventDispatcher
{
    public:
        /** Default constructor */
        CStateManager(irr::IrrlichtDevice* device);
        /** Default destructor */
        virtual ~CStateManager();

        CState* createState(const irr::core::stringc& name);
        void registerState(CStateFactory* factory);

        bool update(void);

        CState* getActiveState(void)
        {
            return MainState;
        }

        irr::IrrlichtDevice* getDevice(void)
        {
            return Device;
        }
        CInput_catcher* getInput(void)
        {
            return Input;
        }
    protected:
        irr::core::list<CState*> States;
        irr::core::list<CState*> States2Remove;
        irr::core::array<CStateFactory*> Factories;
        CState* MainState;
        irr::IrrlichtDevice* Device;
        irr::u32 LastTime;
        CInput_catcher* Input;

        friend class CState;
        void addState(CState* state);
        void removeState(CState* state);
        void state2remove(CState* state);
    private:
};

#endif // CSTATEMANAGER_H
