#ifndef _RIFLE_H
#define _RIFLE_H 1

#include <irrlicht.h>
#include "../Physics/Physics.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Rifle
{
	public:

		Rifle();
		virtual ~Rifle();

		void setParent(ICameraSceneNode *node);

		void initialize(void);
		void setPosition(vector3df position);
		void setRotation(vector3df rotation);
		void setScale(vector3df scale);
		void shoot();
		void registerIrrlichtDevice(IrrlichtDevice &device);

	private:

		IrrlichtDevice *m_irrDevice;

		IAnimatedMesh *m_mesh;
		IAnimatedMeshSceneNode *m_node;

		vector3df m_position, m_rotation, m_scale;
		Physics *m_cPhysics;
};

#endif
