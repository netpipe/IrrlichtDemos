# include<iostream>
# include<iomanip>
# include<math.h>
# include<time.h>
//# include<windows.h>
# include<string.h>
# include<string>
# include<irrlicht.h>
//#include <irrKlang.h>


#include "InGameEventReceiver.h"



#define PI 3.141592653f

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
//using namespace irrklang;
using namespace std;


        void CheckKeyStates(void);
		InGameEventReceiver m_cInGameEvents;

enum
{
    Is_Walking,
    Is_Attacking,
    Is_Being_Attacked,
    Is_Dead,
    Is_Special_Attacking,
    Animation_Number,
};

enum
{
    PISTOL,
    UZI,
    SHOTGUN,
    BARREL,
    GRENADE,
    FAKEWALL,
    CLAYMORE,
    ROCKET,
    CHARGE,
    RAILGUN,
    TOTAL_GUNS,
    DEVIL,
};
enum
{
    NOT_ACTIVE,
    ACTIVE,
    DOING,
    COMPLETED,
};
enum
{
    TUTORIAL,
//FIND_BLUEPRINTS,
    HAMMER_NAILS,
    RETURN_HN,
    STEAL_FRIDGE,
    ASSEMBLE_CHASIS,
    STEAL_ENGINE,
    ASSEMBLE_ENGINE,
    STEAL_TUB,
    GET_GAS,
    RETURN_TUB,
    STEAL_VACUUM,
    ASSEMBLE_FUEL_PUMP,
    STEAL_STAND,
    ATTACH_WHEELS,
    STEAL_BED,
    RETURN_BED,
    STEAL_SHUTTERS,
    ASSEMBLE_PROP,
    STEAL_GATE,
    ATTACH_ROTOR,
    STEAL_CONTROL,
    INSTALL_CONTROL,
    CRANK_UP,
    TAKE_OFF,
};
class Cevents
{
public:
    int state, messagenum, steps, possloc;
    bool complete[10];
    float range, time;
    vector3df eventpos[10];
    IAnimatedMeshSceneNode *Eventnode[10];
};
class Cexplosion
{
public:
    int subs, state;
    float damage, range, anitime, dettime;
    IBillboardSceneNode *Explodenode;
    ILightSceneNode *Elight;
};
class Cenemy
{
public:
    bool devil, animation[Animation_Number], dpcheck;
    f32 anitime;
    float health, cooldown, hitcool, movespeed;
    vector3df heading, hitheading, bodyrotation;
    IAnimatedMeshSceneNode *Bodynode, *Headnode, *Footlnode, *Footrnode, *Armlnode, *Armrnode;
    ILightSceneNode *redeye;
    int level;
};
class Cgun
{
public:
    std::string gunname;
    bool unlocked;
    float cooldown, firetimer, range, damage;
    IAnimatedMeshSceneNode *Gunnode;
    int ammo, gunnum, maxammo;
};

class Cbullet
{
public:
    bool hit;
    int gorigin, hitnum, hitpart;
    float velocity, range, damage, timer;
    f32 oldfdt;
    vector3df heading, oldpos;
    IAnimatedMeshSceneNode *Bulletnode;
    IAnimatedMeshSceneNode *Locketnode;
    IAnimatedMeshSceneNode *Devilnode;
};

class Cmine
{
public:
    int gorigin, state, subs;
    float timer, dettime, damage, range;
    IAnimatedMeshSceneNode *Minenode;
    ILightSceneNode *blink;
};

enum
{
    INACTIVE,
    PLACED,
    TRIGGERED,
    EXPLODED,
    EXPLODING,
    WAITING,
    DRAWING,
};

class Cplayer
{
public:
    int active_gun, level;
    float health, deathtimer, invulntimer, ass_kick_timer;
    bool invuln, is_getting_ass_kicked;
    vector3df ass_kick;
    IAnimatedMeshSceneNode *Playernode;
};

class Cdeathpack
{
public:
    int state, gorigin;
    IAnimatedMeshSceneNode *packnode;
    float timer;
};

class Cheadup
{
public:
    std::string text;
    float timer;
    bool mission;
};

