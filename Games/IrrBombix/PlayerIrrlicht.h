/*
 * Copyright 2011
 * Copyright 2009 Mathias Kraus <k.hias@gmx.de>
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of 
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLAYERIRRLICHT_H
#define PLAYERIRRLICHT_H

#include "irrlicht.h"
#include "XYElement.h"

class PlayerSettings;
class Cell;
//class XYElement;
class ArenaIrrlicht;
class IJElementIrrlicht;
class BombIrrlicht;

#include "Bonus/BonusIrrlicht.h"

/**
 * @brief This class represents the Main character of the game.
 */
class PlayerIrrlicht: public XYElement
{

private:
    irr::core::stringc m_playerName;
    irr::f32 m_askedXSpeed, m_askedYSpeed;
    irr::f32 m_maxSpeed;
    int m_direction;
    int m_bombPower;
    int m_maxBombArmory;
    int m_bombArmory;
    bool m_death;
    int m_points;
    bool m_onIce;
    bool m_falling;
    bool m_throwBomb;
    bool m_kickBomb;
    irr::u32 deadtime;
    irr::core::array<int> m_shields;
    //irr::f32 m_normalSpeed;
    int m_normalBombArmory;
    irr::core::array<BombIrrlicht*> bombArsenal;
    irr::core::array<BonusIrrlicht*> Bonustaken;
    bool m_moveMirrored;
    int m_badBonusMillisecondsElapsed;
    bool wonRound;




public:
    PlayerIrrlicht(irr::f32 p_x, irr::f32 p_y, const irr::core::stringc & p_playerID, ArenaIrrlicht *p_arena);
    ~PlayerIrrlicht();
    irr::core::stringc getPlayerName() const;
    void init();
    void goUp();
    void goDown();
    void goRight();
    void goLeft();
    void button1Pressed();
    void button1Released();
    void button2Pressed();
    void button2Released();
    void move(irr::f32 x, irr::f32 y);
    void update();
    void updateMove();
    void updateDirection();
    irr::f32 getAskedXSpeed() const;
    void setAskedXSpeed(irr::f32 x);
    irr::f32 getAskedYSpeed() const;
    void setAskedYSpeed(irr::f32 y);
    void setArena(ArenaIrrlicht *aI);
    int direction();
    int getBombPower() const;
    void setBombPower(int bp);
    int getSpeed() const;
    void setSpeed(int bp);
    void decrementBombArmory();
    int getBombArmory();
    bool shield(int n_ExplosionID);
    bool hasShield();
    bool hasThrowBomb();
    bool setThrowBomb();
    bool hasKickBomb();
    void setKickBomb();
    bool hasBadBonus();
    void die();
    bool isAlive() const;
    void resurrect();
    int points() const;
    void setpoints(int  p);
    void addPoint();
    void emitGameUpdated();
    int signZeroPositive(const irr::f32 value);
    int sign(const irr::f32 value);
    void igniteBomb();
    BombIrrlicht *getbombspot();
    void clearbombspot();
    void toArsenal(BombIrrlicht *abomb);
    void incRestrain(int i);
    void incMirror(int i);
    void incScatty(int i);
    bool getWonRound() const;
    int lastXSpeed;
    int lastYSpeed;
    irr::f32 m_normalSpeed;

    int m_restrain;
    int m_mirror;
    int m_scatty;
    int m_slow;
    int m_hyperactive;

    void startUfo();

	bool isWalkable(Cell* aCell);
    bool isUfo;
    enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};

    keyStatesENUM getbutton1();
    keyStatesENUM getbutton2();






    irr::core::array<BonusIrrlicht*> *getBonustaken()
        {
            return &Bonustaken;
        }

        void setBonustaken(irr::core::array<BonusIrrlicht*> Bonustaken)
        {
            this->Bonustaken = Bonustaken;
        }

private:

    BombIrrlicht *bombspot;
    Cell *getAskedNextCell();
    void stopMoving();

    keyStatesENUM m_but1;
    keyStatesENUM m_but2;



};

#endif
