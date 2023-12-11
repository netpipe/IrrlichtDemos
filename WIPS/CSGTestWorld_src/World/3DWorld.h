
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef WORLD3D_H
#define WORLD3D_H

#include "ML_Transform.h"

#include "GX_RenderingTriangle.h"

#include "GL/glut.h"

#include "World.h"

class gxModel;

class World3D : public World
{
public:
	World3D();
	virtual ~World3D();

	//void Update(float fDelta);
	//void Render();

protected:

	//virtual void UpdateSimulation(float fDelta);
	//virtual void RenderScene();

//private:

};

#endif // WORLD3D_H
