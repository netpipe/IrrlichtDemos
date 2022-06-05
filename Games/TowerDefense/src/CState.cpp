#include "CState.h"
#include "CStateManager.h"
CState::CState(CStateManager* manager)
{
    Manager = manager;
    Manager->addState(this);
}

CState::~CState()
{
    Manager->removeState(this);
}

void CState::remove(void)
{
    Manager->state2remove(this);
}

