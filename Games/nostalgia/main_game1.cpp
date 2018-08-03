/*log_str.append(L"time on  ",15);
log_str.append(core::stringw(time_on_target),15);
logger->log(log_str.c_str());
log_str =("   ");*/

#include "main_game1.h"

using namespace irr;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

extern SIrrlichtCreationParameters paramIrr;

core::stringc modelfile;
core::stringw MessageText;
core::stringw Caption;
core::stringw str;
IGUIFont* font2;
scene::ISceneManager* smgr;
IrrlichtDevice *Device;
video::IVideoDriver* driver;
scene::ISceneCollisionManager* iscm;
scene::IAnimatedMesh* anim_mesh;
shields shield;

//correction for mouse move vs canon
//f32 correctionx= 1, correctiony = 1.5,correctionz = 2.0;
irr::core::vector3df camera_pos;


//basic animated objects
//keep them equal for shadow off-on in init_level except tour number
const int tank_number=20;
const int jet_number=20;
const int shell_number =20;
const int obu_number = 20;
const int missile_number = 20;
const int tower_number =4;
const int number_of_level = 4;

jets jet[jet_number];
cshell shell[shell_number];
tanks tank[tank_number];
tours tour[tower_number];
obus obu[obu_number];
mobus mobu[obu_number];
missiles missile[missile_number];



//touche pas en bas==============================================================
//Global parameters and objects

int current_level;
int current_wave;
//int points;
bool game_over=false;
int nshell=-1;//nombre de shell
int n_tour;
ITimer *timer;
bool new_game= false;
extern irr::core::array<SParticleImpact> Impacts_obus, Impacts_mobus;

//kept for troobleshooting
scene::ICameraSceneNode* Camerafps = {0};
scene::ITriangleSelector* selector = 0;

core::stringw log_str(L"   ");
//for animation and gameplay
bool initialize = false;
bool Pause = false;

//gui interface
IGUIStaticText* audio_text;
core::stringw audio_reverse;
bool audio_reverse_normal;
u32 master_volume;
u32 music_volume;
u32 transparency;
IGUIWindow* toolbox=0;

//variable pour les futurs vrais noms et autre paramètres globaux
wchar_t *nomdujeux = L"Nostalgia";
char *root = "/media";
extern IGUIComboBox* cmbScreenResolution;

///pressing de d key start this routine for troobleshooting

void debug()
{for ( int i =0; i < data_level[current_level].max_shellc; i++)

     {log_str.append(L"level # ",15);
     log_str.append(core::stringw(current_level),15);
     log_str.append(L"    wave # ",15);
     log_str.append(core::stringw(current_wave),15);
     log_str.append(L"    game over ",15);
     log_str.append(core::stringw(game_over),15);
     log_str.append(L"    #tanks ",15);
     log_str.append(core::stringw(tank[0].n_tank),15);
     log_str.append(L"    #jets ",15);
     log_str.append(core::stringw(jet[0].n_jet),15);
     logger->log(log_str.c_str());
     log_str =("   ");
     }

}
//==============================================================================
//	toggles between various cameras

void setActiveCamera ( scene::ICameraSceneNode* newActive )
{
	if ( 0 == Device )
		return;
	newActive->setInputReceiverEnabled ( true );
	Device->getSceneManager()->setActiveCamera ( newActive );
}
//==============================================================================
//aiming canon
void mircanon()
{	if (!initialize||!Device->isWindowActive()) return;
//	video::IVideoDriver* driver = Device->getVideoDriver();
	core::vector3df canonrot;
	f32 offsetx, offsety;
	cursorcontrol = Device->getCursorControl();
	cursorpos = cursorcontrol->getRelativePosition();

//screen_center = 0.5 , gets degree of rotation from mouse position
	offsetx = (cursorpos.X-screen_center)*110;
	offsety = (cursorpos.Y-screen_center)*110;
//	canonrot= (canon->getRotation());
//X & Y are inverted, x move rotate on y axis, vice-versa
	canonrot.Y =offsetx;
	canonrot.X =offsety;
	canonrot.Z=0;
	canon->setRotation(canonrot);
	angle_node->setRotation(canonrot);
	anglex_degree = canonrot.X*-1;
//keep cursor in window
if (cursorpos.X < 0.05)
		{cursorcontrol->setPosition(0.1,cursorpos.Y);}
if (cursorpos.X > 0.95)
		{cursorcontrol->setPosition(0.9,cursorpos.Y);}
if (cursorpos.Y < 0.05)
		{cursorcontrol->setPosition(cursorpos.X,0.1);}
if (cursorpos.Y > 0.95)
		{cursorcontrol->setPosition(cursorpos.X,0.9);}
}

