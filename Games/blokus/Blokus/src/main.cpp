
#include "common.h"
#include "draw.h"
#include "gui.h"
#include "system.h"
#include "blokus.h"
#include "sound.h"
//#include "html.h"
#include <algorithm>    // std::rotate

using namespace std;
//�O���[�o���ϐ��p��
IrrlichtDevice *device;
IVideoDriver   *driver;
ISceneManager  *smgr;
UserGUI        *userGui;
//CSound         *sound;
//TagTree tagTree;

s32 SCENE = 0;
s32 GAMECNT = 0;
s32 STATE = 0;

position2d<s32> mouse;

void playWave(s32 num)
{
	c8 waveData[9][64] =
	{
		"",
		"data/sound/get.wav",
		"data/sound/choice.wav",
		"data/sound/error.wav",
		"data/sound/rotate.wav",
		"data/sound/ok.wav",
		"data/sound/test.wav",
		"data/sound/start.wav",
		"data/sound/complete.wav",
	};

	//�Q�[���V�[���ȊO�̓X���[
	if(num == 4 && SCENE/100 != 1)
		return;

	if(num < 0 || num > 8)
		return;

//	sound->play(waveData[num],false);
}

void playBgm(s32 num)
{
	c8 bgmData[5][64] =
	{
		"",
		"data/sound/be01.wav",
		"data/sound/choice.wav",
		"data/sound/whistle.mp3",
		"data/sound/loop3.mp3",
	};

	if(num < 0 || num > 4)
		return;

//	sound->play(bgmData[num],true);
}

void stopBgm()
{
//	sound->stop(true);
}

