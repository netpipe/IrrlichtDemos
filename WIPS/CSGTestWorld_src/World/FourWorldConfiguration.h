
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef FOURWORLDCONFIGURATION_H
#define FOURWORLDCONFIGURATION_H

class World;

void RunFourWorldConfiguration(
	World * pWorld1, World * pWorld2, World * pWorld3, World * pWorld4,
	char * sName,
	float fSplitProportionW, float fSplitProportionH,
	int argc, char** argv);

World * GetFourWorldConfigurationWorld(int nID);

#endif // FOURWORLDCONFIGURATION_H

