/*

Menu - header file

*/

#ifndef __Menu_h
#define __Menu_h

#include <irrlicht.h>

using namespace irr;

#include <Init.h>
#include <EventReceiver.h>
#include <Effect.h>


class cMenu {
public:

	// constructor
	cMenu();
	
	// destructor
	~cMenu();
	
	void Initialize(cInit *setInit);
	
	void drawMenu();
	
	bool checkInput();
	
	s32 getExit();


private:	

	cInit *init;
	cEventReceiver *eventReceiver;
	video::IVideoDriver *driver;
	IrrlichtDevice *device;
	cEffect menuEffects;
	
	scene::IParticleSystemSceneNode *menuEffectRight;
	scene::IParticleSystemSceneNode *menuEffectLeft;
	core::vector3df menuEffectRightPosition;
	core::vector3df menuEffectLeftPosition;
	
	// storing size of main menu items
	core::array<wchar_t*> menuItemName;
	core::array<core::rect<s32> > menuItemPos;	

	s32 exit;
	s32 mpos;
	
	// mouse cursor
	video::ITexture *mouseCursor;
	core::position2d<s32> mousePosition;
	core::position2d<s32> mousePositionTemp;
	core::position2d<s32> exMousePosition;

	video::ITexture *bg;
	video::ITexture *atomic;
	gui::IGUIFont *font;

};

#endif
