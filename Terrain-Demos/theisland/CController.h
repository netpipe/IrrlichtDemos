#ifndef _CController
#define _CController

#include "include.h"

namespace island
{
	class CController : public irr::IEventReceiver
	{
	public:
		CController(CCore* core,irr::scene::ISceneNode* islandNode);
		~CController();

		//calculates new island angles
		void inclinedPlane();

		//calculates the new difficulty and adds new wagons/parts
		void difficulty();

		//recieves events
		bool OnEvent(const irr::SEvent & event);

	private:
		//place a ballon on the surface
		void placeBallon();

		//removes a ballon
		void removeBallon();

		//faces (the trainparts) to the middle of the island. It will look as if they circle around the vulcane
		void faceTarget(irr::scene::ISceneNode* node);

		//vulcane starts smoking
		void erupt();

		//Level up train
		void LevelUpTrain(irr::s32 trainNumber);

		//default pointer to the core.
		CCore* core;

		//first: dummy child off the island. second: ballon model above the point
		irr::core::map<irr::scene::ISceneNode*,irr::scene::ISceneNode*> ballons;
		irr::core::list<irr::scene::ISceneNode*> trainParts;

		//Player camera - controlled by player
		irr::scene::ICameraSceneNode* playerCamera;

		//the island
		irr::scene::ISceneNode* island;

		//island selector
		irr::scene::IMetaTriangleSelector* islandSelector;

		//last animation time
		irr::s32 lastAnimationTime;
		irr::s32 nextLevelUpTime[2];

		//time the app started
		irr::s32 startTime;
		irr::s32 level;

		//momentum forces
		irr::f32 momentumForceX;
		irr::f32 momentumForceZ;

		irr::gui::IGUIStaticText* bar1_points;
		irr::gui::IGUIStaticText* bar2_ballons;
		irr::gui::IGUIStaticText* bar3_wagons;

		//ctrl pressed (for removing ballons)
		bool ctrlPressed;

		//particle / vulcane
		irr::scene::IParticleSystemSceneNode* fireEmitter;
		
		bool gamelost;
	};
}

#endif