
///class to initialize and activate objects
/*				log_str2.append(L"move  ",7);
            	       log_str2.append(core::stringw(movement.Z),5);
				log_str2.append(L"lapse  ",15);
				log_str2.append(core::stringw(lapse),5);
				log_str2.append(L"yet  ",17);
				log_str2.append(core::stringw(yet),15);
				log_str2.append(L"now  ",17);
				log_str2.append(core::stringw(now),15);
				log_str2.append(L"last  ",17);
				log_str2.append(core::stringw(last_time),15);
                logger->log(log_str2.c_str());
                log_str2 =("   ");

                log_str2.append(core::stringw(position.X),5);
				log_str2.append(L"    Y ",7);
				log_str2.append(core::stringw(position.Y),5);
				log_str2.append(L"    Z ",7);
				log_str2.append(core::stringw(position.Z),5);
                logger->log(log_str2.c_str());
                log_str2 =("   ");

                 ///pour update tanks glicth
                log_str2.append(L"  old X ",9);
				log_str2.append(core::stringw(oldpos.X),5);
				log_str2.append(L"   oldY ",9);
				log_str2.append(core::stringw(oldpos.Y),5);
				log_str2.append(L"   oldZ ",9);
				log_str2.append(core::stringw(oldpos.Z),5);
                 log_str2.append(L"  rotX  ",17);
            logger->log(log_str2.c_str());
            log_str2 =("   ");
                */
#include "cshell.h"
using namespace irr;

struct SParticleImpact
	{
		irr::u32 when;
		core::vector3df pos;
		core::vector3df outVector;
		irr::scene::ISceneNode* node;

	};
extern core::array<SParticleImpact> Enemy_Impacts;
int jets::n_jet =0;
int tanks::n_tank=0;
int missiles::n_missile =-1;
int obus::n_obus = -1;
int mobus::n_mobus = -1;
irr::core::vector3df down_vector(0.0f,-30.0f,0.0f);
irr::core::vector3df jet_down_vector(0.0f,-1000.0f,0.0f);
extern jets jet[];
extern tanks tank[];
extern tours tour[];
extern obus obu[];
extern mobus mobu[];
extern cshell shell[];
extern missiles missile[];
///extern const int jet_number;
//extern irrklang::ISoundSource* impact_sound;
extern irr::scene::ITriangleSelector* selector;
extern irr::scene::ISceneManager* smgr;
extern int n_tour;
extern irr::IrrlichtDevice* Device;
extern bool pause;
extern irr::ILogger* logger;
extern int nshell;///nombre de shell
//extern scene::IMeshSceneNode* canon;
extern int current_level;
extern int current_wave;
//extern int points;
extern shields shield;
irr::f32 amplitude_dir = 30;
irr::u32 safety_band = 500;
irr::f64 distance;
irr::core::array<SParticleImpact> Impacts_obus,Impacts_mobus;

///for sound
//extern irrklang::ISoundEngine* irrsound;
//extern irrklang::ISoundSource* canon_sound;
///small class for shield effect
shields::shields()
{
    shield_effect_on = false;
    movx =5;
    movy=3;
}
shields::~shields()
{}
///for the shells we shoot
cshell::cshell()
{
};
cshell::~cshell(){};
///obus from tank
obus::obus(){};
obus::~obus(){};
///mobus from vaissel
mobus::mobus(){};
mobus::~mobus(){};
///troobleshooting
core::stringw log_str2(L"      ");
///for towers
tours::tours()
{isVisible = false;
isDestroy =false;
damage = 0;///maybe implement it some day
};
tours::~tours(){};
///missile const & destr
missiles::missiles()
{isVisible = false;
target =0;
///position = deadzone;
};
missiles::~missiles()
{};




///fonctions for jets enemy
jets::jets()
     {
     isVisible = false;
     };


jets::~jets(){};
///fonction for tanks enemy

tanks::tanks()
     {
     isVisible = false;
     position = deadzone;
     isDead=false;
     touch_down = false;
     };

tanks::~tanks(){};

