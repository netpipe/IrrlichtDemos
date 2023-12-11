
#ifndef _GUI_H_
#define _GUI_H_

#include "common.h"
#include "draw.h"

//���[�U�[�C�x���g�^�C�v
enum EUSER_MESSAGE_TYPE
{
	EUMT_BUTTON_CLICKED = 0
};

//���[�U�[�C�x���gID
enum USER_ID
{
	UID_NONE = 0,

	//���b�Z�[�W�{�b�N�X�֘A
	UID_OK,
	UID_CANCEL,
	UID_ABORT,
	UID_RETRY,
	UID_IGNORE,
	UID_YES,
	UID_NO,

	//�Q�[���֘A
	UID_CLOSE,
	UID_LOGIN,
	UID_LOGIN1,
	UID_LOGIN2,
	UID_ABOUT,
	UID_RULE,
	UID_HELP,
	UID_HOWTO,
	UID_LOGOUT,
	UID_MENU,
	UID_CHK_MENU,
	UID_BACK_MENU,
	UID_NEWGAME,
	UID_EXIT,

	UID_START,
	UID_GAME,
	UID_STAY,
	UID_WIN,
	UID_LOSE,
	UID_CHK_GIVEUP,
	UID_GIVEUP
};

//���b�Z�[�W�{�b�N�X�^�C�v
enum MESSAGE_TYPE
{
	M_OK = 0,
	M_OKCANCEL,
	M_ABORTRETRYIGNORE,
	M_YESNOCANCEL,
	M_YESNO,
	M_RETRYCANCEL
};

//�}�E�X�f�[�^
typedef struct
{
	position2d<s32> move;

	s32 state;
	position2d<s32> press;		//state = 1
	position2d<s32> release;	//state = 0
}mouseData;

//------------------------------------------------------------------------------
//�x�[�X�R���g���[���N���X
//------------------------------------------------------------------------------
class BaseCtrl
{
public:
	//�R���g���[��������
	void initialize(c8* name)
	{
		ctrlName = name;
		bVisible = true;
		bEnable  = true;
	}

	//�R���g���[�����擾
	const c8* GetCtrlName() const {return ctrlName;}
	//�R���g���[���̉���Ԑݒ�
	void setVisible(bool bState){bVisible = bState;}
	//�R���g���[���̉���Ԏ擾
	bool getVisible(){return bVisible;}
	//�R���g���[���̗L�������ݒ�
	void setEnable(bool bState){bEnable = bState;}
	//�R���g���[���̗L�������擾
	bool getEnable(){return bEnable;}

	void remove(){}

	//�e�R���g���[���擾
	const c8* getParent() const {return parentCtrlName;}

private:
	c8* ctrlName;
	c8* parentCtrlName;
	bool bVisible;
	bool bEnable;
};

//------------------------------------------------------------------------------
//�{�^���R���g���[���N���X
//------------------------------------------------------------------------------
class ButtonCtrl : public BaseCtrl
{
public:
	ButtonCtrl();
	~ButtonCtrl();
	void init();
	void setText(c8* caption);
	bool add(c8* caption, s32 id, s32 x, s32 y, s32 w, s32 h, bool center, c8* help = NULL);
	s32  getID();
	c8*  getName();
	s32  draw(mouseData m_data, bool bFocus);
	s32  getX();
	s32  getY();
private:
	c8* name;
	c8* mess;
	s32 cx,cy,cw,ch;
	s32 hx,hy;
	s32 state;
	s32 ctrlID;
	bool bCenter;
};

//------------------------------------------------------------------------------
//���b�Z�[�W�{�b�N�X
//------------------------------------------------------------------------------
class MessageCtrl : public BaseCtrl
{
public:
	MessageCtrl();
	~MessageCtrl();
	void init();
	bool add(c8* text, c8* caption, u32 type, s32 uid = 0);
	void remove();
	bool isShow();
	void draw(mouseData m_data);
private:
	c8  cCaption[32];
	c8  cText[64];
	s32 cType;
	s32 userID;
	s32 cx,cy,cw,ch;
	s32 width, height;
	s32 space;
	ButtonCtrl btn[3];
};

//------------------------------------------------------------------------------
//���[�U�[GUI
//------------------------------------------------------------------------------
class UserGUI
{
public:
	UserGUI();
	~UserGUI();
	void init();
	void setEnable(s32 num, bool state);
	void setVisible(s32 num, bool state);
	void setText(s32 num, c8* caption);
	void messRemove();
	s32  button(c8* caption, s32 id, s32 x, s32 y, s32 w, s32 h, bool center, c8* help = NULL);
	bool message(c8* text, c8* caption, s32 type, s32 uid = 0);
	bool hasFocus();
	bool onMouse();
	void drawAll();
	void OnEvent(const SEvent& event);
private:
	s32 btnCnt;
	ButtonCtrl *btn;
	MessageCtrl mess;
	bool btnState;
};

#endif //_GUI_H_
