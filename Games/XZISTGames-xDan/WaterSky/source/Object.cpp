
#include "Object.h"
#include "Shader.h"

// Indices are in OBJECT_TYPE enum
const wchar_t *OBJECT_TYPE_STRINGS[] =
{
    L"platform",
    L"scenery",
    L"generator",
    L"fuelCan",
    L"lift",
    L"block",
    L"creature",
    // Objects which cannot be created by editor
    L"actor",
    L"player"
};

OBJECT_TYPE OBJECT_TYPE_FROM_STRING(const wchar_t *str)
{
    for (int i = 0; i < OBJECT_COUNT; i ++)
    {
        if ( wcscmp(OBJECT_TYPE_STRINGS[i], str) == 0) return (OBJECT_TYPE)i;
    }
    printf("ERROR: OBJECT_TYPE_FROM_STRING: Invalid object!\n");
    return OBJECT_TYPE_ERROR;
}

Object::Object(WorldBase *world)
    : WorldBase(world)
{
    sceneNode = NULL;
    dAnimator = NULL;
    soundAnimator = NULL;
    
    newID();
    
    // add to World's object list.
    objects->push_back(this);
    
    // everything active by default
    active = true;
    activatableExclamationMark = NULL;
    exclamationMarkGrowing = false;
    
    // Unchangeable point count by default
    limitedPointCount = true;
}

Object::~Object()
{
    remove();
}

void Object::remove()
{
    sceneNode->removeAnimators();
    sceneNode->remove();
    sceneNode = NULL;
    
    // remove from World's Object list
    for (unsigned int i = 0; i < objects->size(); i ++)
    {
        if ( (*objects)[i] == this)
        {
            objects->erase(i);
            break;
        }
    }
}

u32 Object::newID()
{
    // get a unique non zero id
    id = rand()+1;
    for (u32 i = 0; i < objects->size(); i ++)
    {
        if ( (*objects)[i] != this && (*objects)[i]->getID() == id )
        {
            id = rand()+1;
            i = 0;
        }
    }
    return id;
}

u32 Object::getID()
{
    return id;
}

core::vector3df Object::getPosition()
{
    if (dAnimator) dAnimator->putTransformation(sceneNode);
    sceneNode->updateAbsolutePosition();
    return sceneNode->getPosition();
}

core::vector3df Object::getRotation()
{
    if (dAnimator) dAnimator->putTransformation(sceneNode);
    sceneNode->updateAbsolutePosition();
    return sceneNode->getRotation();
}

void Object::setPosition(core::vector3df pos)
{
    sceneNode->setPosition(pos);
    if (dAnimator) dAnimator->takeTransformation(sceneNode);
    sceneNode->updateAbsolutePosition();
}

void Object::setRotation(core::vector3df rot)
{
    sceneNode->setRotation(rot);
    if (dAnimator) dAnimator->takeTransformation(sceneNode);
    sceneNode->updateAbsolutePosition();
}

scene::ISceneNode *Object::getNode()
{
    return sceneNode;
}

void Object::preUpdate(f32 dt)
{
    // Shader Test
    if (sceneNode) sceneNode->setMaterialType((video::E_MATERIAL_TYPE)SHADER_MATERIAL_STANDARD);
    
    // check they are all valid, remove any that no longer exist.
    pruneActivatedByObjects();
    
    // All activated-by objects are now valid, so can perform checks...
    // If none exist then active state is unchanged.
    if (activatedByObjects.size())
    {
        active = true;
        for (unsigned int i = 0; i < activatedByObjects.size(); i ++)
        {
            if (!getObject(activatedByObjects[i])->isActive())
            {
                active = false;
                break;
            }
        }
    }
    if (activatableExclamationMark)
    {
        bool nextVisible = false;
        Object *actor = getNearestObject(OBJECT_TYPE_PLAYER);
        if (actor)
        {
            nextVisible = getBoundsDistance(actor) < exclamationMarkDisplayDistance;
        }
        
        nextVisible = nextVisible && !active;
        
        // Grow
        if (!activatableExclamationMark->isVisible() && nextVisible)
        {
            exclamationMarkGrowing = true;
        }
        
        // Shrink
        if (activatableExclamationMark->isVisible() && !nextVisible)
        {
            exclamationMarkGrowing = false;
        }
        
        core::vector3df scale = activatableExclamationMark->getScale();
        core::vector3df scaleDelta = core::vector3df(1,1,1)*6.0*dt;
        
        if (exclamationMarkGrowing)
        {
            activatableExclamationMark->setVisible(true);
            if (scale.Y < 0.0)
            {
                scale.set(0,0,0);
            }
            if (scale.Y < 1.0)
            {
                activatableExclamationMark->setScale(scale+scaleDelta);
            }
        }
        else
        {
            if (scale.Y > 0.0)
            {
                scale -= scaleDelta;
                if (scale.Y < 0.0) scale.set(0,0,0);
                activatableExclamationMark->setScale(scale);
            }
            else
            {
                activatableExclamationMark->setVisible(false);
            }
        }
    }
}