//���V�[�o�[�ݒ�
class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		//GUI�C�x���g����
		userGui->OnEvent(event);

		//�L�[�C�x���g����
		if(event.EventType == EET_KEY_INPUT_EVENT)
		{
			//���[�U�[GUI���D�悩�ǂ���
			if(userGui->hasFocus())
				return false;

			if(event.KeyInput.PressedDown)
			{
				switch(event.KeyInput.Key)
				{
					case KEY_KEY_A:
						if(STATE == 0)
							STATE = 2;
						break;
					case KEY_KEY_S:
						if(STATE == 0)
							STATE = 3;
						break;
					case KEY_KEY_D:
						if(STATE == 0)
							STATE = -2;
						break;
					case KEY_ESCAPE:
						//�f�o�C�X���I�������܂�
						//device->closeDevice();
						userGui->message("�Q�[�����I�����܂��B\n\n��낵���ł����H", "�I���m�F", M_YESNO, UID_EXIT);
						return true;
					default:
						return false;
				}
			}
			return true;
		}

		//�}�E�X�C�x���g����
		if(event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			s32 ret = 0;

			//���[�U�[GUI���D�悩�ǂ���
			if(userGui->hasFocus())
				return false;

			switch(event.MouseInput.Event)
			{
				case EMIE_LMOUSE_PRESSED_DOWN:
					if(SCENE/100 == 1)
					{
						ret = game_click(mouse.X, mouse.Y, BlockNo, TURN);
						playWave(ret);
					}
					if(SCENE == 300)
					{
						if(STATE == 0)
							STATE = 1;
					}
					return true;
				case EMIE_LMOUSE_LEFT_UP:
					return true;
				case EMIE_RMOUSE_PRESSED_DOWN:
					if(STATE == 0)
						STATE = 3;
					playWave(4);
					return true;
				case EMIE_MOUSE_WHEEL:
					//���]
					if(event.MouseInput.Wheel == 1)
					{
						if(STATE == 0)
							STATE = 2;
					}
					//����]
					else if(event.MouseInput.Wheel == -1)
					{
						if(STATE == 0)
							STATE = -2;
					}
					playWave(4);
					return true;
				case EMIE_MOUSE_MOVED:
					mouse.X = event.MouseInput.X;
					mouse.Y = event.MouseInput.Y;
					return true;
				default:
					return true;
			}
		}

		//���[�U�[�C�x���g����
		if(event.EventType == EET_USER_EVENT)
		{
			//�C�x���g���N������ID
			s32 id = event.UserEvent.UserData2;
			c8 name[6][8] = {"","BLUE","GREEN","RED","YELLOW",""};
			c8 mess[64];
			s32 ret = 1;

			switch(event.UserEvent.UserData1)
			{
				//�{�^���������ꂽ���̏���
				case EUMT_BUTTON_CLICKED:
					switch(id)
					{
						case UID_LOGOUT:
							userGui->message("�Q�[�����I�����܂��B\n\n��낵���ł����H", "�I���m�F", M_YESNO, UID_EXIT);
							break;
						case UID_MENU:
							if(SCENE/100 == 1)
								playBgm(1);//3
							SCENE = 0;
							break;
						case UID_CHK_MENU:
							userGui->message("�Q�[���𒆒f����\n�^�C�g����ʂ֖߂�܂��B\n\n��낵���ł����H", "���f�m�F", M_YESNO, UID_BACK_MENU);
							break;

						case UID_LOGIN1:
							stopBgm();
							ret = 7;
							game_init();
							SCENE = 101;
							break;
						case UID_LOGIN2:
							stopBgm();
							ret = 7;
							game_init();
							SCENE = 102;
							break;
						case UID_GAME:
							stopBgm();
							SCENE = 103;
							break;
						case UID_START:
							stopBgm();
							playBgm(1);
							SCENE = 110;
							break;
						case UID_WIN:
							SCENE = 120;
							break;
						case UID_LOSE:
							SCENE = 130;
							break;
						case UID_CHK_GIVEUP:
							sprintf(mess, "�v���C���[:%s �M�u�A�b�v���܂��B\n\n��낵���ł����H", name[TURN]);
							userGui->message(mess, "�M�u�A�b�v�m�F", M_YESNO, UID_GIVEUP);
							break;

						case UID_RULE:
							SCENE = 200;
							break;
						case UID_HOWTO:
							SCENE = 300;
							break;


						//���b�Z�[�W�{�b�N�X
						case UID_GIVEUP:
							game_giveup(TURN);
							userGui->messRemove();
							break;
						case UID_BACK_MENU:
							stopBgm();
							playBgm(1);//3
							userGui->messRemove();
							SCENE = 0;
							break;
						case UID_CANCEL:
						case UID_NO:
							ret = 3;
							userGui->messRemove();
							break;
						case UID_OK:
							userGui->messRemove();
							break;
						case UID_YES:
						case UID_EXIT:
							//�f�o�C�X���I�������܂�
							device->closeDevice();
							break;
						default:
							SCENE = id;
							break;
					}
					playWave(ret);
					return true;
					break;
				default:
					return true;
			}
		}
		return false;
	}
};

int setRotate(s32 mouseState, vector3df *rot, int num)
{
	static s32 count = 0;
	s32 value  = 30;
	s32 cnt90  = 90/value;
	s32 cnt180 = 180/value;

	switch(mouseState)
	{
		case 1://�E�N���b�N�F�z�u
			count++;
			if(count == cnt90)
			{
				count = 0;
				mouseState = 0;
			}
			break;
		case 2://�z�C�[�����]�F�E��]����
		case -2://�z�C�[������]�F����]����
			if(mouseState == 2)
				rot->Z += value;
			else
				rot->Z -= value;

			count++;
			if(count == cnt90)
			{
				if(mouseState == 2)
					rotate(2,num);	//�E��]
				else
					rotate(3,num);	//����]
				count = 0;
				mouseState = 0;
			}
			break;
		case 3://�E�N���b�N�F���]����
			switch((int)(rot->Z))
			{
				case 0:
				case 180:
				case -180:
					if(rot->Z == 0)
						rot->Y += value;
					else
						rot->Y -= value;
					break;
				case 90:
				case -270:
					if(rot->Y == 0)
						rot->X += value;
					else
						rot->X -= value;
					break;
				case 270:
				case -90:
					if(rot->Y == 0)
						rot->X -= value;
					else
						rot->X += value;
					break;
			}

			count++;
			if(count == cnt180)
			{
				rotate(0,num);//���]
				count = 0;
				mouseState = 0;
			}
			break;
	}

	if(abs(rot->X) == 360)
		rot->X = 0;
	if(abs(rot->Y) == 360)
		rot->Y = 0;
	if(abs(rot->Z) == 360)
		rot->Z = 0;

	return mouseState;
}

