#include "Rifle.h"

Rifle::Rifle()
{
	m_node = NULL;
}

Rifle::~Rifle()
{

}

void Rifle::registerIrrlichtDevice(IrrlichtDevice &device)
{
	m_irrDevice = &device;
}

void Rifle::initialize(void)
{
	m_mesh = m_irrDevice->getSceneManager()->getMesh("data/models/weapons/Rifle_03.obj");
	m_node = m_irrDevice->getSceneManager()->addAnimatedMeshSceneNode(m_mesh);
//	m_node->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/models/weapons/Rifle_S073_CLR.bmp"));
	m_node->setMaterialFlag(EMF_LIGHTING, false);
}

void Rifle::setParent(ICameraSceneNode *node)
{
	if((m_node != NULL) && (node != NULL))
	m_node->setParent(node);
}

void Rifle::setPosition(vector3df position)
{
	m_node->setPosition(position);
	m_node->updateAbsolutePosition();
}

void Rifle::shoot()
{
	m_cPhysics->createBox(btVector3(0,-100,30), btVector3(50,50,50), 0.0f);
}

void Rifle::setRotation(vector3df rotation)
{
	m_node->setRotation(rotation);
	m_node->updateAbsolutePosition();
}

void Rifle::setScale(vector3df scale)
{
	m_node->setScale(scale);
	m_node->updateAbsolutePosition();
}