void Object::activatedBy(u32 otherID)
{
    assert(otherID != id);
    
    Object *obj = getObject(otherID);
    
    if (!obj)
    {
        printf("activatedBy: invalid object ID\n");
        return;
    }
    
    if (!isActivatedBy(otherID))
    {
        activatedByObjects.push_back(otherID);
    }
}

void Object::activatedBy(Object *other)
{
    activatedBy(other->getID());
}

void Object::clearActivatedBy()
{
    activatedByObjects.clear();
}

void Object::pruneActivatedByObjects()
{
    for (u32 i = 0; i < activatedByObjects.size(); i ++)
    {
        if (!getObject(activatedByObjects[i]))
        {
            activatedByObjects.erase(i);
            i --;
        }
    }
}

bool Object::isActivatedBy(u32 otherID)
{
    for (u32 i = 0; i < activatedByObjects.size(); i ++)
    {
        if (otherID == activatedByObjects[i]) return true;
    }
    return false;
}

void Object::reservePoints(u32 count)
{
    while (points.size() < count)
    {
        points.push_back(LocRot());
    }
}

void Object::addPoint(LocRot point)
{
    points.push_back(point);
}

void Object::setPoint(u32 index, LocRot point)
{
    assert(index < points.size());
    points[index] = point;
}

LocRot &Object::getPoint(u32 index)
{
    assert(index < points.size());
    return points[index];
}

void Object::renderRelations(bool isSelected, s32 activePoint)
{
    // Render any activated-by relationships
    
    video::SMaterial matArrow;
    matArrow.MaterialType = video::EMT_SOLID;
    //matArrow.Lighting = false;
    matArrow.Wireframe = true; // then it doesn't interfere with selection...
    driver->setMaterial( matArrow );
    
    const char *arrowName = "activatedByArrowMesh";
    
    smgr->addArrowMesh(arrowName,
        video::SColor(255, 200,200,200),
        video::SColor(255, 200,200,200),
        4, // cylinder
        4, // cone
        1.0, // height
        0.5, // cylinder height
        0.2, // width 1
        0.2 // width 2
        );
    
    scene::IMesh *arrow = smgr->getMesh(arrowName)->getMesh(0);
    
    for (u32 i = 0; i < activatedByObjects.size(); i ++)
    {
        Object *obj = getObject(activatedByObjects[i]);
        core::vector3df scale = core::vector3df(1,(obj->getPosition()-getPosition()).getLength(),1);
        
        // Get rotation
        core::matrix4 rot;
        core::vector3df up = (obj->getPosition()-getPosition()).normalize();
        core::quaternion quatRot(up.Z, 0.f, -up.X, 1 + up.Y);
        quatRot.normalize();
        quatRot.getMatrix(rot);
        
        // Set rotation and translation
        core::matrix4 mat;
        mat.setRotationDegrees( rot.getRotationDegrees() );
        mat.setTranslation( getPosition() );
        
        // Set scale
        core::matrix4 smat;
        smat.setScale(scale);
        mat *= smat;
        
        driver->setTransform(video::ETS_WORLD, mat);
        
        for (u32 j = 0; j < arrow->getMeshBufferCount(); j ++)
        {
            driver->drawMeshBuffer( arrow->getMeshBuffer(j) );
        }
    }
    
    // Render (wireframe) mesh at all control points IF SELECTED
    
    if (!isSelected) return;
    
    // If we had none animated mesh scene nodes (e.g. a particle system on its own), would need
    // to create+render some sort of debug mesh.
    if (sceneNode->getType() != scene::ESNT_ANIMATED_MESH) return;
    
    scene::IMesh *mesh = ((scene::IAnimatedMeshSceneNode *)sceneNode)->getMesh()->getMesh(0);
    
    video::SMaterial matTrans;
    matTrans.MaterialType = video::EMT_SOLID;
    matTrans.Lighting = false;
    matTrans.Wireframe = true;
    driver->setMaterial( matTrans );
    
    core::matrix4 mat = sceneNode->getAbsoluteTransformation();
    
    for (u32 i = 0; i < points.size(); i ++)
    {
        mat.setTranslation( points[i].pos );
        mat.setRotationDegrees( points[i].rot );
        driver->setTransform(video::ETS_WORLD, mat);
        
        if ((s32)i == activePoint-1) // point is > 0 (a control point, not object position or none)
        {
            matTrans.Thickness = 3.0;
            matTrans.Lighting = false;
        }
        else
        {
            matTrans.Thickness = 1.0;
            matTrans.Lighting = true;
        }
        
        driver->setMaterial( matTrans );
    
        for (u32 j = 0; j < mesh->getMeshBufferCount(); j ++)
        {
            driver->drawMeshBuffer( mesh->getMeshBuffer(j) );
        }
    }
}

