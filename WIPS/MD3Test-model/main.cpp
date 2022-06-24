#include <irrlicht.h>

#include "CQ3ModelMesh.h"
#include "CQ3ModelSceneNode.h"
#include "CQ3ModelMeshFileLoader.h"
#include "os.h"

using namespace irr;
using namespace core;
using namespace io;
using namespace gui;
using namespace video;
using namespace scene;
using namespace quake3;

bool                       ExitApplication = false;
IrrlichtDevice            *Device = NULL;
video::IVideoDriver       *Driver = NULL;
scene::ISceneManager      *Smgr = NULL;
io::IFileSystem           *Fs = NULL;
gui::IGUIEnvironment      *Env = NULL;
video::ITexture           *BGImage = NULL;
core::position2d<s32>      BGPosition = core::position2d<s32>(150,150);
video::SColor              BGColor = video::SColor(255,255,255,255);
core::stringc              path;
IQ3ModelSceneNode          *MeshHead = NULL;
IQ3ModelSceneNode          *MeshUpper = NULL;
IQ3ModelSceneNode          *MeshLower = NULL;
core::array<core::stringc>  Shaders;
core::stringc               PlayerName;
core::stringc               SkinName;
scene::ISceneNode*          Cube = NULL;
int                         CurrentAnimation=0;


void initDevice()
{
    core::dimension2di videoDim ( 800, 600 );

    SIrrlichtCreationParameters s;
    s.DriverType = video::EDT_OPENGL;
    s.WindowSize = videoDim;
    s.Bits = 32;
    s.Fullscreen = false;
    s.AntiAlias = true;
    s.HighPrecisionFPU = true;
    s.Vsync = true;
    s.Stencilbuffer = true;

	Device = createDeviceEx(s);
	Device->setWindowCaption(L"MD3 loader");
	Device->setResizeAble(true);
	Device->getFileSystem()->addFolderFileArchive ( "./media/", true, false );
	Device->getFileSystem()->addZipFileArchive("./media/koollight.pk3", true, false );
	Device->getFileSystem()->addZipFileArchive("./media/Q4Gladiator.pk3", true, false );
	Device->getFileSystem()->addZipFileArchive("./media/dragon.pk3", true, false );

	Fs = Device->getFileSystem();

	Env = Device->getGUIEnvironment();

	Driver = Device->getVideoDriver();
	Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	Smgr = Device->getSceneManager();

	Smgr->setAmbientLight(video::SColorf(1.0f,1.0f,1.0f));
	Smgr->addLightSceneNode();
//	Smgr->addLightSceneNode(0, core::vector3df(50,-50,100), video::SColorf(1.0f,1.0f,1.0f),20000);

	Smgr->addCameraSceneNodeMaya();

    os::Printer::Logger = Device->getLogger();
}

void closeDevice()
{
    Device->drop();
}


void player(const c8* playerName, const c8 *skinName, const core::array<core::stringc>& shadersName);
void setSkin(const c8 * skinName);
void Dragon(const c8 *skinName);
void KoolLight(const c8 *skinName);
void Q4Gladiator(const c8 *skinName);
void nextAnimation(int);


