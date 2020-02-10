
#ifndef __Lift_h
#define __Lift_h

#include "Object.h"



class Lift : public Object
{
public:
    Lift(WorldBase *, LocRot = LocRot(), LocRot = LocRot());
    
    Object *createNew();
    
    void setStartPoint(LocRot);
    void setEndPoint(LocRot);
    
    void update(f32);
    
    virtual OBJECT_TYPE getType() {return OBJECT_TYPE_LIFT; }
    
private:
    
    enum // Control point indices
    {
        START = 0,
        END = 1
    };
    
    static const f32 speed = 2.0;
    
    bool moving;
    bool goingStartToEnd;
    bool actorGotOff;
};

#endif