//==============================================================================
//===========user targets an enemy=================================================
void targeting()
{scene::ISceneNode* target;
     int index=0;
     mire_pos.X+=32;
     mire_pos.Y+=32;
/// is it a jet
     target = iscm->getSceneNodeFromScreenCoordinatesBB(mire_pos, jet_mask);
/// is it a tank
if (target->getID() < 1) target = iscm->getSceneNodeFromScreenCoordinatesBB(mire_pos, tank_mask);
/// if none of both, reset time_on_target
if (target->getID() < 1)
{time_on_target = 0;
return;///exit routine
}
///we have target, since how long ?
if (target)
//on a un jet ou tank sous la mire
    {now = timer->getRealTime();
     if (time_on_target > 1)
//on reviens, si assez long, on lock et launch
          {
               if ((now - time_on_target) > time_to_lock)//on a le lock
               {
               while (missile[index].isVisible){index++;}
               missile[index].launch(target);
               time_on_target =0;
               //place constant tone here
//               irrsound->play2D(launch_sound,false);
               }
               //si assez long délai, on beep
               if ((now - time_to_beep) > 350)

               {
    //           irrsound->play2D(targeting_sound);
   //            time_to_beep = now;
               }
          }
          //we are on target, start timers
     else {
          time_on_target = now;//on démarre le timer to lock
          time_to_beep = now;//timer to beep
          }

    }
//on a rien sous la mire, on reset les compteurs
else {
     time_on_target =0;
     time_to_beep = 0;
     }
}
//==============================================================================
//===========shooting bullet====================================================
void tir()
{
	//if (!selector)
	//	return;
	if (nshell == data_level[current_level].max_shellc -1)
	    {// play sound empty barrel
//		irrsound->play2D(clink_sound);
		return;
	    }

    timer=Device->getTimer();
	// create shell data when use
	nshell++;
	int fill=0;
	while (shell[fill].isVisible){fill++;}
	shell[fill].position=canon->getPosition()+ core::vector3df(1,6,-5);
	shell[fill].isVisible = true;
	shell[fill].pshell->setVisible(true);
	now = timer->getRealTime();
	shell[fill].last_time = now;
//inverting angles x rotation gives y angle, vice versa
//make it rad
	shell[fill].direction.X = sinf ( core::DEGTORAD * canon->getRotation().Y) * correctiony;
	shell[fill].direction.Y = sinf (-core::DEGTORAD * canon->getRotation().X) * correctionx;
	shell[fill].direction.Z = cosf ((core::DEGTORAD * canon->getRotation().Y)*correctionz) + cosf ((core::DEGTORAD * canon->getRotation().X)*correctionz);
	shell[fill].direction.normalize();
// play sound cannon shooting shell
//	if (canon_sound)
//	irrsound->play2D(canon_sound);
	points-=10;
}
///Main update loop
//updating shell position, and tanks, jets, etc=================================
//checking if they reach ground without impact==================================
void update()
{
	core::triangle3df triangle;
	core::line3d<f32> line;
	core::vector3df movement,v;
	core::vector3df out;
	core::vector3df end;
	core::vector3df start;
	SParticleImpact imp;
	imp.when = 0;
	now = timer->getRealTime();

	//update les shell
	if (nshell > -1)
	{
		for ( int i =0; i < data_level[current_level].max_shellc; i++)
			{if (shell[i].isVisible)
                {
               	f32 lapse = now - shell[i].last_time;
				if (lapse < 17) continue;
				shell[i].direction += data_level[current_level].gravityc*lapse;
				movement = shell[i].direction*data_level[current_level].shell_speedc*lapse;
				shell[i].position += movement;
				shell[i].pshell->setPosition(shell[i].position);
				shell[i].last_time = now;
				}
			}

//checking collision with ground
//if so, changing attributes of shell to stop updating
for ( int i =0; i < data_level[current_level].max_shellc; i++)
        {if (!shell[i].isVisible) continue;
        start = shell[i].position;
        end = shell[i].position + movement;
        line.setLine(start, end);

	if (smgr->getSceneCollisionManager()->getCollisionPoint(
        line, selector, end, triangle))
		{out = triangle.getNormal();
		out.setLength(0.06f);
//getting data for pos, time of impact
		imp.when = 1;
		imp.outVector = out;
		imp.pos = shell[i].position;
//one shell less
		shell[i].isVisible = false;
        shell[i].position = deadzone;
        nshell--;
        shell[i].pshell->setVisible(false);
		}
//if shell is too far off, reset it
		else if ((shell[i].position.Y < 0)
            ||(shell[i].position.Z> data_level[current_level].length)
            ||(shell[i].position.Z<-300))

		{
        shell[i].isVisible = false;
        shell[i].position = deadzone;
        nshell--;
        shell[i].pshell->setVisible(false);
        imp.when = 1;
        imp.outVector = core::vector3df(0.0f,0.06f,0.0f);
        shell[i].position.Y+=5;
        imp.pos = shell[i].position- movement;
          }

		if (imp.when)
		{
		// create impact note
		imp.when = timer->getRealTime()+ 100;
		Impacts.push_back(imp);
		imp.when =0;
		}
}
}
//jets get updated

if (jet[0].n_jet > -1)
     {
     for (int i =0; (i<data_level[current_level].max_jetc+current_wave);i++)
     { irr::u32 now = timer->getRealTime();
     if ((now-jet[i].last_time) < 23)continue;
     jet[i].now = now;
     jet[i].update_jets(now);
     }
     }
//tanks get updated
if (tank[0].n_tank > -1)
     {
     for (int i =0; (i<data_level[current_level].max_tankc+current_wave);i++)
     {
     irr::u32 now = Device->getTimer()->getRealTime();
     if ((now-tank[i].last_time) < 29)continue;
     //tank[i].now = now;
     tank[i].update_tanks(now);
     }
     }
//obus get updated
if (obu[0].n_obus > -1)
     {
     for (int i =0; (i<data_level[current_level].max_obus+current_wave);i++)
     {
     if (!obu[i].isVisible)continue;
     irr::u32 now = Device->getTimer()->getRealTime();
     if ((now-obu[i].last_time) < 31)continue;
     obu[i].update_obus(now);
     }
     }
//mobus get updated
if (mobu[0].n_mobus > -1)
     {
     for (int i =0; (i<data_level[current_level].max_obus+current_wave);i++)
     {
     if (!mobu[i].pobus->isVisible())continue;
     irr::u32 now = Device->getTimer()->getRealTime();
     if ((now-mobu[i].last_time) <37)continue;
     mobu[i].update_mobus(now);
     }
     }
//missiles get updated
if (missiles::n_missile > -1)
     {
for (int i =0; (i<data_level[current_level].max_missile);i++)
     { irr::u32 now = Device->getTimer()->getRealTime();
     if ((now-missile[i].last_time) < 29)continue;
     missile[i].now = now;
     missile[i].update_missiles(now);
     }
     }
return;
}
//checks collision between bullet and enemys====================================
//==============================================================================
void check_collision()
{
core::vector3df pos;
core::vector3df out;
SParticleImpact imp;
f64 distance;
imp.when = 0;
//checking collision between shells and enemy
if ((jet[0].n_jet < 0)&(tank[0].n_tank<0)) return;//no jet no tank to hit, exit
if (nshell <0) return;//no shells on screen exit
	for ( int i =0; i < data_level[current_level].max_shellc; i++)
	{if (!shell[i].isVisible)continue;//if shell is not visible , next shell
	    for (int j =0; j<data_level[current_level].max_jetc+current_wave; j++)//check with jets
	    {if (jet[j].isDead) continue;//already dead, next jet
			if(shell[i].position.getDistanceFrom(jet[j].position)< collide_distance)
//bullet and jet collide
	   		{jet[j].dying = true;
//one less shell
	   		shell[i].isVisible = false;
	   		shell[i].position=deadzone;
	   		shell[i].pshell->setVisible(false);
	   		nshell--;
	   		out = jet[j].position;
			out.setLength(0.06f);
//getting data for pos, time of impact
			imp.when = 1;
			imp.outVector = core::vector3df(0.0f,0.3f,0.0f);
			imp.pos = jet[j].position;
			imp.node=jet[j].jet_mesh;
			if (imp.when)
				{
// create impact note
				imp.when = Device->getTimer()->getRealTime()+ 100;
				Enemy_Impacts.push_back(imp);
				}
			}
         }
for (int j =0; j<data_level[current_level].max_tankc+current_wave; j++)
	    {if (tank[j].isDead) continue;
//	    pos = tank[j].position;
	    distance = shell[i].position.getDistanceFrom(tank[j].position);
		if(distance< collide_distance*1.5f)
//bullet and tank collide, tank is dead
	   		{tank[j].dying = true;
//one less shell
	   		shell[i].isVisible = false;
	   		shell[i].position=deadzone;
	   		shell[i].pshell->setVisible(false);
	   		nshell--;
	   		out = tank[j].position;
			out.setLength(0.04f);
//getting data for pos, time of impact
			imp.when = 1;
			imp.outVector = core::vector3df(0.0f,-0.3f,0.0f);
			imp.pos = tank[j].position;
			imp.node=tank[j].tank_mesh;
			if (imp.when)
				{
// create impact note
				imp.when = Device->getTimer()->getRealTime()+ 100;
				Enemy_Impacts.push_back(imp);
				}
			}
         }
     }
}
//==============================================================================
///itirate thru impacts notes and make explosions particules
//==============================================================================
void createShellImpacts()
{
	core::line3df line;
	core::vector3df pos;
	u32 now = Device->getTimer()->getRealTime();
	for (s32 i=0; i<(s32)Impacts.size(); ++i)
		if (now > Impacts[i].when)
		{
// create shells smoke
			scene::IParticleSystemSceneNode* pas = smgr->addParticleSystemSceneNode
			(false, 0, -1, Impacts[i].pos);
			pas->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));
			scene::IParticleEmitter* em = pas->createBoxEmitter(
				core::aabbox3d<f32>(0,0,0,7,7,7),
				Impacts[i].outVector, 80,100, video::SColor(0,170,65,45),video::SColor(0,180,75,55),
				300,700, 20);

			pas->setEmitter(em);
			em->drop();
			scene::IParticleAffector* paf = pas->createFadeOutParticleAffector();
			pas->addAffector(paf);
			paf->drop();
			pas->setMaterialFlag(video::EMF_LIGHTING, false);
			pas->setMaterialTexture(0, driver->getTexture("media/smoke.bmp"));
			pas->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
			scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(1500);
			pas->addAnimator(anim);
			anim->drop();
			// play impact sound
			line.setLine(Impacts[i].pos,canon->getPosition());
			pos = line.getMiddle();
//			if (impact_sound) irrsound->play3D(impact_sound, pos);
			// delete entry
			Impacts.erase(i);
			i--;
		}
//impacts from obus
now = Device->getTimer()->getRealTime();
	for (s32 i=0; i<(s32)Impacts_obus.size(); ++i)
		if (now > Impacts_obus[i].when)
		{
// create smoke particle system
			scene::IParticleSystemSceneNode* pas = 0;
			pas = smgr->addParticleSystemSceneNode(false, 0, -1, Impacts_obus[i].pos);
			pas->setParticleSize(core::dimension2d<f32>(10.0f, 10.0f));
			scene::IParticleEmitter* em = pas->createBoxEmitter(
				core::aabbox3d<f32>(-3,-3,-3,8,8,8),
				Impacts_obus[i].outVector, 400,600, video::SColor(0,125,255,125),video::SColor(0,125,255,125),
				800,1000, 120);
               pas->setEmitter(em);
			em->drop();
			scene::IParticleAffector* paf = pas->createFadeOutParticleAffector();
			pas->addAffector(paf);
			paf->drop();
			pas->setMaterialFlag(video::EMF_LIGHTING, false);
			pas->setMaterialTexture(0, driver->getTexture("media/smoke.bmp"));
			pas->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
			scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(1800);
			pas->addAnimator(anim);
			anim->drop();
			// play impact sound
			line.setLine(pos,canon->getPosition());
			pos = line.getMiddle();
//			if (impact_sound) irrsound->play3D(impact_sound, pos);
               Impacts_obus.erase(i);
			i--;
		}
