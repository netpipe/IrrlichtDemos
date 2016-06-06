#include <irrlicht.h>
#include <iostream>
#include <vector>
#include <defines.hpp>
#include <globals.hpp>
#include <variklis/matke/taskas.hpp>
#include <variklis/garsas/garsai.hpp>
#include <variklis/garsas/garsas.hpp>
#include <variklis/garsas/ogg_garsas.hpp>

KGarsai		sounds;
KGarsas		sound_roll_over, sound_click, sound_rotate, sound_true, sound_false;
KGarsas		music_win, music_lose;
KOggGarsas	music;
KTaskas		sound_position;


using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;

enum {
	MODE_MENU,
	MODE_GAME,
};

video::IVideoDriver* driver;
scene::ISceneManager* smgr;
int width = 800,
	height = 600,
	vsync = 1,
	fullscr = 0,
	bits = 32,
	antialiasing = 1;
float music_volume = 1.0,
	sound_volume = 1.0;
float ratio;
int mode = MODE_MENU;
bool quit = false;

void GameCreate();
void SetVolume(float, bool);
void createLevel();
void Randomize(bool);
bool RotateRandom(int nr, bool dirr,float dtime);


void LoadConfig()
{
	IrrXMLReader* xml = createIrrXMLReader("config.xml");
	if (xml != 0){
		while(xml->read())
		{
			if(xml->getNodeType() == EXN_ELEMENT){
				width = xml->getAttributeValueAsInt("x");
				height = xml->getAttributeValueAsInt("y");
				vsync = xml->getAttributeValueAsInt("vsync");
				//fullscr = xml->getAttributeValueAsInt("fullscr");
				bits = xml->getAttributeValueAsInt("bits");
				antialiasing = xml->getAttributeValueAsInt("antialiasing");
				music_volume = xml->getAttributeValueAsFloat("music_volume");
				sound_volume = xml->getAttributeValueAsFloat("fx_volume");
			}
		}
		delete xml;
	}
}

void SaveConfig()
{
	ofstream file("config.xml");
	file<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Resolution";
	file<<" x=\""<<width<<"\" y=\""<<height<<"\" fullscr=\""<<fullscr<<"\" vsync=\""<<vsync
		<<"\" bits=\""<<bits<<"\" antialiasing=\""<<antialiasing<<"\" music_volume=\""<<music_volume
		<<"\" fx_volume=\""<<sound_volume<<"\">";
	file.close();
}

struct framestr{
    IMeshSceneNode* p;//Animated
    int rotating;//1-uzverstas, 2-atverstas, 3-verciamas
    bool dir;
};
std::vector<framestr>frame;
struct cubestr{
    IMeshSceneNode *p;
    int rotating;//1-uzverstas, 2-atverstas, 3-verciamas,4-jau surastas
    int img;
    int dir;
};
std::vector<cubestr>cube;
IMeshSceneNode *p;

bool mouse_enabled;

class Button
{
	public:
	ITexture *tx_norm,
		*tx_over;
	float x,
		y,
		width,
		height;
	int tx_width,
		tx_height;
	bool active;

	Button(int _x, int _y, const io::path &t1, const io::path &t2, float _ratio){
		x = _x * _ratio;
		y = _y * _ratio;
		tx_norm = driver->getTexture(t1);
		tx_over = driver->getTexture(t2);
		tx_width = tx_norm->getSize().Width;
		tx_height = tx_norm->getSize().Height;
		width = tx_width * _ratio;
		height = tx_height * _ratio;
		active = false;
	}
	~Button(){};
	void MouseMove(int _x, int _y){
		bool active_last = active;
		active = _x >= x && _x <= x + width && _y >= y && _y <= y + height;
		if(!active_last && active){
			sound_roll_over.Groti();
		}
	}
	void Draw(){
		driver->draw2DImage(active ? tx_over : tx_norm,
			rect<s32>(x, y, x + width, y + height),
			rect<s32>(0, 0, tx_width, tx_height), 0, 0, true);
	}
};

class Texture
{
	public:
	ITexture *tx;
	float x, y, width, height, tx_width, tx_height;
	float ratio_;

	Texture(float _x, float _y, float _width, float _height, const io::path &t, bool full = true){
		x = _x;
		y = _y;
		tx = driver->getTexture(t);
		tx_width = tx->getSize().Width;
		tx_height = tx->getSize().Height;
		if(tx_width / tx_height > (float)_width / (float)_height){
			if(full){
				this->width = tx_width / tx_height * _height;
				this->height = _height;
				ratio_ = _height / tx_height;
			}
			else{
				this->width = _width;
				this->height = tx_height / tx_width * _width;
				ratio_ = _width / tx_width;
			}
		}
		else{
			if(full){
				this->width = _width;
				this->height = tx_height / tx_width * _width;
				ratio_ = _width / tx_width;
			}
			else{
				this->width = tx_width / tx_height * _height;
				this->height = _height;
				ratio_ = _height / tx_height;
			}
		}
	}
	~Texture(){}
	void Draw(){
		driver->draw2DImage(tx,
			rect<s32>(x, y, x + this->width, y + this->height),
			rect<s32>(0, 0, tx_width, tx_height), 0, 0, true);
	}
};

class ScrollBar
{
	public:
	float x_min, x_max, shift, pos;
	Button *obj;
	Texture *bgr;
	bool active;
	bool drag;
	bool changed;

