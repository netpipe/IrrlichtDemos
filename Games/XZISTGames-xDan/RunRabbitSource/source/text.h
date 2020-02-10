
#ifndef __TEXT_H
#define __TEXT_H

#include <irrlicht.h>
using namespace irr;

struct Text;

// screen centre
int screenHW();
int screenHH();

// text with special effects?
void loadFonts();
void updateTexts(double dt);
void removeText(Text *text);
void removeAllTimedTexts();
void removeAllTexts();
Text *blinkMessage(core::stringw str, int y, double ttl);
void shoutMessage(core::stringw str, int y);

// standard text
gui::IGUIStaticText *staticText(core::stringw str, int x, int y, int flag, char *fontFile);

#endif