Object *Object::clone()
{
    XMLNode xTemp = XMLNode::createXMLTopNode(L"object");
    serializeToXML(xTemp);
    
    Object *newObj = createNew();
    newObj->deserializeFromXML(xTemp);
    newObj->newID(); // Keep a unique ID
    return newObj;
}

void Object::serializeToXML(XMLNode &xObject)
{
    xObject.addAttribute(L"type", OBJECT_TYPE_STRINGS[getType()]);
    xObject.addAttribute(L"id", core::stringw(id).c_str() );
    xObject.addAttribute(L"active", core::stringw(active).c_str() );
    // Position
    xObject.addAttribute(L"pos_x", core::stringw(getPosition().X).c_str() );
    xObject.addAttribute(L"pos_y", core::stringw(getPosition().Y).c_str() );
    xObject.addAttribute(L"pos_z", core::stringw(getPosition().Z).c_str() );
    // Rotation
    xObject.addAttribute(L"rot_x", core::stringw(getRotation().X).c_str() );
    xObject.addAttribute(L"rot_y", core::stringw(getRotation().Y).c_str() );
    xObject.addAttribute(L"rot_z", core::stringw(getRotation().Z).c_str() );
    
    // Data file name
    if (fileName.size()) xObject.addAttribute(L"file", fileName.c_str());
    
    // Points
    if (points.size())
    {
        XMLNode xPoints = xObject.addChild(L"points");
        for (u32 i = 0; i < points.size(); i ++)
        {
            XMLNode xPoint = xPoints.addChild(L"point");
            // Position
            xPoint.addAttribute(L"pos_x", core::stringw(points[i].pos.X).c_str() );
            xPoint.addAttribute(L"pos_y", core::stringw(points[i].pos.Y).c_str() );
            xPoint.addAttribute(L"pos_z", core::stringw(points[i].pos.Z).c_str() );
            // Rotation
            xPoint.addAttribute(L"rot_x", core::stringw(points[i].rot.X).c_str() );
            xPoint.addAttribute(L"rot_y", core::stringw(points[i].rot.Y).c_str() );
            xPoint.addAttribute(L"rot_z", core::stringw(points[i].rot.Z).c_str() );
        }
    }
    
    // activated-by
    if (activatedByObjects.size())
    {
        XMLNode xActivatedBy = xObject.addChild(L"activatedByObjects");
        for (u32 i = 0; i < activatedByObjects.size(); i ++)
        {
            XMLNode xOther = xActivatedBy.addChild(L"object");
            xOther.addAttribute(L"id", core::stringw(activatedByObjects[i]).c_str());
        }
    }
}