	ScrollBar(float bgr_x, float bgr_y, float bgr_ratio, float bgr_ratio2, const io::path &bgr_img,
	  const io::path &obj_img, const io::path &obj_img2){
		bgr = new Texture(bgr_x * bgr_ratio, bgr_y * bgr_ratio, 1, 1, bgr_img);
		bgr->width = bgr->tx_width * bgr_ratio * bgr_ratio2;
		bgr->height = bgr->tx_height * bgr_ratio * bgr_ratio2;
		obj = new Button(bgr_x, bgr_y, obj_img, obj_img2, bgr_ratio);
		obj->width *= bgr_ratio2;
		obj->height *= bgr_ratio2;
		x_min = bgr_x * bgr_ratio;
		x_max = x_min + bgr->width - obj->width;
		active = false;
		drag = false;
		changed = false;
	}
	~ScrollBar(){}
	void MouseMove(int _x, int _y, bool _down, bool _up){
		changed = false;
		if(!drag)
			obj->MouseMove(_x, _y);
		active = obj->active;
		if(active){
			if(_down){
				drag = true;
				shift = obj->x - _x;
			}
			if(_up){
				drag = false;
			}
			if(drag){
				pos = _x + shift;
				if(pos < x_min)
					pos = x_min;
				if(pos > x_max)
					pos = x_max;
				obj->x = pos;
				pos = (pos - x_min) / (x_max - x_min);
				changed = true;
			}
		}
	}
	void Set(float val){
		pos = val;
		obj->x = x_min + (x_max - x_min) * val;
	}
	void Draw(){
		bgr->Draw();
		obj->Draw();
	}
};

/*void MouseDraw(){
	driver->draw2DImage(tx_mouse,
		rect<s32>(tx_mouse->x, y, x + width, y + height),
		rect<s32>(0, 0, tx_width, tx_height), 0, 0, true);
}*/

bool menu_created = false;
bool game_created = false;
bool credits = false;
bool game_won;
bool game_lost;
Button *b_start, *b_credits, *b_quit, *b_game_quit;
//Button *mouse;
Texture *tx_mouse, *tx_mouse1, *tx_mouse2, *tx_mouse3;
scene::ISceneNode *menu_3D, *menu_tv[3], *menu_remas, *menu_border1, *menu_border2, *menu_border3;
Texture *menu_bg, *menu_border, *menu_credits,
	*game_bg, *game_hud, *game_timer, *game_win, *game_lose;
ScrollBar *scrl_music, *scrl_sound;
float game_time_max = 300.0;
float game_time;
float menu_anim = 0.0;
int menu_anim_state = 0;
bool menu_anim_active = true;

int show;//kuri kuba reikia versti
int showing;//kiek atverstu kubu
int showid[2];//atversto kubo id
int points;//kiek atverstu poru
bool hide;//kai true reik uzversti 2 atverstus skirtingus kubus
float time1;//kiek laiko praejo nuo kubo atvertimo
bool timeenabled;
bool GameLoaded;
bool swap_;//swap frame
float time2;//swap timer
int swapnr;//kuria detale sukti
float cubeangle;//cube angle
bool cubedir;


