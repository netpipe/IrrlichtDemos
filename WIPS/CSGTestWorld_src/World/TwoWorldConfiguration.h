
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef TWOWORLDCONFIGURATION_H
#define TWOWORLDCONFIGURATION_H

class World;

void RunTwoWorldConfiguration_VerticalSplit(
	World * pWorld1, World * pWorld2,
	char * sName,
	float fSplitProportionW,
	int argc, char** argv);

void RunTwoWorldConfiguration_HorizontalSplit(
	World * pWorld1, World * pWorld2,
	char * sName,
	float fSplitProportionH,
	int argc, char** argv);

World * GetTwoWorldConfigurationWorld(int nID);

#endif // TWOWORLDCONFIGURATION_H

