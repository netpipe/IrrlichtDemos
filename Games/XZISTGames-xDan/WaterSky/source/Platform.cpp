
#include "Platform.h"
#include "ODEMeshAnimator.h"

/* Same as Scenery but with collisions (ODE Geometry) */

Platform::Platform(WorldBase *world,
const wchar_t *meshFileName, core::vector3df pos, core::vector3df rot)
    : Object(world)
{
    setFileName(meshFileName);
    
    // transform both scene node and ODE geometry
    setPosition(pos);
    setRotation(rot);
}

Object *Platform::createNew()
{
    return new Platform(this);
}

void Platform::setFileName(const wchar_t *fileName)
{
    Object::setFileName(fileName);    
    createMesh();    
}    

void Platform::createMesh()
{
    core::vector3df pos,rot;
    
    if (sceneNode)
    {
        pos = getPosition();
        rot = getRotation();
        sceneNode->remove();
        sceneNode = NULL;
    }
    
    if (fileName.size())
    {
        sceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh( core::stringc(fileName.c_str()).c_str() ));
    }
    
    if (sceneNode)
    {
        // add a static collision mesh
        
        // See if there is a separate (low poly) collision mesh...
        // (scene node mesh filename plus ".collision")
        scene::IAnimatedMesh *dMesh = smgr->getMesh( core::stringc( (fileName+L".collision").c_str()).c_str() );
        
        // If not, then use the scene node mesh.
        if (!dMesh) dMesh = ((scene::IAnimatedMeshSceneNode *)sceneNode)->getMesh();
        
        dAnimator = new ODEMeshAnimator( sceneNode, dMesh, dPhysics, 0);
        sceneNode->addAnimator(dAnimator);
        dAnimator->drop();
    }
    else
    {
        sceneNode = smgr->addCubeSceneNode(1.0);
    }
    
    // transform both scene node and ODE geometry to old location
    setPosition(pos);
    setRotation(rot);
}

