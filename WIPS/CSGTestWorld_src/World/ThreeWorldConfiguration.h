
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef THREEWORLDCONFIGURATION_H
#define THREEWORLDCONFIGURATION_H

class World;

enum ThreeWorldConfigurationsEnum
{
	ThreeWorldConfigurationsEnum_VerticalSplitLeft,
	ThreeWorldConfigurationsEnum_VerticalSplitRight,
	ThreeWorldConfigurationsEnum_HorizontalSplitTop,
	ThreeWorldConfigurationsEnum_HorizontalSplitBottom
};

void RunThreeWorldConfiguration(
	World * pWorld1, World * pWorld2, World * pWorld3,
	char * sName,
	ThreeWorldConfigurationsEnum eConfig, float fSplitProportionW, float fSplitProportionH,
	int argc, char** argv);

World * GetThreeWorldConfigurationWorld(int nID);

void UseMainWindowContext();
void UseOffscreenContext();

#endif // FOURWORLDCONFIGURATION_H