/// data for waves in level
wave data_wave[3]={
///wave one
///all pass to randomizer as upper limit
     {
     1///current wave
,     false///wave complete
,    4000///longest time after start of wave before enemy appear
,	10000///longuest delay between salvo
,	30///width of shooting window for enemy
,	5000///longuest time between two changes of direction
,   4000///minimum time between two salvo from enemies
     }
,
///wave2
     {
     2///current wave
,     false///wave complete
,     3000///time after start of wave before enemy appear
,	9000///longuest delay between salvo
,	20///width of shooting window for enemy
,	4000///longuest time between two changes of direction
,   3000///minimum time between two salvo from enemies
     }
,
///wave 3
 {
     3///current wave
,     false///wave complete
,    1000///time after start of wave before enemy appear
,	5000///longuest delay between salvo
,	16///width of shooting window for enemy
,	2500///longuest time between two changes of direction
,   1500///minimum time between two salvo from enemies
     }

};
///data for each level
///level 1
level data_level[4]={
///mars world
{
"media/sol_mars.ms3d"///terrain map
,"textures/mars.jpg"///bounding box
,false///level_complete
, 3/// number of shell
, 2///number of tanks=2
, 1///number of jets
,3///number of obus from tanks or jets
,5///number of missile for this level
,3///max wave in this level
,2500///map width even number only
,500///map height even number only
,2500///map lenght even number only
, 1.2f///shell speed
, 0.07f///tank speed
, 0.12f///jet speed
, 20///max angle of dir change even number only
, irr::core::vector3df(0.0f,-0.000676f,0.0f)///gravity of world
},
///level 2
 ///QUAD
{
"media/sol_quad.ms3d"///terrain map
, "textures/quad_fog.jpg"///bounding box
,false///level_complete
, 5/// number of shell
, 3///3///number of tanks
, 3///3///number of jets
,5///number of obus from tanks
,6///number of missile for this level
,3///max wave in this level
,2500///map width even number only
,500///map height even number only
,2500///map lenght even number only
, 1.2f///shell speed
, 0.09f///tank speed
, 0.15f///jet speed
,26///max angle of dir change even number only
, irr::core::vector3df(0.0f,-0.0008f,0.0f)
}///gravity of world
,
///level 3
 ///ICELAND
{
"media/sol_glace.ms3d"///terrain map
, "textures/glace.jpg"///bounding box
,false///level_complete
, 7/// number of shell
, 5//5///number of tanks
, 5//5///number of jets
,7///number of obus from tanks
,12///number of missile for this level
,3///max wave in this level
,2500///map width even number only
,500///map height even number only
,2500///map lenght even number only
, 1.2f///shell speed
, 0.1f///tank speed
, 0.18f///jet speed
,30///max angle of dir change even number only
, irr::core::vector3df(0.0f,-0.0005f,0.0f)///gravity of world
}
,
///level 4
 ///desert hills
{"media/sol_hills_anim.ms3d"///terrain map
, "textures/sol_hills.png"///bounding box
,false///level_complete
, 10/// number of shell
, 5///number of tanks
, 5///number of jets
,5///number of obus from tanks
,10///number of missile for this level
,3///max wave in this level
,2500///map width even number only
,500///map height even number only
,2500///map lenght even number only
, 1.2f///shell speed
, 0.08f///tank speed
, 0.16f///jet speed
,30///max angle of dir change even number only
, irr::core::vector3df(0.0f,-0.001f,0.0f)///gravity of world
}
};

///! generates a pseudo random number
/// with value between zero and limit
irr::u32 Randomizer::random(irr::u32 limit)
     {irr::u32 random_number;
     if (limit < 1) limit = 1;
     int rdm = rand();
     random_number = rdm%limit;
     return random_number;

	};

///jets fonction for updating, falling, etc
///sets jet pos, dir, etc

void jets::init_jets(irr::u32 yet)
{irr::u32 x=0,y,z= data_level[current_level].length-50;
irr::core::line3df line;
irr::core::vector3df outCollisionPoint, origin, start, end;
irr::core::triangle3df  outTriangle;

     isVisible=true;
     jet_mesh->setVisible(true);
     isDead = false;
     dying =false;
     touch_down= false;
      ///setting initial position
     x= Randomizer::random(data_level[current_level].width)/2;
     x+=data_level[current_level].width/4;
     y = Randomizer::random(data_level[current_level].heigth/2);
     y+=data_level[current_level].heigth/2;
     position=(irr::core::vector3df(x+5,y,z));
     jet_mesh->setPosition(position);
	jet_mesh->setRotation(irr::core::vector3df(0,0,0));
     ///direction
	direction=initial_dir_move;
	direction= direction.normalize();
	jet_mesh->setLoopMode(true);
     ///when do the jet start to move
     last_time= Randomizer::random(data_wave[current_wave].appear/(current_wave+1)) + yet;
     ///next shooting
     next_shoot= Randomizer::random(data_wave[current_wave].salve/(current_wave+1))
                + yet+data_wave[current_wave].minbetweensalvo;
     ///next dir change
    next_direction_change= Randomizer::random(data_wave[current_wave].redirection)
        +(data_wave[current_wave].redirection/2) + yet;
     jets::n_jet++;

}
///jet have been hit, must fall down
void jets::kill_jets()

