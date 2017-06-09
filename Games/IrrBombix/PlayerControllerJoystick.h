/*
 * ControllerManager.h
 *
 *  Created on: Mar 20, 2011
 *      based on Irrlicht example 19
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_

#include "irrlicht.h"
class PlayerIrrlicht;
class IrrlichtEventReceiver;


class PlayerControllerJoystick {
public:
	PlayerControllerJoystick();
	~PlayerControllerJoystick();
    int getControllernr() const
    {
        return controllernr;
    }

    PlayerIrrlicht *getPlayer() const
    {
        return player;
    }

    void setControllernr(int controllernr)
    {
        this->controllernr = controllernr;
    }

    void setPlayer(PlayerIrrlicht *player)
    {
        this->player = player;
    }
    void update();

    irr::SEvent::SJoystickEvent & GetJoystickState()
    {
        return this->JoystickState;
    }

    void setJoystickState(irr::SEvent::SJoystickEvent JoystickState)
    {
        this->JoystickState = JoystickState;
    }

    bool getActive() const
    {
        return active;
    }

    void setActive(bool active)
    {
        this->active = active;
    }

private:
	PlayerIrrlicht* player;
	int controllernr;
	bool active;
	irr::SEvent::SJoystickEvent JoystickState;
	// calibration
	irr::f32 maxmoveHorizontal;
	irr::f32 maxmoveVertical;
	irr::f32 minmoveHorizontal;
	irr::f32 minmoveVertical;
};

#endif /* PLAYERCONTROLLER_H_ */
