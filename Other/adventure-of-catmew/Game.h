// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#include "gfx/LeeuwenGfx.h"
#include "Configuration.h"

#ifndef _GameHeader_
#define _GameHeader_

using namespace lew;
using namespace gfx;

enum E_DIRECTION
{
LEFT = 0,
RIGHT = 1
};

class Game : public Reference
{
	public:
		Game(const std::string pConfig);
		~Game();
		void close()
		{
			exit = true;
		};
		IRoot* getRoot() const;
		float getDeltaTime() const;
		gfx::ITexture* createSkyGradientTexture(gfx::Color StartColor,gfx::Color EndColor);
		bool update();
		Vector wPos;
		Vector cwPos;
		bool facing;
	
	private:
		void updateDeltaTime();
		
		conf::Configuration* GameConfiguration;
		IRoot* RT;	
		bool exit;
		float DeltaTimeAverage;
		int DeltaTimeIndex;
		int DeltaTime[25];
		int LastFrameTime;
};

#endif

