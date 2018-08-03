
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include <stdlib.h>
#include <stdio.h>

#include "SingleWorldConfiguration.h"
#include "TwoWorldConfiguration.h"
#include "ThreeWorldConfiguration.h"
#include "FourWorldConfiguration.h"

#include "2DOrthoWorld.h"

#include "CSGPlayWorld.h"

int main_SingleWorld(int argc, char** argv)
{
	World * pWorld = new World();
	//World * pWorld = new OrthoWorld();

	RunSingleWorldConfiguration(pWorld, "SingleWorldConfig", argc, argv);

	delete pWorld;

	return EXIT_SUCCESS;
}

int main_TwoWorld_VerticalSplit(int argc, char** argv)
{
	World * pWorld1 = new World();
	World * pWorld2 = new OrthoWorld();

	RunTwoWorldConfiguration_VerticalSplit(pWorld1, pWorld2, "TwoWorldVerticalConfig", 0.7f, argc, argv);

	delete pWorld1;
	delete pWorld2;

	return EXIT_SUCCESS;
}

int main_TwoWorld_HorizontalSplit(int argc, char** argv)
{
	World * pWorld1 = new World();
	World * pWorld2 = new OrthoWorld();

	RunTwoWorldConfiguration_HorizontalSplit(pWorld1, pWorld2, "TwoWorldHorizontalConfig", 0.7f, argc, argv);

	delete pWorld1;
	delete pWorld2;

	return EXIT_SUCCESS;
}

int main_FourWorld(int argc, char** argv)
{
	World * pWorld1 = new World();
	World * pWorld2 = new OrthoWorld();
	World * pWorld3 = new OrthoWorld();
	World * pWorld4 = new World();

	RunFourWorldConfiguration(pWorld1, pWorld2, pWorld3, pWorld4, "FourWorldConfig", 0.5f, 0.8f, argc, argv);

	delete pWorld1;
	delete pWorld2;
	delete pWorld3;
	delete pWorld4;

	return EXIT_SUCCESS;
}

int main_ThreeWorld(int argc, char** argv)
{
	World * pWorld1 = new World();
	World * pWorld2 = new OrthoWorld();
	World * pWorld3 = new OrthoWorld();

	//RunThreeWorldConfiguration(pWorld1, pWorld2, pWorld3, "ThreeWorldConfig!", ThreeWorldConfigurationsEnum_VerticalSplitLeft, 0.2f, 0.7f, argc, argv);
	//RunThreeWorldConfiguration(pWorld1, pWorld2, pWorld3, "ThreeWorldConfig!", ThreeWorldConfigurationsEnum_VerticalSplitRight, 0.8f, 0.7f, argc, argv);
	//RunThreeWorldConfiguration(pWorld1, pWorld2, pWorld3, "ThreeWorldConfig!", ThreeWorldConfigurationsEnum_HorizontalSplitTop, 0.7f, 0.2f, argc, argv);
	//RunThreeWorldConfiguration(pWorld1, pWorld2, pWorld3, "ThreeWorldConfig!", ThreeWorldConfigurationsEnum_HorizontalSplitBottom, 0.3f, 0.8f, argc, argv);

	RunThreeWorldConfiguration(pWorld3, pWorld2, pWorld1, "VisualStudioConfig", ThreeWorldConfigurationsEnum_HorizontalSplitTop, 0.2f, 0.8f, argc, argv);

	delete pWorld1;
	delete pWorld2;
	delete pWorld3;

	return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
	//return main_SingleWorld(argc, argv);
	//return main_TwoWorld_VerticalSplit(argc, argv);
	//return main_TwoWorld_HorizontalSplit(argc, argv);
	//return main_FourWorld(argc, argv);

	//return main_ThreeWorld(argc, argv);


	//World * pWorld = new CSGWorld();
	World * pWorld = new CSGPlayWorld();
	//World * pWorld = new OrthoWorld();

	RunSingleWorldConfiguration(pWorld, "CSGPlayWorld", argc, argv);

	delete pWorld;

	return EXIT_SUCCESS;
}
