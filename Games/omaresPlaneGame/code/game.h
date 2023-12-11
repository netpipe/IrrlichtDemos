#include "StdAfx.h"


#if !defined(GAME_H)
#define GAME_H



class CGame : public IEventReceiver {
protected:
	// misc
	int lastFPS;
int health;
bool die;
bool firedone;
vector3df	prevn1rot;
bool fired;
public:
	// Irrlicht  vars
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	IrrlichtDevice *device;
	irr::gui::IGUIEnvironment* guiEnv;
	irr::gui::IGUIImage* windowBackground;
	E_DRIVER_TYPE driverType;
///////////////menu vars

	int resX;
	int resY;
	bool fullscreen;
	int lowRes;
	int highRes;
	int opengl;
	int directx;
	bool startGame;
	irr::gui::IGUIListBox*				resolutionListBox;
	irr::gui::IGUIListBox*				driverBox;
	irr::gui::IGUISkin*					skin;
	irr::gui::IGUIFont*					fontShruti6;
	irr::gui::IGUIFont*					fontShruti10;
	irr::gui::IGUIFont*					fontTrebucheat14B;
	irr::gui::IGUIFont*					fontVeranda16;
	irr::gui::IGUIButton*				startButton;
	irr::gui::IGUICheckBox*				fullScreenCheck;

	// Scene vars////////////////////////
	ICameraSceneNode *camera;
	//house stuff
	ISceneNode*house;
	int houseno;
	/////////
	//treestuff
	ISceneNode* tree;
	int treeno;
	//terrain stuff
	IAnimatedMesh* terrain;
	ISceneNode* terrainnode;
///player
ISceneNode* playernode;
	///////player
ISceneNode* enemynode;


//soind

	/////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Irrlicht Callbacks
	virtual bool OnEvent(const SEvent &event);

	//////////////////////////////////////////////////////////////////////////


	// Game Functions
	void Init();
	void Start();



	void Finish();

	void Housegen(int max);

	void Treegen(int max);
	void Cloudgen(int max);


	void player(ISceneNode *node,ICameraSceneNode *camera);

	void enemy(ISceneNode *node,ICameraSceneNode *camera );

	void sound();
	///menu functions

	void menu();
	void createWindow();
};

#endif
