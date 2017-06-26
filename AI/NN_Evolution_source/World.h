
// Minimum of 3 seconds between the camera changing the vehicle it is following.
// Stops the camera flicking back and forth too quickly
#define MIN_CAMERA_CHANGE_FOLLOW_TIME 3.f

// This class controls the main application logic
// (3D world/environment simulation, and evolution)
class World
{
	Evolver *evolver;
	std::vector<Vehicle *> vehicles;
	
	ItemManager *itemManager;
	int itemCount;
	
	int popSize;
	float genTime;
	
	core::vector2df arenaSize;
	
	float timeSinceLastGen;
	
	float cameraChangeFollowTime;
	Vehicle *bestVehicle;
	Vehicle *lastBestVehicle;
	
	scene::ITriangleSelector *collisionTriangles;
	
	
	void createVehicles(std::vector<NeuralNetwork> &neuralNetworks)
	{
		itemManager = new ItemManager(this, itemCount);
		
		for (int i = 0; i < popSize; i ++)
			vehicles.push_back( new Vehicle(neuralNetworks[i], collisionTriangles, itemManager, randPos()) );
		
		timeSinceLastGen = 0.f;
		cameraChangeFollowTime = 0.f; // can start following a vehicle immediately
	}
	
	void destroyVehicles()
	{
		if (itemManager)
		{
			delete itemManager;
			itemManager = NULL;
		}
		
		setCameraFollow(NULL);
		
		bestVehicle = NULL;
		lastBestVehicle = NULL;
		
		for (int i = 0; i < vehicles.size(); i ++)
			delete vehicles[i];
		
		vehicles.clear();
	}
	
	Vehicle *getBestPerformingVehicle()
	{
		if (!vehicles.size())
			return NULL;
		
		Vehicle *best = vehicles[0];
		
		for (int i = 1; i < vehicles.size(); i ++)
		{
			if (vehicles[i]->getPerformance() > best->getPerformance())
				best = vehicles[i];
		}
		
		return best;
	}
	
	void setCameraFollow(Vehicle *vehicle)
	{
		engine.getCameraController()->setFollowVehicle(vehicle);
	}
	
public:
	World()
	{
		srand(time(NULL)); // don't want the same behaviour each time...
		
		// Add a light
		scene::ILightSceneNode *light = engine.getSceneManager()->addLightSceneNode(
				NULL,
				core::vector3df(0,3,0),
				video::SColorf(), 1/0.4
				);
		video::SLight &slight = light->getLightData();
		slight.AmbientColor = video::SColorf(0.5,0.5,0.4);
		slight.DiffuseColor = video::SColorf(0.7,0.7,0.3);
		slight.SpecularColor = video::SColorf(0.5,0.5,0.5);		
		
		// create the 3D environment
		
		engine.getSceneManager()->addAnimatedMeshSceneNode(
			engine.getSceneManager()->getMesh("ground.b3d")
			);
		
		scene::IAnimatedMeshSceneNode *wallNode = engine.getSceneManager()->addAnimatedMeshSceneNode(
			engine.getSceneManager()->getMesh("walls.b3d")
			);
		
		// collision mesh
		collisionTriangles = engine.getSceneManager()->createOctTreeTriangleSelector(
			wallNode->getMesh()->getMesh(0), wallNode, 1);
		
		
		arenaSize = core::vector2df(9,9);//worldNode->getBoundingBox().getExtent().X,
				//worldNode->getBoundingBox().getExtent().Z)
				//- 1.f; // don't want vehicles appearing right at the edges
		
		evolver = NULL;
		itemManager = NULL;
		
		bestVehicle = NULL;
		lastBestVehicle = NULL;
	}

	~World()
	{
		collisionTriangles->drop();
		
		if (evolver)
			delete evolver;
		
		destroyVehicles();
	}
	
	void startEvolution(EvolutionParams &params)
	{
		this->popSize	= params.popSize;
		this->genTime	= params.genTime;
		this->itemCount	= params.itemCount;
		
		engine.getScreenLog()->writeLine(L"Evolution started!");
		
		NeuralNetwork nn(params.initialRandScale);
		nn.addLayer(4); // input layer
		nn.addLayer(params.hiddenLayerSize);
		nn.addLayer(2); // output layer
		
		evolver = new Evolver(
				nn,
				params.popSize,
				params.eliteCount,
				params.selectorType,
				params.probCrossover,
				params.probMutate,
				params.mutateScale
				);
		
		createVehicles(evolver->getPopulation());
	}
	
	void stopEvolution()
	{
		delete evolver;
		evolver = NULL;
		destroyVehicles();
	}
	
	/*
	// Not needed
	const std::vector<Vehicle *> &getVehicles()
	{
		return vehicles;
	}
	*/
	
	// chooses a random position in the world
	core::vector3df randPos()
	{
		return core::vector3df( fRand2()*arenaSize.X/2.f, 0, fRand2()*arenaSize.Y/2.f );
	}

	void update(f32 dt)
	{
		if (!evolver) // Do nothing if the evolver hasn't been started
			return;
		
		// could use Irrlicht's timers but this is more convenient...
		// a huge amount of precision is not needed
		timeSinceLastGen += dt;
		
		// move vehicles etc.
		for (int i = 0; i < vehicles.size(); i ++)
			vehicles[i]->update(dt);
		
		// Create a new generation of vehicles
		if (timeSinceLastGen >= genTime)
		{
			// get fitnesses of vehicles
			// set the neural network fitnesses from these
			for (int i = 0; i < vehicles.size(); i ++)
				evolver->setFitness(i, vehicles[i]->getPerformance());
			
			float lastFittest = evolver->getFittest().getFitness();
			
			// destroy old NN controlled vehicles
			destroyVehicles();
			
			// create next generation of NNs,
			// taking into account the fitnesses
			evolver->step();
			
			// recreate vehicles with the new NNs
			createVehicles(evolver->getPopulation());
			
			// Log some stats about the generation to the display
			core::stringw logText = L"Generation ";
			logText += evolver->getGenCount();
			logText += L". Best vehicle fitness in last generation was ";
			logText += engine.getGUI()->numberToString(lastFittest);
			engine.getScreenLog()->writeLine(logText.c_str());
		}
	}
	
	void draw(float dt)
	{
		for (int i = 0; i < vehicles.size(); i ++)
			vehicles[i]->draw();
		
		// make the camera follow the fittest vehicle
		if (cameraChangeFollowTime <= 0.f)
		{
			bestVehicle = engine.getWorld()->getBestPerformingVehicle();
			
			if (bestVehicle && bestVehicle != lastBestVehicle)
			{
				if (lastBestVehicle)
					lastBestVehicle->setHighlight(false);
				
				// colour it red so it can be seen easily
				bestVehicle->setHighlight(true);
				
				// and let the camera follow it when in follow mode
				setCameraFollow(bestVehicle);
				
				lastBestVehicle = bestVehicle;
				cameraChangeFollowTime = MIN_CAMERA_CHANGE_FOLLOW_TIME;
			}
		}
		
		// Count down until a different vehicle can be followed
		cameraChangeFollowTime -= dt;
	}
};

