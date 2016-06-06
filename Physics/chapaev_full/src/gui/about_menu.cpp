// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "about_menu.h"
#include "../events.h"
using namespace irr;
using namespace gui;
using namespace core;

AboutMenu::AboutMenu (IGUIEnvironment *env, Settings *settings) {
    const int dx = 560;
    const int dy = 400;
    const int x = settings->GetScreenWidth () / 2 - dx / 2;
    const int y = settings->GetScreenHeight () / 2 - dy / 2;
	wn = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false, L"About");
	wn->getCloseButton ()->setVisible (false);
	env->addButton (rect<s32> (500, 370, 550, 390), wn, EV_ABOUT_OK, L"OK");
	stringw s = L"Chapaev (OSRPG project test), "
		"game for two human players :P\n\n"
		"To start, press ESC (hide GUI)\n\n"
		"The purpose of the game is to hit opposite team boxes with your "
		"boxes so that they fall off the chess board. If you made one or "
		"more opposite team boxes "
		"fall off, you save the right to play again. If you've thrown your "
		"box in vain, opposite team comes into play. And vice versa, and "
		"so on.\nThe team who has one or more boxes left on the board, "
		"while all opposite team boxes are off the board, wins. However, "
		"there may occur a draw.";
	env->addStaticText (s.c_str (), rect<s32> (10, 30, 550, 350), false, true,
			wn);
	Show (false);
}

void AboutMenu::Show (bool state) {
	wn->setVisible (state);
}

