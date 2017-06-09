/*
 * Based on
 * ARKANOID - Breakoutclone Game 4 Wizzard of OZ4
 * Autor: Dirk Hoeschen alias RubbelDieK@tz
 * Email: rubbel@c-base.org
 * This Game is public doMain.
 */


#ifndef IRRLICHT_H_
#define IRRLICHT_H_

#endif /* IRRLICHT_H_ */

#include <irrlicht.h>

class GameIrrlicht;
class ArenaIrrlicht;
class IrrlichtEventReceiver;

class GameSceneIrrlicht {
public:
	//	virtual bool OnEvent(SEvent event)
	GameSceneIrrlicht(GameIrrlicht* p_game);
	bool InitDriver();
	void CamStats();
	void StartGame();
	void setLight();
	void InitLevel();
	void SoundFx();
	void scaleNode(irr::scene::ISceneNode* mesh, irr::f32 desiredX);
	void DrawLoop();
	void DrawPlate(int i, int j, bool visible);
	void DrawCrate(int i, int j, bool visible);
	void DrawBrick(int i, int j, bool visible);
	void update();
	void updatePlayers();
	void cleanup();
	/**
	 * Initializes class
	 */
	void init();
    ArenaIrrlicht* getArenaIrrlicht();

    void setArenaIrrlicht(ArenaIrrlicht *m_ArenaIrrlicht);
    irr::f32 xytoxy(irr::f32 gx,bool isy);
    irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::scene::ICameraSceneNode* cam;
	irr::scene::ICameraSceneNode* dyncam;
	irr::scene::ICameraSceneNode* fpscam;
	irr::scene::ICameraSceneNode* overheadcam;
	irr::scene::ICameraSceneNode* beautycam;
	irr::IrrlichtDevice * device;

private:
	irr::scene::ISceneNode* batter;
	irr::scene::IAnimatedMeshSceneNode* ball;
	irr::core::vector3df bpos;

	irr::gui::IGUIStaticText* statusText;
	irr::gui::IGUIEnvironment * env;
	irr::gui::IGUIButton * butt1;
	irr::gui::IGUIButton * butt2;

	//irr::video::IVideoDriver* driver;
	//irr::scene::ISceneManager* smgr;
	irr::scene::ISceneNodeAnimator* anim;
	irr::scene::ISceneNodeAnimator* anim2;

	IrrlichtEventReceiver* eventreceiver;

	/** The Game instance */


	/* public variables */
	int screenwidth;
	//int   screenwidth = 320;
	int screenheight;
	//int   screenheight =200;

	GameIrrlicht* m_game;

	/* array with the bricks/crates */
	irr::core::array<irr::scene::ISceneNode*> crates;
	irr::core::array<irr::scene::ISceneNode*> plates;
	irr::core::array<irr::scene::ISceneNode*> bricks;
	irr::core::array<irr::scene::ISceneNode*> players;
	irr::core::array<irr::scene::ISceneNode*> bombs;

	ArenaIrrlicht* m_ArenaIrrlicht;

	int cx;
	int cz;
	int dz;
	int zstart;

	void drawClock();
};
