#include <irrlicht.h>
#include <iostream>
#include "hud.h"
#include "Logger.h"
using namespace irr;
namespace HUD{
    void HUD::initHUD(){
                this->font = this->env->getFont("./media/font/myfont.xml");
                if(this->font)
                    this->skin->setFont(this->font);
                /*ADD TEXTS*/
                this->playerName = this->env->addStaticText(L"", core::rect<s32>(1,1,250,50),false,false,0,INFO_PLAYER);
                this->powertext = this->env->addStaticText(L"Power", core::rect<s32>(5,600-32,150,600),false,false,0,POWER_TEXT);
                /*ADD IMAGE-BASED elements*/
                video::ITexture* image = this->driver->getTexture("media/power.png");
                core::rect<s32> pos = this->powertext->getAbsoluteClippingRect();
                core::rect<s32> pos_power = core::rect<s32>(
                    core::position2di(((pos.UpperLeftCorner.X+pos.LowerRightCorner.X)/4),pos.UpperLeftCorner.Y-image->getSize().Height+150),
                    core::position2di(((pos.UpperLeftCorner.X+pos.LowerRightCorner.X)/4)+50,pos.LowerRightCorner.Y-45));
                this->power_rect = pos_power;
                pos_power.UpperLeftCorner.Y = pos_power.LowerRightCorner.Y-1;
                this->initialPosPower = pos_power;
                this->power = this->env->addImage(pos_power,0,POWER_BAR);
                this->power->setImage(image);
                this->power->setScaleImage(true);
                this->powerRatio = 0;
    }
    HUD::HUD(IrrlichtDevice* device, video::IVideoDriver* video){
                this->device = device;
                this->env = device->getGUIEnvironment();
                this->skin = this->env->getSkin();
                this->driver = video;
                this->initHUD();

    }
    void HUD::animatePower(){
                core::rect<s32> power_pos = this->power->getAbsoluteClippingRect();

                if(this->velocity > 0){
                power_pos.UpperLeftCorner.Y = power_pos.UpperLeftCorner.Y * this->velocity;
                if(power_pos.UpperLeftCorner.Y >= this->power_rect.LowerRightCorner.Y) this->velocity = SPEED_LOW;
                }
                else
                     power_pos.UpperLeftCorner.Y = power_pos.UpperLeftCorner.Y * this->velocity;
                if(power_pos.UpperLeftCorner.Y <= this->power_rect.UpperLeftCorner.Y) this->velocity =  SPEED_HIGH;
                this->power->setRelativePosition(power_pos);
                core::rect<s32> rect  = this->power->getAbsolutePosition();
                this->powerRatio = ((f32)rect.getHeight() / (f32)MAX_HEIGHT);
                log2p("Power: ", this->powerRatio);

        }

    f32 HUD::getPower(){
        log1p("Power bar: ", this->powerRatio);
        return (this->powerRatio > 1) ? 1 : this->powerRatio;
    }

    void HUD::setPlayerName(core::stringw name){
        if(this->playerName)
            this->playerName->setText(name.c_str());
    }
    void HUD::resetPower(){
        // core::rect<s32> power_pos = this->power->getAbsoluteClippingRect();
        // power_pos.UpperLeftCorner.Y = power_pos.LowerRightCorner.Y-1;
        this->power->setRelativePosition(core::rect<s32>(this->initialPosPower));
    }
}
