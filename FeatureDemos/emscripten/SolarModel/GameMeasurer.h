#ifndef __COSMIC_GAMECOMPARER_H_INCLUDED__
#define __COSMIC_GAMECOMPARER_H_INCLUDED__

#include "GameGlobals.h"

class GameMeasurer
{
public:
	void Init();
	void Open();
	void Close();
	bool Event(SEvent event);
	void Done();

	bool IsVisible();

private:
	gui::IGUIImage *imgMain;
	gui::IGUIButton *butClose;

	gui::IGUIImage* ruler;
	gui::IGUIStaticText *rulerInfo;
	gui::IGUIImage *rulerNet;
	gui::IGUIImage *rulerLine;
	gui::IGUIStaticText *curMouseDistance;

	bool msLB;
	core::position2d<s32> msLBPos;
	core::position2df msLBuserPos;

	core::position2df userPos;
	float userZoom;
	float zoomMul;

	struct Body
	{
		core::stringw name;
		gui::IGUIStaticText *guiText;
		gui::IGUIImage *guiImage;
		// 1.0f == 1 000 km
		float dist; // distance from the Sun
		float size; // diameter
	};
	core::array<Body> bodies;
	void BodyAdd(wchar_t *name, char *tex, float size, float dist);
	int GetScreenXCoordFromDistance(float distance);
	float GetDistanceFromScreenXCoord(int screenXCoord);
	core::stringw GetDistanceAsString(float distanceKM);
	void BodyUserZoomChanged();
	void BodyUserPosChanged();
};

#endif