{
points+=500;
isVisible = false;
///left dead enemies visible, looks more like war field :)
///jet_mesh->setVisible(false);
///position = deadzone;
///jet_mesh->setPosition(deadzone);
direction = irr::core::vector3df(0,0,0);
jets::n_jet--;
}
///shooting shell
void jets::shoot()
{
     int fill=0;
	while (mobu[fill].isVisible)
        {fill++;
        if (fill == data_level[current_level].max_obus) return;}
     mobu[fill].direction= direction;
	mobu[fill].position= position + irr::core::vector3df(0,-15,0);
	mobu[fill].isVisible = true;
	mobu[fill].pobus->setVisible(true);
	mobu[fill].pobus->setLoopMode(true);
	mobu[fill].pobus->setAnimationSpeed(14.0);
	mobu[fill].last_time = 0;
	mobu[0].n_mobus++;
}
///function for updating the jets
void jets::update_jets(irr::u32 yet)
{
	irr::core::vector3df rotate;
	if(dying && isVisible)
     {Is_dying();
     return;
     }
     if ( yet >last_time)
     {
		if (isVisible)
		{
               ///jet has reach end of terrain safely
               if (position.Z < -30)
                    {kill_jets();
                    points-=1000;
                    return;
                    }
            irr::core::vector3df movement,oldpos;
            irr::f32 lapse = (yet - last_time );
            if ((lapse) > 100) lapse =10;;///on a fait une pause
            movement = direction * lapse*data_level[current_level].jet_speedc;
            position +=movement;
            rotate =jet_mesh->getRotation();
            rotate.Z=0;
            if (rotate.X >360) rotate.X-=360;
            jet_mesh->setRotation(rotate);
            jet_mesh->setPosition(position);
            if (yet > next_shoot)
                {jets::shoot();
                next_shoot= Randomizer::random(data_wave[current_wave].salve/(current_wave+1)) + yet
                +data_wave[current_wave].minbetweensalvo;
                }
///jet is about to fall on side, redirect it
         if ((position.X > data_level[current_level].width -safety_band)||
            (position.X < safety_band))
            {
            direction.X*=-1.0;
            rotate = jet_mesh->getRotation();
            rotate.Y=direction.getHorizontalAngle().Y+180.0f;
            if (rotate.Y > 360) rotate.Y-=360;
            jet_mesh->setRotation(rotate);
            next_direction_change=
            Randomizer::random(data_wave[current_wave].redirection)
                    +(data_wave[current_wave].redirection/2) + yet;
            }
///time to change direction ?
            if (yet > next_direction_change)
            {
            irr::u32 i = Randomizer::random(data_level[current_level].redir_max_angle);
            direction.X= sin(core::DEGTORAD*(s32)(i-(data_level[current_level].redir_max_angle/2)));
            direction = direction.normalize();
            rotate.Y=direction.getHorizontalAngle().Y+180.0f;
            if (rotate.Y > 360) rotate.Y-=360;
            jet_mesh->setRotation(rotate);
            next_direction_change=
            Randomizer::random(data_wave[current_wave].redirection)
                +(data_wave[current_wave].redirection/2) + yet;
            }

        last_time = yet;

        }
    }

}
void jets::Is_dying()
{irr::core::line3df line;
irr::core::vector3df outCollisionPoint, milieu, offset, rotate, movement,out;
irr::core::triangle3df  outTriangle;
SParticleImpact imp;
irr::f32 lapse = (now - last_time ) * data_level[current_level].jet_speedc;
 ///setup dying last movement and duration
    if (!isDead)
    {
    int i=Randomizer::random(20);///30
    direction.X= i*0.05;
    i = Randomizer::random(20)+50;///40
    direction.Y=i*-0.5;
    direction.Z*=0.05;
    direction = direction.normalize();
    rotation.X=-80;
    isDead=true;
    TimeToDie = now+Randomizer::random(3000)+3000;
    out = position;
			out.setLength(0.06f);
///getting data for pos, time of impact
			imp.when = 1;
			imp.outVector = core::vector3df(0.0f,0.3f,0.0f);
			imp.pos = position;
			imp.node=jet_mesh;
			if (imp.when)
				{
/// create impact note
				imp.when = Device->getTimer()->getRealTime()+ 100;
				Enemy_Impacts.push_back(imp);
				}
    }
///jet is really dead
if (now > TimeToDie)
{
kill_jets();
return;
}
if (!touch_down)
{
direction+= data_level[current_level].gravityc*lapse*2;
///rotation.X+=11;
rotation.Y+=2*lapse;
movement = direction *lapse;
position+=movement;
jet_mesh->setRotation(rotation);
jet_mesh->setPosition(position);
last_time =now;
}

///if not on ground checking if close
          if (!touch_down)
               {line.setLine(position, (position + irr::core::vector3df(0,15,0)));///jet_down_vector
			if (smgr->getSceneCollisionManager()->getCollisionPoint
                   (line, selector, outCollisionPoint, outTriangle)||(position.Y < -10))
                    {
                    touch_down = true;
                    jet_mesh->setLoopMode(false);
                    jet_mesh->setPosition(position- irr::core::vector3df(0,5,0));
                    scene::IParticleSystemSceneNode* pas = 0;
                    pas = smgr->addParticleSystemSceneNode(false, 0, -1,position
                    ,core::vector3df(0.0f,0.0f,0.0f)
                    ,core::vector3df(3.0f,4.0f,3.0f));
                    scene::IParticleEmitter* em = pas->createBoxEmitter(
                    core::aabbox3d<f32>(-10,-10,-10,15,15,15),
                    core::vector3df(0.04f,-0.04f,-0.04f),
                    700,1200,
                    video::SColor(100,20,55,80), video::SColor(150,40,60,90),
                    2000,2500,180);
                    pas->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));
                    pas->setEmitter(em);
                    em->drop();
                    scene::IParticleAffector* paf = pas->createFadeOutParticleAffector(video::SColor(0,0,0,0), 2000);
                    pas->addAffector(paf);
                    paf->drop();
                    pas->setMaterialFlag(video::EMF_LIGHTING, false);
                    pas->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
                    pas->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("media/explosion.png"));
                    pas->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
                    scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(2500);
                    pas->addAnimator(anim);
                    anim->drop();
                    }
               }

}
///===========================================================================
///
///===========================================================================
///tanks fonction for updating, falling, etc
///sets tank pos, dir, etc
void tanks::init_tanks(irr::u32 now)
{irr::f32 x=0,y=data_level[current_level].heigth,z= data_level[current_level].length-50;
irr::core::line3df line;
irr::core::vector3df outCollisionPoint, origin, start, end;
irr::core::triangle3df  outTriangle;

     isVisible=true;
     isDead = false;
     dying =false;
     touch_down= false;
      int i =Randomizer::random(4);
     target = tour[i].anim_node->getPosition();
      ///setting initial position
     i= Randomizer::random(data_level[current_level].width/2);
     i+=data_level[current_level].width/4;
     x=i*1.0;
     start = irr::core::vector3df(x, y ,z);
     end = irr::core::vector3df(x, y-500, z);
     line.setLine(start,end);
     smgr->getSceneCollisionManager()->getCollisionPoint
                   (line, selector, outCollisionPoint, outTriangle);
     y = outCollisionPoint.Y+28;
     position=(irr::core::vector3df(x,y,z));
     tank_mesh->setPosition(position);
     ///direction
	direction=initial_dir_move;
     ///when do the tank start to move
     last_time=Randomizer::random(data_wave[current_wave].appear/(current_wave+1)) + now;
     ///next shooting
     next_shoot= Randomizer::random(data_wave[current_wave].salve/(current_wave+1))
                + now+data_wave[current_wave].minbetweensalvo;;
     ///next dir change
     next_direction_change= Randomizer::random(data_wave[current_wave].redirection)
        +data_wave[current_wave].redirection/2 + now;
     tank[0].n_tank++;
     }