void MenuCreate()
{
	if(!menu_created){
		menu_bg = new Texture(0, 0, width, height, "data/gui/MeniuFonas.png");
		menu_border = new Texture(0, 0, 1, height, "data/gui/Meniu.png");
		menu_credits = new Texture(0, 0, 1, height, "data/gui/MeniuContacts.png");
		menu_credits->x = width;
		scrl_music = new ScrollBar(120, 610, menu_border->ratio_, 0.3, "data/gui/sound_bg.png",
			"data/gui/sound_obj.png", "data/gui/sound_obj2.png");
		scrl_music->Set(music_volume);
		scrl_sound = new ScrollBar(120, 655, menu_border->ratio_, 0.3, "data/gui/sound_bg.png",
			"data/gui/sound_obj.png", "data/gui/sound_obj2.png");
		scrl_sound->Set(sound_volume);

		b_start = new Button(31, 171, "data/gui/start.png", "data/gui/start2.png", menu_border->ratio_);
		b_credits = new Button(31, 317, "data/gui/credits.png", "data/gui/credits2.png", menu_border->ratio_);
		b_quit = new Button(31, 463, "data/gui/quit.png", "data/gui/quit2.png", menu_border->ratio_);

	//lankelis 0, 2.516
		IMesh *mesh = smgr->getMesh("data/gui/telikas.obj");
		menu_tv[0] = smgr->addMeshSceneNode(mesh);
		menu_tv[0]->setMaterialFlag(EMF_LIGHTING, false);
		menu_tv[0]->setMaterialTexture( 0, driver->getTexture("data/gui/tv1.jpg") );
		menu_tv[0]->setPosition(vector3df(0.0, 0.0, -1.263));
		mesh = smgr->getMesh("data/gui/telikas.obj");
		menu_tv[1] = smgr->addMeshSceneNode(mesh);
		menu_tv[1]->setMaterialFlag(EMF_LIGHTING, false);
		menu_tv[1]->setMaterialTexture( 0, driver->getTexture("data/gui/tv2.jpg") );
		menu_tv[1]->setPosition(vector3df(-2.755, 0.0, 6.098));
		mesh = smgr->getMesh("data/gui/telikas.obj");
		menu_tv[2] = smgr->addMeshSceneNode(mesh);
		menu_tv[2]->setMaterialFlag(EMF_LIGHTING, false);
		menu_tv[2]->setMaterialTexture( 0, driver->getTexture("data/gui/tv3.jpg") );
		menu_tv[2]->setPosition(vector3df(2.839, 0.0, 8.013));
		mesh = smgr->getMesh("data/gui/remas.obj");
		menu_remas = smgr->addMeshSceneNode(mesh);
		menu_remas->setMaterialFlag(EMF_LIGHTING, false);
		menu_remas->setMaterialTexture( 0, driver->getTexture("data/gui/remas.jpg") );
		mesh = smgr->getMesh("data/gui/rems.obj");
		menu_border1 = smgr->addMeshSceneNode(mesh);
		menu_border1->setMaterialFlag(EMF_LIGHTING, false);
		menu_border1->setMaterialTexture( 0, driver->getTexture("data/gui/rems.jpg") );
		menu_border1->setPosition(vector3df(0.0, 0.0, -2.516));
		mesh = smgr->getMesh("data/gui/rems.obj");
		menu_border2 = smgr->addMeshSceneNode(mesh);
		menu_border2->setMaterialFlag(EMF_LIGHTING, false);
		menu_border2->setMaterialTexture( 0, driver->getTexture("data/gui/rems.jpg") );
		menu_border2->setPosition(vector3df(2.839, 0.0, 6.749));
		menu_border2->setRotation(vector3df(0.0, 0.0, 180.0));
		mesh = smgr->getMesh("data/gui/rems.obj");
		menu_border3 = smgr->addMeshSceneNode(mesh);
		menu_border3->setMaterialFlag(EMF_LIGHTING, false);
		menu_border3->setMaterialTexture( 0, driver->getTexture("data/gui/rems.jpg") );
		menu_border3->setPosition(vector3df(-2.755, 0.0, 4.833));
		menu_3D = smgr->addEmptySceneNode();
		menu_3D->addChild(menu_tv[0]);
		menu_3D->addChild(menu_tv[1]);
		menu_3D->addChild(menu_tv[2]);
		menu_3D->addChild(menu_remas);
		menu_3D->addChild(menu_border1);
		menu_3D->addChild(menu_border2);
		menu_3D->addChild(menu_border3);
		menu_3D->setPosition(vector3df(3, -16, 12));
		//menu_3D->setScale(core::vector3df(10.0, 10.0, 10.0));
	}
	else{
		menu_3D->setVisible(true);
		//menu_tv[0]->setVisible(true);
		//menu_remas->setVisible(true);
		menu_3D->setRotation(vector3df(0.0, 0.0, 0.0));
		menu_tv[0]->setRotation(vector3df(0.0, 0.0, 0.0));
		menu_tv[1]->setRotation(vector3df(0.0, 0.0, 0.0));
		menu_tv[2]->setRotation(vector3df(0.0, 0.0, 0.0));
		menu_anim = 0.0;
	}
}

void MenuClear()
{
	menu_3D->setVisible(false);
	credits = false;
	//mouse->active = false;
	tx_mouse = tx_mouse1;
	//menu_tv[0]->setVisible(false);
	//menu_remas->setVisible(false);
}

void MenuMouse(int _x, int _y, bool _down, bool _up)
{
	b_start->MouseMove(_x, _y);
	b_credits->MouseMove(_x, _y);
	b_quit->MouseMove(_x, _y);
	scrl_music->MouseMove(_x, _y, _down, _up);
	scrl_sound->MouseMove(_x, _y, _down, _up);
	//mouse->active = b_start->active || b_credits->active || b_quit->active || scrl_music->active || scrl_sound->active;
	bool active = b_start->active || b_credits->active || b_quit->active || scrl_music->active || scrl_sound->active;
	tx_mouse = active ? tx_mouse2 : tx_mouse1;
	if(_down){
		if(b_start->active){
			sound_click.Groti();
			MenuClear();
			mode = MODE_GAME;
			GameCreate();
		}
		else if(b_credits->active){
			sound_click.Groti();
			credits = !credits;
		}
		else if(b_quit->active)
			quit = true;
	}
	if(scrl_music->changed){
		SetVolume(scrl_music->pos, true);
		music_volume = scrl_music->pos;
	}
	if(scrl_sound->changed){
		SetVolume(scrl_sound->pos, false);
		sound_volume = scrl_sound->pos;
	}
}

