#ifndef CSPRING_H_INCLUDED
#define CSPRING_H_INCLUDED

#include <IrrNewt.hpp>
#include <irrlicht.h>
using namespace irr;

class CSpring{
private:
    newton::IBody *Body_Parent;
    newton::IBody *Body_Child;
    f32 Strength;
    f32 Length;
public:
    CSpring();
    CSpring(newton::IBody *parent,newton::IBody *child,f32 strength=1,f32 length = -1);
    ~CSpring();
    void update();
    void setStrength(f32 pstrength);
    f32 getStrength();
    void setLength(f32 plength);
    f32 getLength();
    void setParent(newton::IBody *pparent);
    newton::IBody *getParent();
    void setChild(newton::IBody *pchild);
    newton::IBody *getChild();
    void drawDebugData(video::IVideoDriver *driver);

};

#endif // CSPRING_H_INCLUDED