///tank have been hit, must break down
void tanks::kill_tanks()
{///on doit droper les collision animator
points+=500;
isVisible = false;
///position = deadzone;
///tank_mesh->setPosition(deadzone);
direction = irr::core::vector3df(0,0,0);
tank[0].n_tank--;
}
//=================================
void tanks::Is_dying()
{irr::core::line3df line;
irr::core::vector3df outCollisionPoint, milieu, offset, rotate, movement;
irr::core::triangle3df  outTriangle;
irr::f32 convert;
bool move_left = false;
irr::f32 lapse = (now - last_time );
if(lapse>100) lapse =10;
 ///setup dying last movement and duration
     if (!isDead)
     {
     u32 i=Randomizer::random(6);
     direction.Y=i*0.1;
     i=Randomizer::random(6);///40
     convert = i*.1;
     direction.X=convert-0.3;
     i=Randomizer::random(6);
      convert = i*.1;
     direction.Z=convert-.03;
     direction=direction.normalize();
     TimeToDie = now+Randomizer::random(3000)+3000;
     isDead=true;
     }
///tank is really dead
if (now > TimeToDie)
{
kill_tanks();
return;
}

if (!touch_down)
		{rotate = tank_mesh->getRotation();
		direction+= data_level[current_level].gravityc*lapse;
		if (direction.X <0) rotate.X+=0.3;
		else rotate.X-=0.4;
		if (direction.Y <0) rotate.Y+=0.5;
		else rotate.Y-=0.3;
		movement = direction  * data_level[current_level].tank_speedc * lapse;
		position+=movement;
		tank_mesh->setRotation(rotate);
		tank_mesh->setPosition(position);
		last_time =now;
		}
///if not on ground checking if close
          if ((!touch_down)&&(movement.Y<0))
               {line.setLine(position, (position+down_vector/4));
			if (smgr->getSceneCollisionManager()->getCollisionPoint
                   (line, selector, outCollisionPoint, outTriangle))
                    {rotate = tank_mesh->getRotation();
                    ///offset.Y = outCollisionPoint.Y- milieu.Y-3;
                    position +=movement;
                    position.Y=line.getMiddle().Y-3.0;
                    direction  = irr::core::vector3df(0.0f,0.0f,0.0f);
                    tank_mesh->setRotation(rotate);
                    tank_mesh->setPosition(position);
                    touch_down=true;
                    }

               }


}

