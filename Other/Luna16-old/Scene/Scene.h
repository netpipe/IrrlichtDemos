#ifndef _SCENE_H_
#define _SCENE_H_ 1

#include <irrlicht.h>
#include "../constants.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Scene
{
  public:
    Scene();
    virtual ~Scene();

    void registerIrrDevice(IrrlichtDevice &device);

    //! Adds a 3-point light setup to the scenemanager, using front, back and fill lights.
    void setupLights(void);

    //! Sets default material attributes to a IAnimatedMeshSceneNode. Look in constants.h to change the variables.
    /** \param node: Node to set material attributes on.
        \param materialIndex: Which material on the node to change attributes on. */
    void setGenericMaterial(IAnimatedMeshSceneNode *node, s32 materialIndex);

    //! Sets default material attributes to a ITerrainSceneNode. Look in constants.h to change the variables.
    /** \param node: Node to set material attributes on.
        \param materialIndex: Which material on the node to change attributes on. */
    void setGenericMaterial(ITerrainSceneNode *node, s32 materialIndex);

  private:
    IrrlichtDevice *m_irrDevice;

    //! Front light, also referred to as the main light.
    ILightSceneNode *m_lightFront;

    //! Back light, should be opposite of the main light apart from Y-axis orientation.
    ILightSceneNode *m_lightBack;

    //! Fill light, a side light that acts as an ambient fill.
    ILightSceneNode *m_lightFill;
};

#endif