void Menu(float delta, bool pre)
{
	if(pre){
		if(delta > 0.1)
			delta = 0.1;
		float credits_x = credits ? (width - menu_credits->width) : (width - 500.0 * menu_credits->ratio_);
		float credits_posl = credits_x - menu_credits->x;
		menu_credits->x += credits_posl * delta * 2.0;
		menu_bg->Draw();
		//
		menu_anim += delta;
		if(menu_anim > 5.0){
			menu_anim_active = true;
			menu_anim = 0.0;
			++menu_anim_state;
			if(menu_anim_state >= 6)
				menu_anim_state = 0;
		}
		if(menu_anim < 2.0){
			if(menu_anim_state % 2 == 0){
				int num = (int)(menu_anim_state / 2.0);
				menu_tv[num]->setRotation(vector3df(0.0, 0.0, menu_tv[num]->getRotation().Z + delta * 90.0));
			}
			else{
				menu_3D->setRotation(vector3df(0.0, 0.0, menu_3D->getRotation().Z + delta * 90.0));
				menu_tv[0]->setRotation(vector3df(0.0, 0.0, menu_tv[0]->getRotation().Z - delta * 90.0));
				menu_tv[1]->setRotation(vector3df(0.0, 0.0, menu_tv[1]->getRotation().Z - delta * 90.0));
				menu_tv[2]->setRotation(vector3df(0.0, 0.0, menu_tv[2]->getRotation().Z - delta * 90.0));
			}
		}
		else{
			if(menu_anim_active){
				if(menu_anim_state % 2 == 0){
					int num = (int)(menu_anim_state / 2.0);
					if(menu_tv[num]->getRotation().Z > 270.0)
						menu_tv[num]->setRotation(vector3df(0.0, 0.0, 0.0));
					else if(menu_tv[num]->getRotation().Z > 90.0)
						menu_tv[num]->setRotation(vector3df(0.0, 0.0, 180.0));
					else if(menu_tv[num]->getRotation().Z < -270.0)
						menu_tv[num]->setRotation(vector3df(0.0, 0.0, 0.0));
					else if(menu_tv[num]->getRotation().Z < -90.0)
						menu_tv[num]->setRotation(vector3df(0.0, 0.0, -180.0));

					if(menu_3D->getRotation().Z > 270.0)
						menu_3D->setRotation(vector3df(0.0, 0.0, 0.0));
					else if(menu_3D->getRotation().Z > 90.0)
						menu_3D->setRotation(vector3df(0.0, 0.0, 180.0));
				}
				menu_anim_active = false;
			}
		}
	}
	else{
		menu_border->Draw();
		menu_credits->Draw();
		b_start->Draw();
		b_credits->Draw();
		b_quit->Draw();
		scrl_music->Draw();
		scrl_sound->Draw();
	}
}

void GameCreate()
{
	if(!game_created){
		game_created = true;
		createLevel();

		game_bg = new Texture(0, 0, width, height, "data/gui/MeniuZaidimeFonas.png");
		game_hud = new Texture(0, 0, 1, 1, "data/gui/MeniuZaidimeHUD.png");
		game_hud->width = width;
		game_hud->height = game_hud->tx_height / game_hud->tx_width * game_hud->width;
		game_hud->ratio_ = game_hud->width / game_hud->tx_width;
		game_hud->y = height - game_hud->tx_height * game_hud->ratio_;
		b_game_quit = new Button(48, 59 + (height - game_hud->height) / game_hud->ratio_, "data/gui/game_quit.png", "data/gui/game_quit2.png", game_hud->ratio_);
		game_timer = new Texture(0, 0, 1, 1, "data/gui/MeniuZaidimeTimeBar.png");
		game_timer->x = 319 * game_hud->ratio_;
		game_timer->y = 91 * game_hud->ratio_ + height - game_hud->height;
		game_timer->width = 488 * game_hud->ratio_;
		game_timer->height = 25 * game_hud->ratio_;
		game_win = new Texture(0, 0, width, height, "data/gui/game_win.png", false);
		game_win->x = (width - game_win->width) / 2.0;
		game_win->y = (height - game_win->height) / 2.0;
		game_lose = new Texture(0, 0, width, height, "data/gui/game_lose.png", false);
		game_lose->x = (width - game_lose->width) / 2.0;
		game_lose->y = (height - game_lose->height) / 2.0;
		music.Groti();
	}
	else{
		Randomize(false);
	}
	frame[0].p->setVisible(true);
	p->setVisible(true);
	game_time = game_time_max;
	game_won = false;
	game_lost = false;
}

void GameClear()
{
	frame[0].p->setVisible(false);
	p->setVisible(false);
	tx_mouse = tx_mouse1;
}

void GameMouse(int x, int y, bool _down)
{
	b_game_quit->MouseMove(x, y);

	if(_down){
		if(b_game_quit->active){
			music_lose.Stabd();
			music_win.Stabd();
			music.Groti();
			sound_click.Groti();
			GameClear();
			mode = MODE_MENU;
			MenuCreate();
		}
	}

	if(!_down)
		return;

	ISceneNode* selectedNode;
    int nodeid;
    if (mouse_enabled){
        selectedNode = smgr->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(position2d<s32>(x,y),512);//getSceneNodeFromCameraBB(cam,512);
        if (selectedNode)
        {
            nodeid=selectedNode->getID();
        }
        printf("id %i\n",nodeid);
        if (nodeid>=512&&nodeid<539&&cube[nodeid-512].rotating!=4){//pazymetas kubelis

            switch (cube[nodeid-512].rotating)
            {
                case 1: {
                    show=nodeid;
                    printf("reik versti\n");
                    printf("show %i\n",show);
                    mouse_enabled=false;
                    vector3df a =cube[nodeid-512].p->getRotation();

                    if(a.Z==0)cubedir=true;
                    else cubedir=false;
                    sound_rotate.Stabd();
                    sound_rotate.Groti();
                }
                break;
               /* case 2: {
                    show=nodeid;
                    printf("reik versti2\n");
                    printf("show %i\n",show);
                    mouse_enabled=false;
                }
                break;*/
            }
        }//if rastas
    }
}