class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		// Escape swaps Camera Input
		if (event.EventType == EET_KEY_INPUT_EVENT &&
			event.KeyInput.PressedDown == false)
		{
			switch (event.KeyInput.Key)
			{
			  case irr::KEY_ESCAPE : ExitApplication = true; break;
			  case irr::KEY_PLUS   : nextAnimation(1); break;
			  case irr::KEY_MINUS  : nextAnimation(-1); break;
			  default: break;
			}
		}

		if (event.EventType == EET_GUI_EVENT)
		{
			switch(event.GUIEvent.EventType)
			{
			case EGET_MENU_ITEM_SELECTED:
				{
					// a menu item was clicked

					IGUIContextMenu* menu = (IGUIContextMenu*)event.GUIEvent.Caller;
					s32 id = menu->getItemCommandId(menu->getSelectedItem());

					switch(id)
					{
					case 110: Dragon("blue"); break;
					case 120: Dragon("Dark_Dragon"); break;
                    case 130: Dragon("Gollum"); break;
             	    case 140: Dragon("Gorenaire"); break;
             	    case 150: Dragon("lazzaridus"); break;
             	    case 160: Dragon("Nagafen"); break;
             	    case 170: Dragon("Percy"); break;
             	    case 180: Dragon("Radaghast"); break;
             	    case 190: Dragon("red"); break;
             	    case 195: Dragon("Rrazzia"); break;

					case 210: KoolLight("blue"); break;
					case 220: KoolLight("dark"); break;
					case 230: KoolLight("default"); break;
					case 240: KoolLight("dood"); break;
					case 250: KoolLight("phaltor"); break;
					case 260: KoolLight("red"); break;
					case 270: KoolLight("veteran"); break;
					case 280: KoolLight("woody"); break;

					case 310: Q4Gladiator("arco"); break;
					case 315: Q4Gladiator("blue"); break;
					case 320: Q4Gladiator("default"); break;
					case 325: Q4Gladiator("DUR_red"); break;
					case 330: Q4Gladiator("DUR_red_red"); break;
					case 335: Q4Gladiator("EvilD"); break;
					case 340: Q4Gladiator("Ogro_red"); break;
					case 345: Q4Gladiator("Ogro_red_red"); break;
					case 350: Q4Gladiator("Panzer"); break;
					case 355: Q4Gladiator("red"); break;
					case 360: Q4Gladiator("TIMEX_blue"); break;
					case 365: Q4Gladiator("TIMEX_blue_blue"); break;
					case 370: Q4Gladiator("TIMEX_red"); break;
					case 375: Q4Gladiator("TIMEX_red_red"); break;
					case 380: Q4Gladiator("Tweety"); break;
					}
				break;
				}

				default: return false;
			}

		}

		return false;
	}
};