int playersetup(Cplayer &player);
Cgun gunsetup(Cgun gun[]);
int Createbullet(Cbullet &bullet, Cgun &gun, Cplayer, ICameraSceneNode* bcamera);
Cenemy hitcheck(Cbullet &bullet, Cenemy enemy[], Cmine barrel[], Cmine fakewall[], int &zarg, int &zargnum, Cexplosion &explosion, int &explodenum, ICameraSceneNode* Camera, f32 frameps, Cplayer &player, int &killcount);
int spawn(Cenemy &spawnee, Cenemy others[], vector3df spawns[]);
void update(Cenemy updatee, f32 FDT);
void rotate(Cenemy &rTank, f32 timediff, float angleRotation);
void separate(Cenemy &sepZombie, Cenemy otherzombie, float maxrotsep, float maxsep, float sepdist, f32 sepfdt);
void center(Cenemy &cenZombie, vector3df apos, float maxrotcen, f32 cenfdt);
void animate(Cenemy &azombie, f32 frameDeltaTime, Cplayer &player, ICameraSceneNode* Camera, Cgun gun[], Cdeathpack &pack, int &pcount);
void enemysetup(Cenemy &senemy);
Cenemy attackcheck(Cenemy enemy[], ICameraSceneNode* Camera);
void attackhitcheck(Cenemy enemy, Cplayer &player, ICameraSceneNode* Camera);
Cmine minetriggercheck(Cenemy enemy[], Cmine mine[], Cplayer player, Cexplosion &explosion, f32 framedt, int &expcount);
Cmine claymoresetup(Cmine clay[], IAnimatedMesh* claymesh, ISceneManager* smgr);
Cmine chargesetup(Cmine charges[], IAnimatedMesh* chargemesh, ISceneManager* smgr);
Cmine barrelsetup(Cmine barrel[], IAnimatedMesh* barrelmesh, ISceneManager* smgr);
Cmine wallsetup(Cmine wallfake[], IAnimatedMesh* wallfakemesh, ISceneManager* smgr);
void createmine(Cmine &mine, Cplayer player, Cgun &gun, ICameraSceneNode* Camera);
Cenemy explosioncheck(Cenemy enemy[], Cplayer &player, Cexplosion &explosion, ICameraSceneNode* Camera, int &killcount);
void createexplosion(Cexplosion &explosion, vector3df position, float damage, int subs, float range);
Cmine explosioncheckwall(Cmine wall[], Cexplosion &explosion, ICameraSceneNode* Camera);
Cmine explosioncheckbarrel(Cmine barrel[], Cexplosion &explosion, Cexplosion &newexplosion, Cplayer player, ICameraSceneNode* Camera, int &expcount);
Cdeathpack setupdeathpacks(Cdeathpack pack[], ISceneManager* smgr);
Cgun collectdeathpack(Cplayer &player, Cdeathpack &pack, Cgun gun[], ICameraSceneNode* Camera, Cheadup &headup);
void createdeathpack(Cenemy enemy, Cdeathpack &pack, Cgun gun[]);
void ammocounter(int ammo, ITexture* numbers[], IVideoDriver* driver, int height, int killcount, float kktimer, int hp, int width, float actiontime, float actionlength);
void devilattackcheck(Cenemy &devil, Cmine mine[], Cmine fakewall[], ICameraSceneNode* Camera, f32 fdt);
Cevents eventsetup(Cevents events[]);
void eventcheck(Cevents &check, ICameraSceneNode* Camera, float &actiontime, int &gamestep, Cplayer player, f32 fdt, IBillboardSceneNode* arrow, ISceneManager* smgr);

#include "main.h"
#include "collision.h"
#include "weapon.h"
#include "zombie.h"
enum
{
    ID_IsNotPickable,
};
enum
{
    BLAST_ZONE,
    BURN,
    DARKNESS,
    HEARTBEAT,
    MEATGRINDER,
    ON_THE_JOB,
    OUTBREAK,
    ROADKILL,
};

