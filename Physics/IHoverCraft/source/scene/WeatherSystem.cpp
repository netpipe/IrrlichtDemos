#include "WeatherSystem.h"
#include "../core/Game.h"

WeatherSystem::WeatherSystem(Game* game) :
	game(game),
	fogEnabled(false),
	lightEnabled(false)
{
//	//setup default fog
//	this->game->getVideoDriver()->setFog(
//		irr::video::SColor(0, 255, 255, 255), //color
//		true, //linear fog Set this to true for linear fog, otherwise exponential fog is applied.
//		50, //start
//		3000, //end Only used in linear fog mode (linearFog=true). Specifies where fog ends.
//		0.01, //density Only used in expotential fog mode (linearFog=false). Must be a value between 0 and 1.
//		false, //pixel fog 	Set this to false for vertex fog, and true if you want pixel fog.
//		false //range fog Set this to true to enable range-based vertex fog. The distance from the viewer is used to compute the fog, not the z-coordinate. This is better, but slower. This is only available with D3D and vertex fog.
//	);

	//setup ambient light
	this->game->getSceneManager()->setAmbientLight(irr::video::SColorf(0.0f, 0.0f, 0.0f, 0.0f));

	//setup shadow color
	this->game->getSceneManager()->setShadowColor(irr::video::SColor(120, 0, 0, 0));
}

const void WeatherSystem::setFog(const bool fogEnabled)
{
	this->fogEnabled = fogEnabled;

	setMaterialFlag fog(irr::video::EMF_FOG_ENABLE, this->fogEnabled);
	ISceneNode_applyToScene(fog, this->game->getSceneManager()->getRootSceneNode());
}

const void WeatherSystem::setLight(const bool lightEnabled)
{
	this->lightEnabled = lightEnabled;

	setMaterialFlag light(irr::video::EMF_LIGHTING, this->lightEnabled);
	ISceneNode_applyToScene(light, this->game->getSceneManager()->getRootSceneNode());
}