// Should use set() methods rather than attributes directly, in case
// there is any extra setup stuff needing doing (e.g. loading a mesh)
void Object::deserializeFromXML(XMLNode &xObject)
{
    id = wcstol(xObject.getAttribute(L"id", 0), NULL,0);
    if (id == 0) printf("Error: deserializeFromXML: id is zero or not present\n");
    active = wcstol(xObject.getAttribute(L"active", 0), NULL,0);
    // Position
    setPosition(core::vector3df(
        wcstod(xObject.getAttribute(L"pos_x", 0), NULL),
        wcstod(xObject.getAttribute(L"pos_y", 0), NULL),
        wcstod(xObject.getAttribute(L"pos_z", 0), NULL)
        ));
    // Rotation
    setRotation(core::vector3df(
        wcstod(xObject.getAttribute(L"rot_x", 0), NULL),
        wcstod(xObject.getAttribute(L"rot_y", 0), NULL),
        wcstod(xObject.getAttribute(L"rot_z", 0), NULL)
        ));
    
    // Data file name
    setFileName(xObject.getAttribute(L"file", 0));
    
    // Points
    points.clear();
    XMLNode xPoints = xObject.getChildNode(L"points", 0);
    int pointCount = xPoints.nChildNode(L"point");
    for (int i = 0; i < pointCount; i ++)
    {
        XMLNode xPoint = xPoints.getChildNode(L"point", i);
        addPoint(LocRot(
            core::vector3df(
                wcstod(xPoint.getAttribute(L"pos_x", 0), NULL),
                wcstod(xPoint.getAttribute(L"pos_y", 0), NULL),
                wcstod(xPoint.getAttribute(L"pos_z", 0), NULL)
                ),
            core::vector3df(
                wcstod(xPoint.getAttribute(L"rot_x", 0), NULL),
                wcstod(xPoint.getAttribute(L"rot_y", 0), NULL),
                wcstod(xPoint.getAttribute(L"rot_z", 0), NULL)
                )
            ));
    }
    
    // Activated-by
    activatedByObjects.clear();
    XMLNode xActivatedBy = xObject.getChildNode(L"activatedByObjects", 0);
    int activatedBySize = xActivatedBy.nChildNode(L"object");
    for (int i = 0; i < activatedBySize; i ++)
    {
        XMLNode xOther = xActivatedBy.getChildNode(L"object", i);
        activatedByObjects.push_back(wcstol(xOther.getAttribute(L"id"), NULL,0));
    }
}

void Object::addActivatableExclamationMark()
{
    if (activatableExclamationMark) return;
    
    assert(sceneNode);
    
    activatableExclamationMark = smgr->addAnimatedMeshSceneNode(
        smgr->getMesh("exclaim.b3d"),
        sceneNode,
        -1,
        core::vector3df(0, sceneNode->getBoundingBox().MaxEdge.Y+exclamationMarkHoverHeight, 0)
        );
    
    activatableExclamationMark->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    
    activatableExclamationMark->setScale(core::vector3df(0,0,0));
    activatableExclamationMark->setVisible(false);
    exclamationMarkGrowing = false;
    
}

void Object::createSoundAnimator()
{
    if (!sceneNode)
    {
        printf("Error:Object::addSoundAnimator: No scene node.\n");
        return;
    }
    soundAnimator = new SoundAnimator(sengine);
    sceneNode->addAnimator(soundAnimator);
    soundAnimator->drop();
}

void Object::removeObject(Object *other)
{
    assert(other != this);
    
    // Why do we have loop here :/ Can just delete other??
    for (unsigned int i = 0; i < objects->size(); i ++)
    {
        if ( (*objects)[i] == other )
        {
            delete (*objects)[i]; // (removes itself from list in deconstructor)
            return;
        }
    }
}

f32 Object::getBoundsDistance(Object *other)
{
    scene::ISceneNode *otherNode = other->getNode();
    
    core::aabbox3df box1 = sceneNode->getBoundingBox();
    core::aabbox3df box2 = otherNode->getBoundingBox();
    
    return sceneNode->getAbsolutePosition().getDistanceFrom(other->getPosition())
        - (box1.MaxEdge - box1.getCenter()).getLength()
        - (box2.MaxEdge - box2.getCenter()).getLength();
}

Object *Object::getNearestObject()
{
    return getNearestObject(sceneNode->getPosition());
}

Object *Object::getNearestObject(OBJECT_TYPE type)
{
    return getNearestObject(sceneNode->getPosition(), type);
}

Object *Object::getNearestObject(core::vector3df pos)
{
    Object *found = NULL;
    f32 lastDist;
    for (unsigned int i = 0; i < objects->size(); i ++)
    {
        f32 dist = (*objects)[i]->getPosition().getDistanceFrom(pos);
        
        if (dist < lastDist || i == 0)
        {
            found = (*objects)[i];
            lastDist = dist;
        }
    }
    
    return found;
}

Object *Object::getNearestObject(core::vector3df pos, OBJECT_TYPE type)
{
    Object *found = NULL;
    f32 lastDist;
    int typeCount = 0;
    for (unsigned int i = 0; i < objects->size(); i ++)
    {
        if ( (*objects)[i]->getType() == type)
        {
            f32 dist = (*objects)[i]->getPosition().getDistanceFrom(pos);
            
            if (dist < lastDist || typeCount == 0)
            {
                found = (*objects)[i];
                lastDist = dist;
            }
            
            typeCount ++;
        }
    }
    
    return found;
}

Object *Object::getObject(u32 id)
{
    for (u32 i = 0; i < objects->size(); i ++)
    {
        if ( (*objects)[i]->getID() == id )
        {
            return (*objects)[i];
        }
    }
    return NULL;
}

