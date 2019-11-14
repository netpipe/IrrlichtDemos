#ifndef __COSMIC_GAMEMAPDESCRIPTION_H_INCLUDED__
#define __COSMIC_GAMEMAPDESCRIPTION_H_INCLUDED__


#include "GameGlobals.h"


#define GAMEMAPDESCRIPTION_MAXKEYCOUNT 8
#define GAMEMAPDESCRIPTION_MAXDESCCOUNT 64


struct GameMapDescriptionItem
{
	int keycount;
	core::stringc key[GAMEMAPDESCRIPTION_MAXKEYCOUNT];
	core::stringc title;
	core::stringc desc;
};


class GameMapDescription
{
public:
	int ItemsCount;
	struct GameMapDescriptionItem Items[GAMEMAPDESCRIPTION_MAXDESCCOUNT];

	GameMapDescription();
	~GameMapDescription();
	void Load(core::stringc fn);
	void GetDescription(const core::stringc key, core::stringc &title,
		core::stringc &desc);
};


#endif
