#include "water.h"

//HealthCell *HealthCell::m_healthcell = NULL;
//HealthCell HealthCell::m_default_healthcell_buffer;


WaterNode *WaterNode::Instance()
{
//  if(!m_healthcell)
 //   m_healthcell = &m_default_healthcell_buffer;

  return m_WaterNode;
}

//void HealthCell::Add(vector3df position)
//{
 // ISceneNode *sphere = irrDevice->getSceneManager()