void Game(float dtime, bool pre)
{
	if(pre){
		game_bg->Draw();

    if (show>0&&show!=showid[0] && !game_lost && !game_won){
        mouse_enabled=false;
        vector3df ang1 =cube[show-512].p->getRotation();
        if(ang1.Z<0){ang1.Z=ang1.Z+360;}
      //  printf("vercia %i %f\n",show,ang1.Z);
        //cube[show-512].rotating=3;
       // if(cubedir){
        ang1.Z=ang1.Z+500.0f*dtime;
       // }
        //else ang1.Z=ang1.Z-500.0f*dtime;
        //printf("rotat %i\n",cube[show-512].rotating);
        cube[show-512].p->setRotation(vector3df(0,0,ang1.Z));
        if(ang1.Z>=180&&cube[show-512].dir==1)//pilnai atverte
        {
            cube[show-512].p->setRotation(vector3df(0,0,180));
            cube[show-512].rotating=2;//atverte
            cube[show-512].dir=0;
            mouse_enabled=true;
            //printf("rotat %i\n",);
            switch(showing){//irasyti i atverstu kubu sarasa
                case 0:{
                    showid[0]=show;
                    showing++;
                   // printf("a\n");
                   }
                break;
                case 1:{showid[1]=show;
                    showing++;
                   // printf("b\n");
                   }
                break;
            }
            show=0;
        }
        if(ang1.Z>=360&&cube[show-512].dir==0)//pilnai atverte
        {
            cube[show-512].p->setRotation(vector3df(0,0,0));
            cube[show-512].rotating=2;//atverte
            cube[show-512].dir=1;
            mouse_enabled=true;
            //printf("rotat %i\n",);
            switch(showing){//irasyti i atverstu kubu sarasa
                case 0:{
                    showid[0]=show;
                    showing++;
                    //printf("a\n");
                    }
                break;
                case 1:{showid[1]=show;
                    showing++;
               //     printf("b\n");
               }
                break;
            }
            show=0;
        }

        //printf("rodo %i\n",showing);
    if(showing==2 && !game_lost){//jei 2 atversti kubai
        if(cube[showid[0]-512].img==cube[showid[1]-512].img)//rasti vienodi kubai
        {points++;
			//printf("rasti vienodi %i %i\n",showid[0],showid[1]);
			if (points==13){//GameOver(false);
				music.Pauze();
				music_win.Groti();
				game_won = true;
				mouse_enabled = false;
			}
			cube[showid[0]-512].rotating=4;
			cube[showid[1]-512].rotating=4;
			sound_true.Groti();
        }
        else{
            mouse_enabled=false;
            hide=true;
            sound_false.Groti();
        }
		showing=0;
    }
    }
    if(hide && !game_lost && !game_won){
        if(time1==0){timeenabled=true;}
        if(timeenabled){time1=time1+1.1f*dtime;}
        if(time1>0.4){
            mouse_enabled=false;
            vector3df ang3=cube[showid[0]-512].p->getRotation();
            vector3df ang4=cube[showid[1]-512].p->getRotation();
            cube[showid[0]-512].rotating=3;
            cube[showid[1]-512].rotating=3;
            ang3.Z=ang3.Z+500.0f*dtime;
            ang4.Z=ang4.Z+500.0f*dtime;
            cube[showid[0]-512].p->setRotation(vector3df(0,0,ang3.Z));
            cube[showid[1]-512].p->setRotation(vector3df(0,0,ang4.Z));
            printf("ang %f %f\n",ang3.Z,ang4.Z);
            if(ang3.Z>=180&&cube[showid[0]-512].dir==1){
                cube[showid[0]-512].dir=0;
                timeenabled=false;
                cube[showid[0]-512].p->setRotation(vector3df(0,0,180));
                //cube[showid[0]-512].p->setRotation(vector3df(0,0,0));
                hide=false;
                cube[showid[0]-512].rotating=1;
                //cube[showid[1]-512].rotating=1;
                mouse_enabled=true;
                time1=0.0;
                showid[0]=0;
                //showid[1]=0;
                }
            if(ang3.Z>=360&&cube[showid[0]-512].dir==0){
                cube[showid[0]-512].dir=1;
                timeenabled=false;
                cube[showid[0]-512].p->setRotation(vector3df(0,0,0));
                //cube[showid[0]-512].p->setRotation(vector3df(0,0,0));
                hide=false;
                cube[showid[0]-512].rotating=1;
                //cube[showid[1]-512].rotating=1;
                mouse_enabled=true;
                time1=0.0;
                showid[0]=0;
                //showid[1]=0;
                }
            if(ang4.Z>=180&&cube[showid[1]-512].dir==1){
                cube[showid[1]-512].dir=0;
                timeenabled=false;
                cube[showid[1]-512].p->setRotation(vector3df(0,0,180));
                //cube[showid[0]-512].p->setRotation(vector3df(0,0,0));
                hide=false;
                cube[showid[1]-512].rotating=1;
                //cube[showid[1]-512].rotating=1;
                mouse_enabled=true;
                time1=0.0;
                showid[1]=0;
                //showid[1]=0;
                }
            if(ang4.Z>=360&&cube[showid[1]-512].dir==0){
                cube[showid[1]-512].dir=1;
                timeenabled=false;
                cube[showid[1]-512].p->setRotation(vector3df(0,0,0));
                //cube[showid[0]-512].p->setRotation(vector3df(0,0,0));
                hide=false;
                cube[showid[1]-512].rotating=1;
                //cube[showid[1]-512].rotating=1;
                mouse_enabled=true;
                time1=0.0;
                showid[1]=0;
                //showid[1]=0;
                }
            }
		}
		/*time2=time2+1.0f*dtime;
		if(time2>4&&mouse_enabled){
			swap_=true;
			time2=0;
			cubeangle=0;
			mouse_enabled=false;
			srand(time(NULL));
			swapnr=(rand()%10);}//time to swap frame
			swapnr=9;
		//printf("swap %i t %f %f\n",swap,time2,dtime);
		if (swap_){

			int t=frame[swapnr].rotating;//p->getRotation();
			//printf("rot %i\n",t);
			bool dir;
			if (t==1)dir=true;
			else dir=false;
			swap_=!RotateRandom(swapnr, dir,dtime);
			if(!swap_){mouse_enabled=true;}
		}*/


			//vector3df ang2=frame[0].p->getRotation();
		//ang2.Z=ang2.Z+30.0f*dtime;
		//frame[0].p->setRotation(vector3df(0,0,ang2.Z));
	}
	else{
		tx_mouse = tx_mouse1;
		if(b_game_quit->active)
			tx_mouse = tx_mouse2;
		else if(!mouse_enabled)
			tx_mouse = tx_mouse3;

		if(dtime > 0.1)
			dtime = 0.1;
		if(!game_won)
			game_time -= dtime;
		if(game_time < 0.0){
			game_time = 0.0;
			mouse_enabled = false;
			game_lost = true;
			music.Pauze();
			music_lose.Groti();
		}
		game_hud->Draw();
		game_timer->Draw();
		game_timer->x = 319 * game_hud->ratio_ + 488 * game_hud->ratio_ * (game_time_max - game_time) / game_time_max;
		game_timer->width = 488 * game_hud->ratio_ * game_time / game_time_max;
		b_game_quit->Draw();
		if(game_won)
			game_win->Draw();
		if(game_lost)
			game_lose->Draw();
	}
}

