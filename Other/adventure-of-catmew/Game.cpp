// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#include "Game.h"

Game::Game(const std::string pConfig)
{
	RT = 0;
	exit = false;

	
		GameConfiguration = new conf::Configuration(pConfig);

		conf::Section* videoCfg = GameConfiguration->getSection("video");

		float resx = conf::toNumber(videoCfg->getKeyData("Resolutionx"));
		float resy = conf::toNumber(videoCfg->getKeyData("Resolutiony"));
		bool Fullscreen = conf::toBoolean(videoCfg->getKeyData("Fullscreen"));
		bool VSync = conf::toBoolean(videoCfg->getKeyData("Vsync"));

		RT = createRoot(Vector(resx,resy), Fullscreen, VSync);
		RT->lock();

		facing = RIGHT;
	
	DeltaTimeAverage	= 1.0f;
	DeltaTimeIndex		= 0;
	LastFrameTime		= -1;

	for(int i = 0; i != 25; i++)
		DeltaTime[i] = 0;
		
}

gfx::ITexture* Game::createSkyGradientTexture(gfx::Color StartColor,gfx::Color EndColor)
{
	gfx::ITexture* SkyTexture = getRoot()->getVideoDriver()->createEmptyTexture(Vector(getRoot()->getDimension().x,getRoot()->getDimension().y),true);	gfx::Color SkyColor;	
	for(int i = 0; i < getRoot()->getDimension().x; ++i)
	{	
		SkyColor = (StartColor * (i / getRoot()->getDimension().x)) + (EndColor * (1 - (i / getRoot()->getDimension().x)));
		//SkyColor.a = 1.0f;
		for(int v = 0; v < getRoot()->getDimension().y; ++v)
		SkyTexture->setPixel(Vector(i,v),SkyColor);
	}
	SkyTexture->update();
	return SkyTexture;
}

Game::~Game()
{
	
}

IRoot* Game::getRoot() const
{
	return RT;
}


float Game::getDeltaTime() const
{
	return DeltaTimeAverage;
}

bool Game::update() 
{
	updateDeltaTime();
	
	wPos = RT->getSceneManager()->getWorldPosition();
	cwPos = wPos + RT->getDimension().x * 0.5f;
	
	if(exit)
		return false;

	return RT->loop();
}

void Game::updateDeltaTime()
{
	DeltaTime[DeltaTimeIndex] = RT->getTime()-LastFrameTime;	
	LastFrameTime = RT->getTime();

	DeltaTimeIndex++;

	if(DeltaTimeIndex >= 25)
		DeltaTimeIndex = 0;


	float Temp = 0.0f;
	for(int i = 0; i != 25; i++)
		Temp += DeltaTime[i];

	DeltaTimeAverage = Temp/25.0f;
}

