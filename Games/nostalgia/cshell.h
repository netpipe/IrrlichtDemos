//structur to manipulate shells

#ifndef __CSHELL_H_INCLUDED
#define __CSHELL_H_INCLUDED
#include <irrlicht.h>
//#include "irrKlang.h"
#include "constants.h"

const irr::core::vector3df deadzone(1000.0,0.0,-200.0);
const irr::core::vector3df initial_dir_move (0.0,0.0,-1.0);

class shields
	{
	public:
	irr::scene::IMeshSceneNode* bbsn;
	irr::core::vector3df rotation;
	irr::core::vector3df position;
	irr::u32 last_time;
	irr::u32 until_time;
	irr::u32 next_cut;
	irr::u32 cut_lenght;
	irr::s32 lapse;
	irr::s32 movx;
	irr::s32 movy;
	bool shield_effect_on;
	void shield_effect();
	shields();
	~shields();
	};

class cshell
	{
	public:
	bool	isVisible;
	static int n_shell;
	irr::f32 speed;
	irr::scene::ISceneNode* pshell;
	irr::core::vector3df angle;
	irr::core::vector3df direction;
	irr::core::vector3df position;
	irr::u32 last_time;
	cshell();
	~cshell();
	};
class obus
{    public:
	bool	isVisible;
	static int n_obus;
	irr::f32 speed;
	irr::scene::ISceneNode* pobus;
	irr::core::vector3df angle;
	irr::core::vector3df direction;
	irr::core::vector3df position;
	irr::u32 last_time;
	obus();
	~obus();
	void update_obus(irr::u32);
     };
class mobus
{    public:
	bool	isVisible;
	static int n_mobus;
	irr::f32 speed;
	irr::scene::IAnimatedMeshSceneNode* pobus;
	irr::core::vector3df angle;
	irr::core::vector3df direction;
	irr::core::vector3df position;
	irr::u32 last_time;
	mobus();
	~mobus();
	void update_mobus(irr::u32);
     };
class missiles
     {	public:
	static int n_missile;
	static irr::u32 time_to_beep;
	bool isVisible;
	irr::scene::IMeshSceneNode* missile_mesh;
    irr::scene::ISceneNode* target;
	irr::u32 now;
	irr::f32 speed;
	irr::u32 last_time;
	irr::core::vector3df rotation;
	irr::core::vector3df direction;
	irr::core::vector3df position;
	missiles();
	~missiles();
	void init_missiles(irr::u32);
	void update_missiles(irr::u32);
	void hit_enemy();
	void kill_missile();
     void launch(irr::scene::ISceneNode *);

     };
class tours
     {
     public:
     bool isVisible;
     bool isDestroy;
     int alive;
     irr::u32 damage;
     irr::scene::IAnimatedMeshSceneNode* anim_node;
     tours();
     ~tours();
     void under_attack();
     };

typedef struct
	{irr::c8* map;
	irr::c8* world;
	bool level_complete;
	int max_shellc;
	int max_tankc;
	int max_jetc;
	int max_obus;
	int max_missile;
	int max_wave;
	irr::u32 width;
	irr::u32 heigth;
	irr::u32 length;
	irr::f32 shell_speedc;
	irr::f32 tank_speedc;
	irr::f32 jet_speedc;
	irr::u32 redir_max_angle;
	irr::core::vector3df gravityc;
	}level;
//structur for aggressivity of enemy in waves
typedef struct
	{
     int this_wave;
     bool wave_complete;
     irr::u32 appear;//time after start of wave before enemy appear
	irr::u32 salve;//longuest delay between salvo
	irr::u32 accuracy;//width of shooting window for enemy
	irr::u32 redirection;//longuest time between two changes of direction
	irr::u32 minbetweensalvo;//minimum time between two shots from enemies

     }wave;
//class of enemy tanks
class tanks
		{
		public:
		static int n_tank;
		bool	isVisible;
		bool isDead;
		bool dying;
		bool touch_down;
		int damage;
		int shoot_precision;
		int shoot_interval;
		irr::u32 now;
		irr::u32 TimeToDie;
		irr::f32 speed;
		irr::u32 last_time;
		irr::u32 next_direction_change;
		irr::u32 next_shoot;
		irr::scene::IAnimatedMeshSceneNode* tank_mesh;
		irr::core::vector3df target;
		irr::core::vector3df rotation;
		irr::core::vector3df oldrotate;
		irr::core::vector3df direction;
		irr::core::vector3df position;
		tanks();
		~tanks();
		void init_tanks(irr::u32);
		void update_tanks(irr::u32);
        void kill_tanks();
        void Is_dying();
        void setLevel(int);
        void shoot();
		};

//class of enemy jets
class jets
	{
	public:
	static int n_jet;
	bool isVisible;
     bool isDead;
	bool dying;
	bool touch_down;
	int damage;
	int shoot_precision;
	int shoot_interval;
	irr::u32 now;
	irr::u32 TimeToDie;
	irr::f32 speed;
	irr::u32 last_time;
	irr::u32 next_direction_change;
	irr::u32 next_shoot;
	irr::scene::IAnimatedMeshSceneNode* jet_mesh;
	irr::core::vector3df target;
	irr::core::vector3df rotation;
	irr::core::vector3df direction;
	irr::core::vector3df position;
	jets();
	~jets();
	void init_jets(irr::u32);
	void update_jets(irr::u32 );
    void kill_jets();
    void Is_dying();
    void setLevel(int);
    void shoot();
	};

	//! generates a pseudo random number
	//with an upper limit
	class Randomizer
	{
	public:
	static irr::u32 random(irr::u32);
	};

#endif//cshell included