//impacts from mobus
now = Device->getTimer()->getRealTime();
	for (s32 i=0; i<(s32)Impacts_mobus.size(); ++i)
		if (now > Impacts_mobus[i].when)
		{core::vector3df pos=Impacts_mobus[i].pos;
// create smoke particle system
			scene::IParticleSystemSceneNode* pas = smgr->addParticleSystemSceneNode
			(false, 0, -1, Impacts_mobus[i].pos);
			pas->setParticleSize(core::dimension2d<f32>(10.0f, 10.0f));
			scene::IParticleEmitter* em = pas->createRingEmitter(core::vector3df(0.0f,0.0f,0.0f),
			30,20,	core::vector3df(0.0f,0.08f,0.0f), 1200,1500,
               video::SColor(0,100,100,255),video::SColor(100,125,125,255),
 				1100,1200, 180);
            pas->setEmitter(em);
			em->drop();
			scene::IParticleAffector* paf = pas->createFadeOutParticleAffector();
			pas->addAffector(paf);
			paf->drop();
			pas->setMaterialFlag(video::EMF_LIGHTING, false);
			pas->setMaterialTexture(0, driver->getTexture("media/smoke.bmp"));
			pas->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(1500);
			pas->addAnimator(anim);
			anim->drop();
			// play impact sound
			line.setLine(pos,canon->getPosition());
			pos = line.getMiddle();
//			if (impact_sound) irrsound->play3D(impact_sound, pos);
               Impacts_mobus.erase(i);
			i--;
		}
}
//==============================================================================
///create enemy explosion when hit
//==============================================================================
void createEnemyImpacts()
{u32 now = Device->getTimer()->getRealTime();
	for (s32 i=0; i<(s32)Enemy_Impacts.size(); ++i)
		if (now > Enemy_Impacts[i].when)
		{
// create smoke particle system
			scene::IParticleSystemSceneNode* ps = 0;
			core::vector3df pos =Enemy_Impacts[i].pos;
		//it is a jet
			if (Enemy_Impacts[i].node->getID() & jet_mask)
			{	scene::IParticleSystemSceneNode* pas = 0;
			pas = smgr->addParticleSystemSceneNode(false, Enemy_Impacts[i].node, -1);
			pas->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));
			scene::IParticleEmitter* em = pas->createBoxEmitter(
				core::aabbox3d<f32>(-5,-5,-5,5,5,5),
				core::vector3df(0.0f,0.2f,0.0f), 100,300,
                    video::SColor(0,95,95,150),video::SColor(50,150,100,250),
				500,1000, 20);

			pas->setEmitter(em);
			em->drop();
			scene::IParticleAffector* paf = pas->createFadeOutParticleAffector(video::SColor(0,0,0,0),2500);
			pas->addAffector(paf);
			paf->drop();
			pas->setMaterialFlag(video::EMF_LIGHTING, false);
			pas->setMaterialTexture(0, driver->getTexture("media/jet_fume.png"));
			pas->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
			scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(5000);
			pas->addAnimator(anim);
			anim->drop();

			}
			//it is a tank
			if (Enemy_Impacts[i].node->getID() & tank_mask)
			{ps = smgr->addParticleSystemSceneNode(false, Enemy_Impacts[i].node, -1,core::vector3df(0.0f,0.0f,0.0f),core::vector3df(0.0f,0.0f,0.0f),core::vector3df(3.0f,3.0f,3.0f) );
			scene::IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-10,-10,-10,15,15,15),
			core::vector3df(0.0f,0.05f,0.0f),
			500,1000,
			video::SColor(0,20,55,180), video::SColor(0,40,50,190),
			1500,3000,20);
			ps->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));
			ps->setEmitter(em);
			em->drop();
			scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(video::SColor(0,0,0,0), 500);
			ps->addAffector(paf);
			paf->drop();
			ps->setMaterialFlag(video::EMF_LIGHTING, false);
			ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
			ps->setMaterialTexture(0, driver->getTexture("textures/tank_fume.png"));
			ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
			scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(4500);
			ps->addAnimator(anim);
			anim->drop();
			}

			// play impact sound
//			if (impact_sound) irrsound->play3D(impact_sound, pos );
			// delete entry
			Enemy_Impacts.erase(i);
			i--;
		}
}
//==============================================================================
///if canon or towers get hit, activate shield effect
void shields::shield_effect()
{
/*video::SMaterial mat = shield->getMaterial(0);
video::ITexture * shield_image = mat.getTexture(0);
u32 alpha;*/
irr::u32 now = timer->getRealTime();
///first loop
if (until_time == 0)
    {

    until_time = now+1300;
    last_time =now-5;
    cut_lenght = Randomizer::random(10);
    next_cut = (Randomizer::random(100));
    next_cut+=now;
    rotation.Y=Randomizer::random(360);
    rotation.X=Randomizer::random(360);
    bbsn->setRotation(rotation);
    bbsn->setVisible(true);
//    if (shield_sound) irrsound->play2D(shield_sound);
    }
///time to stop effect
if (now > until_time)
    {
    bbsn->setVisible(false);
    shield_effect_on =false;
    until_time =0;
    return;
    }
///time to hide the shield effect
if(now > next_cut && bbsn->isVisible())
    {
    rotation.X=Randomizer::random(360);
    bbsn->setRotation(rotation);
    bbsn->setVisible(false);
    next_cut = Randomizer::random(500);
    next_cut+=now;
    }
///time to show the shield effect
if( now > (next_cut + cut_lenght)&& !bbsn->isVisible())
    {
    bbsn->setVisible(true);
    cut_lenght = Randomizer::random(50);
    next_cut = Randomizer::random(500);
    next_cut+=now;
    }
if ( (now -last_time) > 10)
{

position = bbsn->getPosition();
movx*=-1;
movy*=-1;
position.X+=movx;
position.Y+=movy;
bbsn->setPosition(position);
last_time= now;
}

}
//==============================================================================
///all towers were destroyed, game over
void all_game_over()
{

game_over=true;
Pause = true;
shield.bbsn->setVisible(false);
mire_jets=false;
if(cursorcontrol)
     (cursorcontrol->setVisible(true));

}
///request a new game

void NewGame()
{
//const core::list<scene::ISceneNode*>& child_node;
core::list<scene::ISceneNode*>::ConstIterator child_node;
irr::scene::ESCENE_NODE_TYPE nodetype;
current_level=0;
current_wave=0;
points = 0;
game_over=false;
Pause = false;
mire_jets = false;
if(cursorcontrol)
(cursorcontrol->setVisible(false));
//reinitialize tours_removing smoke particule system
	for (int i=0;i<tower_number;i++)
	{
	child_node = tour[i].anim_node->getChildren().begin();
	while (child_node != tour[i].anim_node->getChildren().end())
	{
    nodetype = (*child_node)->getType();
    if((*child_node)->getType() == scene::ESNT_PARTICLE_SYSTEM )
    {(*child_node)->remove();
    break;}
    child_node++;
	}
	tour[i].anim_node->setLoopMode(true);
	tour[i].isDestroy = false;
	}
n_tour=tower_number;
tour[n_tour-1].alive=n_tour;
//reinitialize the tanks
 for (int j=0; j<tank_number; j++)
     {
	tank[j].tank_mesh->setPosition(core::vector3df((15*j), (20*j)+50,100));
	}
	tank[0].n_tank=-1;
//reinitialize the jets
     for (int j=0; j<jet_number; j++)
     {
	jet[j].jet_mesh->setPosition(core::vector3df(15*j,20*j+100,100));
	jet[j].jet_mesh->setRotation(irr::core::vector3df(0,0,0));
	}
	jet[0].n_jet = -1;
//reinitialize  missiles
     for (int j=0; j<missile_number; j++)
     {
	missile[j].missile_mesh->setPosition(core::vector3df(-10*j,-10*j+100,10));
	missile[j].missile_mesh->setRotation(irr::core::vector3df(0,0,0));
     }
     missile[0].n_missile = -1;
     for (int i = 0; i < number_of_level;i++)
          {
          data_level[i].level_complete = false;
          for (int i = 0; i < data_level[i].max_wave;i++)
               {
               data_wave[i].wave_complete = false;
               }
          }


init_level();
}
//==============================================================================
///open help box
void HelpText()
{
     gui::IGUIEnvironment* env;
     env = Device->getGUIEnvironment();
     if (!Pause)Pause=true;
     //SHOW CURSOR
     gui::ICursorControl* cursorcontrol = Device->getCursorControl();
     cursorcontrol->setVisible(true);
     //MAKE WINDOW LISIBLE
     video::SColor color = env->getSkin()->getColor(EGDC_3D_FACE);//ACTIVE_CAPTION
     color.setAlpha(255);
     env->getSkin()->setColor(EGDC_3D_FACE ,color);
     env->getSkin()->setColor(EGDC_ACTIVE_CAPTION ,color);
	// create modal message box with the text
	// loaded from the xml file.
		io::IXMLReader* xml = Device->getFileSystem()->createXMLReader("xml_files/Help.xml");
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case io::EXN_TEXT:
			// in this xml file, the only text which occurs is the messageText
			MessageText = xml->getNodeData();
			break;
		case io::EXN_ELEMENT:
			{
				if (core::stringw("messageText") == xml->getNodeName())
					Caption = xml->getAttributeValue(L"caption");
			}
			break;
		}
	}
	if (xml)
		xml->drop(); // don't forget to delete the xml reader
	Device->getGUIEnvironment()->addMessageBox
        (Caption.c_str(), MessageText.c_str(),true, EMBF_OK,0,5001);

}

