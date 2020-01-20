#ifndef PLAYER_H
#define PLAYER_H
#include <irrlicht.h>
#include "keyboard.h"
#include "ball.h"
#include "target.h"
#include "camera.h"
#include "hud.h"
#include "../enviroment/castle.h"
#include "cannon.h"
using namespace irr;
using namespace KEYBOARD;
 /** Enum PlAYER_TYPE: HUMAN or AI.
 * For now only HUMAN is avaliable
 */
 enum PLAYER_TYPE{
    HUMAN=0,
    AI
 };
 /** Enum PLAYER_POSITION, is the position rotation: STRAIGHT or OPPOSITE
 * It is used to set the rotation of the players context.
 */
 enum PLAYER_POSITION{
    STRAIGHT,
    OPPOSITE
 };
 /** Player handles everthing about player-releated context, that means ::Castle, ::Cannon, ::Target, ::Ball, ::Camera.
 * There are 2 type of player: AI or HUMAN. Only HUMAN is available for now.
 * This class will build castle, setting up cannons, handles user inputs.
 */
class Player{
    private:
        scene::ISceneManager* smgr;
        video::IVideoDriver* driver;
        irr::IrrlichtDevice* device;
        /** Main camera of the player */
        Camera* camera;
        Physics* physics;
        /** Castle of the player */
        Castle* castle;
        /** Cannon of the player */
        Cannon* cannon;
        /** Side of the player ::PLAYER_POSITION */
        PLAYER_POSITION side;
    public:
        /** Keyboard of the player */
        Keyboard *keyboard;
        /** Name of the player */
        core::stringw name;
        /** Type of player ::PLAYER_TYPE */
        PLAYER_TYPE type;
        /** Default constructor. This Will initialize cannon, castle and keyboard.
        * @param position vector3df position of the castle.
        * @param keyboard ::Keyboard keyboard map of the player
        * @param type ::PLAYER_TYPE type of player
        * @param side ::PLAYER_POSITION side of player
        * @see ::Cannon
        * @see ::GameManager::loop();
        * @see ::KEYBOARD::Keyboard::OnEvent();
        * @see ::Castle
        */
        Player(IrrlichtDevice* device,scene::ISceneManager* smgr, video::IVideoDriver* driver, core::vector3df position, Physics* physics, Keyboard* keyboard, PLAYER_TYPE type, PLAYER_POSITION side = STRAIGHT);
        ~Player();
        /** Get cannon irrlicht node @return IMeshscene node*/
        scene::IMeshSceneNode* getNode();
        /** Get how far Z the cannon can SHOOT. The value is random withing cannon power.
        * It is used to setup the second castle position
        */
        core::vector3df getCannonRange();
        /** Set Active camera */
        void focusCamera();
        /** Setup cannon based on player side */
        void setCannon();
        /** Main loop of player
        * @param hud HUD hud of player.
        * @return bool true when player consume his turn. That means the ball is on ground and the user has consumed the SHOOT event
        * @see ::GameManager::loop();
        * @see ::HUD::HUD::animatePower();
        * @see ::Keyboard::IsKeyDown();
        * @see ::Cannon::shoot();
        * @see ::Ball::moveCamera();
        */
        bool loop(HUD::HUD* hud);
        /** Reset cannon angle and rotation */
        void reset();
        /** Check if target position is the same or it is changed.
        @return true if target is moved enough(that means it is hitted hard)
        @see ::Castle::checkTarget()
        */
        bool checkTarget();




};
#endif
