
#include "Scenery.h"

/* This is basically the same as a Platform
   but without collisions */

Scenery::Scenery(WorldBase *world,
const wchar_t *meshFileName, core::vector3df pos, core::vector3df rot)
    : Object(world)
{
    setFileName(meshFileName);
    
    setPosition(pos);
    setRotation(rot);
}

Object *Scenery::createNew()
{
    return new Scenery(this);
}

void Scenery::setFileName(const wchar_t *fileName)
{
    Object::setFileName(fileName);
    createMesh();
}

void Scenery::createMesh()
{
    core::vector3df pos,rot; // Default positions (0,0,0)
    
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
    
    if (!sceneNode)
    {
        sceneNode = smgr->addCubeSceneNode(1.0);
    }
    
    // transform to old location
    setPosition(pos);
    setRotation(rot);
}

/* Left as an example.
void Scenery::serializeToXML(XMLNode &xObject)
{
    XMLNode xPlatform = xObject.addChild(OBJECT_TYPE_STRINGS[getType()]);
    xPlatform.addAttribute(L"meshFileName", meshFileName.c_str());
    
    Object::serializeToXML(xObject);
}

void Scenery::deserializeFromXML(XMLNode &xObject)
{
    XMLNode xPlatform = xObject.getChildNode(OBJECT_TYPE_STRINGS[getType()], 0);
    
    setMesh( xPlatform.getAttribute(L"meshFileName", 0) );
    
    // Position etc...
    Object::deserializeFromXML(xObject);
}
*/