//tips text
void TipsText()
{
     	// remove tool box if already there
	IGUIEnvironment* env = Device->getGUIEnvironment();
	IGUIElement* root = env->getRootGUIElement();
	IGUIElement* e = root->getElementFromId(5002, true);
	if (e) e->remove();
	if(!Pause)Pause=true;

	// create modal message box with the text
	// loaded from the xml file.
	io::IXMLReader* xml = Device->getFileSystem()->createXMLReader("xml_files/tips.xml");
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case io::EXN_TEXT:
			// in this xml file, the only text which occurs is the messageText
			MessageText = xml->getNodeData();
			break;
		case io::EXN_ELEMENT:
			{
				if (core::stringw("messageText") == xml->getNodeName())
					Caption = xml->getAttributeValue(L"caption");
			}
			break;
		}
	}
	if (xml)
		xml->drop(); // don't forget to delete the xml reader
	Device->getGUIEnvironment()->addMessageBox(Caption.c_str(), MessageText.c_str(),true
	,EMBF_OK,0, 5002);

}
void AboutText()
{IGUIWindow* about=0;
	// create modal message box with the text
	// loaded from the xml file.
		io::IXMLReader* xml = Device->getFileSystem()->createXMLReader("xml_files/about.xml");
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case io::EXN_TEXT:
			// in this xml file, the only text which occurs is the messageText
			MessageText = xml->getNodeData();
			break;
		case io::EXN_ELEMENT:
			{
				if (core::stringw("messageText") == xml->getNodeName())
					Caption = xml->getAttributeValue(L"caption");
			}
			break;
		}
	}
	if (xml)
		xml->drop(); // don't forget to delete the xml reader
	about = Device->getGUIEnvironment()->addMessageBox(Caption.c_str(), MessageText.c_str(),true
	,EMBF_OK,0, 5003);


}

void Createmenu()
{	// create menu
	gui::IGUIContextMenu* menu = Device->getGUIEnvironment()->addMenu();
	menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
	//sous menu file
	gui::IGUIContextMenu* submenu;
	submenu = menu->getSubMenu(0);
	submenu->addItem(L"New Game", 100);
	submenu->addSeparator();
	submenu->addItem(L"Sound & Parametres (S)",102);
	submenu->addItem(L"Change Video mode (V)",103);
	submenu->addSeparator();
	submenu->addItem(L"Quit Game (Q)", 109);
	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Help (F1)", 500);
	submenu->addItem(L"Tips to play", 502);
	submenu->addSeparator();
	submenu->addItem(L"About the game", 501);
}

//event rreceiver for imputs of mouse& keyboard
#include <irrlicht.h>
class MyEventReceiver : public IEventReceiver
{

public:
	virtual bool OnEvent(const SEvent& event)

	{if (!initialize||!Device->isWindowActive()) return false;

		// PRESSE C swaps Camera Input fix ou mobile

                       //canon movement if camera is active not with fpscamera
                if ((Device->getSceneManager()->getActiveCamera() == Camera) && event.EventType == EET_KEY_INPUT_EVENT);
                { irr::core::vector3df position;
                //move canon right
                    if ( event.KeyInput.Key == irr::KEY_RIGHT&& smgr->getActiveCamera() == Camera)
                    {
                    if (canon->getPosition().X <
                    (data_level[current_level].width/2+data_level[current_level].width/4))
                    {
                    position =canon->getPosition();
                    position.X+=data_level[current_level].shell_speedc*2;
                    canon->setPosition(position);
                    Camera->setPosition(position+offset_camera);
                    angle_node->setPosition(position+offset_angle_node);
                    shield.bbsn->setPosition(position);
                    Camera->setTarget(core::vector3df(position.X,data_level[current_level].heigth/4,data_level[current_level].length/3));

                    return true;
                    }

                    }
                     if ( event.KeyInput.Key == irr::KEY_LEFT && smgr->getActiveCamera() == Camera)
                    {
                    if (canon->getPosition().X >
                    (data_level[current_level].width/2-data_level[current_level].width/4))
                    {
                    position =canon->getPosition();
                    position.X-=data_level[current_level].shell_speedc*2;
                    canon->setPosition(position);
                    Camera->setPosition(position+offset_camera);
                    angle_node->setPosition(position+offset_angle_node);
                    shield.bbsn->setPosition(position);
                    Camera->setTarget(core::vector3df(position.X,data_level[current_level].heigth/4,data_level[current_level].length/3));
                    return true;
                    }

                    }
                }
               if (event.EventType == EET_KEY_INPUT_EVENT//on a pressé une touche
                    && !event.KeyInput.PressedDown)
               {



                    //c change camera
                    if ( Device && event.KeyInput.Key == irr::KEY_KEY_C)
                    {
                    scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera ();
                    if ( camera )
                    {
                    if (Camera->isInputReceiverEnabled() )
                    {Camera->setInputReceiverEnabled (false );
                    Camerafps->setInputReceiverEnabled(true);
                    Device->getSceneManager()->setActiveCamera(Camerafps);
                    return true;
                    }
                    else if (Camerafps->isInputReceiverEnabled())
                    {Camerafps->setInputReceiverEnabled (false);
                    Camera->setInputReceiverEnabled (true);
                    Device->getSceneManager()->setActiveCamera(Camera);
                    return true;
                    }
                    }
                    else smgr->setActiveCamera(Camera);
                    return true;
                    }
                    //d call debug logging routine
                    if ( event.KeyInput.Key == irr::KEY_KEY_D)
                    {debug();
                    }
                    //F1 call help text
                    if ( event.KeyInput.Key == irr::KEY_F1)
                    {
                    HelpText();
                    return true;
                    }
                    //on active/désactive le curseur et la Pause
                    if ( event.KeyInput.Key == irr::KEY_KEY_P)
                    {gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                    Pause=!Pause;
                    cursorcontrol->setVisible(Pause);
                    return true;
                    }
                    //quit game
                    if ( Device && event.KeyInput.Key == irr::KEY_KEY_Q)
                    {Device->closeDevice();
                    }
                    //call parameters window
                    if (event.KeyInput.Key == irr::KEY_KEY_S)
                    {
                    gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                    cursorcontrol->setVisible(true);
                    if (!Pause)Pause=true;
//                    sound_parameters();
                    return true;
                    }
                    //calls  video settings
                    if (event.KeyInput.Key == irr::KEY_KEY_V)
                    {
                    gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                    cursorcontrol->setVisible(true);
                    if (!Pause)Pause=true;
                    change_settings();
                    return true;
                    }
               }//end case key pressed
    if (event.EventType == EET_MOUSE_INPUT_EVENT &&(!Pause))
    {
//////////shoot bullets
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL)
            {tir();  }
/////////Target
		if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
               {time_on_target = 0 ;
               mire_jets = true;}

          if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		  {mire_jets = false;
		  return true;
            }

		if (event.MouseInput.Event == EMIE_MOUSE_MOVED && Device->isWindowActive())
		{
          mircanon();
          return true;
		}
    }

//////////on sélectionne dans le menu
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Device->getGUIEnvironment();
          switch(event.GUIEvent.EventType)
               {case EGET_MENU_ITEM_SELECTED:
                    {// a menu item was clicked
                    IGUIContextMenu* menu = (IGUIContextMenu*)event.GUIEvent.Caller;
                    s32 id = menu->getItemCommandId(menu->getSelectedItem());
                    switch(id)
                    {
                    case 100: // File -> New game
						NewGame();
						return true;

				case 102:	// sound parameters
//                         sound_parameters();
                         return true;

                    case 103: //video settings & options
                         change_settings();
                         return true;

                    case 109: // File -> Quit
                         Device->closeDevice();
                         return true;

                    case 211://apply change setttings
                        save_settings(paramIrr);
                         return true;

					case 500://about box
                        HelpText();
                        return true;

                    case 501://about box
                        AboutText();
                        return true;
                    case 502://TIPS box
                        TipsText();
                        return true;
					}
				break;
				}
				//end menu selected


			case EGET_SCROLL_BAR_CHANGED:
				// control skin transparency
				if (id == 104)
                        {s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
                        data.setAlpha(pos);
                        transparency = pos;
                        for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
                        {video::SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
                        col.setAlpha(pos);
                        env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
                        }
                        return true;
                        }
				if (id == 115)//controls sounds level
                        {s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
                        master_volume = (100-pos);
//                        irrsound->setSoundVolume(master_volume*.01);
//                        impact_sound->setDefaultVolume(master_volume*.01);
 //                       canon_sound->setDefaultVolume(master_volume*.01);
  //                      clink_sound->setDefaultVolume(master_volume*.01);
   //                     targeting_sound->setDefaultVolume(master_volume*.01);
    //                    launch_sound->setDefaultVolume(master_volume*.01);
                        now = timer->getRealTime();
                        if ((now - time_to_beep) > 300)
                        {//irrsound->play2D(targeting_sound);
                        time_to_beep = now;
                        }
                        return true;
                        }
                    if (id == 116)//controls music volume
                        {s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
                        {//music_volume = (100-pos);
//                        music_sound->setVolume(music_volume*.01);
                        }
                        return true;
                        }
                         //end croll bar changed
               case EGET_BUTTON_CLICKED:
                    //reverse audio requested, left is right and vice versa
                    if (id == 106)
                         {audio_reverse_normal =!audio_reverse_normal;
                         if (audio_reverse_normal)
                         {audio_reverse = "normal";
//                         sound_dir =irrklang::vec3df(0,1,0);
                         }
                         else
                         {audio_reverse = "reverse";
 //                        sound_dir =irrklang::vec3df(0,-1,0);
                         }
  //                       audio_text->setText(audio_reverse.c_str());
  //                       irrsound->setListenerPosition(Camera->getPosition()
  //                       ,(Camera->getTarget()-Camera->getAbsolutePosition())
  //                       ,irrklang::vec3df(0,0,0), sound_dir);
                         return true;
                         }

                    if (id == 108)//save audio and transparency
                         {save_parameters();
                         return true;
                         }
                    if (id==112)//close change settings window
                         {
                         IGUIElement *Ige;//
                         IGUIElement* root = env->getRootGUIElement();
                         Ige = root->getElementFromId( 5000, true);
                         Ige->remove();
                         Pause = false;
                         gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                         cursorcontrol->setVisible(false);
                         return true;
                         }
                    if (id==211)//apply change settings
                         {
                         save_settings(paramIrr);
					return true;
                         }
                    if (id==212)//close change settings
                         {
                         IGUIElement *Ige;//
                         IGUIElement* root = env->getRootGUIElement();
                         Ige = root->getElementFromId( 200, true);
                         Ige->remove();
                         Pause = false;
                         gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                         cursorcontrol->setVisible(false);
                         return true;
                         }
                    //end button clicked
               case EGET_COMBO_BOX_CHANGED:
                    if (id==207)
                         {
                         s32 i=cmbScreenResolution->getSelected();
                         modify_settings(i);
                         return true;
                         }
               case EGET_CHECKBOX_CHANGED:
                    if (id==221)//set fullscreen
                         {
                         paramIrr.Fullscreen = !paramIrr.Fullscreen;
                         return true;
                         }
                     if (id==222)//set stencilbuffer
                         {
                         paramIrr.Stencilbuffer = !paramIrr.Stencilbuffer;
                         return true;
                         }
                     if (id==223)//set antialias
                         {
                         paramIrr.AntiAlias = !paramIrr.AntiAlias;
                         return true;
                         }
                     if (id==224)//set vsync
                         {
                         paramIrr.Vsync = !paramIrr.Vsync;
                         return true;
                         }
                    //end combo box changed
               case (EGET_MESSAGEBOX_OK )://EGET_ELEMENT_CLOSED||
                    if (id==5000)//sound setting window wants to close
                        {Pause = false;
                        return false;
                        }
                    if (id == 5001)//help box wants to close
                        {Pause = false;
                        gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                        cursorcontrol->setVisible(false);
                        return false;
                        }
                    if  (id==5002)//tips box window wants to close
                        {Pause = false;
                        gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                        cursorcontrol->setVisible(false);
                        return false;
                        }
                    if  (id==5003)//about box window wants to close
                        {Pause = false;
                        gui::ICursorControl* cursorcontrol = Device->getCursorControl();
                        cursorcontrol->setVisible(false);
                        return false;
                        }

                  break;// end switch event type
                }
			}

