#ifndef MYEVENTRECEIVER_H
#define MYEVENTRECEIVER_H

#include "stdDefs.h"

class myEventReceiver : public IEventReceiver{
  private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];

  public:
    virtual bool OnEvent(const SEvent& event);
    void updateText(IGUIStaticText* txt);

};

extern datPlayer Player;
extern array<datWaypoint> Waypoints;
extern array<datEnemy> Enemies;
extern ISceneManager* smgr;
extern IrrlichtDevice* device;

#endif // MYEVENTRECEIVER_H
