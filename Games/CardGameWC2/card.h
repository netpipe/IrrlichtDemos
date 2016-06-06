#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include "cardgame.h"

class GameManager;

enum E_CARD_SUIT {ECS_NULL=0, ECS_DIAMONDS, ECS_CLUBS, ECS_HEARTS, ECS_SPADES, ECS_COUNTER, ECS_FORCE_8_BIT=255};
enum E_CARD_NUMBER {ECN_NULL=0, ECN_COUNTER=14, ECN_FORCE_8_BIT=255};

struct Face
{
    E_CARD_NUMBER value;
    E_CARD_SUIT suit;
};

class card
{
    GameManager* manager;
    irr::core::stringc tex_front_file;
    irr::video::ITexture* tex_front;
    irr::video::ITexture* tex_back;
    irr::scene::IBillboardSceneNode* node;
    irr::u32 ID;
    static irr::u32 numID;
    bool shown;
    bool selected;
    Face value;

public:
    card(GameManager* Manager, Face Value);

    Face getFace();
    bool isShown();
    bool isSelected();
    irr::u32 getID();
    irr::scene::IBillboardSceneNode* getNode();

    void flip();
    void select();
    void setPosition(irr::f32 X, irr::f32 Y, irr::f32 Z);
    irr::f32 getDepth();

protected:
    void setFace(Face Value);
};

#endif