return false;
		}



};



/*
==================================================================================================*/
///========INITIALIZE SOUND ENGINE =================================================================
void startIrrKlang()
{
//	irrsound = irrklang::createIrrKlangDevice();
	//if (!irrsound)
//	Device->getGUIEnvironment()->addMessageBox(
	//	Caption.c_str(), L"The Irrklang device could not be loaded. " \
	//	L"Maybe files are misplaced.");
//		return;}
	//set sound listener on canon, with option of reverse audio
	if (audio_reverse_normal)
          {//audio_reverse = "normal";
//          sound_dir =irrklang::vec3df(0,1,0);
          }
    else
          {//audio_reverse = "reverse";
    //      sound_dir =irrklang::vec3df(0,-1,0);
          }
// 	irrsound->setListenerPosition(Camera->getAbsolutePosition(),
//	(Camera->getTarget()-Camera->getAbsolutePosition()),irrklang::vec3df(0,0,0)
//	, sound_dir);
//	irrsound->setSoundVolume(master_volume*.01);
//	irrsound->setDefault3DSoundMaxDistance(data_level[current_level].length/2);
//	irrsound->setRolloffFactor(0.03);
	// play music
//	music_sound = irrsound->play2D("media/Wicked Synth Loop.wav", true, false, true);
//	if (music_sound)
//	{
//		music_sound->setVolume(music_volume*.01);
//	}
/*
	// preload sound effectsESM_NO_STREAMING
	impact_sound = irrsound->addSoundSourceFromFile("media/impact.wav",irrklang::ESM_NO_STREAMING,true);
	impact_sound->setDefaultMinDistance(20);
	impact_sound->setDefaultMaxDistance(1000);
	impact_sound->setDefaultVolume(1.0);
	//shooting a shell
	canon_sound = irrsound->addSoundSourceFromFile("media/canon_sound.wav",irrklang::ESM_NO_STREAMING,true);
	canon_sound->setDefaultVolume(0.5f);
	//no more shells for now
	clink_sound = irrsound->addSoundSourceFromFile("media/empty_gun.wav",irrklang::ESM_NO_STREAMING,true);
	clink_sound->setDefaultVolume(0.5f);
	//missiles related sound
     targeting_sound = irrsound->addSoundSourceFromFile("media/targeting.wav",irrklang::ESM_NO_STREAMING,true);
	targeting_sound->setDefaultVolume(0.5f);
	//lauching a missile
	launch_sound = irrsound->addSoundSourceFromFile("media/missile_launch.wav",irrklang::ESM_NO_STREAMING,true);
     launch_sound->setDefaultVolume(0.5f);
     //shield impact sound
     shield_sound = irrsound->addSoundSourceFromFile("media/shield.wav",irrklang::ESM_NO_STREAMING,true);
     shield_sound->setDefaultVolume(0.5f);
*/

}

