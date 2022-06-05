#include "CStateManager.h"
#include "CStateFactory.h"
#include "CState.h"
#include "CInput_catcher.h"

CStateManager::CStateManager(irr::IrrlichtDevice* device)
{
    //ctor
    MainState = NULL;
    Device = device;
    LastTime = 0;
    Input = new CInput_catcher;
    Device->setEventReceiver(Input);
}

CStateManager::~CStateManager()
{
    //dtor
    irr::core::list<CState*>::Iterator it = States.begin();
    while(it != States.end())
    {
        CState* s = *it;
        delete s;
        it = States.begin();
    }
    while(Factories.size() > 0)
    {
        delete Factories[0];
        Factories.erase(0);
    }
    States2Remove.clear();

    if (Device)
    {
        Device->closeDevice();
        Device->drop();
        Device = NULL;
    }
    printf("Call destructor");
}
void CStateManager::removeState(CState* state)
{
    irr::core::list<CState*>::Iterator it = States.begin();
    while(it != States.end())
    {
        if (*it == state)
        {
            if (MainState == *it)
            {
                MainState->becomeSlaveState();
                MainState = NULL;
            }
            States.erase(it);
            return;
        }
        ++it;
    }
}
void CStateManager::addState(CState* state)
{
    printf("Add State...");
    irr::core::list<CState*>::Iterator it = States.begin();
    while(it != States.end())
    {
        if (*it == state)
            return;
        ++it;
    }
    States.push_back(state);
    printf("done\n");
}
void CStateManager::registerState(CStateFactory* factory)
{
    for (irr::u32 i=0; i<Factories.size(); ++i)
    {
        if (Factories[i]->Name == factory->Name)
        {
            delete factory;
            return;
        }
    }
    Factories.push_back(factory);
}
CState* CStateManager::createState(const irr::core::stringc& name)
{
    for (irr::u32 i=0; i<Factories.size(); ++i)
    {
        if (Factories[i]->Name == name)
        {
            CState* state = Factories[i]->createState(this);
            return state;
        }
    }
    return NULL;
}

bool CStateManager::update(void)
{
    //update world
    if (!MainState)
    {
        if (States.getSize() <= 0)
        {
            printf("No states avaible\n");
            return false;
        }
        printf("Use first state as MainState\n");
        irr::core::list<CState*>::Iterator it = States.begin();
        MainState = *it;
        MainState->becomeMainState();
    }

    if (LastTime == 0)
    {
        LastTime = Device->getTimer()->getTime();
    }
    irr::u32 TimeDiff = Device->getTimer()->getTime()-LastTime;
    LastTime = Device->getTimer()->getTime();

    if (MainState)
        MainState->update(TimeDiff/1000.f);

    //remove states to delete
    if (States2Remove.getSize() > 0)
    {
        printf("remove states...");
        irr::core::list<CState*>::Iterator it = States2Remove.begin();
        while(it != States2Remove.end())
        {
            CState* state = *it;
            if (state == MainState)
            {
                MainState->becomeSlaveState();
                MainState = NULL;
            }
            delete state;
            ++it;
        }
        States2Remove.clear();
        printf("done\n");
    }

    return true;
}

void CStateManager::state2remove(CState* state)
{
    irr::core::list<CState*>::Iterator it = States2Remove.begin();
    while(it != States2Remove.end())
    {
        if (*it == state)
            return;
        ++it;
    }
    States2Remove.push_back(state);
}




