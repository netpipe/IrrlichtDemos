
// Libraries
#include <irrlicht.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using namespace irr;

// Some utility functions
#include "utils.h"

// This includes-within-a-struct approach allows nice Java style OO programming
// with entire classes in headers rather than seperated into .cpp and .h files.
// See here: http://strlen.com/rants/javaclassesincpp.html
struct modules
{
	#include "Neuron.h"
	#include "NeuralNetwork.h"
	#include "Selector.h"
	#include "Evolver.h"
	
	#include "EvolutionParams.h"
	#include "GUIElementIDs.h"
	#include "ItemManager.h"
	#include "VehicleSensor.h"
	#include "Vehicle.h"
	#include "World.h"
	#include "CameraController.h"
	#include "GUI.h"
	#include "ScreenLog.h"
	#include "Engine.h"
	
	static Engine engine;
};

modules::Engine modules::engine;

int main()
{
	modules::engine.main();
	return 0;
}