//initialize this wave with data from array=====================================
//==============================================================================
bool init_wave()
{
    //points for remaining towers
if (current_wave !=0)
     {
	    for (int i=0;i<tower_number;i++)
	    {if (!tour[i].isDestroy)points+=200;
          }
     }
//two routines, first if we come from another level
//maybe theyre was more tank then current level
//reset jets and tanks
for (int i=0;i<tank_number ;i++)
    {
    tank[i].tank_mesh->setPosition(core::vector3df((15*i), (20*i)+50,100));
    }
for (int i=0;i< jet_number;i++)
    {
    jet[i].jet_mesh->setPosition(core::vector3df((15*i),(20*i)+100,100));
    }
//initialize tanks and jets for current wave-level
for (int i=0;i< data_level[current_level].max_jetc+current_wave;i++)
     {u32 time =Device->getTimer()->getRealTime();
     jet[i].init_jets(time);
     }

for (int i=0;i< data_level[current_level].max_tankc+current_wave;i++)
     {u32 time =Device->getTimer()->getRealTime();
     tank[i].init_tanks(time);
     }

//initialize shells and obus

	for(int i=0; i < shell_number; i++)
	{
	shell[i].isVisible = false;
    shell[i].position = deadzone;
    shell[i].pshell->setVisible(false);
	shell[i].speed= data_level[current_level].shell_speedc;
	shell[i].last_time = timer->getRealTime();
	}
	nshell =-1;

	for(int i=0; i < obu_number; i++)
	{
	obu[i].isVisible = false;
    obu[i].position = deadzone;
    obu[i].pobus->setVisible(false);
	obu[i].speed= data_level[current_level].shell_speedc;
	obu[i].last_time =0;
	}
	obu[0].n_obus =-1;

	for(int i=0; i < obu_number; i++)
	{
	mobu[i].isVisible = false;
    mobu[i].position = core::vector3df(-10*i,-10*i+100,10);
    mobu[i].pobus->setPosition(core::vector3df(-10*i,-10*i+100,10));
    mobu[i].pobus->setVisible(false);
	mobu[i].speed= data_level[current_level].shell_speedc;
	mobu[i].last_time=0;
	}
	mobu[0].n_mobus =-1;
 for (int j=0; j<missile_number; j++)
    {missile[j].isVisible = false;
    missile[j].position = core::vector3df(-10*j,-10*j+100,10);
	missile[j].missile_mesh->setPosition(core::vector3df(-10*j,-10*j+100,10));
	missile[j].missile_mesh->setVisible(false);
	missile[j].last_time =0;
    }
    missile[0].n_missile = -1;

//display current level and wave
wave_text=("VAGUE # ");
wave_text.append(core::stringw(current_wave+1),4);
level_on_time = Device->getTimer()->getRealTime()+3000;
data_wave[current_wave].wave_complete = false;

return true;
}
//initialize this level with data from array====================================
//==============================================================================

bool init_level()
    {
    irr::core::vector3df outCollisionPoint, origin, start, end;
    irr::core::triangle3df  outTriangle;
    irr::core::line3df line;
    f32 y=0;
    ///to fog the shadows
    core::list<scene::ISceneNode*>::ConstIterator child_node;
    scene::IMeshCache *meshcache;
    scene::ESCENE_NODE_TYPE nodetype=scene::ESNT_TEXT;
        if (current_level !=0)
        {
            for (int i=0;i<tower_number;i++)
            {if (!tour[i].isDestroy)points+=1000;
            }
        }
    //load parameters particular to the current level
    //number and speed of bullets, enemy; gravity of this world,
    //first we cleanup
    //remove selector
    if(selector) selector->drop();
    //reomove terrain and skydome texture and mesh
    //from previous level

    if(terrain)
        {
        terrain->remove();
        meshcache= smgr->getMeshCache();
        meshcache->removeMesh(ISM2);
        SkyBox->remove();
        }
        ///terrain_mesh = smgr->getMesh(data_level[current_level].map);
         ISM2 =
        (scene::ISkinnedMesh*)smgr->getMesh(data_level[current_level].map);
        ISM2->setMaterialFlag(video::EMF_LIGHTING, true);
        terrain = smgr->addAnimatedMeshSceneNode(ISM2,0, 17);
        /*
    /// add fog to level #2
        if(current_level == 1)
        {
        terrain->setMaterialFlag(video::EMF_FOG_ENABLE,true);
        driver->setFog(video::SColor(0, 235, 235, 255),
        true,
        10.0f,
        1000.0f,
        5.0f,
        true,
        true);

            for (int i=0;i<tank_number ;i++)
            {
            tank[i].tank_mesh->setMaterialFlag(video::EMF_FOG_ENABLE,true);
            jet[i].jet_mesh->setMaterialFlag(video::EMF_FOG_ENABLE,true);
            mobu[i].pobus->setMaterialFlag(video::EMF_FOG_ENABLE, true);
            missile[i].missile_mesh->setMaterialFlag(video::EMF_FOG_ENABLE, true);
            shell[i].pshell->setMaterialFlag(video::EMF_FOG_ENABLE, true);
            obu[i].pobus->setMaterialFlag(video::EMF_FOG_ENABLE, true);
    ///turn on fog for child of tank = = shadow
            psvsn[i]->setVisible(false);
            }
            }
            else
    ///watch it, tank and jet have to be same number
            {
            terrain->setMaterialFlag(video::EMF_FOG_ENABLE,false);
            for (int i=0;i<tank_number ;i++)
            {
            tank[i].tank_mesh->setMaterialFlag(video::EMF_FOG_ENABLE,false);
            jet[i].jet_mesh->setMaterialFlag(video::EMF_FOG_ENABLE,false);
            mobu[i].pobus->setMaterialFlag(video::EMF_FOG_ENABLE, false);
            missile[i].missile_mesh->setMaterialFlag(video::EMF_FOG_ENABLE, false);
            shell[i].pshell->setMaterialFlag(video::EMF_FOG_ENABLE, false);
            obu[i].pobus->setMaterialFlag(video::EMF_FOG_ENABLE, false);
            psvsn[i]->setVisible(true);
            }
        }

*/
        if (terrain == 0)
            {Device->getGUIEnvironment()->addMessageBox(
            Caption.c_str(), L"The terrain could not be loaded. " \
            L"Maybe it is not a supported file format.");
            return false;
            }
    // position the world for easy bullet and move calculation
         terrain->setPosition(core::vector3df(10,10,10));
    // on cré le sélecteur pour que les objet n'enfonce pas dans le sol
            ///selector = smgr->createOctTreeTriangleSelector(terrain_mesh, terrain);
            selector = smgr->createOctTreeTriangleSelector(ISM2, terrain);
            terrain->setTriangleSelector(selector);
            if(!selector) return false;
            bool OK = init_wave();
            Device->getGUIEnvironment()->getFont("media/mediumfont.png");
            level_text=("NIVEAU ");
            level_text.append(core::stringw(current_level+1),4);
            if (!OK)Device->getGUIEnvironment()->addMessageBox(
                Caption.c_str(), L"Next wave could not be initialized. " \
                L"Press OK");

    //add towers
        for (int i=0;i<tower_number;i++)
        {
        start =core::vector3df(data_level[current_level].width/14*(i+5.48f),data_level[current_level].heigth,250.0f);
        end=core::vector3df(data_level[current_level].width/14*(i+5.48f),-200,250.0f);
        line.setLine(start,end);
        smgr->getSceneCollisionManager()->getCollisionPoint
                       (line, selector, outCollisionPoint, outTriangle);
         y = outCollisionPoint.Y+15;
        tour[i].anim_node->setPosition(core::vector3df(data_level[current_level].width/14*(i+5.48f),y,250.0f));
        }
    //make the skybox
    skydome = driver->getTexture(data_level[current_level].world);
    SkyBox = smgr->addSkyDomeSceneNode(skydome,10,10,1.0,1.5);
    SkyBox->setRotation(core::vector3df(0.0f,180.0f,0.0f));
    return true;
}



