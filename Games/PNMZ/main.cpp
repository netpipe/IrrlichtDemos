/*
	PNMZ - Please No More Zombies
	Copyright 2005 DracSoft



		LICENSE
		-------
		This program is free software; you can redistribute it and/or
		modify it under the terms of the GNU General Public License
		as published by the Free Software Foundation; either version 2
		of the License, or (at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program; if not, write to the Free Software
		Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

		A copy of the full license may be found here:
		http://www.gnu.org/licenses/gpl.html


	NOTE **Irrlicht animations are loaded with 8 frames per frame**
*/


//TODO: Version constant
//TODO: Reorganize and split code into several files
//TODO: Reduce global usage in namespace game::
//TODO: Correct animations. Set them to the correct start/end
//TODO: Make a GetXZDistance function to improve performance.
//TODO: Make a IntersectXZ functino to improve performance.
//TODO: HUD
//TODO: Better GUI for settings
//TODO: More randomly placed stuff in city generator (cars, lamps, stairways, etc)
//TODO: More weapons (machine gun, rocket launcher)
//TODO: Ammo implementation
//TODO: Fix UV Mapping on zombies to work correctly. Irrlicht can't seem to load ms3d animations correctly


//INCLUDES
#include <irrlicht.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

//CONSTANTS
#define _PNMZ_VERSION L"0.15 Alpha"
#define _ANIMATION_LENGTH 80
#define _DEATH_FPS 120
#define _WALK_FPS 80
#define _MORPH_FPS 15
#define _GUN_FPS 640
#define _HIT_FPS 320
#define _ATTACK_FPS 200
#define _WALK_ANIM 1
#define _DEATH_ANIM 2
#define _ZOMBIE_ANIM 3
#define _ZOMBIE_HIT_ANIM 4
#define _HUMAN_HIT_ANIM 5
#define _ATTACK_ANIM 6
#define _MORPH_ANIM 7
#define _MODEL_OFFSET (-90)
#define _CAM_VIS 700
#define _CAM_VIS_ENTITY_MOD 0.5f

#define _ENTITY_SID 7
#define _BUILDING_SID 2
#define _WEAPON_SID 3
#define _SIDEWALK_SID 4
#define _WALL_SID 5
#define _GROUND_SID 6

#define _PISTOL_DMG 15
#define _ZOMBIE_DMG 10

//NAMESPACES
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace std;

//GAMESPACE
namespace game
{
    //ENUMERATIONS
    enum GUIOBJ {
        GUIOBJ_LEVELSIZE,
        GUIOBJ_HUMANCOUNT,
        GUIOBJ_ZOMBIECOUNT,
        GUIOBJ_SPACER,
        GUIOBJ_MAXSIZE,
        GUIOBJ_MINSIZE,
        GUIOBJ_MAXHEIGHT,
        GUIOBJ_MINHEIGHT,
        GUIOBJ_OFFSETMAX,
        GUIOBJ_HUMANSPEED,
        GUIOBJ_ZOMBIESPEED,
        GUIOBJ_STARTBUTTON,
        GUIOBJ_DEBUGMODE
    };

    //STRUCTURES
    struct entity_struct
    {
        float                   movSpeed; //units per second
        float                   rotSpeed; //degrees per second
        IAnimatedMeshSceneNode* node;
        bool                    isZombie;
        bool                    isDead;
        bool                    isMorphing;
        bool                    wasHidden;
        long                    attackTimeMs;
        int                     health;
        ISceneNode*             target;
        int                     targetid;
    };

    //GLOBALS
    int     screenWidth=640;
    int     screenHeight=480;
    int     fullScreen=0;

    int     levelSize;
    int     lightStr;
    int     colCount;
    int     rowCount;
    int     humanCount=100;
    int     zombieCount=100;
    int     deadCount=0;
    int     spacer=140;
    int     maxheight=300;
    int     minheight=35;
    int     maxSize=120;
    int     minSize=40;
    int     offsetMax=100;
    int     mapSize=5;
    int     zombieFOV=30;

    int     playerRotSpeed=120;
    float     playerMovSpeed=.1f;
    int     playerHealth=100;
    int     cameraHeight=10;
    float   gravity=0.0f;

    float   lampLife=35.0f;
    float   lampLifeMax=35.0f;
    float   lampLifeRate=1.0f;
    float   lampLifeChargeRate=3.0f;

    int     zombieAttackDelayMs=1000;
    float   zombieVision=75.0f;
    float   zombieAttackDistance=15.0f;
    float   zombieAttackDistanceAI=5.0f;
    float   zombieSpeed=15.0f;
    float   humanSpeed=10.0f;
    float   zombieRotSpeed=90.0f;
    float   humanRotSpeed=90.0f;
    float   aiColLookAhead=-8.0f;
    float   aiColLookAngle=22.5f;

    int     particleCount=10;
    float   particleGravity=-0.3f;

    float   freePositionPadding=15.0f;

    bool    isDebug=false;
    bool    showMap=false;
    bool    showGUI=true;

    long    lastTimeMs=0;
    long    currentTimeMs=0;
    long    lastCutoffTimeMs=0;
    long    lastAIColTimeMs=0;

    IrrlichtDevice*             device;
    IVideoDriver*               driver;
    ISceneManager*              scene;
    IGUIEnvironment*            gui;
    ITexture**                  buildingTexArray;
    ITexture**                  buildingNormalArray;
    ITexture*                   sidewalkTex;
    ITexture*                   groundTex;
    ITexture*                   wallTex;
    ITexture*                   pistolTex;
    ITexture*                   humanTex;
    ICameraSceneNode*           camera;
    ICameraSceneNode*           mapcamera;
    IAnimatedMesh*              box;
    IAnimatedMesh*              human;
    IAnimatedMesh*              fatman;
    IAnimatedMesh*              pistol;

    ISceneNode*                 ground;
    ISceneNode*                 wall1;
    ISceneNode*                 wall2;
    ISceneNode*                 wall3;
    ISceneNode*                 wall4;
    IAnimatedMeshSceneNode*     w_pistol;
    IAnimatedMeshSceneNode*     w_current;
    IAnimatedMeshSceneNode*     player;
    ILightSceneNode*            playerLamp;
    ILightSceneNode*            weaponLight;

    ISceneNode**                buildingArray;
    ISceneNode**                sidewalkArray;
    struct entity_struct*       entityArray;
}

namespace ai
{
    float wrapAngle(float value)
    {
        while (value < 0.0f)
            value += 360.0f;

        while (value >= 360.0f)
            value -= 360.0f;

        return value;
    }

    vector3df getTargetAngle(ISceneNode* ScnNode, ISceneNode* trgt)
    {
        vector3df v = ScnNode->getPosition();
        vector3df r = trgt->getPosition();
        vector3df angle;
        float x,y,z;
        x = r.X - v.X;
        y = r.Y - v.Y;
        z = r.Z - v.Z;
        angle.Y = atan2 (x, z);
        angle.Y *= (180 / PI);
        angle.Y-=90;

        angle.Y = wrapAngle(angle.Y);

        float z1 = sqrt(x*x + z*z);
        angle.X = atan2 (z1, y);
        angle.X *= (180 / PI);
        angle.X -= 90;

        angle.X = wrapAngle(angle.X);

        return angle;
    }

    void MoveForward(ISceneNode* ScnNode, vector3df Curforward, f32 speed)
    {
        ScnNode->getAbsoluteTransformation().rotateVect(Curforward);
        vector3df newPos= ScnNode->getPosition() + (Curforward * speed);
        ScnNode->setPosition(newPos);
    }

    f64 GetDistance(ISceneNode* ScnNode, ISceneNode* trgt)
    {
        vector3df pos = ScnNode->getPosition();
        return  pos.getDistanceFrom(trgt->getPosition());
    }

    float GetTimedAmount(long elapsedTime, float unitsPerSecond) //used for rotation and movement
    {
        return (float)(elapsedTime/1000.0) * unitsPerSecond;
    }
}

