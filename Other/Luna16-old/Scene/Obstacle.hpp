#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
//#include "Globals.hpp"

class Obstacle
{
	private:
		bool obstacleExists;
		array<ISceneNode*> obstacleNode;

	public:
		static Obstacle *Instance();

		void SetDefaultValues(void);
		void Add(ISceneNode *node);
		void Clear(void);
		void CheckCollisionWithPlayer(void);

	protected:
		static Obstacle *m_obstacle;
		static Obstacle m_default_obstacle_buffer;
};

#endif // OBSTACLE_H