///tanks shoot obus
void tanks::shoot()
{
     irr::core::line3df line;
     irr::core::vector3df outCollisionPoint, origin, start, end;
     u32 find =0;
	if (!selector || pause)
		return;
	if (obu[0].n_obus == data_level[current_level].max_obus -1)
	{return;
	}
	/// create obus data when use
	int fill=0;
        while (obu[fill].isVisible)
            {fill++;
            if (fill == data_level[current_level].max_obus-1) return;
            }
        obu[fill].position=position + irr::core::vector3df(0,6,5);
        obu[fill].isVisible = true;
        obu[fill].pobus->setVisible(true);
        obu[fill].last_time =0;
        obu[0].n_obus++;
///Using player shooting direction to target
        if (nshell >-1)
        {
        find =0;
            while (shell[find].isVisible==false)
                {find++;
                }
        obu[fill].direction = shell[find].direction;
        obu[fill].direction.Z*=-1;
        obu[fill].direction.X*=-1.1;
        obu[fill].direction.Y*=0.9;
        obu[fill].direction.normalize();
        if (obu[fill].direction.Y < 0.0) obu[fill].direction.Y*=-1.0;
        }
///utiliser la direction du tank comme direction initiale pour l'obus
        else
        {
        obu[fill].direction = direction;
        int x =Randomizer::random(data_wave[current_wave].accuracy);
        obu[fill].direction.X = x*.01;
        obu[fill].direction.Y = data_level[current_level].gravityc.Y*
        position.getDistanceFrom(target)*-0.06;
        obu[fill].direction.Z=data_level[current_level].shell_speedc*-1.0;
        obu[fill].direction.normalize();
          }
///animate tank turrel
scene::ISceneNode* isn;
isn = tank_mesh->getJointNode("turrel");
irr::core::vector3df newrotation= obu[fill].direction.getHorizontalAngle();
newrotation.Y+=180.0f;
if (isn !=0)
    isn->setRotation(newrotation);
 ///play sound cannon shooting shell
//	if (canon_sound)
	//irrsound->play3D(canon_sound,position);
}
///================================================================================
///function for updating the tank array,
void tanks::update_tanks(irr::u32 yet)
{
	irr::core::vector3df outCollisionPoint, milieu, offset, rotate;
	irr::core::triangle3df  outTriangle;
    now=yet;
	if(dying && isVisible)
     {Is_dying();
     return;
     }
		if (isVisible)
			{
               ///tank has reach end of terrain safely
               if (position.Z < 20  )
                    {kill_tanks();
                    points-=1000;
                    }
            irr::core::vector3df movement,oldpos;
            irr::f32 lapse = (yet - last_time );
            if ((lapse) > 100) lapse = 10;
			movement = direction *data_level[current_level].tank_speedc* lapse;
			irr::core::line3df line;
			line.setLine(position, position + down_vector);//+irr::core::vector3df(0.0,10.0,0.0)
			if (smgr->getSceneCollisionManager()->getCollisionPoint
                   (line, selector, outCollisionPoint, outTriangle))
                    {milieu = line.getMiddle();
                    offset.Y = outCollisionPoint.Y- milieu.Y;
                    oldpos=position;
                    position.Y+=offset.Y+5;
                    position +=movement;
                    oldpos=position-oldpos;
                    }

                    else
                    {
                    position.Y=5;
                    position += movement;
                    oldpos =position;
                    oldpos=position-oldpos;
                    }
                    oldpos.normalize();
                    rotate=oldpos.getHorizontalAngle();
                    rotate.Z=0;
                    tank_mesh->setRotation(rotate);
                    tank_mesh->setPosition(position);
        if (yet > next_shoot)
            {tanks::shoot();
            next_shoot= Randomizer::random(data_wave[current_wave].salve/(current_wave+1))
                        + yet+data_wave[current_wave].minbetweensalvo;
            }
///tank is about to fall on side, redirect it
        if ((position.X > data_level[current_level].width -safety_band)||
     	(position.X < safety_band))
        {
        direction.X*=-1.0;
        rotate = tank_mesh->getRotation();
        rotate.Y = direction.X*-90;
        tank_mesh->setRotation(rotate);
        next_direction_change=
        Randomizer::random(data_wave[current_wave].redirection)
            +(data_wave[current_wave].redirection/2) + yet;
        }
///time to change direction ?
        if (yet > next_direction_change)
        {irr::u32 i = Randomizer::random(data_level[current_level].redir_max_angle);
        direction.X= sin(core::DEGTORAD*((s32)i-data_level[current_level].redir_max_angle/2));
        direction = direction.normalize();
        rotate.Y = direction.getHorizontalAngle().Y;
        tank_mesh->setRotation(rotate);
        next_direction_change=
        Randomizer::random(data_wave[current_wave].redirection)
            +(data_wave[current_wave].redirection/2) + yet;
        }

     }
     last_time = yet;


for (int i=0;i<n_tour;i++)
	{if (tour[i].isDestroy)continue;
	distance =position.getDistanceFrom(tour[i].anim_node->getPosition());
	if(distance< (collide_distance*2))
		{ 	tour[i].under_attack();
			}
     }

}
///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
///updating mobus shot by jets (missile-obus)
void mobus::update_mobus(irr::u32 now)
{core::triangle3df triangle;
	core::line3d<f32> line;
	core::vector3df movement,rotate;
	core::vector3df out;
	core::vector3df end;
	core::vector3df start;
	SParticleImpact imp;
	imp.when = 0;
	if (last_time ==0) {last_time = now;return;}///returning from pause
///update les mobus
if (isVisible)
{
f32 lapse = now - last_time;
if(lapse>100) lapse =10;
direction += data_level[current_level].gravityc*lapse;
movement = direction * data_level[current_level].jet_speedc*lapse;
position += movement;
pobus->setPosition(position);
last_time = now;
///checking collision with ground
///if so, changing attributes of obus to stop updating
	start = position;
	end = position + movement;
	line.setLine(start, end);

	if (smgr->getSceneCollisionManager()->getCollisionPoint(
			line, selector, end, triangle))
		{out = triangle.getNormal();
		out.setLength(0.02f);
///getting data for pos, time of impact
		imp.when = 1;
		imp.outVector = out;
		imp.pos = position- movement;
///one obus less
		isVisible = false;
          position = deadzone;
          mobu[0].n_mobus--;
          pobus->setVisible(false);
		}
		if (position.Y < -5)
		{
        imp.when = 1;
        imp.outVector = core::vector3df (0.1,.1,0.1);
        imp.pos = position- movement;
        isVisible = false;
        position = deadzone;
        mobu[0].n_mobus--;
        pobus->setVisible(false);
          }
		if (imp.when)
		{///checking impact with tower
               for (int i=0;i<n_tour;i++)
	         {if (tour[i].isDestroy)continue;
	         if(start.getDistanceFrom(tour[i].anim_node->getPosition())< (collide_distance*6))
               {    if (tour[i].isDestroy==false)
                    {tour[i].under_attack();
                    }
               }
               }
		/// create impact note
		imp.when = Device->getTimer()->getRealTime()+ 100;
		Impacts_mobus.push_back(imp);
		}
     if((position.getDistanceFrom(canon->getPosition()+offset_shield)< (collide_distance*4)))shield.shield_effect_on =true;
rotate =direction.getHorizontalAngle();
rotate.X-=180;
pobus->setRotation(rotate);

    }
}
///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
///update shell shot by tanks
void obus::update_obus(irr::u32 now)
{core::triangle3df triangle;
	core::line3d<f32> line;
	core::vector3df movement;
	core::vector3df out;
	core::vector3df end;
	core::vector3df start;
	SParticleImpact imp;
	imp.when = 0;
	if (last_time ==0) {last_time = now; return;}///returning from pause
	///update les shell




f32 lapse = now - last_time;
if(lapse>100) lapse =10;
direction += data_level[current_level].gravityc*lapse;
movement = direction * speed*lapse;
position += movement;
pobus->setPosition(position);
last_time = now;

///checking collision with ground
///if so, changing attributes of obus to stop updating
	start = position;
	end = position + movement;
	line.setLine(start, end);

	if (smgr->getSceneCollisionManager()->getCollisionPoint(
			line, selector, end, triangle))
		{out = triangle.getNormal();
		out.setLength(0.09f);
///getting data for pos, time of impact
		imp.when = 1;
		imp.outVector = out;
		imp.pos = position- movement;
///one obus less
		isVisible = false;
          position = deadzone;
          obu[0].n_obus--;
          pobus->setVisible(false);
		}
		if (position.Y < -5)
		{
        imp.when = 1;
        imp.outVector = core::vector3df (0.1,.1,0.1);
        imp.pos = position- movement;
        isVisible = false;
        position = deadzone;
        obu[0].n_obus--;
        pobus->setVisible(false);
          }

		if (imp.when)
		{///checking impact with tower
          for (int i=0;i<n_tour;i++)
               {if (tour[i].isDestroy)continue;
                    if(start.getDistanceFrom(tour[i].anim_node->getPosition())
                    < (collide_distance*3))
                    {if (!tour[i].isDestroy) tour[i].under_attack();
                    }
          }
//obus hadi hit the canon
        if((position.getDistanceFrom(canon->getPosition()+offset_shield)< collide_distance*4))shield.shield_effect_on =true;

		/// create impact note
		imp.when = Device->getTimer()->getRealTime()+ 100;
		Impacts_obus.push_back(imp);
		}

}
void tours::under_attack()
	{this->isDestroy = true;
	this->anim_node->setLoopMode(false);
	 scene::IParticleSystemSceneNode* psn = 0;
			psn = smgr->addParticleSystemSceneNode(false,  this->anim_node, -1);
			psn->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));
			scene::IParticleEmitter* em = psn->createBoxEmitter(
				core::aabbox3d<f32>(-5,-5,-5,5,5,5),
				core::vector3df (0,0.1,0), 200,400, video::SColor(0,255,255,255),video::SColor(0,255,255,255),
				900,1300, 10);
        	psn->setEmitter(em);
			em->drop();
			scene::IParticleAffector* paf = psn->createFadeOutParticleAffector(video::SColor(0,0,0,0),5000);
			psn->addAffector(paf);
			paf->drop();
			psn->setMaterialFlag(video::EMF_LIGHTING, false);
			psn->setMaterialTexture(0, Device->getVideoDriver()->getTexture("media/smoke.bmp"));
			psn->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	        tour[n_tour-1].alive--;
	        shield.shield_effect_on =true;
	}
	///=========================================================================
	///launching a missile
