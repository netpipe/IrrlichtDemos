#ifndef WEATHERSYSTEM_H_
#define WEATHERSYSTEM_H_

#include <ISceneNode.h>

class Game;

/**
 * Provides functions to manipulate the "weather" in the scene.
 */
class WeatherSystem
{
private:

	Game* game;

	struct setMaterialFlag
	{
		setMaterialFlag(const irr::video::E_MATERIAL_FLAG& flag, bool value) :
			Flag(flag), Value(value)
		{
		}

		void operator()(irr::scene::ISceneNode* node)
		{
			node->setMaterialFlag(Flag, Value);
		}

		irr::video::E_MATERIAL_FLAG Flag;
		bool Value;
	};


	// this template applies function to `node' and all children of `node' recursively
	template< class Fun >
	void ISceneNode_applyToScene(Fun& function, irr::scene::ISceneNode* node) const
	{
		function(node);

		irr::core::list<irr::scene::ISceneNode*>::ConstIterator it = node->getChildren().begin();
		for (; it != node->getChildren().end(); ++it)
			ISceneNode_applyToScene(function, *it);
	}

//	// this template recursively applies function to every node in the scene graph
//	template< class Fun >
//	void ISceneNode_applyToScene(const Fun& function, irr::scene::ISceneManager* smgr) const
//	{
//		ISceneNode_applyToScene(function, smgr->getRootSceneNode());
//	}

	bool fogEnabled;

	///True if dynamic lighing is enabled.
	bool lightEnabled;

public:

	///Constructor.
	WeatherSystem(Game* game);

	///Enable or disable fog for an entire scene.
	const void setFog(const bool fogEnabled);

	///Enable or disable dynamic lighting for an entire scene.
	const void setLight(const bool lightEnabled);

	//getter

	const bool isFogEnabled() const { return this->fogEnabled; }

	const bool isLightEnabled() const { return this->lightEnabled; }
};

#endif /*WEATHERSYSTEM_H_*/