IAnimatedMeshSceneNode* node[5];
IAnimatedMeshSceneNode* cristal;

void peaceMake()
{
	//�m�[�h�̍쐬
	IAnimatedMesh* mesh = smgr->getMesh("data/cube.x");
	node[0] = smgr->addAnimatedMeshSceneNode(mesh,0);
	node[1] = smgr->addAnimatedMeshSceneNode(mesh,node[0]);
	node[2] = smgr->addAnimatedMeshSceneNode(mesh,node[0]);
	node[3] = smgr->addAnimatedMeshSceneNode(mesh,node[0]);
	node[4] = smgr->addAnimatedMeshSceneNode(mesh,node[0]);

	node[1]->setPosition(vector3df(0,1,0));
	node[2]->setPosition(vector3df(-1,1,0));
	node[3]->setPosition(vector3df(0,-1,0));
	node[4]->setPosition(vector3df(1,0,0));

	//node[0]->setMaterialTexture(0, driver->getTexture("icon.png"));
}

void peaceDraw(f32 px, f32 py)
{
	static vector3df rot(0,0,0);
	f32 pz = 0;

	STATE = setRotate(STATE, &rot, BlockNo);

	if(STATE == 1)
		pz = 1;

	node[0]->setRotation(rot);
	node[0]->setPosition(vector3df(px,py,pz));

	//�V�[���̕`��
	smgr->drawAll();
}

//�w�i�ƃA���t�@�̐ݒ�
s32 scene_base(SColor color, c8* mess)
{
	if(GAMECNT < 60)GAMECNT++;
	s32 alpha = GAMECNT*4;
	SColor col[3] = {0x00FFFFFF, color, 0xFF000000};
	col[0] = alpha<<24|0x00FFFFFF;

	//�w�i
	FillRect2(driver,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,col[1],col[2],col[2],col[1],PS_SPECIAL);

	if(mess && strlen(mess) > 0)
	{
		if(color != 0xFFFFFFFF)
		{
			//�^�C�g��
			FillRect(driver,40,100,560,16,color);
			Printf(driver, 50,100,0,col[0],mess);
			Printf(driver, 51,100,0,col[0],mess);
			DrawLine(driver,40,118,600,118,col[0]);

			//Blokus�k�����S
			DrawImage(driver, "data/title.png", 20,20,512,128,0,0,false,col[0],col[0],col[0],col[0],0.5,0.5);
		}

		//�߂�{�^��
		if(SCENE/100 == 1 && SCENE >= 103)
			userGui->button("�^�C�g����ʂ�",UID_CHK_MENU,500,450,140,27,false,"�^�C�g����ʂɖ߂�܂��B");
		else
			userGui->button("�^�C�g����ʂ�",UID_MENU,500,450,140,27,false,"�^�C�g����ʂɖ߂�܂��B");
	}

	return alpha;
}

