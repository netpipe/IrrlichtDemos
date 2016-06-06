
#include "CPlayerSceneNode.h"

//     _
//   _/_\_
// (______)    -- Player ship
//

        CPlayerSceneNode::CPlayerSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id):
                    CGameSceneNode(parent, smgr, id)
        {
            // the ship flies, obviously
            movement = GM_FLYING;
            
            // get the driver
            video::IVideoDriver* driver = smgr->getVideoDriver();
            
            // set up variables

            energy  =maxenergy= 1000;
            playerspeed     = 1.2;
            maxvel          = 2.0;
            maxdistx        = 1000;
            maxdisty        = 400;
            velocity        = core::vector3df(0,0,0);
            lasttime        = 0;

            
            // load the mesh
            scene::IAnimatedMesh *mesh = smgr->getMesh("data/mesh/playership.obj");
            
            // add the scene node
            ship = SceneManager->addAnimatedMeshSceneNode( mesh, this);
        	ship->setMaterialTexture(0, driver->getTexture("data/texture/playership.bmp") );

        	ship->setPosition(core::vector3df(0,0,0));
            ship->setRotation(core::vector3df(0,90,0));
            ship->setScale(core::vector3df(0.5,0.5,0.5));
            
            // get ships bbox
            BBox = core::aabbox3d<f32>(ship->getBoundingBox());
            
            // add the shield
            hasshield = true;
            shield    = new CShieldSceneNode(this, smgr, 0, 1000, 0.1, 1.4, 1,7,0);
            
            // add the target
            scene::ISceneNode *starget = smgr->addBillboardSceneNode(ship, core::dimension2d<f32>(50, 50), core::vector3df(0,0,50));
            video::ITexture *ttex = driver->getTexture("data/texture/target.bmp");
            driver->makeColorKeyTexture( ttex, core::position2d<s32>(0,0) );
            starget->setMaterialTexture(0, ttex ); 
            starget->setMaterialFlag(video::EMF_LIGHTING, false); 
            starget->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            
            // add energy bar
            hasenergy = true;
            energybar = smgr->addBillboardSceneNode( this, core::dimension2d<f32>(size*30,size*1.5),core::vector3df(0,size,-0.01));
            energybar->setMaterialTexture(0, sfxtextures[8] );
            energybar->setMaterialFlag(video::EMF_LIGHTING, false);            

            // light 
            smgr->addLightSceneNode( ship, core::vector3df(0,0,-6), video::SColorf(1,1,128),100);
            
            // boosters
            CBoosterSceneNode *x
              = new CBoosterSceneNode( ship, smgr, 0, exhaust, this, core::aabbox3d<f32>(-2.762,-0.02,-0.1, -0.6775*2,0.02,-0.4), core::vector3df(0,0,0),100, 500, video::SColor(255, 0, 0, 0), video::SColor(255, 255, 255, 255), 40, 100, 0);
            x->drop();
            x = new CBoosterSceneNode( ship, smgr, 0, exhaust, this, core::aabbox3d<f32>(-0.4, -0.4, -0.1,  0.4, 0.4, 0.1), core::vector3df(0,0,0),100, 500, video::SColor(255, 0, 0, 0), video::SColor(255, 255, 255, 255), 40, 100, 0);
            x->drop();
            x = new CBoosterSceneNode( ship, smgr, 0, exhaust, this, core::aabbox3d<f32>(1.355,-0.02,-0.1,  2.762,0.02,0.1), core::vector3df(0,0,0),100, 500, video::SColor(255, 0, 0, 0), video::SColor(255, 255, 255, 255), 40, 100, 0);            
            x->drop(); 
            
        } 
        
        CPlayerSceneNode::~CPlayerSceneNode()
        { 
            shield->remove();
            shield->drop();
            
        }
        
        void CPlayerSceneNode::OnPostRender(u32 time)
        {

            if (lasttime==0)
            {
                lasttime = time;
                return;
            }
            u32 timepassed = time-lasttime;
            lasttime = time;
            
            if (!dead)
            {
                // process keyboard input and adjust X and Y velocity
                if (keys[KEY_KEY_A])
                {
                    velocity.X -= 0.02f*timepassed;
                    if (velocity.X < -maxvel)
                       velocity.X = -maxvel;
                }
                if (keys[KEY_KEY_D])
                {
                    velocity.X += 0.02f*timepassed;
                    if (velocity.X > maxvel)
                        velocity.X = maxvel;
                }
                if (keys[KEY_KEY_S])
                {
                    velocity.Y -= 0.01f*timepassed;
                    if (velocity.Y < -maxvel)
                        velocity.Y = -maxvel;
                }
                if (keys[KEY_KEY_W])
                {
                    velocity.Y += 0.01f*timepassed;
                    if (velocity.Y > maxvel)
                       velocity.Y = maxvel;
                }
            }
            else
            {
                // fall
                velocity.Y -= 0.004f*timepassed;
            }
            // z velocity is constant
            velocity.Z += 0.005*timepassed; 
            if (velocity.Z > playerspeed) velocity.Z = playerspeed;

            // air resistance, for each milisec we remove x%           
            for (int n=0; n < timepassed; n++)
                velocity *= 0.996;
            
            // move the player, check for collision, etc
            Move(timepassed);
            
            // constrain ship to box?

            core::vector3df newpos    = getPosition();

            if (!dead)
            {
                bool            poschange = false;
    
                if      (newpos.X < -maxdistx) { poschange=true; newpos.X = -maxdistx; }
                else if (newpos.X >  maxdistx) { poschange=true; newpos.X =  maxdistx; }
                if      (newpos.Y < -maxdisty) { poschange=true; newpos.Y = -maxdisty; }
                else if (newpos.Y >  maxdisty) { poschange=true; newpos.Y =  maxdisty; }
               
                if (poschange) setPosition(newpos);
            }

            ship->setRotation(core::vector3df(velocity.Y*-20,velocity.X*20,0));
 
            this->CGameSceneNode::OnPostRender(time);            
            
        }
