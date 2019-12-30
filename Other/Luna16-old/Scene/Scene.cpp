#include "Scene.h"

Scene::Scene()
{
  m_irrDevice = NULL;
}

Scene::~Scene()
{

}

void Scene::registerIrrDevice(IrrlichtDevice &device)
{
  m_irrDevice = &device;
}

void Scene::setupLights(void)
{
  if(m_irrDevice == NULL)
  {
    printf("Tried to setup lights, but IrrlichtDevice is NULL.\n");
    return;
  }

  m_lightFront = m_irrDevice->getSceneManager()->addLightSceneNode(
    0, // parent
    CONST_LIGHT_FRNT_POS, // position
    CONST_LIGHT_FRNT_COL, // color
    100.0f, // radius
    -1 // id
    );

  m_lightBack = m_irrDevice->getSceneManager()->addLightSceneNode(
    0, // parent
    CONST_LIGHT_BACK_POS, // position
    CONST_LIGHT_BACK_COL, // color
    100.0f, // radius
    -1 // id
    );

  m_lightFill = m_irrDevice->getSceneManager()->addLightSceneNode(
    0, // parent
    CONST_LIGHT_FILL_POS, // position
    CONST_LIGHT_FILL_COL, // color
    100.0f, // radius
    -1 // id
    );
}

void Scene::setGenericMaterial(IAnimatedMeshSceneNode *node, s32 materialIndex)
{
  node->getMaterial(materialIndex).Shininess     = CMT_DEFAULT_SHININESS;
  node->getMaterial(materialIndex).AmbientColor  = CMT_DEFAULT_AMBIENT;
  node->getMaterial(materialIndex).DiffuseColor  = CMT_DEFAULT_DIFFUSE;
  node->getMaterial(materialIndex).EmissiveColor = CMT_DEFAULT_EMISSIVE;
  node->getMaterial(materialIndex).SpecularColor = CMT_DEFAULT_SPECULAR;

  node->getMaterial(materialIndex).Lighting        = CMT_DEFAULT_LIGHTING;
  node->getMaterial(materialIndex).GouraudShading  = CMT_DEFAULT_GOURAUD;

  node->getMaterial(materialIndex).BackfaceCulling   = CMT_DEFAULT_BCKFACECULL;
  node->getMaterial(materialIndex).NormalizeNormals  = CMT_DEFAULT_NRMNORMALS;

  node->getMaterial(materialIndex).FogEnable = CMT_DEFAULT_FOGENABLE;

/*
  &node->getMaterial(materialIndex).MaterialType =
  &node->getMaterial(materialIndex).MaterialTypeParam =
  &node->getMaterial(materialIndex).MaterialTypeParam2 =

  &node->getMaterial(materialIndex).PointCloud
  &node->getMaterial(materialIndex).TextureLayer
  &node->getMaterial(materialIndex).Thickness
  &node->getMaterial(materialIndex).Wireframe
  &node->getMaterial(materialIndex).ZBuffer
  &node->getMaterial(materialIndex).ZWriteEnable
*/
}

void Scene::setGenericMaterial(ITerrainSceneNode *node, s32 materialIndex)
{
  node->getMaterial(materialIndex).Shininess     = CMT_DEFAULT_SHININESS;
  node->getMaterial(materialIndex).AmbientColor  = CMT_DEFAULT_AMBIENT;
  node->getMaterial(materialIndex).DiffuseColor  = CMT_DEFAULT_DIFFUSE;
  node->getMaterial(materialIndex).EmissiveColor = CMT_DEFAULT_EMISSIVE;
  node->getMaterial(materialIndex).SpecularColor = CMT_DEFAULT_SPECULAR;

  node->getMaterial(materialIndex).Lighting        = CMT_DEFAULT_LIGHTING;
  node->getMaterial(materialIndex).GouraudShading  = CMT_DEFAULT_GOURAUD;

  node->getMaterial(materialIndex).BackfaceCulling   = CMT_DEFAULT_BCKFACECULL;
  node->getMaterial(materialIndex).NormalizeNormals  = CMT_DEFAULT_NRMNORMALS;

  node->getMaterial(materialIndex).FogEnable = CMT_DEFAULT_FOGENABLE;
}