namespace game
{
    namespace anim_cb
    {
        //for when something dies
        class CDeathEnd : public IAnimationEndCallBack
        {
        public:

            virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node)
            {
                node->setPosition(vector3df(0.0f,-1.0f*_CAM_VIS*2.0f+100.0f,0.0f));
                node->updateAbsolutePosition();
                node->setAnimationSpeed(0);
                node->setVisible(false);
            }
        };

        //for when weapon fire has stopped
        class CWeaponEnd : public IAnimationEndCallBack
        {
        public:

            virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node)
            {
                node->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
                weaponLight->setVisible(false);
            }
        };

        //for when monsters get hit
        class CHitEnd : public IAnimationEndCallBack
        {
        public:
            int entityid;

            CHitEnd(int ent_id) {
                entityid=ent_id;
            }

            virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node)
            {
                //entityArray[entityid].node->removeAll();

                if (entityArray[entityid].health > 0)
                    if (entityArray[entityid].isZombie) {
                        entityArray[entityid].node->setFrameLoop(_ANIMATION_LENGTH*(_ZOMBIE_ANIM-1)+1,_ANIMATION_LENGTH*_ZOMBIE_ANIM-1);
                        entityArray[entityid].node->setLoopMode(true);
                        entityArray[entityid].node->setAnimationSpeed(_WALK_FPS);
                    }else{
                        entityArray[entityid].node->setFrameLoop(_ANIMATION_LENGTH*(_WALK_ANIM-1)+1,_ANIMATION_LENGTH*_WALK_ANIM-1);
                        entityArray[entityid].node->setLoopMode(true);
                        entityArray[entityid].node->setAnimationSpeed(_WALK_FPS);
                    }
            }
        };

        //for when human morphs to zombie
        class CMorphEnd : public IAnimationEndCallBack
        {
        public:
            int entityid;

            CMorphEnd(int ent_id) {
                entityid=ent_id;
            }

            virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node)
            {
                //entityArray[entityArray[entityid].targetid].node->setTexture(...zombie...);
                entityArray[entityid].node->setFrameLoop(_ANIMATION_LENGTH*(_ZOMBIE_ANIM-1)+1,_ANIMATION_LENGTH*_ZOMBIE_ANIM-1);
                entityArray[entityid].node->setLoopMode(true);
                entityArray[entityid].node->setAnimationSpeed(_WALK_FPS);
                entityArray[entityid].isMorphing=false;
                entityArray[entityid].movSpeed=zombieSpeed;
                entityArray[entityid].rotSpeed=zombieRotSpeed;
            }
        };

        //for when monsters attack
        class CAttackEnd : public IAnimationEndCallBack
        {
        public:
            int entityid;

            CAttackEnd(int ent_id) {
                entityid=ent_id;
            }

            virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node)
            {
                entityArray[entityid].node->setFrameLoop(_ANIMATION_LENGTH*(_ZOMBIE_ANIM-1)+1,_ANIMATION_LENGTH*_ZOMBIE_ANIM-1);
                entityArray[entityid].node->setLoopMode(true);
                entityArray[entityid].node->setAnimationSpeed(_WALK_FPS);

                if (entityArray[entityid].target==camera)
                {
                    if (!isDebug && ai::GetDistance(entityArray[entityid].node, entityArray[entityid].target) <= zombieAttackDistance)
                    {
                        playerHealth -= _ZOMBIE_DMG;
                    }
                }
                else
                {
                    if (entityArray[entityArray[entityid].targetid].health <= 50)
                    {
                        entityArray[entityArray[entityid].targetid].node->setFrameLoop(_ANIMATION_LENGTH*(_MORPH_ANIM-1)+1,_ANIMATION_LENGTH*_MORPH_ANIM-1);
                        entityArray[entityArray[entityid].targetid].node->setAnimationSpeed(_MORPH_FPS);
                        entityArray[entityArray[entityid].targetid].node->setLoopMode(false);
                        entityArray[entityArray[entityid].targetid].node->setAnimationEndCallback(new anim_cb::CMorphEnd(entityArray[entityid].targetid));
                        entityArray[entityArray[entityid].targetid].isZombie=true;
                        entityArray[entityArray[entityid].targetid].isMorphing=true;
                        entityArray[entityid].target=0;
                    }
                    else
                    {
                        entityArray[entityArray[entityid].targetid].health -= _ZOMBIE_DMG;
                    }
                }
            }
        };

    }
}

