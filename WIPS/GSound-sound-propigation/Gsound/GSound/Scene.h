#include "Wall.h"
#include "Listner.h"
#include "Source.h"
#include "Ray.h"
#pragma once

class Scene
{
public:
	Scene(void);
	~Scene(void);
	Wall* walls[6];
	Scene(Wall*,Wall*,Wall*,Wall*,Wall*,Wall*);
	Listner* listner;
	Source source;
	Ray* rays[200];
	int i;
};