//�^�C�g�����
void scene_title()
{
	s32 num;
	s32 alpha = scene_base(0xFFFFFFFF, NULL);
	SColor col = alpha<<24|0x00FFFFFF;

	DrawImage(driver, "data/title.png", 0,120,512,128,0,0,false,col);
	DrawImage(driver, "data/irrlichtlogo2.png", 20,20,128,128,0,0,false,col);

	userGui->button("Blokus",		    UID_LOGIN1,	500,330,140,27,false,"4�l�ΐ�pBlokus���J�n���܂��B");
	num = userGui->button("Blokus Duo",	UID_LOGIN2,	500,360,140,27,false,"2�l�ΐ�pBlokusDuo���J�n���܂��B");
//	userGui->button("Blokus�Ƃ�",	    UID_ABOUT,	500,330,140,27,false,"Blokus�ɂ��ďЉ�܂��B");
	userGui->button("���[������",	    UID_RULE,	500,390,140,27,false,"Blokus�̊�{���[�����Љ�܂��B");
	userGui->button("������@",		    UID_HOWTO,	500,420,140,27,false,"������@���Љ�܂��B");
//	userGui->button("�w������ɂ�",     UID_HELP,	500,420,140,27,false,"Blokus���w��������@���Љ�܂��B");
	userGui->button("�I��",			    UID_LOGOUT,	500,450,140,27,false,"�Q�[�����I�����܂��B");

	bold(  driver, 228,276,0,col,"Irrlicht Blokus ver 0.1");
	Printf(driver, 244,308,0,col,"2007 IRRLESSON SOFT");
	Printf(driver, 228,324,0,col,"http://lesson.ifdef.jp/");

	DrawImage(driver, "data/board.png", 256,344,128,37,0,390,false,col);

	Printf(driver, 204,388,0,col,"Blokus�J����:Bernard Tavitian");
//	Printf(driver, 184,404,0,col,"BGM�f��:(c)2000-2007 taitai studio");
	Printf(driver, 192,404,0,col,"libpng:(c)2001-2007 Greg Roelofs");

	//Blokus Duo�͖�����
	userGui->setEnable(num,false);
}

//�Q�[�����
void scene_game()
{
	s32 num, start;
	static s32 localCnt = 0;
	s32 alpha = scene_base(0xFFFFFFFF, "�Q�[�����");
	SColor col = alpha<<24|0x00FFFFFF;
	SColor sub = 0xFFFFFFFF;

	vector3df rot;
	STATE = setRotate(STATE, &rot, BlockNo);

	//Blokus�k�����S
	DrawImage(driver, "data/title.png", 0,0,512,128,0,0,false,sub,sub,sub,sub,0.5,0.5);

	game_board();
	game_block(TURN);
//	game_state();

	//�s�[�X�\��
	game_peace(mouse.X, mouse.Y, BlockNo, TURN);

	if(TURN == 5 && SCENE == 103)
		SCENE = 120;

	switch(SCENE)
	{
		case 110://start
			localCnt = 0;
			SCENE = 111;
			game_start();
			break;
		case 111://ready
			localCnt++;
			if(localCnt < 240)
				localCnt++;
			game_state(localCnt,0);
			if(localCnt == 240)
			{
				localCnt = 0;
				SCENE = 112;
			}
			break;
		case 112://go
			localCnt++;
			if(localCnt < 240)
				localCnt++;
			game_state(0xFF,1);
			if(localCnt == 240)
			{
				localCnt = 0;
				SCENE = 103;
			}
			break;
		case 120://win
			stopBgm();
			playWave(8);
			localCnt = 0;
			SCENE = 121;
			break;
		case 121:
			localCnt++;
			if(localCnt < 240)
				localCnt++;
			game_state(localCnt,4);
			if(localCnt == 240)
			{
				localCnt = 0;
				SCENE = 122;
			}
			break;
		case 122:
			localCnt++;
			if(localCnt < 120)
				localCnt++;
			game_state(240,4);
			if(localCnt == 240)
			{
				SCENE = 101;
				TURN = 0;
			}
			break;
		case 130://lose
			game_state(alpha,3);
			if(GAMECNT == 60)
				SCENE = 101;
			break;
	}

	start = userGui->button("�Q�[���J�n",UID_START,			500,390,140,27,false,"4�l�ΐ�pBlokus���J�n���܂��B");
	num   = userGui->button("�M�u�A�b�v",UID_CHK_GIVEUP,	500,420,140,27,false,"�M�u�A�b�v���܂��B");

	//�Q�[���J�n��́u�Q�[���J�n�v�{�^���͉����Ȃ�
	if(SCENE >= 103)
		userGui->setEnable(start, false);

	//�Q�[���J�n�O�E�I����́u�M�u�A�b�v�v�{�^���͉����Ȃ�
	if(TURN == 0 || TURN == 5)
		userGui->setEnable(num, false);
}

