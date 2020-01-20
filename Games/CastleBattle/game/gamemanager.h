#include <irrlicht.h>
#include "physics.h"
#include "hud.h"
#include "player.h"
#include "keyboard.h"
#include "Logger.h"
using namespace irr;
/** GameManager is the main manager of the gameplay.
* It will create the players, castle, cannons and will start the loop of the gameplay.
*/
class GameManager{
    private:
        scene::ISceneManager* smgr;
        video::IVideoDriver* driver;
        IrrlichtDevice* device;
        Physics* physics;
        /** Array of players. */
        Player* players[2];
        /** Turn to play: true: first player, false second player */
        bool turn;
        /** World initial position, by default is setted to ::Terrain center */
        core::vector3df worldPosition;
        /**HUD of the gameplay. For now all the players will have the same HUD */
        HUD::HUD* hud;
        /** The keyboard map of each players. For now all players will have same Keyboard map */
        Keyboard keyboard;
        /** Player names.        */
        core::stringw* names;
        /** Set the Irrlicht device event receiver with a Keyboard instance */
        void initKeyboard();
    public:
        /** If someone win, stop is set to true */
        bool stop;
        /** Default constructor.
        * Given players names, and ::Application Irrlicht and ::Physics instances,
        * this will create players, castle, mapping keyboard and start the gameplay.
        * @param Irrlicht instances and Physic instance
        * @param names stringw array of names. Optional. If no specified Player1 and Player2  will be the names.
        * @see ::Launcher.
        * @see ::Player.
        * @see ::Application.
        * @see ::Application::loop().
        */
        GameManager(
          scene::ISceneManager* smgr,
          video::IVideoDriver* driver,
          IrrlichtDevice* device,
          core::vector3df worldPosition,
          Physics* physics,
          core::stringw* names = 0);

          ~GameManager();
         /** Create players and start gameplay. */
        void initGamePlay(core::stringw* names = 0);
        /** Main Gameplay loop.
        * The ::Application main loop is propagate here. This loop is propagate to ::Player::loop()
        * @return true if someone win.
        * @see ::Application::loop().
        * @see ::Player::loop().
        */
        bool loop();
        /** Get the winner  */
        int getWinner();
        /** Get player names        */
        core::stringw* getNames();


};