void ui()
{
	IGUISkin* skin = Env->getSkin();
	IGUIFont* font = Env->getFont("fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	IGUIStaticText* info = Env->addStaticText(L"+ Next animation", rect<int>(250,50,440,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = Env->addStaticText(L"- Previous animation", rect<int>(250,65,440,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = Env->addStaticText(L"Esc Exit application", rect<int>(250,80,440,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	gui::IGUIContextMenu* menu = Env->addMenu();
	menu->addItem(L"Model", -1, true, true);

	gui::IGUIContextMenu *submenu, *subsubmenu;

	submenu = menu->getSubMenu(0);

	submenu->addItem(L"Dragon", -1, true, true);
	   subsubmenu = submenu->getSubMenu(0);
	   subsubmenu->addItem(L"blue", 110);
	   subsubmenu->addItem(L"Dark_Dragon", 120);
	   subsubmenu->addItem(L"Gollum", 130);
	   subsubmenu->addItem(L"Gorenaire", 140);
	   subsubmenu->addItem(L"lazzaridus", 150);
	   subsubmenu->addItem(L"Nagafen", 160);
	   subsubmenu->addItem(L"Percy", 170);
	   subsubmenu->addItem(L"Radaghast", 180);
	   subsubmenu->addItem(L"red", 190);
	   subsubmenu->addItem(L"Rrazzia", 195);

	submenu->addItem(L"Koollight", -1, true, true);
	   subsubmenu = submenu->getSubMenu(1);
	   subsubmenu->addItem(L"blue", 210);
	   subsubmenu->addItem(L"dark", 220);
	   subsubmenu->addItem(L"default", 230);
	   subsubmenu->addItem(L"dood", 240);
	   subsubmenu->addItem(L"phaltor", 250);
	   subsubmenu->addItem(L"red", 260);
	   subsubmenu->addItem(L"veteran", 270);
	   subsubmenu->addItem(L"woody", 280);

	submenu->addItem(L"Q4Gladiator", -1, true, true);
	   subsubmenu = submenu->getSubMenu(2);
	   subsubmenu->addItem(L"arco", 310);
	   subsubmenu->addItem(L"blue", 315);
	   subsubmenu->addItem(L"default", 320);
	   subsubmenu->addItem(L"DUR_red", 325);
	   subsubmenu->addItem(L"DUR_red_red", 330);
	   subsubmenu->addItem(L"EvilD", 335);
	   subsubmenu->addItem(L"Ogro_red", 340);
	   subsubmenu->addItem(L"Ogro_red_red", 345);
	   subsubmenu->addItem(L"Panzer", 350);
	   subsubmenu->addItem(L"red", 355);
	   subsubmenu->addItem(L"TIMEX_blue", 360);
	   subsubmenu->addItem(L"TIMEX_blue_blue", 365);
	   subsubmenu->addItem(L"TIMEX_red", 370);
	   subsubmenu->addItem(L"TIMEX_red_red", 375);
	   subsubmenu->addItem(L"Tweety", 380);

    MyEventReceiver *ev = new MyEventReceiver();
    Device->setEventReceiver(ev);
}

bool draw()
{
    bool active = false;
    if (Device)
    {
        if (Device->run())
        {

            active = true;

            if (Device->isWindowActive())
		    {
			    Driver->beginScene(true, true, BGColor);

                Smgr->drawAll();
                Env->drawAll();

                if (BGImage)
                {
                    Driver->draw2DImage(BGImage, BGPosition,
                        core::rect<s32>(0,0,BGImage->getSize().Width,BGImage->getSize().Height), 0,
                        BGColor, false);
                }


                Driver->endScene();

			    core::stringw str(L"MD3 loader - FPS: ");
			    str.append(core::stringw(Driver->getFPS()));
			    str += L" Tris: ";
			    str.append(core::stringw(Driver->getPrimitiveCountDrawn()));
			    Device->setWindowCaption(str.c_str());
			}
		    else
			   Device->yield();
     	}
    }

    if (ExitApplication)
        active = false;

    return active;
}

void sky()
{
	Smgr->addSkyBoxSceneNode(
		Driver->getTexture("sea_up.tga"),
		Driver->getTexture("sea_dn.tga"),
		Driver->getTexture("sea_rt.tga"),
		Driver->getTexture("sea_lf.tga"),
		Driver->getTexture("sea_ft.tga"),
		Driver->getTexture("sea_bk.tga")
		);
}

void platform(const c8* filename)
{
	scene::ISceneNode* cube = Smgr->addCubeSceneNode();
	cube->setScale( core::vector3df(10,0,10) );
	cube->setPosition( core::vector3df(-50,-30,0) );
	cube->setMaterialTexture(0, Driver->getTexture(filename));

	cube = Smgr->addCubeSceneNode();
	cube->setScale( core::vector3df(0,10,10) );
	cube->setPosition( core::vector3df(-100,-10,0) );
	cube->setMaterialTexture(0, Driver->getTexture(filename));
}

void runner()
{
    while ( draw() );
}

void setBackground(const c8* imageName = NULL,
                   core::position2d<s32> bgPosition = core::position2d<s32>(150,150),
                   video::SColor bgColor = video::SColor(255,255,255,255) )
{
    BGImage = NULL;
    if (imageName)
    {
        BGImage = Driver->getTexture(imageName);
        BGPosition = bgPosition;
        BGColor = bgColor;
    }
}

void drawIcon(const c8* skinName, const core::vector3df &pos)
{
    core::stringc name, filename;

    name = path;
    name += "icon_";
    name += skinName ? skinName : "default";

    ITexture * tx;
    tx = NULL;

    filename = name;
    filename += ".tga";
    if (   !( tx = Driver->getTexture(filename.c_str()) )   )
    {
        filename = name;
        filename += ".jpg";
        tx = Driver->getTexture(filename.c_str());
        if (!tx)
            return;
    }

	if (Cube == NULL)
	{
	    scene::ISceneNode* cube = Smgr->addCubeSceneNode();
	    cube->setScale( core::vector3df(1.5,1.5,1.5) );
	    cube->setPosition( pos );
	    cube->addAnimator( Smgr->createRotationAnimator(core::vector3df(0,1,0)) );

	    Cube = cube;
	}

	Cube->setMaterialTexture(0, tx);
}

IQ3ModelSceneNode* addQ3ModelSceneNode(const c8* playerName, EMD3_MODEL_TYPE type, Animation *Animations, bool preview)
{
    core::stringc filename;
    io::IReadFile * f;

    filename = path;

    switch (type) {
        case MD3_HEAD : filename += "head.md3"; break;
        case MD3_UPPER: filename += "upper.md3"; break;
        case MD3_LOWER: filename += "lower.md3"; break;
        default: filename = playerName;
    }

    f = Fs->createAndOpenFile(filename.c_str());

    if (!f)
        return NULL;

    CQ3ModelMeshFileLoader loader(Fs, Driver);

    CQ3ModelMesh* mesh;
    mesh = NULL;
    if (preview)
       mesh = (CQ3ModelMesh*) loader.createMesh(f, 0, 1);
    else
       mesh = (CQ3ModelMesh*) loader.createMesh(f);

    if (!mesh)
        return NULL;

    CQ3ModelSceneNode* node;
    ISceneNode* parent = Smgr->getRootSceneNode();
    node = new CQ3ModelSceneNode((IQ3ModelMesh*)mesh, parent, Smgr);
    if (!node)
        return NULL;
    node->addShadowVolumeSceneNode();

    if (Animations)
        node->setAnimationConfig(Animations);

    return (IQ3ModelSceneNode *)node;
}

void setShader(const c8 * shaderName)
{
    if (MeshHead && MeshUpper && MeshLower)
    {
        core::stringc     filename;
        io::IReadFile    *f;

        filename = "scripts/";
        filename+= shaderName;
        filename.make_lower();
        filename+= ".shader";
        f = Fs->createAndOpenFile( filename.c_str() );

        quake3::ShadersParser * shaders;
        shaders = new quake3::ShadersParser(f);

        MeshHead->setShaderFromList(shaders, Fs);
        MeshUpper->setShaderFromList(shaders, Fs);
        MeshLower->setShaderFromList(shaders, Fs);

        shaders->drop();
    }

}

void setSkin(const c8 * skinName)
{
    if (MeshHead && MeshUpper && MeshLower)
    {
        core::stringc        filename;
        core::stringc        SkinName;
        io::IReadFile       *f;
        IQ3ModelSceneNode   *node = NULL;

        SkinName = skinName;
        if (SkinName == "")
            SkinName = "default";
        else
            SkinName.make_lower();

        u32 i;
        for (i = (u32)MD3_HEAD; i < (u32)MD3_WEAPON; i++)
        {
            filename = path;
            switch ((EMD3_MODEL_TYPE)i)
            {
                case MD3_HEAD : filename += "head_";  node = MeshHead;  break;
                case MD3_UPPER: filename += "upper_"; node = MeshUpper; break;
                case MD3_LOWER: filename += "lower_"; node = MeshLower; break;
                default: break;
            }
            filename += SkinName;
            filename += ".skin";

            f = Fs->createAndOpenFile( filename.c_str() );

            quake3::Skin* skin;
            skin = new quake3::Skin(f, Driver);
            node->setSkin(skin);
            skin->drop();
            node->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        for (i = 0; i < Shaders.size(); i++)
           setShader( Shaders[i].c_str() );


        drawIcon(SkinName.c_str(), core::vector3df(0,-17.05,-50));
    }
}

bool loadPlayer(const c8 *playerName)
{
    core::stringc filename;
    io::IReadFile * f;

    filename = path;
    filename += "animation.cfg";
    f = Fs->createAndOpenFile(filename.c_str());
    Animation *Animations;
    if (f)
    {
        Animations = new quake3::Animation(f);
        if (!Animations)
        {
            Animations->drop();
            Animations = NULL;
        }
    }
    else Animations = NULL;

    IQ3ModelSceneNode *mesh_head, *mesh_upper, *mesh_lower;
    mesh_head  = addQ3ModelSceneNode(playerName, MD3_HEAD, Animations, false);
    mesh_upper = addQ3ModelSceneNode(playerName, MD3_UPPER, Animations, false);
    mesh_lower = addQ3ModelSceneNode(playerName, MD3_LOWER, Animations, false);

    if (!mesh_head || !mesh_upper || !mesh_lower)
    {
        if (mesh_head)  mesh_head->drop();
        if (mesh_lower) mesh_lower->drop();
        if (mesh_upper) mesh_upper->drop();
        return false;
    }

    MeshHead  = mesh_head;
    MeshUpper = mesh_upper;
    MeshLower = mesh_lower;
    MeshLower->setPosition(core::vector3df(-50,-5,0));

    MeshUpper->addJointToNode(MeshHead);
    MeshLower->addJointToNode(MeshUpper);

    MeshUpper->setAnimation(quake3::TORSO_STAND);
    MeshLower->setAnimation(quake3::LEGS_WALK);

    return true;
}

void player(const c8* playerName, const c8 *skinName, const core::array<core::stringc>& shadersName)
{
    core::stringc newPlayerName = playerName;
    newPlayerName.make_lower();

    core::stringc newSkinName = skinName;
    newSkinName.make_lower();

    if (MeshHead && newPlayerName == PlayerName)
    {
        if (newSkinName != SkinName)
        {
            setSkin(skinName);
            SkinName = newSkinName;
        }
        return;
    }

    if ( MeshHead ) // newPlayerName != PlayerName
    {
        MeshHead->setVisible(false);
        MeshUpper->setVisible(false);
        MeshLower->setVisible(false);
        Smgr->getRootSceneNode()->removeChild(MeshHead);
        Smgr->getRootSceneNode()->removeChild(MeshUpper);
        Smgr->getRootSceneNode()->removeChild(MeshLower);
        MeshHead->drop();
        MeshUpper->drop();
        MeshLower->drop();
        MeshHead = NULL;
        MeshUpper = NULL;
        MeshLower = NULL;
    }

    path = "models/players/";
    path += playerName;
    path += "/";
    path.make_lower();

    PlayerName = newPlayerName;
    SkinName = newSkinName;
    Shaders = shadersName;

    loadPlayer(playerName);
    setSkin(SkinName.c_str());
}

void nextAnimation(int increment)
{
    if (!MeshUpper || !MeshLower)
        return;



    CurrentAnimation+= increment;
    switch (CurrentAnimation)
    {
        case 0: MeshUpper->setAnimation(TORSO_STAND);
                MeshLower->setAnimation(LEGS_WALKCR);
                break;
        case 1: MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_WALK);
                break;
        case 2: MeshUpper->setAnimation(TORSO_STAND);
                MeshLower->setAnimation(LEGS_RUN);
                break;
        case 3: MeshUpper->setAnimation(TORSO_STAND);
                MeshLower->setAnimation(LEGS_BACK);
                break;
        case 4: MeshUpper->setAnimation(TORSO_DROP);
                MeshLower->setAnimation(LEGS_SWIM);
                break;
        case 5: MeshUpper->setAnimation(TORSO_DROP);
                MeshLower->setAnimation(LEGS_SWIM);
                break;
        case 6: MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_JUMP);
                break;
        case 7: MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_LAND);
                break;
        case 8: MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_JUMPB);
                break;
        case 9: MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_LANDB);
                break;
        case 10:MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_IDLE);
                break;
        case 11:MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_IDLECR);
                break;
        case 12:MeshUpper->setAnimation(TORSO_STAND2);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 13:MeshUpper->setAnimation(TORSO_GESTURE);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 14:MeshUpper->setAnimation(TORSO_ATTACK);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 15:MeshUpper->setAnimation(TORSO_ATTACK2);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 16:MeshUpper->setAnimation(TORSO_DROP);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 17:MeshUpper->setAnimation(TORSO_RAISE);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 18:MeshUpper->setAnimation(TORSO_GETFLAG);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 19:MeshUpper->setAnimation(TORSO_GUARDBASE);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 20:MeshUpper->setAnimation(TORSO_PATROL);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 21:MeshUpper->setAnimation(TORSO_FOLLOWME);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 22:MeshUpper->setAnimation(TORSO_AFFIRMATIVE);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 23:MeshUpper->setAnimation(TORSO_NEGATIVE);
                MeshLower->setAnimation(LEGS_TURN);
                break;
        case 24:MeshUpper->setAnimation(BOTH_DEATH1);
                MeshLower->setAnimation(BOTH_DEATH1);
                break;
        case 25:MeshUpper->setAnimation(BOTH_DEAD1);
                MeshLower->setAnimation(BOTH_DEAD1);
                break;
        case 26:MeshUpper->setAnimation(BOTH_DEATH2);
                MeshLower->setAnimation(BOTH_DEATH2);
                break;
        case 27:MeshUpper->setAnimation(BOTH_DEAD2);
                MeshLower->setAnimation(BOTH_DEAD2);
                break;
        case 28:MeshUpper->setAnimation(BOTH_DEATH3);
                MeshLower->setAnimation(BOTH_DEATH3);
                break;
        case 29:MeshUpper->setAnimation(BOTH_DEAD3);
                MeshLower->setAnimation(BOTH_DEAD3);
                break;

        default:
                if (CurrentAnimation >= 30)
                    CurrentAnimation = 0;
                else
                    CurrentAnimation = 29;
                nextAnimation(0);
                break;
    };

}

void Q4Gladiator(const c8 *skinName = NULL)
{
    core::array<core::stringc> sn;
    sn.push_back("Q4gladiator");

    player("Q4Gladiator", skinName, sn);
}

void KoolLight(const c8 *skinName = NULL)
{
    core::array<core::stringc> sn;
    sn.push_back("koollight");
    sn.push_back("phaltor");
    sn.push_back("woody");

    player("koollight", skinName, sn);
}

void Dragon(const c8 *skinName = NULL)
{
    core::array<core::stringc> sn;

    player("dragon", skinName, sn);
}

int main()
{
    initDevice();

    setBackground("wait.jpg");
    draw();

    ui();
    sky();
    platform("AMBROSIL.jpg");
    setBackground();

    KoolLight("phaltor");

    runner();

    closeDevice();

    return 0;
}