//���[������
void scene_rule()
{
	s32 num;
	s32 alpha = scene_base(0xFF000080, "���[������");
	SColor col  = alpha<<24|0x00FFFFFF;
	SColor sub  = alpha<<24|0x00FFFF00;
	SColor blue = alpha<<24|0x000000FF;
	SColor red  = alpha<<24|0x00FF0000;

	switch(SCENE)
	{
		case 200:
			userGui->button("����",201,500,390,140,27,false,"���̉�ʂɐi�݂܂��B");
			num = userGui->button("�O��",200,500,420,140,27,false,"�O�̉�ʂɖ߂�܂��B");
			userGui->setEnable(num, false);
			bold(  driver, 50,132,0,col,"1.�X�^�[�g�|�C���g�ɍŏ��̃s�[�X��u�����I");
			FillRect(driver,50,150,480,1,col,PS_DOT);
			Printf(driver, 50,164,0,col,"�@�e�v���C���[�́@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�A");
			bold(driver,   50,164,0,sub,"�@�@�@�@�@�@�@�@�{�[�h�Ɉ󂳂ꂽ�X�^�[�g�|�C���g���m�F��");
			Printf(driver, 50,180,0,col,"�@���̃}�X�𖄂߂�悤�ɏ��߂̃s�[�X��u���܂��B");
			Printf(driver, 50,212,0,col,"�@�X�^�[�g�|�C���g�́c");
			Printf(driver, 50,228,0,col,"�@Blokus�̏ꍇ�́A�{�[�h��4���ɂȂ�܂��B");
			Printf(driver, 50,244,0,col,"�@BlokusDuo�̏ꍇ�́A�{�[�h�̒����t�߂ɂ���܂��B");

			//�X�^�[�g�|�C���g
			DrawImage(driver,"data/board.png", 100,280,100,100,  0,0,false, col);
			DrawImage(driver,"data/start.png", 102,282, 32, 32,  0,0,false, sub);

			//�z�u�~�X
			DrawImage(driver,"data/board.png", 230,280,100,100,  0,0,false, col);
			DrawImage(driver,"data/block.png", 240,290, 45, 45,  0,0,false, sub);
			DrawImage(driver,"data/result.png",216,330,128,128,128,0,false, red);

			//�z�u����
			DrawImage(driver,"data/board.png", 360,280,100,100,  0,0,false, col);
			DrawImage(driver,"data/block.png", 370,290, 45, 45, 45,0,false, sub);
			DrawImage(driver,"data/result.png",346,330,128,128,  0,0,false,blue);
			break;

		case 201:
			userGui->button("����",202,500,390,140,27,false,"���̉�ʂɐi�݂܂��B");
			userGui->button("�O��",200,500,420,140,27,false,"�O�̉�ʂɖ߂�܂��B");
			bold(  driver, 50,132,0,col,"2.�p�Ɗp���Ȃ��ăs�[�X��u���Ă������I");
			FillRect(driver,50,150,480,1,col,PS_DOT);
			bold(  driver, 50,164,0,sub,"�@�����ē����F�̃s�[�X���A�ӓ��m�ł������Ă̓_���B");
			Printf(driver, 50,180,0,col,"�@�������A�Ⴄ�F�̊p��ӂƂ������̂͑��v�B");
			Printf(driver, 50,196,0,col,"�@�s�[�X�͉�]�����Ă��A�Ђ�����Ԃ��Ēu���Ă�OK�I");

			Printf(driver, 50,228,0,col,"�@�������u���ꍇ�́A�p�������Ȃ�悤�ɔz�u�B");
			Printf(driver, 50,244,0,col,"�@�܂��́A����̊p���ӂ����悤�ɔz�u���Ă������B");

			//�z�u�~�X
			DrawImage(driver,"data/board.png", 100,280,100,100,  0, 0,false, col);
			DrawImage(driver,"data/block.png", 110,290, 45, 45, 45, 0,false, sub);
			DrawImage(driver,"data/block.png", 155,290, 45, 45,  0,45,false, sub);
			DrawImage(driver,"data/result.png", 86,330,128,128,128, 0,false, red);

			//�z�u����
			DrawImage(driver,"data/board.png", 230,280,100,100,  0, 0,false, col);
			DrawImage(driver,"data/block.png", 240,290, 45, 45, 45, 0,false, sub);
			DrawImage(driver,"data/block.png", 285,290, 45, 45, 45,45,false, sub);
			DrawImage(driver,"data/result.png",216,330,128,128,  0, 0,false,blue);

			//�z�u����
			DrawImage(driver,"data/board.png", 360,280,100,100,  0, 0,false, col);
			DrawImage(driver,"data/block.png", 370,290, 45, 45, 45, 0,false, sub);
			DrawImage(driver,"data/block.png", 415,290, 45, 45, 45,75,false, sub);
			DrawImage(driver,"data/result.png",346,330,128,128,  0, 0,false,blue);
			break;

		case 202:
			num = userGui->button("����",202,500,390,140,27,false,"���̉�ʂɐi�݂܂��B");
			userGui->button("�O��",201,500,420,140,27,false,"�O�̉�ʂɖ߂�܂��B");
			userGui->setEnable(num, false);
			bold(  driver, 50,132,0,col,"3.��������u�����l���Q�[���̏��ҁI");
			FillRect(driver,50,150,480,1,col,PS_DOT);
			Printf(driver, 50,164,0,col,"�@�S���u���Ȃ��Ȃ�����");
			bold(driver,   50,164,0,sub,"�@�@�@�@�@�@�@�@�@�@�@�Q�[���I���ł��B");
			Printf(driver, 50,180,0,col,"�@�茳�Ɏc�����s�[�X�̃}�X�ڂ�");
			bold(driver,   50,180,0,sub,"�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���v�����Ȃ��v���C���[�������I");
			Printf(driver, 50,196,0,col,"�@�u�����s�[�X�̃}�X�ڂ����̂܂ܓ��_�ɂȂ�܂��B");

			Printf(driver, 50,228,0,col,"�@�@�@�@�@�@�@�@�@�Ƃ��āc");
			bold(driver,   50,228,0,red,"�@�{�[�i�X�|�C���g");
			Printf(driver, 50,244,0,col,"�@�����̃s�[�X��S�Ă������Ƃ��ł����ꍇ�́A");
			bold(driver,   50,244,0,sub,"�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@+15�_");
			Printf(driver, 50,260,0,col,"�@����ɁA�Ō�ɂP�}�X�̃s�[�X�ŏオ�����ꍇ�́A");
			bold(driver,   50,260,0,sub,"�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@+5�_");

			Printf(driver, 50,292,0,col,"�@�傫�ȃs�[�X����ǂ�ǂ�u���Ă�����");
			Printf(driver, 50,308,0,col,"�@�Ō�͂P�}�X�̃s�[�X�ŏオ��悤�ɂ���΂낤�I");
			break;
	}
}