//initialize world with all objects that stay thru all the game
//==============================================================================
bool init()
{
    //for trblesh
    log_str =("   ");
    timer=Device->getTimer();
    current_level =0;//=============================================================
    current_wave =0;
    points = 0;
//initialize the basic elements of the game
//that stay thru all levels
//on load le canon
//printf("%c",Device->getFileSystem->getWorkingDirectory);
  	scene::IMesh* mesh =smgr->getMesh("media/canon.ms3d");
	canon = smgr->addMeshSceneNode (mesh);
    canon->setPosition(core::vector3df(data_level[current_level].width/2,data_level[current_level].heigth/4,-30));
    canon->setMaterialFlag(video::EMF_LIGHTING,true);
	canon->setScale(core::vector3df(0.15,0.15,0.15));

// add a dynamic camera scene node
	Camerafps = smgr->addCameraSceneNodeFPS(0, 50.0f, .10f, -1, 0, 0, false);
	Camerafps->setPosition(core::vector3df(1250.0f,800.0f,500.0f));//(canon->getPosition());
	Camerafps->setFarValue(4000.0f);
	Camerafps->setTarget(core::vector3df(1000,100,1000));
// add the static camera that will eventually move...
	Camera = smgr->addCameraSceneNode();

	Camera->setRotation(core::vector3df(0.0f,0.0f,0.0f));
	Camera->setPosition(canon->getPosition()+offset_camera);
	Camera->setTarget(core::vector3df(data_level[current_level].width/2,data_level[current_level].heigth/4,data_level[current_level].length/3));
	Camera->setFarValue(3000.0f);
	smgr->setActiveCamera (Camera);
//initialize random generator
	srand(Device->getTimer()->getRealTime());
///load the tanks
	//anim_mesh
     scene::ISkinnedMesh* ISM = (scene::ISkinnedMesh*)smgr->getMesh("media/tank.ms3d");
     for (int j=0; j<tank_number; j++)
     {
	tank[j].tank_mesh = smgr->addAnimatedMeshSceneNode(ISM,0,(j+tank_mask));
	if (tank[j].tank_mesh == 0)
	{Device->getGUIEnvironment()->addMessageBox(
	Caption.c_str(), L"The tanks could not be loaded. " \
	L"Maybe it is not a supported file format.",j);
	return false;}
	tank[j].tank_mesh->setMaterialFlag(video::EMF_LIGHTING,1);
	tank[j].tank_mesh->setPosition(core::vector3df((15*j), (20*j)+50,100));
//	psvsn[j]= tank[j].tank_mesh->addShadowVolumeSceneNode((j+tank_mask+2048), true, 2000.0f);
	tank[j].tank_mesh->setLoopMode(false);
	tank[j].tank_mesh->animateJoints(false);
	tank[j].tank_mesh->setJointMode(scene::EJUOR_CONTROL );
	}
	tank[0].n_tank=-1;
///load the jets
	anim_mesh = smgr->getMesh("media/vaissel_anim.ms3d");//jet.ms3d");for normal jets
	if (anim_mesh == 0)
	    {Device->getGUIEnvironment()->addMessageBox(
     	Caption.c_str(), L"The vaissel could not be loaded. " \
        L"Maybe it is not a supported file format.");
        return false;
        }
     for (int j=0; j<jet_number; j++)
     {
	jet[j].jet_mesh = smgr->addAnimatedMeshSceneNode(anim_mesh,0,(j+jet_mask)
	,core::vector3df (15*j,20*j+100,100)
	,core::vector3df (0, 0, 0)
	,core::vector3df (1.0f, 1.0f, 1.0f));
	//jet[j].jet_mesh->setAnimationSpeed(5);
	if (jet[j].jet_mesh == 0)
	    {Device->getGUIEnvironment()->addMessageBox(
     	Caption.c_str(), L"The jets could not be loaded. " \
          L"Maybe it is not a supported file format.",j);
          return false;
          }
	jet[j].jet_mesh->setMaterialFlag(video::EMF_LIGHTING, true);
	jet[j].jet_mesh->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	jet[j].jet_mesh->setLoopMode(true);
	jet[j].jet_mesh->setAnimationSpeed(14.0);
	}
	jet[0].n_jet = -1;
//load missiles
	mesh = smgr->getMesh("media/missile.ms3d");
    for (int j=0; j<missile_number; j++)
    {
	missile[j].missile_mesh = smgr->addMeshSceneNode(mesh,0,j+missile_mask);
	if (missile[j].missile_mesh == 0)
	    {Device->getGUIEnvironment()->addMessageBox(
     	Caption.c_str(), L"The missiles could not be loaded. " \
        L"Maybe it is not a supported file format.",j);
        return false;
        }
	missile[j].missile_mesh->setMaterialFlag(video::EMF_LIGHTING, 1);
	missile[j].missile_mesh->setPosition(core::vector3df(-10*j,-10*j+100,10));
    }
     missile[0].n_missile = -1;
//load fortress
	for (int i=0;i<tower_number;i++)
	{
	anim_mesh=smgr->getMesh("media/tour_anim.ms3d");
	if (anim_mesh == 0)
	    {Device->getGUIEnvironment()->addMessageBox(
     	Caption.c_str(), L"The missiles could not be loaded. " \
          L"Maybe it is not a supported file format.");
          return false;}
	tour[i].anim_node = smgr->addAnimatedMeshSceneNode(anim_mesh,0,i+tour_mask);
	tour[i].anim_node->setScale(core::vector3df(0.7f,0.7f,0.7f));
//	tour[i].anim_node->addShadowVolumeSceneNode(-1, true, 3000.0f);
	tour[i].anim_node->setLoopMode(true);
	tour[i].anim_node->setAnimationSpeed(18);
	}
	n_tour=tower_number;
    tour[n_tour-1].alive=n_tour;

	smgr->setShadowColor(video::SColor(220,0,0,0));
//data indication
	pts_pos = core::rect<s32>(driver->getScreenSize().Width - tableau_x
	,driver->getScreenSize().Height/6-tableau_y
	,driver->getScreenSize().Width
	,driver->getScreenSize().Height/6+tableau_y);

    angle_pos = pts_pos;
    angle_pos.UpperLeftCorner.Y+=40;
    angle_pos.LowerRightCorner.Y+=40;
    gravity_pos = pts_pos;
    gravity_pos.UpperLeftCorner.Y+=80;
    gravity_pos.LowerRightCorner.Y+=80;
    level_pos = core::rect<s32>((driver->getScreenSize().Width/2)-60
	,(driver->getScreenSize().Height/6)-60
	,(driver->getScreenSize().Width/2)+30
	,(driver->getScreenSize().Height/6)+30);
    wave_pos = core::rect<s32>(level_pos.UpperLeftCorner.X
    ,level_pos.UpperLeftCorner.Y+80
    ,level_pos.LowerRightCorner.X
    ,level_pos.LowerRightCorner.Y+80);
//angle indicator
    scene::IMesh* angle = smgr->getMesh("media/angle_indicator.ms3d");
	angle_node = smgr->addMeshSceneNode(angle);
    angle_node->setVisible(true);
	angle_node->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
    angle_node->getMaterial(0);
    angle_node->setPosition(canon->getPosition()+offset_angle_node);
    angle_node->updateAbsolutePosition();
	cross_hair = driver->getTexture("media/cross_hair.png");

//display game title...3d splash
	scene::IMesh* message = smgr->getMesh("media/message.ms3d");
	scene::IMeshSceneNode* message_node = smgr->addMeshSceneNode(message,0,2001);
	message_node->setVisible(true);
	message_node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	message_node->setMaterialFlag(video::EMF_LIGHTING,false);
    message_node->setPosition(core::vector3df(1050,250,200));
	message_node->updateAbsolutePosition();
	message_node->addAnimator(smgr->createRotationAnimator(core::vector3df(0.1f,0.5f,0.2f)));
	message_node->addAnimator(smgr->createDeleteAnimator(7000));
//shield
   smgr->addBillboardSceneNode
    (NULL, irr::core::dimension2df(500.0f, 500.0f),
    offset_shield,  -1 );
    scene::IMesh *imesh = smgr->getMesh("media/shield.ms3d");
    shield.bbsn = smgr->addMeshSceneNode(imesh, 0,-1, canon->getPosition(),
 core::vector3df(0, 0, 0),
 core::vector3df(1.0f, 1.0f, 1.0f),
  false
 );

    //video::ITexture * shield_image = driver->getTexture("media//shield.png");
    shield.bbsn->setMaterialFlag(video::EMF_LIGHTING, true);
    shield.bbsn->setMaterialFlag(video::EMF_ZBUFFER, false);
    //driver->makeColorKeyTexture(shield_image, video::SColor(125,0,0,0));
    shield.bbsn->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    //shield.bbsn->getMaterial(0).MaterialTypeParam =.01f;
    //shield.bbsn->setMaterialTexture(0, shield_image);
    shield.bbsn->setVisible(false);
    shield.bbsn->setPosition(canon->getPosition());


//ERASE CURSOR
	gui::ICursorControl* cursorcontrol = Device->getCursorControl();
    if(cursorcontrol)
        {cursorcontrol->setVisible(false);
        cursorcontrol->setPosition(0.5f,0.5f);
        }
//bannier up the screen
        bar_message.append("     ",5);
        bar_message.append("NOSTALGIA",35);
        bar_message.append("         ",10);
        bar_message.append("PRESS F1 FOR HELP",25);
        bar_message.append("         ",10);
        bar_message.append("PRESS P TO PAUSE AND ACCESS MENU",35);
return true;
}
//Main part, initialize engine, lights, etc
//==============================================================================
//==============================================================================
int main()
{
    //loads video settings
    load_settings();
    //loads audio settings and skin transparency
    printf("loading params..");
    load_parameters();
    data.setAlpha(transparency);
	// create Device and exit if creation failed
	MyEventReceiver receiver;
	paramIrr.EventReceiver = &receiver;
	paramIrr.Fullscreen = false;
	Device = createDeviceEx(paramIrr);
	if (!Device)
		return 1;
    printf("Device Created..");
	Device->setResizeAble(false);
	Device->setWindowCaption(nomdujeux);
///for troobleshooting
	logger = Device->getLogger();
///handle to video driver
	driver = Device->getVideoDriver();
///setting up gui and manager
	IGUIEnvironment* env = Device->getGUIEnvironment();
	smgr = Device->getSceneManager();
	iscm = smgr->getSceneCollisionManager();
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
/// add our media directory as "search path"
	Device->getFileSystem()->addFolderFileArchive ( "media/" );
///init the world
    printf("Device Created..");
	initialize = init();
	if (!initialize){Device->getGUIEnvironment()->addMessageBox(
		Caption.c_str(), L"The game could not be initialize. " \
		L"Maybe engine not running");
		return -1;}
///initialise the bullet array
	for(int i=0; i < shell_number; i++)
	{
	shell[i].pshell =smgr->addSphereSceneNode(4,16,0,-1,Camera->getPosition());
	shell[i].pshell->setMaterialTexture(0, driver->getTexture("textures/shell.bmp"));
	shell[i].pshell->setMaterialFlag(video::EMF_LIGHTING, 1);
	shell[i].speed= data_level[current_level].shell_speedc;
	}
///initialise the obus array
	for(int i=0; i < obu_number; i++)
	{
	obu[i].pobus =smgr->addSphereSceneNode(10,16,0,50+i,core::vector3df(300+i,75+i,0));
	obu[i].pobus->setMaterialTexture(0, driver->getTexture("textures/obus.bmp"));
	obu[i].pobus->setMaterialFlag(video::EMF_LIGHTING, 1);
    obu[i].speed= data_level[current_level].shell_speedc;
	}

///initialise the mobus array
     anim_mesh = smgr->getMesh("media/mobu_anim.ms3d");
     for(int i=0; i < obu_number; i++)
	{
	mobu[i].pobus =smgr->addAnimatedMeshSceneNode(anim_mesh,0,i);
	mobu[i].pobus->setMaterialFlag(video::EMF_LIGHTING, 1);
    mobu[i].speed= data_level[current_level].jet_speedc;
	}
//=================================================================//!init first level
	bool OK = 1;//1 should be >> init_level(); but i changed it to get things loading.
	init_level();
	if (!OK){Device->getGUIEnvironment()->addMessageBox(
		Caption.c_str(), L"The LEVEL could not be initialize. " \
		L"Maybe files missing");
		return -1;}


	// set a nicer font
	IGUISkin *skin= env->createSkin(EGST_WINDOWS_METALLIC );
     env->setSkin(skin);
     for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
          {
          video::SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
          col.setAlpha(transparency);
          env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
		}
	IGUIFont* font = env->getFont("media/fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);
     Createmenu();
	// create toolbar
	gui::IGUIToolBar* bar = env->addToolBar();
	// create fps text pour afficher les infos dans la barre titre
	IGUIStaticText* message_text_bar = env->addStaticText(L"", core::rect<s32>(100,4,500,23), true, false, bar);
	IGUIStaticText* fpstext_bar = env->addStaticText(L"", core::rect<s32>(20,4,90,23), true, false, bar);
///set up light
	light = smgr->addLightSceneNode(0
	,core::vector3df(1600,300,-50)
	,video::SColorf(0.7f, 1.0f, 1.0f, 1.0f)
	, 5000.f);
	video::SLight lightdata;
	lightdata = light->getLightData();
//	lightdata.Type = video::ELT_SPOT;
//	lightdata.OuterCone = 70.0f;
//	lightdata.Attenuation=irr::core::vector3df(0.15f,0.f,0.f);
//	light->setRotation(irr::core::vector3df(20,-40,0));
    light->setLightData(lightdata);

/// on efface le curseur au départ
	cursorcontrol = Device->getCursorControl();
	if(cursorcontrol)
     (cursorcontrol->setVisible(false));
///load mire image and custom cursor
    mire = driver->getTexture("media/mire.png");
    mire_jets = false;
///data display on screen
    font2 = env->getFont("media/mediumfont.png");
///Démarre son, start sound
    startIrrKlang();
/// load the irrlicht engine logo
    env->addImage(driver->getTexture("media/irrlichtlogo.bmp"),
			core::position2d<s32>(driver->getScreenSize().Width-100, driver->getScreenSize().Height-40));


/// draw everything

while(Device->run() && driver)
	{
	//	if (Device->isWindowActive())
     //     {
          driver->beginScene(true, true, video::SColor(0,0,0,0));
          if (!Pause & !game_over)///if not on Pause we update everything
			{
            check_collision();
			update();
               if(mire_jets) targeting();///target cursor
               if(shield.shield_effect_on) shield.shield_effect();///been hit, shield effect
            createShellImpacts();
			createEnemyImpacts();
			}
            smgr->drawAll();
            env->drawAll();
///updating data display, points, gravity, angle of canon
			font2->draw(pts.c_str(), pts_pos,data);
			font2->draw(angle_text.c_str(),angle_pos,data);
			font2->draw(gravity_text.c_str(),gravity_pos,data);
///splash at beginning of wave, current level, current wave
          if(timer->getRealTime() < level_on_time)
            {
            font2->draw(level_text.c_str(), level_pos,data);
            font2->draw(wave_text.c_str(), wave_pos,data);
            }
///draw angle crosshair indicator
            crosshair_pos = iscm->getScreenCoordinatesFrom3DPosition(angle_node->getAbsolutePosition());
           	driver->draw2DImage(cross_hair,crosshair_pos,core::rect< s32 >(0,0,64,64)
			,NULL, data, true);
//draw crosshair player is targeting a jet or tank
          if (mire_jets)
			{
            mire_pos = cursorcontrol->getPosition();//get cursor position
            driver->draw2DImage(mire,mire_pos,core::rect< s32 >(0,0,64,64)
			,NULL, video::SColor(150,255,255,255), true);
                }
			driver->endScene();
///Wave complete ?

          if ((jet[0].n_jet < 0) && (tank[0].n_tank < 0)&&(!game_over))
               {data_wave[current_wave].wave_complete = true;
               current_wave++;
               if (current_wave == data_level[current_level].max_wave)
                    {data_level[current_level].level_complete = true;
                    }
                    else
                    {
                    bool ok = init_wave();
                if (!ok)
                    Device->getGUIEnvironment()->addMessageBox(
                    Caption.c_str(), L"Next wave could not be initialized. " \
		          L"Press OK");
                    }
               }
///level complete ?
          if ((data_level[current_level].level_complete == true)&&(!game_over))
                {
                current_level++;
                current_wave=0;
                init_level();
                }
///all level completed
          if((data_level[current_level].level_complete == true) &&
			(current_level == number_of_level-1))
			{Device->getGUIEnvironment()->addMessageBox(
                    L"CONGRATULATIONS & THANKS FOR TRYING NOSTALGIA", L"    All levels are complete. Stay tune for next game " \
		          L"Press <OK>");
	        game_over=true;
	        Pause = true;
               if(cursorcontrol)
                    (cursorcontrol->setVisible(true));
                    current_level++;
               }
///all tower destroy ? => game over
            if ((tour[n_tour-1].alive ==0)&& (!game_over))
               {Device->getGUIEnvironment()->addMessageBox(
                L"GAME OVER THANKS FOR TRYING NOSTALGIA"
                , L"     All air raffinery are destroyed. Use File->New game       " \
		        L"Press <OK>");
                    all_game_over();
               }


//add indication in top bar keep it in case trblshoot need
//kept for troobleshooting
         /*   camera_pos = Camerafps->getPosition();
            int k =4;
            str += L"FPS: ";
            str.append(core::stringw(driver->getFPS()));
            str += L"  X: " ;str.append(core::stringw(camera_pos.X),k);
			str += L"  Y: " ;str.append(core::stringw(camera_pos.Y),k);
			str += L"  Z: " ;str.append(core::stringw(camera_pos.Z),k);*/
//ecrit la rotation de la camera
/*
			str.append(core::stringw(driver->getFPS()));str += L" rotation ";
			str += L"  X: " ;str.append(core::stringw(canon_rot.X),k);
			str += L"  Y: " ;str.append(core::stringw(canon_rot.Y),k);
			str += L"  Z: " ;str.append(core::stringw(canon_rot.Z),k);*/
//display FPS in bar
            fps_text=(L"FPS: ");
            fps_text.append(core::stringw(driver->getFPS()));
			fpstext_bar->setText(fps_text.c_str());
//display message in bar
            message_text_bar->setText(bar_message.c_str());//press F1 for help
//preparing gui display strings
            pts=(L"POINTS:  ");
            pts.append(core::stringw(points),5);
            angle_text=(L"ANGLE-X:");
            angle_text.append(core::stringw(anglex_degree),4);
            gravity_text = (L"GRAVITY:");
            gravity_text.append(core::stringw(data_level[current_level].gravityc.Y*-1500),3);
            gravity_text.append(L" g",2);
	//	}
	//	else
	//	Device->yield();
	}
//clean up
	if (selector) selector->drop();
//	irrsound->removeAllSoundSources();
	Device->drop();

	return 0;
}