int main()
{
    Cplayer player;
    player.level = 0;
    int c, k, bulletcounter = 0, minecounter = 0, chargecounter = 0, barrelcounter = 0, wallcounter = 0, explosioncounter = 0, dpcounter = 0, i, main_zarg, main_zargnum, killcount = 0, oldkillcount = killcount;
    vector3df bulletpos, spawnz[4], camheading, ssheading;
    float kktimer = 2;
    playersetup(player);
    Cgun gun[10];
    gunsetup(gun);
    Cbullet bullet[200];
    Cenemy zombies[200];
    Cevents events[200];
    int gamestep = 0, oldgamestep = -1, spawnlevel = 1;
    float actiontime = 0, frametimer = 0, songtimer = 0;
    int songplaying = 0, playcount = 0, devilchance = 2;
    float songlength[8], zombietimer = 0, zpause = 2;
    bool songplayed[8], gamestart = true;
    for(c=0; c<8; c++)
        songplayed[c] = false;
    songlength[BLAST_ZONE] = 156;
    songlength[BURN] = 128;
    songlength[DARKNESS] = 106;
    songlength[HEARTBEAT] = 175;
    songlength[MEATGRINDER] = 132;
    songlength[ON_THE_JOB] = 220;
    songlength[OUTBREAK] = 134;
    songlength[ROADKILL] = 168;

    eventsetup(events);


///    srand((UINT)time (NULL));

///    songplaying = rand()% 8;
///    songplayed[songplaying] = true;
///    playcount += 1;

//creates Irrlicht driver, device, and scene manager
    E_DRIVER_TYPE driverType;
    driverType = EDT_OPENGL;
    int swid = 1200, shei = 800;
//int swid = 400, shei = 300;
    IrrlichtDevice* device = createDevice(driverType, dimension2d<u32>(swid, shei));
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
///    ISoundEngine* engine = createIrrKlangDevice();
    device->setEventReceiver ( &m_cInGameEvents );
    device->getFileSystem()->addZipFileArchive("./data.zip");

    Cheadup headup;
    Cmine mines[200];
    IAnimatedMesh* claymore = smgr->getMesh("claymore.obj");
    claymoresetup(mines, claymore, smgr);
    Cmine charges[200];
    IAnimatedMesh* charge = smgr->getMesh("chargepack.obj");
    chargesetup(charges, charge, smgr);
    Cmine barrels[200];
    IAnimatedMesh* barrel = smgr->getMesh("barrel.obj");
    barrelsetup(barrels, barrel, smgr);
    Cmine fakewalls[200];
    IAnimatedMesh* fakewall = smgr->getMesh("fakewall.obj");
    wallsetup(fakewalls, fakewall, smgr);
    Cexplosion explosions[200];
    ITexture *exptext = driver->getTexture("asplosion2.png");
    driver->makeColorKeyTexture(exptext, core::position2d<s32>(0,0));
    for(c = 0; c <200; c++)
    {
        explosions[c].state = INACTIVE;
        explosions[c].Explodenode = smgr->addBillboardSceneNode(0, dimension2d<f32>(10,10), vector3df(0,0,0), -1, 4294967295U, 4294967295U);
        explosions[c].Explodenode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
        explosions[c].Explodenode->setMaterialTexture(0, exptext);
        explosions[c].Explodenode->setVisible(false);
        explosions[c].Elight = smgr->addLightSceneNode(explosions[c].Explodenode, vector3df(0,100,0), SColorf(1.f, 1.f, 1.f), 0.f);
    }
    Cdeathpack deathpacks[200];
    setupdeathpacks(deathpacks, smgr);

#include "level.h"




    IBillboardSceneNode* bigarrow;
    ITexture* arrowtext = driver->getTexture("BIGARROW.png");
    driver->makeColorKeyTexture(arrowtext, core::position2d<s32>(0,0));
    bigarrow = smgr->addBillboardSceneNode(0, dimension2d<f32>(136,400), vector3df(0,400,0), -1, 429467295U, 429467295U);
    bigarrow->setMaterialFlag(video::EMF_LIGHTING, false);
    bigarrow->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    bigarrow->setMaterialTexture(0, arrowtext);


    ITriangleSelector* selector = 0;
    selector = smgr->createOctreeTriangleSelector(
                   floornode->getMesh(), floornode, 128);
    floornode->setTriangleSelector(selector);


    ICameraSceneNode* camera =
        smgr->addCameraSceneNodeFPS(0, 100.0f, .2f, ID_IsNotPickable, 0, 0, true, 3.f);
    camera->setPosition(vector3df(200,100,0));
    camera->setTarget(core::vector3df(0,500,1000));

    /*gun[PISTOL].Gunnode->setParent(camera);
    gun[SHOTGUN].Gunnode->setParent(camera);
    gun[SHOTGUN].Gunnode->setVisible(false);*/
    for(c = 0; c < TOTAL_GUNS; c++)
    {
        gun[c].Gunnode->setParent(camera);
        gun[c].Gunnode->setVisible(false);
    }

    scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
                                          selector, camera, core::vector3df(15,35,15),
                                          core::vector3df(0,-3,0), core::vector3df(0,20,0));
    camera->addAnimator(anim);
    anim->drop();

    for(c = 0; c < 200; c++)
    {
        ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
                                       selector, zombies[c].Bodynode, core::vector3df(20,35,20),
                                       core::vector3df(0,0,0), core::vector3df(0,-11,0));
        zombies[c].Bodynode->addAnimator(anim);
        anim->drop();

    }

    for(c = 0; c < 200; c++)
    {
        ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
                                       selector, bullet[c].Bulletnode, core::vector3df(1,1,1),
                                       core::vector3df(0,000,0), core::vector3df(0,0,0));
        bullet[c].Bulletnode->addAnimator(anim);
        anim->drop();

    }


    selector->drop();
    spawnz[0] = vector3df( 0, 10, -2000);
    spawnz[1] = vector3df( 0, 10,2000);
    spawnz[2] = vector3df(-2500, 10, 0);
    spawnz[3] = vector3df(2500, 10,0);

    float sdist, tdist;
    int closest;
    bool close, sgnpump, is_centering;
    vector3df tpos1, tpos2, bpos1, bpos2;

    int cheatnum = 0, cheatlevel = 0;

    IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();

    while(gamestart == false)
    {
        driver->beginScene(true, true, video::SColor(255,200,200,200));
        driver->draw2DImage(Splash, position2d<s32>((swid / 2.f) - (527.f / 2.f), (shei / 2.f) - (294.f / 2.f)), rect<s32>(0,0,527, 294), 0, SColor(255,255,255,255), true);
        driver->endScene();
        if (m_cInGameEvents.getKeyState(    KEY_KEY_E))
            gamestart = true;
    }