namespace game
{
    //EVENT CLASS
    class MyEventReceiver : public IEventReceiver
    {
        public:
	virtual bool OnEvent(const SEvent& event) {

            //GUI events
            if (showGUI)
                if (event.EventType == EET_GUI_EVENT)
                {
                    s32 id = event.GUIEvent.Caller->getID();

                    switch(event.GUIEvent.EventType)
                    {
                        case EGET_CHECKBOX_CHANGED:
                            if (id==GUIOBJ_DEBUGMODE) {
                                isDebug=!isDebug;
                                break;
                            }

                        case EGET_COMBO_BOX_CHANGED:
                            if (id==GUIOBJ_LEVELSIZE) {
                                stringw w;
                                stringc c;
                                IGUIComboBox* cmbo=(IGUIComboBox*)gui->getRootGUIElement()->getElementFromId(GUIOBJ_LEVELSIZE,true);
                                w=cmbo->getItem(cmbo->getSelected());
                                c=w.c_str();
                                levelSize = atoi(c.c_str());

                                w=L"";
                                w+=(levelSize*2);
                                gui->getRootGUIElement()->getElementFromId(GUIOBJ_HUMANCOUNT,true)->setText(w.c_str());
                                gui->getRootGUIElement()->getElementFromId(GUIOBJ_ZOMBIECOUNT,true)->setText(w.c_str());
                            }
                            break;


                        case EGET_BUTTON_CLICKED:
                            if (id==GUIOBJ_STARTBUTTON) {
                                //set all game values and start the game itself
                                stringw w;
                                stringc c;
                                IGUIComboBox* cmbo;

                                cmbo=(IGUIComboBox*)gui->getRootGUIElement()->getElementFromId(GUIOBJ_LEVELSIZE,true);
                                w=cmbo->getItem(cmbo->getSelected());
                                c=w.c_str();
                                levelSize = atoi(c.c_str());
                                cout<<levelSize;
                                colCount=(int)sqrt((double)levelSize);
                                rowCount=levelSize/colCount;

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_HUMANCOUNT,true)->getText();
                                c=w.c_str();
                                humanCount = atoi(c.c_str());

                                cmbo=(IGUIComboBox*)gui->getRootGUIElement()->getElementFromId(GUIOBJ_HUMANSPEED,true);
                                w=cmbo->getItem(cmbo->getSelected());
                                c=w.c_str();
                                humanSpeed = (float) atoi(c.c_str());

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_MAXHEIGHT,true)->getText();
                                c=w.c_str();
                                maxheight = atoi(c.c_str());

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_MAXSIZE,true)->getText();
                                c=w.c_str();
                                maxSize = atoi(c.c_str());

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_MINHEIGHT,true)->getText();
                                c=w.c_str();
                                minheight = atoi(c.c_str());

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_MINSIZE,true)->getText();
                                c=w.c_str();
                                minSize = atoi(c.c_str());

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_OFFSETMAX,true)->getText();
                                c=w.c_str();
                                offsetMax = atoi(c.c_str());

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_SPACER,true)->getText();
                                c=w.c_str();
                                spacer = atoi(c.c_str());

                                w=gui->getRootGUIElement()->getElementFromId(GUIOBJ_ZOMBIECOUNT,true)->getText();
                                c=w.c_str();
                                zombieCount = atoi(c.c_str());

                                cmbo=(IGUIComboBox*)gui->getRootGUIElement()->getElementFromId(GUIOBJ_ZOMBIESPEED,true);
                                w=cmbo->getItem(cmbo->getSelected());
                                c=w.c_str();
                                zombieSpeed = (float) atoi(c.c_str());

                                //start game
                                showGUI=false;
                            }
                            break;
                    }
                }

            //keypresses
            if (!showGUI)
            {
                if (event.KeyInput.PressedDown) {
                    switch (event.KeyInput.Key) {
                        case KEY_KEY_M:
                            showMap = !showMap;
                            return true;

                        case KEY_PLUS:
                            mapSize=std::max(1,mapSize-1);
                            return true;

                        case KEY_MINUS:
                            mapSize=std::min(6,mapSize+1);
                            return true;

                        case KEY_KEY_F:
                            playerLamp->setVisible(!playerLamp->isVisible());
                            return true;
                    }
                }

                if (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
                {
                    if (w_current)
                    {
                        weaponLight->setVisible(true);
                        w_current->setFrameLoop(0,_ANIMATION_LENGTH-1);
                        w_current->setAnimationSpeed(_GUN_FPS);
                        w_current->setLoopMode(false);
                        w_current->setAnimationEndCallback(new anim_cb::CWeaponEnd());

                        line3d<f32> ray = scene->getSceneCollisionManager()->getRayFromScreenCoordinates(position2d<s32>(driver->getScreenSize().Width/2, driver->getScreenSize().Height/2), camera);
                        int eid = -1;
                        float shortestDist = -1.0f;
                        float curDist = 0.0f;

                        //find closest zombie
                        for (int i=0; i<humanCount+zombieCount; i++)
                        {
                            if (entityArray[i].node->getTransformedBoundingBox().intersectsWithLine(ray)) {
                                curDist = ai::GetDistance(camera, entityArray[i].node);
                                if (curDist <= camera->getFarValue()*_CAM_VIS_ENTITY_MOD && (curDist < shortestDist || shortestDist == -1.0f)) {
                                    shortestDist = curDist;
                                    eid=i;
                                }
                            }
                        }

                        //make sure theres not a building in the way
                        if (eid > -1)
                        {
                            ray.end = entityArray[eid].node->getPosition(); //sets the end of the line to the zombies pos

                            for (int i=0; i<levelSize; i++)
                            {
                                if (buildingArray[i])
                                {
                                    if (buildingArray[i]->getTransformedBoundingBox().intersectsWithLine(ray)) {
                                        eid=-1;
                                        break;
                                    }
                                }
                            }
                        }

                        //check the walls and ground (who knows?)
                        if (eid > -1)
                        {
                            if (wall1->getTransformedBoundingBox().intersectsWithLine(ray))
                                eid=-1;
                            else if (wall2->getTransformedBoundingBox().intersectsWithLine(ray))
                                eid=-1;
                            else if (wall3->getTransformedBoundingBox().intersectsWithLine(ray))
                                eid=-1;
                            else if (wall4->getTransformedBoundingBox().intersectsWithLine(ray))
                                eid=-1;
                            else if (ground->getTransformedBoundingBox().intersectsWithLine(ray))
                                eid=-1;
                        }

                        //apply damage
                        if (eid > -1)
                        {
                            if (!entityArray[eid].isDead)
                            {
                                int dmgAmount=0;
                                if (w_current==w_pistol)
                                    dmgAmount=_PISTOL_DMG;

                                entityArray[eid].health -= dmgAmount;

                                //Show bloodspray
                                IParticleSystemSceneNode* ps;
                                IParticleEmitter* em;
                                IParticleAffector* p;
                                ITexture* blood = driver->getTexture("./textures/blood.bmp");
                                ISceneNodeAnimator* delTimer;
                                driver->makeColorKeyTexture(blood, core::position2d<s32>(0,0));

                                for (int z=0; z < particleCount; z++)
                                {
                                    ps = device->getSceneManager()->addParticleSystemSceneNode(false,0,0,vector3df(0,10,0));
                                    ps->setPosition(vector3df(entityArray[eid].node->getPosition().X, entityArray[eid].node->getPosition().Y+10, entityArray[eid].node->getPosition().Z));
                                    em=ps->createPointEmitter(vector3df(0.02f,0.02f,0.02f),50,100,SColor(0,190,0,0),SColor(0,255,0,0),500,1500,180);
                                    ps->setEmitter(em);
                                    em->drop();
                                    ps->setParticleSize(dimension2d<f32>(0.5f,0.5f));
                                    p=ps->createGravityAffector(vector3df(0,particleGravity,0));
                                    ps->addAffector(p);
                                    p->drop();
                                    //p=ps->createFadeOutParticleAffector(SColor(0,0,0,0),1500);
                                    //ps->addAffector(p);
                                    //p->drop();
                                    ps->setMaterialTexture(0, blood);
                                    ps->setMaterialFlag(video::EMF_LIGHTING, false);
                                    ps->setParticlesAreGlobal(true);
                                    ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                                    delTimer = device->getSceneManager()->createDeleteAnimator(1500);
                                    ps->addAnimator(delTimer);
                                    delTimer->drop();
                                }

                                //TODO: Show blood decal


                                //Play impact animation
                                if (entityArray[eid].isZombie)
                                {
                                    entityArray[eid].target = camera;
                                    entityArray[eid].node->setFrameLoop(_ANIMATION_LENGTH*(_ZOMBIE_HIT_ANIM-1)+1,_ANIMATION_LENGTH*_ZOMBIE_HIT_ANIM-1);
                                    entityArray[eid].node->setAnimationSpeed(_HIT_FPS);
                                    entityArray[eid].node->setLoopMode(false);
                                    entityArray[eid].node->setAnimationEndCallback(new anim_cb::CHitEnd(eid));
                                }
                                else
                                {
                                    entityArray[eid].node->setFrameLoop(_ANIMATION_LENGTH*(_HUMAN_HIT_ANIM-1)+1,_ANIMATION_LENGTH*_HUMAN_HIT_ANIM-1);
                                    entityArray[eid].node->setAnimationSpeed(_HIT_FPS);
                                    entityArray[eid].node->setLoopMode(false);
                                    entityArray[eid].node->setAnimationEndCallback(new anim_cb::CHitEnd(eid));
                                }
                            }
                        }
                    }
                }

                //Send all other events to fps cam
                if (camera) {
                    scene->setActiveCamera(camera);
                    camera->OnEvent(event);
                    //camera->OnPostRender(device->getTimer()->getTime());
                }
            }

            return false;
        }
    };


    //FUNCTIONS
    int random(int low,int high)
    {
        return  rand() % (high - low + 1) + low;
    }

    void AddCollisionResponse(ISceneNode *node, ISceneNode *target)
    {
        ITriangleSelector* selector = scene->createTriangleSelectorFromBoundingBox(node);
        node->setTriangleSelector(selector);
        selector->drop();
        ISceneNodeAnimator* anim = scene->createCollisionResponseAnimator(selector, target, vector3df(4,12,1), vector3df(0,gravity,0), vector3df(0,-1,0) );
        target->addAnimator(anim);
        anim->drop();
    }

    void LoadTextures()
    {
        buildingTexArray = new ITexture*[3];
        for (int i=0; i<3; i++){
            stringc path = "./textures/building";
            path += i;
            path += ".png";
            buildingTexArray[i]=driver->getTexture(path.c_str());
        }

        buildingNormalArray = new ITexture*[3];
        for (int i=0; i<3; i++){
            stringc path = "./textures/building-height";
            path += i;
            path += ".bmp";
            buildingNormalArray[i]=driver->getTexture(path.c_str());
            driver->makeNormalMapTexture(buildingNormalArray[i], 2.0f);
        }

        sidewalkTex = driver->getTexture("./textures/sidewalk.png");
        groundTex = driver->getTexture("./textures/road.png");
        wallTex = driver->getTexture("./textures/wall.jpg");
        pistolTex = driver->getTexture("./textures/pistol.jpg");
        humanTex = driver->getTexture("./textures/human.png");
    }

    void LoadMeshes()
    {
        box=device->getSceneManager()->getMesh("./models/box.obj");
        human=device->getSceneManager()->getMesh("./models/human.md2");
        pistol=device->getSceneManager()->getMesh("./models/pistol.md2");

        if (!human || !box || !pistol)
            cout<<"Error loading file mesh."<<endl;
    }

    void InitDevice(MyEventReceiver* receiver)
    {
        //create our window
        device = createDevice(EDT_OPENGL, dimension2d<s32>(screenWidth, screenHeight), 32, (bool)fullScreen, true, false, receiver);
        if (!fullScreen)
            device->setResizeAble(true);

        //seed the random plant
        srand(device->getTimer()->getRealTime());

        driver = device->getVideoDriver();
        scene = device->getSceneManager();
        gui = device->getGUIEnvironment();
    }

    void MoveToFreePosition(ISceneNode* node, int pY)
    {
        bool foundFree=false;

        while (!foundFree)
        {
            int pX=random(spacer,(rowCount-1)*spacer);
            int pZ=random(spacer,(colCount-1)*spacer);
            node->setPosition(vector3df(pX,pY,pZ));

            if (isDebug)
                cout<<"X:"<<pX<<" Y:"<<pY<<" Z:"<<pZ<<endl;

            matrix4 mat=node->getAbsoluteTransformation();
            foundFree=true;

            for (int j=0; j<levelSize; j++) //check for buildings in the way xx units ahead
                if (buildingArray[j])
                {
                    //rot 0 degrees
                    vector3df dirVec = vector3df(mat[8],mat[9],mat[10]);
                    dirVec.normalize();
                    dirVec *= freePositionPadding;
                    dirVec += node->getPosition();

                    //if this one hits rotate right
                    if (buildingArray[j]->getTransformedBoundingBox().isPointInside( dirVec ))
                    {
                        foundFree=false;
                        break;
                    }

                    //rot 90 degrees
                    dirVec = vector3df(mat[8],mat[9],mat[10]);
                    dirVec.normalize();
                    dirVec.rotateXZBy(90, vector3df(0,0,0));
                    dirVec *= freePositionPadding;
                    dirVec += node->getPosition();

                    if (buildingArray[j]->getTransformedBoundingBox().isPointInside( dirVec ))
                    {
                        foundFree=false;
                        break;
                    }

                    //rot 180 degrees
                    dirVec = vector3df(mat[8],mat[9],mat[10]);
                    dirVec.normalize();
                    dirVec.rotateXZBy(180, vector3df(0,0,0));
                    dirVec *= freePositionPadding;
                    dirVec += node->getPosition();

                    if (buildingArray[j]->getTransformedBoundingBox().isPointInside( dirVec ))
                    {
                        foundFree=false;
                        break;
                    }

                    //rot 270 degrees
                    dirVec = vector3df(mat[8],mat[9],mat[10]);
                    dirVec.normalize();
                    dirVec.rotateXZBy(270, vector3df(0,0,0));
                    dirVec *= freePositionPadding;
                    dirVec += node->getPosition();

                    if (buildingArray[j]->getTransformedBoundingBox().isPointInside( dirVec ))
                    {
                        foundFree=false;
                        break;
                    }
                }
        }
    }

    void CreateEntities()
    {
        entityArray = new struct entity_struct[humanCount+zombieCount];

        //create humans
        for (int i=0; i<humanCount; i++)
        {
            entityArray[i].movSpeed=humanSpeed;
            entityArray[i].rotSpeed=humanRotSpeed;
            entityArray[i].isZombie=false;
            entityArray[i].isDead=false;
            entityArray[i].isMorphing=false;
            entityArray[i].attackTimeMs=0;
            entityArray[i].wasHidden=false;
            entityArray[i].health=100;
            entityArray[i].target=NULL;

            entityArray[i].node = scene->addAnimatedMeshSceneNode(human,0,_ENTITY_SID,vector3df(0,0,0),vector3df(0,random(0,359),0),vector3df(1,1,1));
            MoveToFreePosition(entityArray[i].node,0);

            entityArray[i].node->setMaterialFlag(EMF_LIGHTING, true);
            entityArray[i].node->setMaterialTexture(0,humanTex);
            entityArray[i].node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
            entityArray[i].node->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
            entityArray[i].node->setFrameLoop(_ANIMATION_LENGTH*(_WALK_ANIM-1)+1,_ANIMATION_LENGTH*_WALK_ANIM-1);
            entityArray[i].node->setLoopMode(true);
            entityArray[i].node->setAnimationSpeed(_WALK_FPS);
     //       entityArray[i].node->setAutomaticCulling(true);

            AddCollisionResponse(entityArray[i].node,camera);
        }

        //create zombues
        for (int i=humanCount; i<humanCount+zombieCount; i++)
        {
            entityArray[i].movSpeed=zombieSpeed;
            entityArray[i].rotSpeed=zombieRotSpeed;
            entityArray[i].isZombie=true;
            entityArray[i].isDead=false;
            entityArray[i].isMorphing=false;
            entityArray[i].attackTimeMs=0;
            entityArray[i].wasHidden=false;
            entityArray[i].health=100;
            entityArray[i].target=NULL;

            entityArray[i].node = scene->addAnimatedMeshSceneNode(human,0,_ENTITY_SID,vector3df(0,0,0),vector3df(0,random(0,359),0),vector3df(1,1,1));
            MoveToFreePosition(entityArray[i].node,0);

            entityArray[i].node->setMaterialFlag(EMF_LIGHTING, true);
            //entityArray[i].node->setMaterialTexture(0,zombieTex);
            entityArray[i].node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
            entityArray[i].node->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
            entityArray[i].node->setFrameLoop(_ANIMATION_LENGTH*(_ZOMBIE_ANIM-1)+1,_ANIMATION_LENGTH*_ZOMBIE_ANIM-1);
            entityArray[i].node->setLoopMode(true);
            entityArray[i].node->setAnimationSpeed(_WALK_FPS);
//            entityArray[i].node->setAutomaticCulling(true);

            AddCollisionResponse(entityArray[i].node,camera);
        }
    }

    void GenerateCity()
    {
        //corner lights
        SColorf lightColor=SColorf(1.0f, 1.0f, 1.0f, 1.0f);
        lightStr = rowCount*spacer/random(5,8);
        scene->addLightSceneNode(0, vector3df(rowCount*spacer,maxheight,colCount*spacer/2), lightColor, lightStr);
        scene->addLightSceneNode(0, vector3df(-1*spacer,maxheight,-1*spacer), lightColor, lightStr);
        //scene->addLightSceneNode(0, vector3df(-1*spacer,maxheight,colCount*spacer/2), lightColor, lightStr);
        //scene->addLightSceneNode(0, vector3df(rowCount*spacer,maxheight,-1*spacer), lightColor, lightStr);

        //decl vars
        ISceneNode* building;
        ISceneNode* sidewalk;
        int sX,sY,sZ,pX,pZ;
        float pY;

        //generate height normal maps
      //  ITexture* sidewalkNormal=driver->getTexture("./textures/sidewalk-height.bmp");
      //  driver->makeNormalMapTexture(sidewalkNormal, 1.0f);

        //make tiled mesh for ground/walls
        IMesh* tiledMesh = scene->getMeshManipulator()->createMeshCopy(box->getMesh(0));
        scene->getMeshManipulator()->makePlanarTextureMapping(tiledMesh,rowCount*5);

        //make ground
        ground = scene->addMeshSceneNode(tiledMesh,0,_GROUND_SID,vector3df((rowCount)*spacer/2-spacer,-1,(colCount)*spacer/2-spacer),vector3df(0,0,0),vector3df((rowCount)*spacer,1,(colCount)*spacer));
        ground->setMaterialFlag(EMF_LIGHTING, true);
        ground->setMaterialTexture(0,groundTex);
        ground->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
        ground->getMaterial(0).EmissiveColor.set(20, 20, 20, 20);
        AddCollisionResponse(ground,camera);

        //wall 1
        wall1 = scene->addMeshSceneNode(tiledMesh,0,_WALL_SID,vector3df((rowCount*spacer)/2-spacer, -1, 0),vector3df(0,0,0),vector3df(rowCount*spacer, maxheight/3, 1));
        wall1->setMaterialFlag(EMF_LIGHTING, true);
        wall1->setMaterialTexture(0,wallTex);
        wall1->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
        wall1->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
//        wall1->setAutomaticCulling(true);
        AddCollisionResponse(wall1,camera);

        //wall 2
        wall2 = scene->addMeshSceneNode(tiledMesh,0,_WALL_SID,vector3df((rowCount*spacer)/2-spacer, -1, colCount*spacer-spacer),vector3df(0,0,0),vector3df(rowCount*spacer, maxheight/3, 1));
        wall2->setMaterialFlag(EMF_LIGHTING, true);
        wall2->setMaterialTexture(0,wallTex);
        wall2->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
        wall2->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
    //    wall2->setAutomaticCulling(true);
        AddCollisionResponse(wall2,camera);

        //wall 3
        wall3 = scene->addMeshSceneNode(tiledMesh,0,_WALL_SID,vector3df(0, -1, (colCount*spacer)/2-spacer),vector3df(0,0,0),vector3df(1, maxheight/3, colCount*spacer));
        wall3->setMaterialFlag(EMF_LIGHTING, true);
        wall3->setMaterialTexture(0,wallTex);
        wall3->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
        wall3->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
     //   wall3->setAutomaticCulling(true);
        AddCollisionResponse(wall3,camera);

        //wall 4
        wall4 = scene->addMeshSceneNode(box->getMesh(0),0,_WALL_SID,vector3df(rowCount*spacer-spacer, -1, (colCount*spacer)/2-spacer),vector3df(0,0,0),vector3df(1, maxheight/3, colCount*spacer));
        wall4->setMaterialFlag(EMF_LIGHTING, true);
        wall4->setMaterialTexture(0,wallTex);
        wall4->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
        wall4->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
    //    wall4->setAutomaticCulling(true);
        AddCollisionResponse(wall4,camera);

        //drop tiled mesh for ground/walls
        tiledMesh->drop();

        //make buildings and sidewalks
        buildingArray = new ISceneNode*[levelSize];
        sidewalkArray = new ISceneNode*[levelSize];
        for (int i=0; i<levelSize; i++){
            buildingArray[i]=0;
            sidewalkArray[i]=0;
        }

        //make sidewalk tiled mesh
        IMesh* tangentMesh = scene->getMeshManipulator()->createMeshWithTangents(box->getMesh(0));
        tiledMesh = scene->getMeshManipulator()->createMeshWithTangents(box->getMesh(0));
        scene->getMeshManipulator()->makePlanarTextureMapping(tiledMesh,25);

        //go!
        for (int i=1; i<=rowCount; i++)
        {
            for (int j=1; j<=colCount; j++)
            {
                int current = (i-1)*rowCount+j-1;

                sX=random(minSize,maxSize);
                sZ=random(minSize,maxSize);
                sY=random(minheight,maxheight);

                pX=(i-1)*spacer-random(0,offsetMax);
                pZ=(j-1)*spacer-random(0,offsetMax);
                pY=sY/2.0f;

                buildingArray[current] = building = scene->addMeshSceneNode(tangentMesh,0,_BUILDING_SID,vector3df(pX,pY,pZ),vector3df(0,0,0),vector3df(sX,sY,sZ));
                sidewalkArray[current] = sidewalk = scene->addMeshSceneNode(tiledMesh,0,_SIDEWALK_SID,vector3df(pX,0.5f-(random(0,100)/1000.0f),pZ),vector3df(0,0,0),vector3df(sX+spacer/4,1,sZ+spacer/4));

                if (building && sidewalk)
                {
                    building->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
                    building->setMaterialFlag(EMF_LIGHTING, true);
                    int r=random(0,2);
                    building->setMaterialTexture(0,buildingTexArray[r]);
                    building->setMaterialTexture(1,buildingNormalArray[r]);
                    building->getMaterial(0).MaterialTypeParam = 0.035f; // adjust height for parallax effect
                    building->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
                    building->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
//                    building->setAutomaticCulling(true);
                    AddCollisionResponse(building,camera);

                    sidewalk->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
                    sidewalk->setMaterialFlag(EMF_LIGHTING, true);
                    sidewalk->setMaterialTexture(0,sidewalkTex);
//                    sidewalk->setMaterialTexture(1,sidewalkNormal);
                    sidewalk->getMaterial(0).MaterialTypeParam = 0.035f; // adjust height for parallax effect
                    sidewalk->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
                    sidewalk->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
//                    sidewalk->setAutomaticCulling(true);
                    AddCollisionResponse(sidewalk,camera);

                    //random rotation
                    int rot=random(0,3);
                    switch (rot)
                    {
                        case 0:
                            building->setRotation(vector3df(0,0,0));
                            sidewalk->setRotation(vector3df(0,0,0));
                            break;

                        case 1:
                            building->setRotation(vector3df(0,90,0));
                            sidewalk->setRotation(vector3df(0,90,0));
                            break;

                        case 2:
                            building->setRotation(vector3df(0,180,0));
                            sidewalk->setRotation(vector3df(0,180,0));
                            break;

                        case 3:
                            building->setRotation(vector3df(0,270,0));
                            sidewalk->setRotation(vector3df(0,270,0));
                            break;
                    }
                }
            }
        }

        //set player pos
        MoveToFreePosition(camera, cameraHeight);
    }

    void CreatePlayerEnviroment()
    {
        //make fps camera
        SKeyMap keyMap[8];
        keyMap[0].Action = EKA_MOVE_FORWARD;
        keyMap[0].KeyCode = KEY_UP;
        keyMap[1].Action = EKA_MOVE_FORWARD;
        keyMap[1].KeyCode = KEY_KEY_W;

        keyMap[2].Action = EKA_MOVE_BACKWARD;
        keyMap[2].KeyCode = KEY_DOWN;
        keyMap[3].Action = EKA_MOVE_BACKWARD;
        keyMap[3].KeyCode = KEY_KEY_S;

        keyMap[4].Action = EKA_STRAFE_LEFT;
        keyMap[4].KeyCode = KEY_LEFT;
        keyMap[5].Action = EKA_STRAFE_LEFT;
        keyMap[5].KeyCode = KEY_KEY_A;

        keyMap[6].Action = EKA_STRAFE_RIGHT;
        keyMap[6].KeyCode = KEY_RIGHT;
        keyMap[7].Action = EKA_STRAFE_RIGHT;
        keyMap[7].KeyCode = KEY_KEY_D;

        //make camera
        if (!isDebug)
            camera = scene->addCameraSceneNodeFPS(0,playerRotSpeed,playerMovSpeed,0,keyMap,8,true);
        else
            camera = scene->addCameraSceneNodeFPS(0,playerRotSpeed,playerMovSpeed*3,0,keyMap,8,false);
        camera->setPosition(vector3df(rowCount*spacer/2,15,colCount*spacer/2));

        //create player
        player = scene->addAnimatedMeshSceneNode(human,0,-1,vector3df(0,0,0),vector3df(0,0,0),vector3df(1,1,1));
        player->setFrameLoop(1,_ANIMATION_LENGTH*_WALK_ANIM);
        player->setLoopMode(true);
        player->setMaterialFlag(EMF_LIGHTING, true);
        //player->setMaterialTexture(0,playerTex);
        player->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
        player->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);

        //make overhead map
        mapcamera = scene->addCameraSceneNode(0,vector3df(0,maxheight+spacer,0),vector3df(0,0,0),0);

        //set view cutoff
        camera->setFarValue(_CAM_VIS);
        mapcamera->setFarValue(maxheight*2);

        //create weapons
        w_pistol = scene->addAnimatedMeshSceneNode(pistol,camera,_WEAPON_SID,vector3df(1.3,-1.75,2),vector3df(0,180,0),vector3df(2,2,2));
        w_pistol->setAnimationSpeed(0);
        w_pistol->setLoopMode(false);
        w_pistol->setMaterialFlag(EMF_LIGHTING, true);
        w_pistol->setMaterialTexture(0,pistolTex);
        w_pistol->getMaterial(0).EmissiveColor.set(0, 0, 0, 0);
        w_pistol->setVisible(false);

        //set current weapon
        w_current = w_pistol;
        w_current->setVisible(true);

        //flashlight/lamp
        SColorf lightColor=SColorf(1.0f, 1.0f, 1.0f, 1.0f);
        playerLamp=scene->addLightSceneNode(camera, vector3df(0,0,0), lightColor, 35);

        //weapon flash
        SColorf flashColor=SColorf(1.0f, 1.0f, 1.0f, 0.5f);
        weaponLight=scene->addLightSceneNode(camera, vector3df(0,0,0), flashColor, 5);
        weaponLight->setVisible(false);
    }

    void CreateSky()
    {
        ITexture* sky_top=device->getVideoDriver()->getTexture("./textures/sky-top.jpg");
        ITexture* sky_sides=device->getVideoDriver()->getTexture("./textures/sky-sides.jpg");
        scene->addSkyBoxSceneNode(sky_top,0,sky_sides,sky_sides,sky_sides,sky_sides,0,0);
    }

    bool AICanMove(int i)
    {
        float lookAhead=0.0f;
        matrix4 mat=entityArray[i].node->getAbsoluteTransformation();
        vector3df dirVec = vector3df(mat[8],mat[9],mat[10]);
        vector3df movVecLeft;
        vector3df movVecRight;
        vector3df nodePos = entityArray[i].node->getPosition();
        line3d<f32> rayRight;
        line3d<f32> rayLeft;

        //get max lookahead distance in case of low fps
        if (aiColLookAhead < 0)
            lookAhead = min(-1*ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].movSpeed), aiColLookAhead);
        else
            lookAhead = max(ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].movSpeed), aiColLookAhead);

        //right lookahead ray
        movVecRight = dirVec;
        movVecRight.normalize();
        movVecRight.rotateXZBy(aiColLookAngle, vector3df(0,0,0));
        movVecRight *= lookAhead;
        movVecRight += nodePos;
        rayRight = line3d<f32>(nodePos,movVecRight);

        //left lookahead ray
        movVecLeft = dirVec;
        movVecLeft.normalize();
        movVecLeft.rotateXZBy(-1*aiColLookAngle, vector3df(0,0,0));
        movVecLeft *= lookAhead;
        movVecLeft += nodePos;
        rayLeft = line3d<f32>(nodePos,movVecLeft);

        //CHECK FOR BUILDINGS IN THE WAY
        for (int j=0; j<levelSize; j++)
            if (buildingArray[j])
            {
                //if (ai::GetDistance(buildingArray[j],entityArray[i].node) < maxSize)
                //{
                    //if this one hits rotate right
                    if (buildingArray[j]->getTransformedBoundingBox().intersectsWithLine( rayRight ))
                    {
                        entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y + ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
                        return false;
                    }
                    else
                    {
                        //if this hits go left
                        if (buildingArray[j]->getTransformedBoundingBox().intersectsWithLine( rayLeft ))
                        {
                            entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y - ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
                            return false;
                        }
                    }
                //}
            }

        //check walls right
        if (wall1->getTransformedBoundingBox().intersectsWithLine( rayRight ) ||
            wall2->getTransformedBoundingBox().intersectsWithLine( rayRight ) ||
            wall3->getTransformedBoundingBox().intersectsWithLine( rayRight ) ||
            wall4->getTransformedBoundingBox().intersectsWithLine( rayRight ))
        {
            entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y + ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
            return false;
        }

        //check walls left
        if (wall1->getTransformedBoundingBox().intersectsWithLine( rayLeft ) ||
            wall2->getTransformedBoundingBox().intersectsWithLine( rayLeft ) ||
            wall3->getTransformedBoundingBox().intersectsWithLine( rayLeft ) ||
            wall4->getTransformedBoundingBox().intersectsWithLine( rayLeft ))
        {
            entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y - ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
            return false;
        }

        //check for other nodes
        for (int j=0; j<humanCount+zombieCount; j++) //check for other ai nodes in the way
            if (!entityArray[j].isDead && entityArray[j].node->isVisible() && ai::GetDistance(entityArray[i].node,entityArray[j].node) <= zombieAttackDistanceAI-1)
                if (entityArray[i].node!=entityArray[j].node)
                {
                    //if this one hits rotate right
                    if (entityArray[j].node->getTransformedBoundingBox().intersectsWithLine( rayRight ))
                    {
                        entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y + ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
                        return false;
                    }
                    else if (entityArray[j].node->getTransformedBoundingBox().intersectsWithLine( rayLeft ))
                    {
                        entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y - ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
                        return false;
                    }
                }

        //check for player
        /*
        if (ai::GetDistance(entityArray[i].node,player) <= zombieAttackDistance-1)
            if (player->getTransformedBoundingBox().intersectsWithLine( rayRight ))
            {
                entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y + ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
                return false;
            }
            else if (player->getTransformedBoundingBox().intersectsWithLine( rayLeft ))
            {
                entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y - ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed*2), 0));
                return false;
            }
        */

        return true;
    }

    void DoAI()
    {
        //loop through all the monsters
        for (int i=0; i<humanCount+zombieCount; i++)
        {
            if (!entityArray[i].isDead)
            {
                //FIND TARGET
                if (entityArray[i].isZombie)
                {
                    //if player is close enough to chase, then set player to target
                    if (ai::GetDistance(entityArray[i].node, camera)<=zombieVision) {
                        vector3df visVec = ai::getTargetAngle(entityArray[i].node, camera);

                        float yAngle = ai::wrapAngle(visVec.Y-entityArray[i].node->getRotation().Y+_MODEL_OFFSET);

                        if ( yAngle <= zombieFOV || yAngle >= 360-zombieFOV)
                            entityArray[i].target = camera;
                    }

                    if (entityArray[i].target == camera && ai::GetDistance(entityArray[i].node, camera)>zombieVision*1.5)
                            entityArray[i].target = 0;

                    //find a human to attack if player isnt close enough to chase
                    if (entityArray[i].target != camera) {
                        for (int j=0; j<humanCount; j++) {
                            if (!entityArray[j].isZombie && !entityArray[j].isDead) {
                                if (ai::GetDistance(entityArray[i].node, entityArray[j].node)<=zombieVision) {
                                    vector3df visVec = ai::getTargetAngle(entityArray[i].node, entityArray[j].node);

                                    float yAngle = ai::wrapAngle(visVec.Y-entityArray[i].node->getRotation().Y+_MODEL_OFFSET);

                                    if (yAngle <= zombieFOV || yAngle >= 360-zombieFOV) {
                                        entityArray[i].target = entityArray[j].node;
                                        entityArray[i].targetid = j;
                                    }
                                }
                            }
                        }
                    }
                }

                //CORRECT AI HEIGHT
                //TODO: (For player too) Cast a downward ray and just set the height to the nearest thing the ray collides with
                for (int j=0; j<levelSize; j++)
                    if (sidewalkArray[j])
                    {
                        //if this one hits go up 1
                        if (sidewalkArray[j]->getTransformedBoundingBox().isPointInside( vector3df(entityArray[i].node->getPosition().X, 0,entityArray[i].node->getPosition().Z )) )
                        {
                            entityArray[i].node->setPosition( vector3df(entityArray[i].node->getPosition().X, 1, entityArray[i].node->getPosition().Z) );
                            break;
                        }
                        else
                            entityArray[i].node->setPosition( vector3df(entityArray[i].node->getPosition().X, 0, entityArray[i].node->getPosition().Z) );
                    }

                //MOVE TOWARDS TARGET OR ATTACK
                bool canMove=AICanMove(i);

                //NOW MOVE
                if (canMove)
                {
                    if (entityArray[i].attackTimeMs==0 && !entityArray[i].isMorphing)
                        if (!entityArray[i].target ||
                            (entityArray[i].target && entityArray[i].target!=camera && ai::GetDistance(entityArray[i].node, entityArray[i].target) >= zombieAttackDistanceAI) ||
                            (entityArray[i].target==camera && ai::GetDistance(entityArray[i].node, entityArray[i].target) >= zombieAttackDistance))
                        {
                            ai::MoveForward(entityArray[i].node, vector3df(0,0,-1), ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].movSpeed));
                        }
                        else if (entityArray[i].target==camera)
                        {   //attack player
                            vector3df vec=ai::getTargetAngle(entityArray[i].node, entityArray[i].target);
                            vec.Y = ai::wrapAngle(vec.Y+_MODEL_OFFSET);
                            entityArray[i].node->setRotation(vector3df(0,vec.Y,0));

                            entityArray[i].node->setFrameLoop(_ANIMATION_LENGTH*(_ATTACK_ANIM-1)+1,_ANIMATION_LENGTH*_ATTACK_ANIM-1);
                            entityArray[i].node->setAnimationSpeed(_ATTACK_FPS);
                            entityArray[i].node->setLoopMode(false);
                            entityArray[i].node->setAnimationEndCallback(new anim_cb::CAttackEnd(i));
                            entityArray[i].attackTimeMs=currentTimeMs;
                        }
                        else if (!entityArray[entityArray[i].targetid].isDead && !entityArray[entityArray[i].targetid].isMorphing && !entityArray[entityArray[i].targetid].isZombie)
                        {   //attack entity
                            vector3df vec=ai::getTargetAngle(entityArray[i].node, entityArray[i].target);
                            vec.Y = ai::wrapAngle(vec.Y+_MODEL_OFFSET);
                            entityArray[i].node->setRotation(vector3df(0,vec.Y,0));

                            entityArray[i].node->setFrameLoop(_ANIMATION_LENGTH*(_ATTACK_ANIM-1)+1,_ANIMATION_LENGTH*_ATTACK_ANIM-1);
                            entityArray[i].node->setAnimationSpeed(_ATTACK_FPS);
                            entityArray[i].node->setLoopMode(false);
                            entityArray[i].node->setAnimationEndCallback(new anim_cb::CAttackEnd(i));

                            entityArray[entityArray[i].targetid].node->setFrameLoop(_ANIMATION_LENGTH*(_HUMAN_HIT_ANIM-1)+1,_ANIMATION_LENGTH*_HUMAN_HIT_ANIM-1);
                            entityArray[entityArray[i].targetid].node->setAnimationSpeed(_HIT_FPS);
                            entityArray[entityArray[i].targetid].node->setLoopMode(false);
                            entityArray[entityArray[i].targetid].node->setAnimationEndCallback(new anim_cb::CHitEnd(entityArray[i].targetid));
                            entityArray[i].attackTimeMs=currentTimeMs;
                        }

                    //ROTATION AND WANDERING
                    if (entityArray[i].target) {
                        vector3df vec=ai::getTargetAngle(entityArray[i].node, entityArray[i].target);
                        vec.Y = ai::wrapAngle(vec.Y-entityArray[i].node->getRotation().Y+_MODEL_OFFSET);

                        //gradual rotation
                        if (vec.Y > 5 && vec.Y <= 180)
                            entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y + ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed), 0));
                        if (vec.Y > 180 && vec.Y < 355)
                            entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y - ai::GetTimedAmount(currentTimeMs-lastTimeMs, entityArray[i].rotSpeed), 0));

                        //instant rotation
                        //entityArray[i].node->setRotation(vector3df(0,vec.Y,0));
                    }
                    else
                    {
                        //TODO: Wandering
                        //entityArray[i].node->setRotation(vector3df(0, entityArray[i].node->getRotation().Y+0.001, 0));
                    }
                }

            }
        }
    }

    void DrawGame()
    {
        //check if player moved, if so, animate and update pos of model
        if (player->getPosition().X != camera->getPosition().X || player->getPosition().Z != camera->getPosition().Z)
        {
            player->setAnimationSpeed(_WALK_FPS);
            player->setPosition(vector3df(camera->getPosition().X,0,camera->getPosition().Z));
        }
        else
            player->setAnimationSpeed(0);

        player->setRotation(vector3df(0,camera->getRotation().Y-180,0));

        //player height
        for (int j=0; j<levelSize; j++)
            if (sidewalkArray[j]->getTransformedBoundingBox().isPointInside( vector3df(camera->getPosition().X, 0,camera->getPosition().Z )) )
            {
                camera->setPosition( vector3df(camera->getPosition().X, 1, camera->getPosition().Z) );
                break;
            }
            else
                camera->setPosition( vector3df(camera->getPosition().X, 0, camera->getPosition().Z) );

        //DRAW MAIN GAME SCREEN
        if (mapSize!=1)
        {
            driver->clearZBuffer();
            player->setVisible(false);
            driver->setViewPort(rect<s32>(0,0,driver->getScreenSize().Width,driver->getScreenSize().Height));
            scene->setActiveCamera(camera);
            scene->drawAll();
        }

        if (showMap)
        {
            //draw overhead map cam - set player visible. activate mapcam and set the viewpoint. draw.
            driver->clearZBuffer();
            player->setVisible(true);

            //light the minimap
            driver->setAmbientLight(SColorf(1.0f,0.0f,0.0f,1.0f));

            //draw the map
            if (mapSize==1)
                driver->setViewPort(rect<s32>(0,0,driver->getScreenSize().Width/mapSize,driver->getScreenSize().Height/mapSize));
            else
                driver->setViewPort(rect<s32>(15,15,driver->getScreenSize().Width/mapSize,driver->getScreenSize().Height/mapSize));

            scene->setActiveCamera(mapcamera);
            float camH = maxheight*1.1;
            mapcamera->setPosition(vector3df(camera->getPosition().X, camH, camera->getPosition().Z));
            mapcamera->setTarget(camera->getPosition());
            scene->drawAll();

            //disable ambient light
            driver->setAmbientLight(SColorf(0.0f,0.0f,0.0f,0.0f));
        }

        //TODO: draw text (get the function from asciions3d)
        driver->setViewPort(rect<s32>(0,0,driver->getScreenSize().Width,driver->getScreenSize().Height));
    }

    void DoLogic()
    {
        //lamp (flashlight, whatever)
        if (!isDebug) {
            if (playerLamp->isVisible()) {
                lampLife -= ai::GetTimedAmount(currentTimeMs-lastTimeMs, lampLifeRate);
                if (lampLife <= 0)
                    playerLamp->setVisible(false);
            }
            else if (lampLife<lampLifeMax)
                lampLife = std::min(lampLife + ai::GetTimedAmount(currentTimeMs-lastTimeMs, lampLifeChargeRate), lampLifeMax);
        }

        //check life amount
        for (int i=0; i<humanCount+zombieCount; i++)
            if (!entityArray[i].isDead)
            {
                if (entityArray[i].health <= 0) {
                    //they're dead. rotate, play the animation, set the flag
                    if (entityArray[i].isZombie)
                        entityArray[i].node->setRotation(vector3df(0,ai::getTargetAngle(entityArray[i].node, camera).Y+_MODEL_OFFSET,0));

                    entityArray[i].node->setFrameLoop(_ANIMATION_LENGTH*(_DEATH_ANIM-1)+1,_ANIMATION_LENGTH*_DEATH_ANIM-1);
                    entityArray[i].node->setAnimationSpeed(_DEATH_FPS);
                    entityArray[i].node->setLoopMode(false);
                    entityArray[i].node->setAnimationEndCallback(new anim_cb::CDeathEnd());
                    entityArray[i].isDead=true;
                    for (int j=0; j<humanCount+zombieCount; j++)
                        if (entityArray[j].target == entityArray[i].node)
                            entityArray[j].target=0;
                }

                if (entityArray[i].attackTimeMs != 0 && currentTimeMs - entityArray[i].attackTimeMs > zombieAttackDelayMs)
                {
                    entityArray[i].attackTimeMs=0;
                }
            }

        //dead player
        if (playerHealth <= 0)
        {
            device->closeDevice();
            int i;
            cout<<"You're dead!"<<endl;
            cin>>i;
        }

        //correct player height
        /*
        for (int j=0; j<levelSize; j++)
            if (sidewalkArray[j])
            {
                //if this one hits go up 1
                if (sidewalkArray[j]->getTransformedBoundingBox().isPointInside( vector3df(camera->getPosition().X, 0.1,camera->getPosition().Z )) )
                {
                    camera->setPosition( vector3df(camera->getPosition().X, cameraHeight+1, camera->getPosition().Z) );
                    break;
                }
                else
                    camera->setPosition( vector3df(camera->getPosition().X, cameraHeight, camera->getPosition().Z) );
            }
        */
    }

    void DoAnimationCutoff()
    {
        if (currentTimeMs - lastCutoffTimeMs >= 250)
        {
            lastCutoffTimeMs=currentTimeMs;
            for (int i=0; i<humanCount+zombieCount; i++)
            {
                if (!entityArray[i].isDead)
                    if (ai::GetDistance(entityArray[i].node, camera) > camera->getFarValue() * _CAM_VIS_ENTITY_MOD)
                    {
                        entityArray[i].node->setAnimationSpeed(0);
                        entityArray[i].node->setVisible(false);
                        entityArray[i].wasHidden=true;
                    }
                    else if (entityArray[i].wasHidden)
                    {
                        entityArray[i].node->setAnimationSpeed(_WALK_FPS);
                        entityArray[i].node->setVisible(true);
                    }
            }
        }
    }

    void CreateGUI()
    {
        IGUIComboBox *zombieSpeedBox = gui->addComboBox(rect<s32>(80, 240, 155, 262), 0, GUIOBJ_ZOMBIESPEED);
        zombieSpeedBox->addItem(L"5");
        zombieSpeedBox->addItem(L"10");
        zombieSpeedBox->addItem(L"15");
        zombieSpeedBox->addItem(L"20");
        zombieSpeedBox->addItem(L"25");
        zombieSpeedBox->addItem(L"30");
        zombieSpeedBox->addItem(L"35");
        zombieSpeedBox->setSelected(2);
        IGUIComboBox *humanSpeedBox = gui->addComboBox(rect<s32>(80, 216, 155, 238), 0, GUIOBJ_HUMANSPEED);
        humanSpeedBox->addItem(L"5");
        humanSpeedBox->addItem(L"10");
        humanSpeedBox->addItem(L"15");
        humanSpeedBox->addItem(L"20");
        humanSpeedBox->addItem(L"25");
        humanSpeedBox->addItem(L"30");
        humanSpeedBox->addItem(L"35");
        humanSpeedBox->setSelected(1);
        gui->addStaticText(L"Zombie Speed", rect<s32>(10, 240, 80, 260), false, false, 0, -1);
        gui->addStaticText(L"Human Speed:", rect<s32>(10, 216, 80, 236), false, false, 0, -1);
        gui->addEditBox(L"100", rect<s32>(96, 192, 144, 212), true, 0, GUIOBJ_OFFSETMAX);
        gui->addEditBox(L"35", rect<s32>(104, 168, 152, 188), true, 0, GUIOBJ_MINHEIGHT);
        gui->addEditBox(L"300", rect<s32>(104, 144, 152, 164), true, 0, GUIOBJ_MAXHEIGHT);
        gui->addEditBox(L"40", rect<s32>(104, 120, 152, 140), true, 0, GUIOBJ_MINSIZE);
        gui->addEditBox(L"120", rect<s32>(104, 96, 152, 116), true, 0, GUIOBJ_MAXSIZE);
        gui->addEditBox(L"140", rect<s32>(80, 72, 128, 92), true, 0, GUIOBJ_SPACER);
        gui->addButton(rect<s32>(10, 288, 136, 312), 0, GUIOBJ_STARTBUTTON, L"Start The Game!");
        gui->addCheckBox(false, rect<s32>(10, 264, 100, 280), 0, GUIOBJ_DEBUGMODE, L"Debug Mode");
        gui->addStaticText(L"Max Road Offset:", rect<s32>(10, 192, 90, 212), false, false, 0, -1);
        gui->addStaticText(L"Min Building Height:", rect<s32>(10, 168, 100, 188), false, false, 0, -1);
        gui->addStaticText(L"Max Building Height:", rect<s32>(10, 144, 100, 164), false, false, 0, -1);
        gui->addStaticText(L"Min Building Width:", rect<s32>(10, 120, 100, 140), false, false, 0, -1);
        gui->addStaticText(L"Max Building Width:", rect<s32>(10, 96, 100, 116), false, false, 0, -1);
        gui->addStaticText(L"Road Width:", rect<s32>(10, 72, 75, 92), false, false, 0, -1);
        gui->addEditBox(L"98", rect<s32>(80, 48, 128, 68), true, 0, GUIOBJ_ZOMBIECOUNT);
        gui->addStaticText(L"Zombie Count:", rect<s32>(10, 48, 75, 68), false, false, 0, -1);
        gui->addEditBox(L"98", rect<s32>(80, 24, 128, 44), true, 0, GUIOBJ_HUMANCOUNT);
        gui->addStaticText(L"Human Count:", rect<s32>(10, 24, 75, 44), false, false, 0, -1);
        IGUIComboBox *levelSizeBox = gui->addComboBox(rect<s32>(64, 0, 139, 22), 0, GUIOBJ_LEVELSIZE);
        levelSizeBox->addItem(L"16");
        levelSizeBox->addItem(L"36");
        levelSizeBox->addItem(L"49");
        levelSizeBox->addItem(L"64");
        levelSizeBox->addItem(L"81");
        levelSizeBox->addItem(L"100");
        levelSizeBox->addItem(L"121");
        levelSizeBox->addItem(L"144");
        levelSizeBox->setSelected(2);
        gui->addStaticText(L"Level Size:", rect<s32>(10, 0, 60, 20), false, false, 0, -1);
    }

}


