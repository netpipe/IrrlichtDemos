#include "card.h"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

card::card(GameManager* Manager, Face Value):manager(Manager),value(Value)
{
    tex_front_file = "./media/front_";
    tex_front_file += (u32)value.suit;
    tex_front_file += '_';
    tex_front_file += (u32)value.value;
    tex_front_file += ".png";

    tex_back = manager->Device()->getVideoDriver()->getTexture("./media/back.png");
    tex_front = manager->Device()->getVideoDriver()->getTexture( tex_front_file.c_str() );

  //  ID = (u32)this;
    //card::numID++;

    node = manager->Device()->getSceneManager()->addBillboardSceneNode(0, dimension2d<f32>(.25*2.5, .35*2.5), vector3df(0,-3,0), -1 );
    if(node)
    {
        node->setMaterialTexture(0, tex_back);
        node->setMaterialFlag(EMF_LIGHTING, true);
        node->getMaterial(0).AmbientColor = SColor(255, 150, 150, 150);
    }

    shown = false;
    selected = false;
}

void card::flip()
{
    if(shown)
    {
        node->setMaterialTexture(0, tex_back);
        shown=false;
    }
    else
    {
        node->setMaterialTexture(0, tex_front);
        shown=true;
    }

    node->getMaterial(0).AmbientColor = SColor(255, 150, 150, 150);
    node->setMaterialFlag(EMF_LIGHTING, true);
}

Face card::getFace()
{
    return value;
}

u32 card::getID()
{
    return ID;
}

IBillboardSceneNode* card::getNode()
{
    return node;
}

bool card::isShown()
{
    return shown;
}

void card::setFace(Face Value)
{
    value = Value;

    tex_front_file = "./media/front_";
    tex_front_file += (u32)value.suit;
    tex_front_file += '_';
    tex_front_file += (u32)value.value;
    tex_front_file += ".png";

    tex_front->drop();
    tex_front = manager->Device()->getVideoDriver()->getTexture( tex_front_file.c_str() );

    if(shown)
    {
        node->setMaterialTexture(0, tex_front);
        node->getMaterial(0).AmbientColor = SColor(255, 150, 150, 150);
        node->setMaterialFlag(EMF_LIGHTING, true);
    }
}

bool card::isSelected()
{
    return selected;
}

void card::select()
{
    if(selected)
    {
        selected = false;
        node->getMaterial(0).AmbientColor = SColor(255, 150, 150, 150);
    }
    else
    {
        selected = true;
        node->getMaterial(0).AmbientColor = SColor(255, 255, 255, 255);
    }
}

void card::setPosition(f32 X, f32 Y, f32 Z)
{
    node->setPosition( vector3df(X, Y, Z) );
}

f32 card::getDepth()
{
    f32 z = node->getPosition().Z;
    return z;
}
