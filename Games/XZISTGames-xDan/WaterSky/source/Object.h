
#ifndef __Object_h
#define __Object_h

#include "WorldBase.h"
#include "ODEAnimator.h"
#include "SoundAnimator.h"

// EACH OF THESE MUST BE A VALID INDEX INTO OBJECT_TYPE_STRINGS in Object.cpp!
// For an object to be usable it must be defined in the XML language file and
// also have an associated case in Editor.createObject()
// These indices can be modified providing: indices 0 to OBJECT_COUNT-1 are always present
// and after modification the project is rebuilt.
enum OBJECT_TYPE
{
    OBJECT_TYPE_ERROR = -1,
    // Objects which CAN be created by editor
    OBJECT_TYPE_PLATFORM = 0,
    OBJECT_TYPE_SCENERY = 1,
    OBJECT_TYPE_GENERATOR = 2,
    OBJECT_TYPE_FUELCAN = 3,
    OBJECT_TYPE_LIFT = 4,
    OBJECT_TYPE_BLOCK = 5,
    OBJECT_TYPE_CREATURE = 6,
    // Objects which cannot be created by editor
    OBJECT_TYPE_ACTOR = 7,
    OBJECT_TYPE_PLAYER = 8,
};
const int OBJECT_COUNT = 9;

extern const wchar_t *OBJECT_TYPE_STRINGS[];
extern OBJECT_TYPE OBJECT_TYPE_FROM_STRING(const wchar_t *);

struct LocRot
{
    core::vector3df pos,rot;

    LocRot()
    {
    }
    LocRot(core::vector3df pos, core::vector3df rot)
    {
        this->pos = pos;
        this->rot = rot;
    }
};

class ObjectCreator
{
    ObjectCreator()
    {
    }
    // Object's type name is specified here.
    virtual Object *createNew() = 0;
};

class Object : public WorldBase
{
public:
    Object(WorldBase *);
    virtual ~Object();
    void remove();

    u32 newID();
    u32 getID();

    core::vector3df getPosition();
    core::vector3df getRotation();
    void setPosition(core::vector3df);
    void setRotation(core::vector3df);
    void setActive(bool f) { active = f; }
    bool isActive() { return active; }

    virtual void setFileName(const wchar_t *fileName) { this->fileName = fileName; }

    scene::ISceneNode *getNode();

    void preUpdate(f32);
    virtual void update(f32) {};

    ODEAnimator *getPhysicsAnimator() { return dAnimator; }

    void activatedBy(u32);
    void activatedBy(Object *);
    void clearActivatedBy();
    void pruneActivatedByObjects();
    bool isActivatedBy(u32);

    // control points
    bool isLimitedPoints() { return limitedPointCount; }
    u32 getPointCount() { return points.size(); }
    void reservePoints(u32);
    void addPoint(LocRot);
    void setPoint(u32, LocRot);
    LocRot &getPoint(u32);

    void renderRelations(bool,s32);

    Object *clone();
    virtual Object *createNew() = 0;
    virtual void serializeToXML(XMLNode &);
    virtual void deserializeFromXML(XMLNode &);

    virtual OBJECT_TYPE getType() = 0;

protected:
    scene::ISceneNode *sceneNode;
    ODEAnimator *dAnimator;
    SoundAnimator *soundAnimator;

    // activity...
    bool active;
    core::array <u32> activatedByObjects;

    // Only some objects will use this. If they require a mesh file or data file etc.
    core::stringw fileName;

    // Extra position/rotations - e.g. AI paths, lift stop/start locations
    core::array <LocRot> points;
    bool limitedPointCount;

    void addActivatableExclamationMark();
    void createSoundAnimator();

    // remove other object from World list. Any pointers now invalid...
    void removeObject(Object *);
    f32 getBoundsDistance(Object *);
    // from this object's scene node
    Object *getNearestObject();
    Object *getNearestObject(OBJECT_TYPE);
    // from a specified position
    Object *getNearestObject(core::vector3df);
    Object *getNearestObject(core::vector3df, OBJECT_TYPE);

    Object *getObject(u32);

private:
    const static f32 exclamationMarkDisplayDistance = 1.5;
    const static f32 exclamationMarkHoverHeight = 0.3;

    u32 id;
    scene::ISceneNode *activatableExclamationMark;
    bool exclamationMarkGrowing;
};

#endif
