
#ifndef CASTLE_H
#define CASTLE_H
#include <irrlicht.h>
#include <vector>
#include <map>
#include "target.h"
#include "bullethelper.h"
#include "physics.h"


using namespace irr;
const f32 BLOCKS_OFFSET = 0.05f;/*!< Distance between blocks */
const f32 TARGET_OFFSET = 1.f;/*!< Maximum offset of the target from initial positon. @see ::Castle::checkTarget() */
/** The ::Castle handles all things related to the Castles Building, treasure, physics and collisions. */
class Castle{
    private:
        scene::ISceneManager* smgr;
        video::IVideoDriver* driver;
        core::vector3df position;
        /** Last node used */
        scene::IMeshSceneNode* node;
        IrrlichtDevice* device;
        //! Associative array for blocks
        /*! This memorize of each side f, l, r, b, the nodes of the blocks */
        std::multimap<char, scene::IMeshSceneNode*> nodes;
         /** Target node */
        Target* target;
        /** Initial position of the first block */
        core::vector3df initialTargetPos;
        Physics* physics;
        /**
        * Create the block of the castle at a specified position, rotation and scaling.
        * @param position vector3df position
        * @param scale vector3df extent of the block
        * @param rotation vector3df rotation of the block
        * @param side char the side of the block
        * @param color SColor color of the block. Optional.
        */
        void createBlock(core::vector3df position, core::vector3df scale, core::vector3df rotation, char side, video::SColor color = randomColor());
        /** @deprecated Useless method */
        void addToPhysicsWorld();


    public:
        video::SColor COLOR_LIGHT;
        video::SColor COLOR_DARK;
        /** Default constructor.
        * Given irrlicht and bullet instances, this will initialize colors and start building
        * The color palette of the castle is based of 2 type of color randomly generated.
        * COLOR_LIGHT which represent light colors
        * COLOR_DARK which represent dark colors.
        * @see createBlock()
        * @see buildCastle()
        * @see randomColor()
        */
        Castle(scene::ISceneManager* smgr, Physics* physics, IrrlichtDevice* device, video::IVideoDriver* driver, core::vector3df center);
        /** Build the castle.
        * Given the position, this method will start building the castle from that position.
        * For now it will get data of the castle from xml file in media/Castle
        * @param center vector3df position from where to start building
        * @return bool true if castle is successfully builded or false if something went wrong
        */
        bool buildCastle(core::vector3df center);
        /** Get size from a side.
        * There is  a problem: left/right side blocks scale is defined with X and a rotation(90°). So Z is 1.
        * Whenever you call this function you get the width(X) not the actual size based on the irrlicht axis and the overall bounding box.
        * Height is correct.
        * So this will return the sum of the width attribute in xml of a specified side.
        * (f = front, b=back, l=left, r=right )
        * @param side char letter of the side (f = front, b=back, l=left, r=right )
        * @return vector3df
        */
        core::vector3df getSideSize(char side);
        /** Set treasure in the castle */
        void setTreasure();
        /** Get center of the castle in Irrlicht world coordinates */
        core::vector3df calculateAbsoluteCenter();
        /** Check if target is moved
        * TARGET_OFFSET is the max offset of the target from initial position.
        * That means if it will be hitted, if the actual position is greater than previos+targetoffset, the player lose
        * I didn't have so much time to build a more sophisticated system, sry!
        */
        bool checkTarget();

};
#endif