//MAIN FUNCTION
int main(int argc, char *argv[])
{
    //get width/height from loader
    if (argc >= 3) {
        game::screenWidth=atoi(argv[1]);
        game::screenHeight=atoi(argv[2]);
        if (argc >= 4)
            game::fullScreen=atoi(argv[3]);
    }

    //event maker
    game::MyEventReceiver receiver;

    //start loading
    game::InitDevice(&receiver);
    game::CreateGUI();

    //wait for settings
    game::device->getCursorControl()->setVisible(true);
    while (game::device->run())
    {
        game::driver->beginScene(true, true, SColor(0,0,150,150));

        if (game::showGUI)
            game::gui->drawAll();
        else
            break;

        game::driver->endScene();
    }

    //now init game
    game::LoadTextures();
    game::LoadMeshes();
    game::CreatePlayerEnviroment();
    game::GenerateCity();
    game::CreateEntities();
    game::CreateSky();

    //set inital timer
    game::lastTimeMs=game::device->getTimer()->getTime();
    game::device->getCursorControl()->setVisible(false);

    //main loop
    int lastFPS=0;
    if (game::device->isWindowActive())
        while(game::device->run())
        {
            game::driver->beginScene(true, true, SColor(0,0,0,0));

            int currFPS=game::driver->getFPS();
            if (lastFPS!=currFPS)
            {
                lastFPS=currFPS;
                stringw caption=L"Please No More Zombies - Copyright 2005 DracSoft - "; caption+=_PNMZ_VERSION;
                caption+=L" - FPS: "; caption+=lastFPS;

                //debugging
                if (game::isDebug) {
                    caption+=L"  X:";
                    caption+=(int)game::camera->getPosition().X;
                    caption+=L" Y:";
                    caption+=(int)game::camera->getPosition().Y;
                    caption+=L" Z:";
                    caption+=(int)game::camera->getPosition().Z;
                }

                game::device->setWindowCaption(caption.c_str());
            }

            //update timers
            game::lastTimeMs=game::currentTimeMs;
            game::currentTimeMs=game::device->getTimer()->getTime();

            //do game logic (lampLife decreasing, etc), animation optimizing
            game::DoLogic();
            game::DoAnimationCutoff();

            //do ai and drawing
            game::DoAI();
            game::DrawGame();

            game::driver->endScene();
        }

    //cleanup
    game::device->drop();

    return 0;
}