//    switch(songplaying)
//    {
//    case BLAST_ZONE:
//        engine->play2D("Music/Blast Zone.ogg");
//        break;
//    case BURN:
//        engine->play2D("Music/Burn.ogg");
//        break;
//    case DARKNESS:
//        engine->play2D("Music/Darkness.ogg");
//        break;
//    case HEARTBEAT:
//        engine->play2D("Music/Heartbeat.ogg");
//        break;
//    case MEATGRINDER:
//        engine->play2D("Music/Meatgrinder.ogg");
//        break;
//    case ON_THE_JOB:
//        engine->play2D("Music/On the Job.ogg");
//        break;
//    case OUTBREAK:
//        engine->play2D("Music/Outbreak.ogg");
//        break;
//    case ROADKILL:
//        engine->play2D("Music/Roadkill.ogg");
//        break;
//    }

    device->getCursorControl()->setVisible(false);
    u32 then = device->getTimer()->getTime();

    while(device->run())
    {
//CHEAT COOOOOOOOOODE 1337 H4xx0rz
        if(cheatnum <= cheatlevel)
        {
            cheatnum += 1;
            killcount += 1;
        }
        if(gamestep > oldgamestep)
        {
            cout<< gamestep <<endl;
            events[gamestep].state = ACTIVE;
            actiontime = 0;
            switch(gamestep)
            {
            case TUTORIAL:
                spawnlevel = 20;
                devilchance = 2;
                zpause = 1.f;
                break;
            case HAMMER_NAILS:
                spawnlevel = 24;
                devilchance = 2;
                zpause = .95f;
                break;
            case RETURN_HN:
                spawnlevel = 25;
                devilchance = 2;
                zpause = .9f;
                break;
            case STEAL_FRIDGE:
                spawnlevel = 28;
                devilchance = 2;
                zpause = .85f;
                break;
            case ASSEMBLE_CHASIS:
                spawnlevel = 30;
                devilchance = 20;
                zpause = .8f;
                break;
            case STEAL_ENGINE:
                spawnlevel = 34;
                devilchance = 19;
                zpause = .75f;
                break;
            case ASSEMBLE_ENGINE:
                spawnlevel = 35;
                devilchance = 18;
                zpause = .7f;
                break;
            case STEAL_TUB:
                spawnlevel = 36;
                devilchance = 17;
                zpause = .65f;
                break;
            case GET_GAS:
                spawnlevel = 37;
                devilchance = 16;
                zpause = .65f;
                break;
            case RETURN_TUB:
                spawnlevel = 38;
                devilchance = 15;
                zpause = .6f;
                break;
            case STEAL_VACUUM:
                spawnlevel = 40;
                devilchance = 15;
                zpause = .6f;
                break;
            case ASSEMBLE_FUEL_PUMP:
                spawnlevel = 41;
                devilchance = 14;
                zpause = .55f;
                break;
            case STEAL_STAND:
                spawnlevel = 42;
                devilchance = 13;
                zpause = .55f;
                break;
            case ATTACH_WHEELS:
                spawnlevel = 44;
                devilchance = 12;
                zpause = .5f;
                break;
            case STEAL_BED:
                spawnlevel = 45;
                devilchance = 11;
                zpause = .45f;
                break;
            case RETURN_BED:
                spawnlevel = 46;
                devilchance = 10;
                zpause = .45f;
                break;
            case STEAL_SHUTTERS:
                spawnlevel = 47;
                devilchance = 10;
                zpause = .4f;
                break;
            case ASSEMBLE_PROP:
                spawnlevel = 48;
                devilchance = 10;
                zpause = .4f;
                break;
            case STEAL_GATE:
                spawnlevel = 50;
                devilchance = 10;
                zpause = .35f;
                break;
            case ATTACH_ROTOR:
                spawnlevel = 52;
                devilchance = 10;
                zpause = .35f;
                break;
            case STEAL_CONTROL:
                spawnlevel = 55;
                devilchance = 10;
                zpause = .3f;
                break;
            case INSTALL_CONTROL:
                spawnlevel = 60;
                devilchance = 10;
                zpause = .3f;
                break;
            case CRANK_UP:
                spawnlevel = 65;
                devilchance = 10;
                zpause = .25f;
                break;
            case TAKE_OFF:
                spawnlevel = 70;
                devilchance = 10;
                zpause = .25f;
                break;
            }
        }
        oldgamestep = gamestep;

        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f;
        const f32 frameps = 1/(frameDeltaTime);
        then = now;

#include "ammo.h"



//zxc
        driver->beginScene(true, true, video::SColor(255,200,200,200));
        smgr->drawAll();
        driver->setTransform(ETS_WORLD, IdentityMatrix);
        driver->draw2DImage(crosshair, position2d<s32>(swid / 2 - 60, shei / 2 - 60), rect<s32>(0,0,121,121), 0, SColor(255,255,255,255), true);
        driver->draw2DImage(ammo, position2d<s32>(150, shei - 140 ), rect<s32>(0,0,125,128), 0, SColor(255,255,255,255), true);
        driver->draw2DImage(hp, position2d<s32>(250, shei - 120 ), rect<s32>(0,0,104, 93), 0, SColor(255,255,255,255), true);
        driver->draw2DImage(hud, position2d<s32>(-35, shei - 180 ), rect<s32>(0,0,523, 233), 0, SColor(150,2.55 * (100 - player.health), 2.55 * player.health,255), true);
        driver->draw2DRectangle(SColor(200, 2.55 * (100 - player.health), 2.55 * player.health, 0), rect<s32>(350, shei - 90, 350 + (s32)player.health * 5, shei - 40), 0);
        driver->draw2DImage(skull, position2d<s32>(swid - 300, shei - 140 ), rect<s32>(0,0,100, 123), 0, SColor(255,255,255,255), true);
        gun[PISTOL].ammo = 1;
        ammocounter(gun[player.active_gun].ammo, Numbers, driver, shei, killcount, kktimer, player.health, swid, actiontime, events[gamestep].time);
        font->draw(L"graphics.", rect<s32>(10,10,800,800), SColor(255,255,255,255));
        driver->endScene();

        kktimer -= frameDeltaTime * (0.5f + (.01 * killcount));
        if(oldkillcount < killcount)
            kktimer = 2;
        if(kktimer <= 0 && killcount != 0)
        {
            killcount -= 1;
            kktimer = 2;
        }
        oldkillcount = killcount;

        zombietimer += frameDeltaTime;

        for(c = 0; c < spawnlevel; c++) // c < 200
            if(zombies[c].health <= 0 && zombies[c].animation[Is_Dead] == false && zombietimer >= zpause)
            {
                zombietimer = 0;
                int whee = rand()% devilchance;
                zombies[c].devil = false;
                if(whee == 7)
                    zombies[c].devil = true;

                if(zombies[c].devil)
                {
                    zombies[c].Bodynode->setMesh(dbodymesh);
                    zombies[c].Armlnode->setMesh(darmlmesh);
                    zombies[c].Armrnode->setMesh(darmrmesh);
                    zombies[c].Footlnode->setMesh(dfootlmesh);
                    zombies[c].Footrnode->setMesh(dfootrmesh);
                    zombies[c].Headnode->setMesh(dheadmesh);
                    zombies[c].redeye->setRadius(20.f);
                    zombies[c].redeye->setPosition(vector3df(0, 50, 20));
                }
                else
                {
                    zombies[c].Bodynode->setMesh(bodymesh);
                    zombies[c].Armlnode->setMesh(armlmesh);
                    zombies[c].Armrnode->setMesh(armrmesh);
                    zombies[c].Footlnode->setMesh(footlmesh);
                    zombies[c].Footrnode->setMesh(footrmesh);
                    zombies[c].Headnode->setMesh(headmesh);
                    zombies[c].redeye->setRadius(10.f);
                    zombies[c].redeye->setPosition(vector3df(0, 27, 13));
                }

                spawn(zombies[c], zombies, spawnz);
                rotate(zombies[c], frameDeltaTime, 0.1f);
            }

        player.health += (100.f - player.health) * frameDeltaTime * .07f;
        if(player.ass_kick_timer > 0)
            player.ass_kick_timer -= frameDeltaTime;
        if(player.ass_kick_timer <= 0)
            player.is_getting_ass_kicked = false;
        if(player.is_getting_ass_kicked && player.ass_kick_timer > 0)
            camera->setPosition(camera->getPosition() + player.ass_kick * frameDeltaTime * 400);
        frametimer += 1;
        if(frametimer >= 50)
        {
            frametimer = 0;
            for(c=0; c<200; c++)
                if(zombies[c].health > 0)
                    zombies[c].Bodynode->setPosition(vector3df(zombies[c].Bodynode->getPosition().X,-200,zombies[c].Bodynode->getPosition().Z));
        }
        for(c = 0; c < 200; c++)
        {
            if(zombies[c].animation[Is_Dead] && zombies[c].anitime > 3)
                enemysetup(zombies[c]);
            animate(zombies[c], frameDeltaTime, player, camera, gun, deathpacks[dpcounter], dpcounter);
            update(zombies[c], frameDeltaTime);
            tpos1 = zombies[c].Bodynode->getPosition();
            is_centering = true;

            close = false;
            closest = -1;
            sdist = 100;
            for(i = 0; i < 200; i++)
            {
                tpos2 = fakewalls[i].Minenode->getPosition();
                tdist = sqrt((tpos1.X - tpos2.X) * (tpos1.X - tpos2.X) + (tpos1.Z - tpos2.Z) * (tpos1.Z - tpos2.Z));
                if(tdist < sdist && fakewalls[i].state == PLACED)
                {
                    close = true;
                    sdist = tdist;
                    closest = i;
                }
            }
            if(close && sqrt((tpos1.X - camera->getPosition().X) * (tpos1.X - camera->getPosition().X) + (tpos1.Z - camera->getPosition().Z) * (tpos1.Z - camera->getPosition().Z)) > 150)
            {
                is_centering = false;
                separate(zombies[c], zombies[closest], 2.f * PI, 150, sdist, frameDeltaTime);
            }

            if(is_centering)
            {
                close = false;
                closest = -1;
                sdist = 100;
                for(i = 0; i < 200; i++)
                {
                    if(i != c)
                    {
                        tpos2 = zombies[i].Bodynode->getPosition();
                        tdist = sqrt((tpos1.X - tpos2.X) * (tpos1.X - tpos2.X) + (tpos1.Z - tpos2.Z) * (tpos1.Z - tpos2.Z));
                        if(tdist < sdist)
                        {
                            close = true;
                            sdist = tdist;
                            closest = i;
                        }
                    }
                }
                if(close && sqrt((tpos1.X - camera->getPosition().X) * (tpos1.X - camera->getPosition().X) + (tpos1.Z - camera->getPosition().Z) * (tpos1.Z - camera->getPosition().Z)) > 150)
                    separate(zombies[c], zombies[closest], 2.0, 150, sdist, frameDeltaTime);
            }

            if(is_centering)
                center(zombies[c], camera->getPosition(), 1.2 * PI, frameDeltaTime);
        }

        camheading = camera->getTarget() - camera->getAbsolutePosition();
        camheading.Y = 0;
        camheading.normalize();
        if (m_cInGameEvents.getKeyState(    KEY_KEY_W) ) // W
        {
            camera->setPosition(camera->getAbsolutePosition() + camheading * 150.f * frameDeltaTime);
        }
        if (m_cInGameEvents.getKeyState(    KEY_KEY_A)) // A
        {
            ssheading = camheading;
            float oldX;
            oldX = ssheading.X;
            ssheading.X = ssheading.X * cos(PI/2.f) - ssheading.Z * sin(PI/2.f);
            ssheading.Z = oldX * sin(PI/2.f) + ssheading.Z * cos(PI/2.f);
            camera->setPosition(camera->getAbsolutePosition() + ssheading * 150.f * frameDeltaTime);
        }
        if (m_cInGameEvents.getKeyState(    KEY_KEY_S)) // S
        {
            camera->setPosition(camera->getAbsolutePosition() + camheading * 150.f * -frameDeltaTime);
        }
        if (m_cInGameEvents.getKeyState(    KEY_KEY_D)) // D
        {
            ssheading = camheading;
            float oldX;
            oldX = ssheading.X;
            ssheading.X = ssheading.X * cos(-PI/2.f) - ssheading.Z * sin(-PI/2.f);
            ssheading.Z = oldX * sin(-PI/2.f) + ssheading.Z * cos(-PI/2.f);
            camera->setPosition(camera->getAbsolutePosition() + ssheading * 150.f * frameDeltaTime);
        }
        if (m_cInGameEvents.getKeyState(    KEY_KEY_E)) //GetAsyncKeyState(0x45))
            eventcheck(events[gamestep], camera, actiontime, gamestep, player, frameDeltaTime, bigarrow, smgr);
        else
        {
            events[gamestep].state = ACTIVE;
            actiontime = 0;
        }
        if (m_cInGameEvents.getKeyState(    KEY_SPACE) && m_cInGameEvents.getKeyState(    KEY_KEY_E) == false)//(GetAsyncKeyState(0x01) && GetAsyncKeyState(0x45) == false)
        {
///???
            if(gun[player.active_gun].firetimer == 0)
            {
                gun[player.active_gun].firetimer = gun[player.active_gun].cooldown;
                if(player.active_gun != CHARGE || chargecounter < 5)
                    gun[player.active_gun].ammo -= 1;
                if(player.active_gun == PISTOL || player.active_gun == UZI || player.active_gun == RAILGUN || player.active_gun == ROCKET)
                {
                    if(player.active_gun == PISTOL)
//                        engine->play2D("Sounds/Weapons/Rifle_Fire quiet.wav");
                    if(player.active_gun == UZI)
 //                       engine->play2D("Sounds/Weapons/Uzi_Fire (Normalized) (1 Shot) quiet.wav");
                    if(player.active_gun == RAILGUN)
  //                      engine->play2D("Sounds/Weapons/Railgun_Fire quiet.wav");

                    Createbullet(bullet[bulletcounter], gun[player.active_gun], player, camera);
                    if(player.active_gun == ROCKET)
                    {
   //                     engine->play2D("Sounds/Weapons/RocketLauncher_Fire quiet.wav");
                        bullet[bulletcounter].Locketnode->setVisible(true);
                        bullet[bulletcounter].Devilnode->setVisible(false);
                    }
                    else
                    {
                        bullet[bulletcounter].Locketnode->setVisible(false);
                        bullet[bulletcounter].Devilnode->setVisible(false);
                    }
                    bulletcounter += 1;
                    if(bulletcounter > 199)
                        bulletcounter = 0;
                }
                switch (player.active_gun)
                {
                case SHOTGUN:
   //                 engine->play2D("Sounds/Weapons/Shotgun_Fire quiet.wav");
                    sgnpump = false;
                    for(i = 0; i < 5; i++)
                    {
                        Createbullet(bullet[bulletcounter], gun[player.active_gun], player, camera);
                        bullet[bulletcounter].Locketnode->setVisible(false);
                        bullet[bulletcounter].Devilnode->setVisible(false);
                        bulletcounter += 1;
                        if(bulletcounter > 199)
                            bulletcounter = 0;
                    }
                    break;

                case CLAYMORE:
                    createmine(mines[minecounter], player, gun[player.active_gun], camera);
                    minecounter += 1;
                    if(minecounter > 199)
                        minecounter = 0;
                    break;

                case CHARGE:
                    if(chargecounter < 5)
                    {
                        createmine(charges[chargecounter], player, gun[player.active_gun], camera);
                        chargecounter += 1;
                        if(chargecounter > 199)
                            chargecounter = 0;
                    }
                    break;

                case BARREL:
                    createmine(barrels[barrelcounter], player, gun[player.active_gun], camera);
                    barrelcounter += 1;
                    if(barrelcounter > 199)
                        barrelcounter = 0;
                    break;

                case FAKEWALL:
                    createmine(fakewalls[wallcounter], player, gun[player.active_gun], camera);
                    wallcounter += 1;
                    if(wallcounter > 199)
                        wallcounter = 0;
                    break;

                }
            }
        }
        if (m_cInGameEvents.getKeyState(    KEY_KEY_0) && m_cInGameEvents.getKeyState(    KEY_KEY_W) == false)//GetAsyncKeyState((0x02))&1 && GetAsyncKeyState(0x45) == false)
        {
            player.active_gun += 1;
            while(gun[player.active_gun].ammo <= 0 && player.active_gun != PISTOL)
            {
                player.active_gun += 1;
                if(player.active_gun >= TOTAL_GUNS)
                    player.active_gun = PISTOL;
            }
            for(c = 0; c < TOTAL_GUNS; c++)
            {
                gun[c].Gunnode->setVisible(false);
            }
            gun[player.active_gun].Gunnode->setVisible(true);
        }

        if (m_cInGameEvents.LEFTBUTTONCLICKED && m_cInGameEvents.getKeyState(    KEY_KEY_E) == false) //&& player.active_gun == CHARGE)
        {
            for(k=0; k<200; k++)
                if(charges[k].state == PLACED)
                {
                    createexplosion(explosions[explosioncounter], charges[k].Minenode->getAbsolutePosition(), charges[k].damage, charges[k].subs, charges[k].range);
                    explosioncounter += 1;
                    if(explosioncounter > 199)
                        explosioncounter = 1;
                    charges[k].state = INACTIVE;
                    charges[k].Minenode->setPosition(vector3df(0,1000,0));
                    charges[k].Minenode->setVisible(false);
                }
            chargecounter = 0;
        }

        if(gun[player.active_gun].ammo <= 0 && player.active_gun != PISTOL)
        {
            gun[player.active_gun].Gunnode->setVisible(false);
            player.active_gun = PISTOL;
        }

        if(gun[player.active_gun].ammo > 0 || player.active_gun == PISTOL)
            gun[player.active_gun].Gunnode->setVisible(true);

//cooldown gun
        if(gun[player.active_gun].firetimer > 0)
            gun[player.active_gun].firetimer -= (frameDeltaTime * 1000);
        if(gun[player.active_gun].firetimer < 0)
            gun[player.active_gun].firetimer = 0;

//recoil gun
        if(gun[player.active_gun].firetimer > .5 * gun[player.active_gun].cooldown)
            gun[player.active_gun].Gunnode->setRotation( vector3df(10.f * sin( 2.f * gun[player.active_gun].firetimer * PI / gun[player.active_gun].cooldown ), 0, 0));
        else
        {
            gun[player.active_gun].Gunnode->setRotation( vector3df(0,0,0));
            if(player.active_gun == SHOTGUN && sgnpump == false)
            {
       ///         engine->play2D("Sounds/Weapons/Shotgun_PumpAction.wav");
                sgnpump = true;
            }
        }
//update bullets
        for(c = 0; c < 200; c++)
        {
            bullet[c].timer += frameDeltaTime;
            if(bullet[c].timer > 2)
                bullet[c].hit = true;
            if(bullet[c].hit == false)
            {
                if(bullet[c].gorigin == ROCKET)
                    bullet[c].velocity += 5001 * frameDeltaTime;

                hitcheck(bullet[c], zombies, barrels, fakewalls, main_zarg, main_zargnum, explosions[explosioncounter], explosioncounter, camera, 1.0f / frameDeltaTime, player, killcount);
                if(main_zarg == 1)
                {
                    createexplosion(explosions[explosioncounter], barrels[main_zargnum].Minenode->getPosition(), barrels[main_zargnum].damage, barrels[main_zargnum].subs, barrels[main_zargnum].range);
                    explosioncounter += 1;
                    if(explosioncounter > 199)
                        explosioncounter = 0;
                    barrels[main_zargnum].state = INACTIVE;
                    barrels[main_zargnum].Minenode->setVisible(false);
                }
                if(main_zarg == 2)
                {
                    fakewalls[main_zargnum].dettime -= bullet[c].damage;
                    if(fakewalls[main_zargnum].dettime <= 0)
                    {
                        fakewalls[main_zargnum].state = INACTIVE;
                        fakewalls[main_zargnum].Minenode->setVisible(false);
                    }
                }
                bulletpos = bullet[c].Bulletnode->getAbsolutePosition() - camera->getAbsolutePosition();
                if(sqrt(bulletpos.X * bulletpos.X + bulletpos.Z * bulletpos.Z) > bullet[c].range)
                {
                    bullet[c].hit = true;
                    bullet[c].Bulletnode->setPosition(vector3df(0,1000,0));
                }
            }
            if(bullet[c].hit)
            {
                bullet[c].Bulletnode->setVisible(false);
                bullet[c].Bulletnode->setPosition(vector3df(0,1000,0));
            }
        }
// ATTACK CHECKS
        attackcheck(zombies, camera);
        for(c=0; c<200; c++)
        {
            if(zombies[c].devil == true && zombies[c].animation[Is_Walking] == true)
                devilattackcheck(zombies[c], barrels, fakewalls, camera, frameDeltaTime);
            if(zombies[c].devil == true && zombies[c].animation[Is_Attacking] == true && zombies[c].anitime >= 1 && zombies[c].hitcool == 0)
            {
                bullet[bulletcounter].gorigin = DEVIL;
                bullet[bulletcounter].timer = 0;
                bullet[bulletcounter].Bulletnode->setPosition(zombies[c].Bodynode->getPosition());
                bullet[bulletcounter].Bulletnode->setRotation(vector3df(0,zombies[c].Bodynode->getRotation().Y,0));
                bullet[bulletcounter].Bulletnode->setVisible(true);
                bullet[bulletcounter].velocity = 400;
                bullet[bulletcounter].range = 500;
                bullet[bulletcounter].damage = 15;
                bullet[bulletcounter].hit = false;
                bullet[bulletcounter].heading = zombies[c].heading;
                bulletcounter += 1;
                if(bulletcounter > 199)
                    bulletcounter = 0;
                bullet[bulletcounter].Devilnode->setVisible(true);
                zombies[c].hitcool = 999;
            }
        }
        minetriggercheck(zombies, mines, player, explosions[explosioncounter], frameDeltaTime, explosioncounter);

        for(c = 0; c < 200; c++)
        {
            collectdeathpack(player, deathpacks[c], gun, camera, headup);
            if(deathpacks[c].state == PLACED)
                deathpacks[c].timer -= frameDeltaTime;
            if(deathpacks[c].state == PLACED && deathpacks[c].timer < 0)
            {
                deathpacks[c].state = INACTIVE;
                deathpacks[c].packnode->setVisible(false);
            }

            if(explosions[c].state == EXPLODING)
            {
                if(explosions[c].dettime > 0)
                    explosions[c].dettime -= frameDeltaTime;
                if(explosions[c].dettime <= 0)
                {
                    explosioncheck(zombies, player, explosions[c], camera, killcount);
                    explosioncheckwall(fakewalls, explosions[c], camera);
                    explosioncheckbarrel(barrels, explosions[c], explosions[explosioncounter], player, camera, explosioncounter);
                    explosions[c].state = DRAWING;
//                    engine->play2D("Sounds/Weapons/Explosion quiet.wav");
                    explosions[c].anitime = 0;
                    explosions[c].Elight->setRadius(100.f);
                    if(explosions[c].subs > 0)
                        while(explosions[c].subs > 0)
                        {
                            createexplosion(explosions[explosioncounter], explosions[c].Explodenode->getPosition(), explosions[c].damage, -1, 80);
                            explosioncounter += 1;
                            explosions[c].subs -= 1;
                        }
                    vector3df explop = explosions[c].Explodenode->getPosition() - camera->getPosition();
                    explop.normalize();
                    explop *= -30;
                    explosions[c].Elight->setPosition(explop);
                }
            }
            if(explosions[c].state == DRAWING)
            {
                explosions[c].Explodenode->setVisible(true);
                explosions[c].anitime += frameDeltaTime;
                explosions[c].Explodenode->setSize(dimension2d<f32>(explosions[c].anitime * (10 * explosions[c].range),explosions[c].anitime * (10 * explosions[c].range)));
                if(explosions[c].anitime > .25)
                {
                    explosions[c].Explodenode->setSize(dimension2d<f32>(1,1));
                    explosions[c].state = INACTIVE;
                    explosions[c].Explodenode->setVisible(false);
                    explosions[c].Elight->setRadius(0.f);
                }

            }
            if(mines[c].state == PLACED)
            {
                mines[c].blink->setRadius(10.f);
            }
            if(mines[c].state == TRIGGERED)
            {
                if(mines[c].timer < .2)
                    mines[c].blink->setRadius(50.f);
                if(mines[c].timer > .2 && mines[c].timer < .4)
                    mines[c].blink->setRadius(0.f);
                if(mines[c].timer > .4 && mines[c].timer < .6)
                    mines[c].blink->setRadius(50.f);
                if(mines[c].timer > .6 && mines[c].timer < .8)
                    mines[c].blink->setRadius(0.f);
                if(mines[c].timer > .8 && mines[c].timer < 1)
                    mines[c].blink->setRadius(50.f);
                if(mines[c].timer > 1 && mines[c].timer < 1.2)
                    mines[c].blink->setRadius(0.f);
            }
            if(charges[c].state == INACTIVE)
                charges[c].blink->setRadius(0.f);
            if(charges[c].state == PLACED)
            {
                charges[c].blink->setRadius(10.f);
            }
            if(charges[c].state == TRIGGERED)
            {
                if(charges[c].timer < .2)
                    charges[c].blink->setRadius(50.f);
                if(charges[c].timer > .2 && mines[c].timer < .4)
                    charges[c].blink->setRadius(0.f);
                if(charges[c].timer > .4 && mines[c].timer < .6)
                    charges[c].blink->setRadius(50.f);
                if(charges[c].timer > .6 && mines[c].timer < .8)
                    charges[c].blink->setRadius(0.f);
                if(charges[c].timer > .8 && mines[c].timer < 1)
                    charges[c].blink->setRadius(50.f);
                if(charges[c].timer > 1 && mines[c].timer < 1.2)
                    charges[c].blink->setRadius(0.f);
            }
            if(charges[c].state == INACTIVE)
                charges[c].blink->setRadius(0.f);
        }
//        songtimer += frameDeltaTime;
//        if(songtimer >= songlength[songplaying] + 16)
//        {
//            while(songplayed[songplaying] == true)
//                songplaying = rand() % 8;
//            switch(songplaying)
//            {
//            case BLAST_ZONE:
//                engine->play2D("Music/Blast Zone.ogg");
//                break;
//            case BURN:
//                engine->play2D("Music/Burn.ogg");
//                break;
//            case DARKNESS:
//                engine->play2D("Music/Darkness.ogg");
//                break;
//            case HEARTBEAT:
//                engine->play2D("Music/Heartbeat.ogg");
//                break;
//            case MEATGRINDER:
//                engine->play2D("Music/Meatgrinder.ogg");
//                break;
//            case ON_THE_JOB:
//                engine->play2D("Music/On the Job.ogg");
//                break;
//            case OUTBREAK:
//                engine->play2D("Music/Outbreak.ogg");
//                break;
//            case ROADKILL:
//                engine->play2D("Music/Roadkill.ogg");
//                break;
//            }
//            songplayed[songplaying] = true;
            songtimer = 0;
            playcount += 1;
            if(playcount >= 8)
                for(c=0; c<8; c++)
                    songplayed[c] = false;
        }
