#ifndef HUD_H
#define HUD_H
#include <irrlicht.h>
#include <iostream>
using namespace irr;
/** Graphic user interface namespace. */
namespace HUD{

    enum{ /*!< Graphic object enum.*/
    POWER_TEXT, /** POWER_TEXT not used */
    POWER_BAR, /** POWER_BAR is power shot bar */
    INFO_PLAYER /** INFO_PLAYER is player turn name */
    };

    const f32 SPEED_HIGH = 1.0098f; /*!< Speed rate when the power goes up */
    const f32 SPEED_LOW = 0.975f; /*!< Speed rate when the power goes down */
    const s32 MAX_HEIGHT = 237; /*!< Max height of the power bar */
    /** HUD of the gameplay. */
    class HUD{
        private:
            /** Gui skin */
            gui::IGUISkin* skin;
            /** Irrlicht device */
            IrrlichtDevice* device;
            /** Font. Default is set to media/font/SuperMario256.bmp */
            gui::IGUIFont* font;
            /** Player name */
            gui::IGUIStaticText* playerName;
            /** Power text below bar */
            gui::IGUIStaticText* powertext;
            /** Image power bar */
            gui::IGUIImage* power;
            /** Clipping power rectangle */
            core::rect<s32> power_rect;
            /** Irrlicht video driver instance */
            video::IVideoDriver* driver;
            /** Speed of bar default is ::SPEED_LOW */
            f32 velocity = SPEED_LOW;
            /** Percentage of power respect to the total area */
            f32 powerRatio;
            /** Private. Initialize HUD, font, set all items on the screen. */

            core::rect<s32> initialPosPower;
            void initHUD();

        public:
            /** Irrlicht GUI Enviroment  */
            gui::IGUIEnvironment* env;
            /** Default constructor
            * @see ::HUD::HUD::initHUD()
            */
            HUD(IrrlichtDevice* device, video::IVideoDriver* video);
            /** Animate power bar */
            void animatePower();
            /** Get ratio of the power selected out of total power  */
            f32 getPower();
            /** Set player name */
            void setPlayerName(core::stringw name);
            /** Reset power */
            void resetPower();



    };
};
#endif
