#include "GameGlobals.h"


bool GameEventReceiver::OnEvent(const SEvent& event)
{
	return GameEvent(event);
}
