/*
 * based on The Game Loop by Koen Witters
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "irrlicht.h"
#include "Serializable.h"

class Game;
class GameView;
class GameSceneIrrlicht;
class PlayerSettings;
class GameIrrlicht;
class IrrlichtEventReceiver;

/**
 * Main window
 */
class Mainwindow: public Serializable{



private :


	IrrlichtEventReceiver* eventreceiver;
    GameSceneIrrlicht* m_GameItemIrrlicht;
    GameIrrlicht* game;

    irr::video::E_DRIVER_TYPE driverType;
    irr::SIrrlichtCreationParameters param;

public:

    static irr::IrrlichtDevice * device;
    static bool endloop;

    Mainwindow();

    ~Mainwindow();

    void initMainwindow();
    

    void loopMainwindow();

	void readFrom(Deserializer* id);
	void writeTo(Serializer* is);

	int screenwidth;
	int screenheight;

};

#endif

