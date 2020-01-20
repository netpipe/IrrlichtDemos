#include "CSpring.h"

CSpring::CSpring(){
    Body_Parent = 0;
    Body_Child = 0;
    Strength = 1;
    Length = 0;
}

CSpring::CSpring(newton::IBody *parent,newton::IBody *child,f32 strength,f32 length){
    Body_Parent = parent;
    Body_Child = child;
    Strength = strength;
    Length = length;
    if(Length<0){
        if(Body_Parent!=0 && Body_Child!=0){
            core::line3d<f32> line;
            line.start = Body_Parent->getNode()->getAbsolutePosition();
            line.end = Body_Child->getNode()->getAbsolutePosition();
            Length = line.getLength();
        }
    }
}

CSpring::~CSpring(){
    Body_Parent = 0;
    Body_Child = 0;
}

void CSpring::update(){
    if(Body_Child!=0 && Body_Parent!=0){
        core::line3d<f32> line;
        line.start = Body_Parent->getNode()->getAbsolutePosition();
        line.end = Body_Child->getNode()->getAbsolutePosition();
        core::vector3df force_direction = line.getVector().normalize();
        f32 curlength = line.getLength();
        f32 factor = curlength-Length;
        core::vector3df force = force_direction*factor;

        Body_Parent->addGlobalForce(Strength*force,line.end);
        Body_Child->addGlobalForce(-Strength*force,line.end);
    }
}

void CSpring::setStrength(f32 pstrength){
    Strength = pstrength;
}

f32 CSpring::getStrength(){
    return Strength;
}

void CSpring::setLength(f32 plength){
    Length = plength;
}

f32 CSpring::getLength(){
    return Length;
}

void CSpring::setParent(newton::IBody *pparent){
    Body_Parent = pparent;
}

newton::IBody *CSpring::getParent(){
    return Body_Parent;
}

void CSpring::setChild(newton::IBody *pchild){
    Body_Child = pchild;
}

newton::IBody *CSpring::getChild(){
    return Body_Child;
}

void CSpring::drawDebugData(video::IVideoDriver *driver){
    if(Body_Child!=0 && Body_Parent!=0){
        video::SMaterial material;
        material.Lighting = false;
        material.Thickness = 3;
        driver->setMaterial(material);
        driver->setTransform(video::ETS_WORLD,core::matrix4());
        driver->draw3DLine(Body_Parent->getNode()->getAbsolutePosition(),Body_Child->getNode()->getAbsolutePosition(),video::SColor(255,255,255,0));
    }
}