//������@
void scene_howto()
{
	s32 alpha  = scene_base(0xFF008000, "������@");
	SColor col = alpha<<24|0x00FFFFFF;
	SColor sub = alpha<<24|0x00FFFF00;
	SColor red = alpha<<24|0x00FF0000;

	Printf(driver, 50,132,0,col,"�}�E�X�ňȉ��̂悤�ɑ��삵�܂��i�ꕔ�L�[�{�[�h�ł��\�ł��j�B");

	bold(driver,   50,164,0,sub,"�}�E�X�ړ�");
	Printf(driver, 50,180,0,col,"�@�s�[�X���ړ����܂��B");

	bold(driver,   50,212,0,sub,"���N���b�N");
	Printf(driver, 50,228,0,col,"�@�s�[�X�̑I��/�z�u�����܂��B");

	bold(driver,   50,260,0,sub,"�z�C�[���iA/D�L�[�j");
	Printf(driver, 50,276,0,col,"�@�s�[�X����]�����܂��B");

	bold(driver,   50,308,0,sub,"�E�N���b�N�iS�L�[�j");
	Printf(driver, 50,324,0,col,"�@�s�[�X�𔽓]�����܂��B");

	DrawImage(driver,"data/mouse.png", 480,180,128,128,  0, 0,false, col);
	switch(STATE)
	{
		case 1:
			DrawImage(driver,"data/mouse.png", 480,180,128,40,128, 0,false, red);
			break;
		case 2:
		case -2:
			DrawImage(driver,"data/mouse.png", 480,180,128,40,128,80,false, red);
			break;
		case 3:
			DrawImage(driver,"data/mouse.png", 480,180,128,40,128,40,false, red);
			break;
	}

	//�s�[�X�`��
	peaceDraw(2,0);
}