void SetVolume(float val, bool music_)
{
	if(music_){
		music.Garsumas(val);
		music_win.Garsumas(val);
		music_lose.Garsumas(val);
	}
	else{
		sound_roll_over.Garsumas(val);
		sound_click.Garsumas(val);
		sound_rotate.Garsumas(val);
		sound_true.Garsumas(val);
		sound_false.Garsumas(val);
	}
}


class MyEventReceiver : public IEventReceiver
{
public:
	struct SMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		SMouseState() : LeftButtonDown(false) { }
	} MouseState;

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
				case EMIE_LMOUSE_PRESSED_DOWN:
					MouseState.LeftButtonDown = true;
					break;

				case EMIE_LMOUSE_LEFT_UP:
					MouseState.LeftButtonDown = false;
					break;

				case EMIE_MOUSE_MOVED:
					MouseState.Position.X = event.MouseInput.X;
					MouseState.Position.Y = event.MouseInput.Y;
					//mouse->x = MouseState.Position.X;
					//mouse->y = MouseState.Position.Y;
					tx_mouse->x = MouseState.Position.X;
					tx_mouse->y = MouseState.Position.Y;
					break;

				default:
					break;
			}

			switch(mode){
				case MODE_MENU:
					MenuMouse(MouseState.Position.X, MouseState.Position.Y,
						event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN,
						event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP);
					break;
				case MODE_GAME:
					GameMouse(MouseState.Position.X, MouseState.Position.Y,
						event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN);
					break;
			}
		}

		return false;
	}

	const SMouseState & GetMouseState(void) const
	{
		return MouseState;
	}

	MyEventReceiver()
	{
	}
} receiver;


void Randomize(bool ini){
	int n,n2,tm;
    std::vector<int>randc;//random cube
    char buf [16];

	for (int i=0;i<13;i++){
		randc.push_back(i);
		randc.push_back(i);
	}

    srand(time(NULL));
    for(int r=0;r<50;r++){
        n=rand() %26;
        n2=rand() %26;
        tm=randc[n];
        randc[n]=randc[n2];
        randc[n2]=tm;
    }
    points=0;
    hide=false;
    time1=0;
    for (int s=0;s<26;s++){
    sprintf(buf, "data/%d.jpg", randc[s]+1);
    cube[s].p->setMaterialTexture(0,driver->getTexture(buf));
    cube[s].rotating=1;
    cube[s].img=randc[s];
    }
    for(int i = 0; i < 26; i++){
		cube[i].p->setRotation(vector3df(0,0,0));
		cube[i].dir = true;
		cube[i].rotating = 1;
    }
    showid[0] = -1;
    showid[1] = -1;
    showing = 0;
    mouse_enabled = true;
}

