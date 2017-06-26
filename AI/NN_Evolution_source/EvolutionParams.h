
#define DEFAULT_EVOLUTION_PARAMS {20.f,60,20,0,EST_ROULETTE_WHEEL,0.8,0.1,10.f,4,1.f}

// All the parameters that the user can configure using the GUI
struct EvolutionParams
{
	// World params
	float genTime; // seconds that a generation lasts
	int itemCount;
	
	// GA parameters
	int popSize;
	int eliteCount;
	E_SELECTOR_TYPE selectorType;
	float probCrossover;
	float probMutate;
	float mutateScale; // AKA maximum mutation amount
	
	// ANN params
	int hiddenLayerSize;
	float initialRandScale;
};