//DEATH CODE
        if(player.health <= 0)
        {
            camera->setPosition(vector3df(1000,0,1000));
            camera->setTarget(vector3df(0,0,0));
            player.health = 100;
            for(c=1; c<10; c++)
                gun[c].ammo = 0;
        }
    }
 //   device->drop();
 //   engine->drop();
//    return 0;


void eventcheck(Cevents &check, ICameraSceneNode* Camera, float &actiontime, int &gamestep, Cplayer player, f32 fdt, IBillboardSceneNode* arrow, ISceneManager* smgr)
{
    int c, closest, comped = 0;
    float dist[10];

    if(check.possloc > 1)
    {
        closest = 999;
        for(c=0; c<check.possloc; c++)
        {
            if(check.complete[c] == false)
            {
                dist[c] = sqrt(pow(check.eventpos[c].X - Camera->getPosition().X,2) + pow(check.eventpos[c].Z - Camera->getPosition().Z,2));
                if(closest == 999)
                    closest = c;
                else if(dist[c] < dist[closest])
                    closest = c;
            }
        }
        if(closest == 999)
            closest = 0;
    }
    else
    {
        closest = 0;
        dist[0] = sqrt(pow(check.eventpos[closest].X - Camera->getPosition().X,2) + pow(check.eventpos[closest].Z - Camera->getPosition().Z,2));
    }

    vector3df toplayer = check.eventpos[closest] - Camera->getPosition(), cenresult, camerahead;
    float resultmag;
    camerahead = Camera->getTarget() - Camera->getAbsolutePosition();
    camerahead.normalize();
    cenresult = (1000 * camerahead) + toplayer;
    resultmag = sqrt(cenresult.X * cenresult.X + cenresult.Z * cenresult.Z);

    arrow->setPosition(check.eventpos[closest] + vector3df(0,700,0));

    if(player.is_getting_ass_kicked == false && check.range >= dist[closest] && check.state != COMPLETED && resultmag > 1000)
    {
        actiontime += fdt;
        check.state = DOING;
    }
    else
    {
        check.state = ACTIVE;
        actiontime = 0;
    }
    if(actiontime >= check.time)
    {
        check.complete[closest] = true;

        check.Eventnode[closest]->setVisible(false);
        actiontime = 0;
    }
    for(c=0; c<check.possloc; c++)
        if(check.complete[c] == true)
            comped += 1;
    cout<< "completed: " << comped << endl;
    if(comped >= check.steps)
    {
        gamestep += 1;
        check.state = COMPLETED;
    }
}