void createLevel(){
    //28 figuros
    framestr fr;
    cubestr cub;
    IAnimatedMesh* mdl = 0;
    IAnimatedMesh* cb = 0;
    cb = smgr->getMesh("data/tv.obj");
    //frame parts
if (!mdl)
    {
       printf("bad");
    }
    //tx=driver->getTexture("telikas.jpg");
    //matc->setTexture(0,tx);
    //centrinis remas
    mdl = smgr->getMesh("data/1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,smgr->getRootSceneNode(),1,vector3df(0, 0, 0),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/8.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[0].p,2,vector3df(-12.8, 0, 8.3),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f8.jpg"));
    fr.p->setParent(frame[0].p);
    frame.push_back(fr);
    mdl = smgr->getMesh("data/6.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[0].p,3,vector3df(0, 0, 6.8),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f6.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[0].p,4,vector3df(15, 0, 8.1),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[3].p,5,vector3df(0, 0, 0.027),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f2.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/3.obj");//4.42
    fr.p=smgr->addMeshSceneNode(mdl,frame[4].p,6,vector3df(1.8, 0, 1.4),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f3.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/5.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[4].p,7,vector3df(-6.8, 0, 1.4),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f5.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/4.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[5].p,8,vector3df(-2.717, 0, 2.448),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f4.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/7.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[1].p,9,vector3df(7.434, 0, 1.13),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f7.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/9.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[1].p,10,vector3df(-2.8, 0, 1.13),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false); fr.rotating=1;
    fr.p->setMaterialTexture(0,driver->getTexture("data/f9.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[0].p,11,vector3df(3.387, 0, 4.548),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[0].p,12,vector3df(-3.387, 0, 4.548),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[4].p,13,vector3df(8.45, 0, 3.264),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[7].p,14,vector3df(0.009, 0, 3.906),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");//1.938
    fr.p=smgr->addMeshSceneNode(mdl,frame[2].p,15,vector3df(0.0, 0,1.938),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[2].p,16,vector3df(0.0, 0,9.292),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[8].p,17,vector3df(2.875, 0,5.015),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[1].p,18,vector3df(2.695, 0,3.35),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame2.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[1].p,19,vector3df(-7.71, 0,3.373),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    //sitas keisis
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[4].p,20,vector3df(8.45, 0, 7.185),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[5].p,21,vector3df(2.93, 0, 4.725),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[7].p,22,vector3df(2.78, 0, 12.485),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[7].p,23,vector3df(-2.928, 0, 12.485),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[6].p,24,vector3df(2.851, 0, 6.741),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[6].p,25,vector3df(-2.777, 0, 4.765),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[2].p,26,vector3df(4.498, 0, 19.861),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[2].p,27,vector3df(1.658, 0, 17.986),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[2].p,28,vector3df(-1.407, 0, 17.986),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[2].p,29,vector3df(-4.405, 0, 19.861),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[8].p,30,vector3df(2.875, 0, 8.869),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[8].p,31,vector3df(-2.827, 0, 5.043),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[1].p,32,vector3df(2.683, 0, 9.008),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[1].p,33,vector3df(-7.715, 0, 11.01),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[9].p,34,vector3df(2.899, 0, 5.116),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/frame1.obj");
    fr.p=smgr->addMeshSceneNode(mdl,frame[9].p,35,vector3df(-2.816, 0, 5.116),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    fr.p->setMaterialFlag(video::EMF_LIGHTING, false);
    fr.p->setMaterialTexture(0,driver->getTexture("data/f1.jpg"));
    frame.push_back(fr);
    mdl = smgr->getMesh("data/top.obj");
    p=smgr->addMeshSceneNode(mdl,0,50,vector3df(0, 0, -0.5),vector3df(-30, 0,0),vector3df(1.0f, 1.0f, 1.0f),false);
    p->setMaterialFlag(video::EMF_LIGHTING, false);
    p->setMaterialTexture(0,driver->getTexture("data/top.jpg"));
    for (int s=0;s<25;s++){
        //sprintf(buf, "data/%d.jpg", randc[s]+1);
        cub.p=smgr->addMeshSceneNode(cb,frame[10+s].p,512+s,vector3df(0.004, 0, 1.35),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
        cub.p->setMaterialFlag(video::EMF_LIGHTING, false);
        //cub.p->setMaterialTexture(0,driver->getTexture(buf));
        cub.rotating=1;  cub.dir=1;cube.push_back(cub);
    }
    //sprintf(buf, "data/%d.jpg", randc[25]+1);
    cub.p=smgr->addMeshSceneNode(cb,frame[3].p,537,vector3df(0.004, 0, 1.35),vector3df(0, 0, 0),vector3df(1.0f, 1.0f, 1.0f),false);
    cub.p->setMaterialFlag(video::EMF_LIGHTING, false);
    //cub.p->setMaterialTexture(0,driver->getTexture(buf));
    cub.rotating=1; cub.dir=1; cube.push_back(cub);
    frame[9].dir=1;
    Randomize(true);
mouse_enabled=true;
}

bool RotateRandom(int nr, bool dirr,float dtime){//1-10 parts
    bool finished=false;
    time2=0;
    vector3df angl =frame[nr].p->getRotation();
    //
    //frame[nr].rotating=3;
    //printf("vercia rema %i %f\n",dir,angl.Z);
   // if(dir) {
        angl.Z=angl.Z+100.0f*dtime;
        if(angl.Z>=180&&frame[nr].dir==1){angl.Z=180; finished=true; frame[nr].dir=0;frame[nr].rotating=2;}//2
        if(angl.Z>=360&&frame[nr].dir==0){angl.Z=0; finished=true; frame[nr].dir=1; frame[nr].rotating=2;}
        //if(angl.Z<=0){angl.Z=0; finished=true; frame[nr].rotating=2;}
   // }
   // else {angl.Z=angl.Z-100.0f*dtime;
    //    if(angl.Z<=0){angl.Z=0; finished=true; frame[nr].rotating=1;}//1
   // }
    frame[nr].p->setRotation(vector3df(0,0,angl.Z));
//printf("vercia rema po %i %f\n",dir,angl.Z);
cubeangle=cubeangle+100.0f*dtime;
    switch(nr){
        case 9:{
//24.23
            //if(cube[24].rotating==1){
            //if(angl.Z==0||angl.Z==180){if(cube[24].rotating==1)cube[24].rotating=2;
            //else cube[24].rotating=1;}

            //if(cube[24].dir==180)

            if(cube[24].dir==1){cube[24].p->setRotation(vector3df(0,0,-angl.Z));// cube[24].dir=0;
            }
            if(cube[24].dir==0){cube[24].p->setRotation(vector3df(0,0,180-angl.Z)); //cube[24].dir=1;
            }
            //cube[20].p->setRotation(vector3df(0,0,180));
            //else cube[24].p->setRotation(vector3df(0,0,angl.Z));
            vector3df v=cube[24].p->getRotation();
            printf("rot %f %f dir %i\n",angl.Z,v.Z,cube[24].dir);
            //if(angl.Z==180){cube[24].rotating=2; cube[24].p->setRotation(vector3df(0,0,180));printf("a\n");}
            //if(angl.Z==0){cube[24].rotating=1;printf("b\n");}
                //if(cubeangle>180){ cube[24].p->setRotation(vector3df(0,0,180)); cube[24].rotating==2;}
                //else cube[24].p->setRotation(vector3df(0,0,180-cubeangle));
            //}
           // else {
            //    if(cubeangle<0){ cube[24].p->setRotation(vector3df(0,0,0)); cube[24].rotating==1;}
            //    else cube[24].p->setRotation(vector3df(0,0,cubeangle));}

        }
        break;
        case 2:{
        }
        break;
    }
    return finished;
}




int main()
{
	srand(time(NULL));
	//sounds.Sukurti();
	sounds.Uzkrauti("data/sound/");
	music.Sukurti("data/music/Pirmos grupes muzika.ogg", &sound_position, true);
	music_win.Sukurti(sounds.ID("win.wav"), &sound_position, true);
	music_lose.Sukurti(sounds.ID("lose.wav"), &sound_position, true);
	sound_roll_over.Sukurti(sounds.ID("roll_over.wav"), &sound_position, false);
	sound_click.Sukurti(sounds.ID("click.wav"), &sound_position, false);
	sound_rotate.Sukurti(sounds.ID("pasukimas.wav"), &sound_position, false);
	sound_true.Sukurti(sounds.ID("teisingas.wav"), &sound_position, false);
	sound_false.Sukurti(sounds.ID("neteisingas.wav"), &sound_position, false);

	LoadConfig();
	ratio = height / 600.0;

	SetVolume(music_volume, true);
	SetVolume(sound_volume, false);

	SIrrlichtCreationParameters device_params;
	device_params.AntiAlias = antialiasing;
	device_params.Bits = bits;
	device_params.DriverType = video::EDT_OPENGL;
	device_params.EventReceiver = &receiver;
	device_params.Fullscreen = fullscr;
	device_params.Vsync = vsync;
	device_params.WindowSize = core::dimension2d<u32>(width, height);
	IrrlichtDevice* device = createDeviceEx(device_params);

	if (device == 0)
		return 1;

	device->setWindowCaption(L"");
	device->getCursorControl()->setVisible(false);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	MenuCreate();

	//mouse = new Button(0, 0, "data/gui/cursor normal.png", "data/gui/mouse over things.png", ratio);
	tx_mouse1 = new Texture(0, 0, 24, 32, "data/gui/cursor normal.png");
	tx_mouse2 = new Texture(0, 0, 24, 32, "data/gui/mouse over things.png");
	tx_mouse3 = new Texture(0, 0, 24, 32, "data/gui/cursor inactive.png");
	tx_mouse = tx_mouse1;
	scene::ICameraSceneNode * camera = smgr->addCameraSceneNode(0, vector3df(0,-26.5,15), vector3df(0,0,15));
	camera->setUpVector(vector3df(0, 0, -1));

	u32 then = device->getTimer()->getTime();

	music.Groti();
	while(!quit)
	{
		if(!device->run())
			break;
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f;
		then = now;

		/*wchar_t tmp[1024];
		int fps = driver->getFPS();
		vector3df cm;
        cm=camera->getPosition();
		swprintf(tmp, L"MEMORY [fps:%d][taskai:%i][kamera %3.2f %3.2f %3.2f][pele:%i]",
		fps,points,cm.X,cm.Y,cm.Z,mouse_enabled);
		device->setWindowCaption(tmp);*/

		driver->beginScene(true, true, video::SColor(255,113,113,133));
		switch(mode){
			case MODE_MENU:
				Menu(frameDeltaTime, true);
				break;
			case MODE_GAME:
				Game(frameDeltaTime, true);
				break;
		}
		smgr->drawAll();
		switch(mode){
			case MODE_MENU:
				Menu(frameDeltaTime, false);
				break;
			case MODE_GAME:
				Game(frameDeltaTime, false);
				break;
		}
		//MouseDraw();
		tx_mouse->x = receiver.GetMouseState().Position.X;
		tx_mouse->y = receiver.GetMouseState().Position.Y;
		tx_mouse->Draw();
		driver->endScene();

		music.Atnaujinti();
		music_lose.Atnaujinti();
		music_win.Atnaujinti();
		sound_roll_over.Atnaujinti();
		sound_click.Atnaujinti();
		sound_rotate.Atnaujinti();
		sound_true.Atnaujinti();
		sound_false.Atnaujinti();
	}

	device->getCursorControl()->setVisible(true);
	device->drop();
	sounds.Naikinti();
	SaveConfig();

	return 0;
}
