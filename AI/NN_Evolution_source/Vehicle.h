
#define TURN_SPEED		500.f
#define MOVE_SPEED		1.f
#define HIGHLIGHT_COL	video::SColor(255,255,0,0)

class Vehicle
{
	NeuralNetwork nn;
	float performance;
	
	scene::ISceneNode *node;
	scene::ITriangleSelector *collisionTriangles;
	ItemManager *itemManager;
	
	core::vector3df startPos;
	
	std::vector<VehicleSensor *> sensors;
	
	core::vector3df lastPos; // the position in the last update
	core::vector3df move; // how much it *should* have moved last update
	
	video::SColor normalCol;
	
public:
	Vehicle(const NeuralNetwork &nn, scene::ITriangleSelector *collisionTriangles,
			ItemManager *itemManager, core::vector3df startPos)
	{
		this->nn = nn; // stores a copy of the NN
		this->collisionTriangles = collisionTriangles;
		this->itemManager = itemManager;
		this->startPos = startPos;
		
		performance = 0.f;
		
		node = engine.getSceneManager()->addAnimatedMeshSceneNode(
			engine.getSceneManager()->getMesh("vehicle.b3d")
			);
		
		normalCol = node->getMaterial(0).AmbientColor;
		
		// Positioning
		
		node->setPosition(startPos);
		
		lastPos = startPos;
		
		move = core::vector3df(0,0,0);
		
		// Create some sensors
		
		sensors.push_back( new VehicleSensor(node, core::vector3df(0,0,0), core::vector3df(0.25,0,-1), 2.f) );
		sensors.push_back( new VehicleSensor(node, core::vector3df(0,0,0), core::vector3df(-0.25,0,-1), 2.f) );
		
		
		// Collision response
		
		scene::ISceneNodeAnimator *anim = engine.getSceneManager()->createCollisionResponseAnimator(
			collisionTriangles, node,
			node->getBoundingBox().getExtent()/2.f,
			core::vector3df(0,0,0), 
			core::vector3df(0,0,0),
			0.f);
		node->addAnimator(anim);
		anim->drop();
	}
	
	~Vehicle()
	{
		node->remove();
		
		for (int i = 0; i < sensors.size(); i ++)
			delete sensors[i];
	}
	
	void update(float dt)
	{
		// How well is it performing?
		// (for fitness)
		
		/*
		
		// Incur a fitness penalty if the vehicle collides with the environment (walls).
		
		// This has been disabled as it seems like quite an artificial way of measuring fitness...
		// If the goal is to collect as many food items as possible then obstacle avoidance
		// might evolve by itself!
		// Also didn't seem to improve vehicle performance significantly.
		
		// see how much it *really* moved, compared to how much it was supposed to move
		// i.e. if it didn't move as much, then it must have collided with the scenery
		core::vector3df actualMove = node->getPosition()-lastPos;
		lastPos = node->getPosition();
		
		if (move.getDistanceFromSQ(actualMove) > 0.00001)
		{
			// penalise by the number of seconds spent touching an object
			performance -= dt;
		}
		*/
		
		// has picked up an item?
		if (itemManager->collectItem(node->getPosition()))
			performance += 1.f;
		
		
		// Sensors
		
		for (int i = 0; i < sensors.size(); i ++)
			sensors[i]->update();
		
		
		// AI
		std::vector<float> inputs(4);
		std::vector<float> outputs(2);
		
		inputs[0] = sensors[0]->getWallProximity(collisionTriangles);
		inputs[1] = sensors[1]->getWallProximity(collisionTriangles);
		
		inputs[2] = sensors[0]->getItemProximity(itemManager);
		inputs[3] = sensors[1]->getItemProximity(itemManager);
		
		nn.process(inputs, outputs);
		
		// from -1 to 1. Gotten from the NN.
		// NN only outputs 0-1 so is scaled to -1 to 1.
		float steerAngle = outputs[0]*2.f - 1.f;
		
		core::vector3df rot = node->getRotation();
		rot.Y += steerAngle*dt*TURN_SPEED;
		node->setRotation(rot);
		
		move = core::vector3df(0,0,-1)*dt*(outputs[1]*2.f-1.f)*MOVE_SPEED;
		
		core::matrix4 mat;
		mat.setRotationDegrees(rot);
		mat.rotateVect(move);
		
		core::vector3df newPos = node->getPosition() + move;
		
		// ensure that Irrlicht's collision response doesn't move the vehicle
		// out of the 2D plane. (this seems to have fixed a bug where the vehicles would sometimes
		// escape from the arena)
		newPos.Y = 0.f;
		
		node->setPosition(newPos);
	}
	
	void draw()
	{
		for (int i = 0; i < sensors.size(); i ++)
			sensors[i]->draw();
	}
	
	// how well this vehicle has performed.
	// i.e. the fitness
	float getPerformance()
	{
		return performance;
	}
	
	core::vector3df getPosition()
	{
		return node->getPosition();
	}
	
	void setHighlight(bool highlight)
	{
		highlight ? node->getMaterial(0).AmbientColor = HIGHLIGHT_COL
				: node->getMaterial(0).AmbientColor = normalCol;
	}
};