//�G���f�B���O
void scene_ending()
{
	s32 alpha = scene_base(0xFFFFFFFF, "�G���f�B���O");
	SColor col = alpha<<24|0x00FFFFFF;
}

//���C�����[�v
void mainLoop()
{
	static s32 oldScene = -1;

	//������
	if(oldScene != SCENE)
	{
		userGui->init();
		GAMECNT = 0;
	}

	oldScene = SCENE;

	//�V�[������U��
	switch(SCENE/100)
	{
		//title
		case 0:
			scene_title();
			break;
		//game
		case 1:
			scene_game();
			break;
		//rule
		case 2:
			scene_rule();
			break;
		//howto
		case 3:
			scene_howto();
			break;
		//ending
		case 4:
			scene_ending();
			break;
	}
}



#ifdef WIN32
	int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR strCmdLine, int nShowCmd)
#else
	int main()
#endif
{
	MyEventReceiver Receiver;

	device = createDevice(EDT_OPENGL,dimension2d<u32>(WINDOW_WIDTH,WINDOW_HEIGHT),16,false,false,false,&Receiver);
	driver = device->getVideoDriver();
	smgr   = device->getSceneManager();

	userGui = new UserGUI();//GUI�쐬

//	sound = new CSound();

	device->setWindowCaption(L"Irrlicht Blokus");//�E�C���h�E�^�C�g���ݒ�

//	sys_setIcon(hInst);//�A�C�R���ݒ�

	//�f�[�^��
//	device->getFileSystem()->addZipFileArchive("blokus.dat");

	//���C�g
	smgr->addLightSceneNode(0, vector3df(0,0,-1000), SColorf(0xFFFFFFFF), 1000.0f);

	//�J�����ݒ�
	ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0,0,-10), vector3df(0,0,0));

	//draw�n�p�p�^�[���쐬
	drawInit(device, driver);

	peaceMake();

	playBgm(1);//3

	while(device->run())
	{
		//�t���[���`�F�b�N
		if(sys_checkFps(100))
		{
			driver->beginScene(true,true,0xFFFFFFFF);

			mainLoop();

			//GUI�`��
			userGui->drawAll();

			driver->endScene();
		}
///		else Sleep(1);
	}

	driver->drop();

	stopBgm();

	delete userGui;

	return 0;
}