void missiles::launch(scene::ISceneNode *target_on)
{     irr::core::line3df line;
     target = target_on;
     speed =data_level[current_level].jet_speedc*2;
     position = canon->getPosition();
     missile_mesh->setPosition(position);

     line.setLine(position,target->getPosition());
     direction = line.getVector();
     direction.normalize();
     isVisible = true;
     missile_mesh->setVisible(true);
///add smoke trail to missile
          scene::IParticleSystemSceneNode* pas = 0;
		pas = smgr->addParticleSystemSceneNode(false, missile_mesh, -1,core::vector3df(0.0f,0.0f,-30.0f));
		pas->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));
		scene::IParticleEmitter* em = pas->createBoxEmitter(
			core::aabbox3d<f32>(-5,-5,-5,5,5,5),
			direction*-0.1, 200,400,
               video::SColor(0,95,95,150),video::SColor(0,150,100,250),
			200,1200, 10);
        pas->setEmitter(em);
		em->drop();
		scene::IParticleAffector* paf = pas->createFadeOutParticleAffector(video::SColor(0,50,50,50),6000);
		pas->addAffector(paf);
		paf->drop();
		pas->setMaterialFlag(video::EMF_LIGHTING, false);
		pas->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("media/smoke.bmp"));
		pas->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

     points-=30;
     n_missile++;
}
void missiles::update_missiles(u32 yet)
{
	irr::core::vector3df rotate;
	irr::core::line3df line;
	if ( yet >last_time)
     {
		if (isVisible)
		{if (target->isVisible()== false)
               {this->kill_missile();
                return;
               }

          irr::core::vector3df movement,oldpos;
          if ((yet-last_time) > 200) last_time = yet-5;///on a fait une pause
          line.setLine(position,target->getPosition());
          direction = line.getVector();
          direction.normalize();
          irr::f32 lapse = (yet - last_time ) * speed;
          movement = direction * lapse;
          position +=movement;
          rotate =direction.getHorizontalAngle();
          missile_mesh->setRotation(rotate);
          missile_mesh->setPosition(position);
          last_time = yet;

          if (position.getDistanceFrom(target->getPosition()) < collide_distance/10)
          {this->hit_enemy();
          ///kill the missile
          this->kill_missile();
          }
          }
     }

}
void missiles::hit_enemy()
{
///jets *target_jet;
if (target->getID()& jet_mask)///it is a jet
     {jets *target_jet = &jet[target->getID()- jet_mask];
     ///target_jet->dying = true;///kill jet
     target_jet->dying=true;
     }

if (target->getID()& tank_mask)///it is a tank
     {tanks *target_tank = &tank[target->getID()- tank_mask];
     target_tank->dying = true;///kill tank
     //target_tank->kill_tanks();
     }
///make explosion
            scene::IParticleSystemSceneNode* pas = 0;
			pas = smgr->addParticleSystemSceneNode(false, 0, -1,missile_mesh->getPosition()
                    ,core::vector3df(0.0f,0.0f,0.0f)
                    ,core::vector3df(3.0f,3.0f,3.0f));
                    scene::IParticleEmitter* em = pas->createBoxEmitter(
			core::aabbox3d<f32>(0,0,0,25,25,25),
			core::vector3df(0.04f,-0.04f,-0.04f),
			700,1200,
			video::SColor(80,20,55,80), video::SColor(220,40,60,90),
			2000,2500,180);
			pas->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));
			pas->setEmitter(em);
			em->drop();
			scene::IParticleAffector* paf = pas->createFadeOutParticleAffector(video::SColor(0,0,0,0), 2000);
			pas->addAffector(paf);
			paf->drop();
			pas->setMaterialFlag(video::EMF_LIGHTING, false);
			pas->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
			pas->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("media/explosion.png"));
			pas->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(2500);
			pas->addAnimator(anim);
			anim->drop();
//irrsound->play3D(impact_sound, position);

}
void missiles::kill_missile()
{

isVisible=false;
missile_mesh->setVisible(false);
position = deadzone;
missile_mesh->removeAll();
target = 0;
n_missile--;
}
