
#include "gui.h"

mouseData guiMouse;

//���[�U�[�C�x���g���M
void postEvent(s32 eventType, s32 eventId)
{
	SEvent event;
	event.EventType = EET_USER_EVENT;
	event.UserEvent.UserData1 = eventType;
	event.UserEvent.UserData2 = eventId;

	device->postEventFromUser(event);
}

//�}�E�X������
void mouseClear()
{
	guiMouse.state     =  0;
	guiMouse.press.X   = -1;
	guiMouse.press.Y   = -1;
	guiMouse.release.X = -1;
	guiMouse.release.Y = -1;
}

//�}�E�X�̏�Ԏ擾
s32 getState(mouseData m_data, s32 ctrlID, s32 x, s32 y, s32 w, s32 h)
{
	s32 state = 1;

	//�}�E�X���R���g���[����
	if( x <= m_data.move.X && m_data.move.X <= x+w &&
		y <= m_data.move.Y && m_data.move.Y <= y+h)
	{
		//���{�^����������Ă��邩
		if(m_data.state == 1)
		{
			//�������ꏊ���R���g���[������
			if( x <= m_data.press.X && m_data.press.X <= x+w &&
				y <= m_data.press.Y && m_data.press.Y <= y+h)
			{
				state = 3;
			}
		}
		//������Ă��Ȃ�
		else 
		{
			//�����ė������ꏊ������̃R���g���[���̏ꍇ
			if( x <= m_data.press.X && m_data.press.X <= x+w &&
				y <= m_data.press.Y && m_data.press.Y <= y+h && 
				x <= m_data.release.X && m_data.release.X <= x+w &&
				y <= m_data.release.Y && m_data.release.Y <= y+h)
			{
				mouseClear();
				postEvent(EUMT_BUTTON_CLICKED, ctrlID);
			}
			state = 2;
		}
	}
	return state;
}


//------------------------------------------------------------------------------
//�{�^���R���g���[���N���X
//------------------------------------------------------------------------------
ButtonCtrl::ButtonCtrl()
{
	ButtonCtrl::init();
}

ButtonCtrl::~ButtonCtrl()
{

}

//�R���g���[��������
void ButtonCtrl::init()
{
	name = NULL;
	mess = NULL;
	cx = cy = cw = ch = 0;
	hx = hy = 0;
	state = 0;
	ctrlID = 0;
	bCenter = false;
}

//�L���v�V�����ύX
void ButtonCtrl::setText(c8* caption)
{
	name = caption;
}

//�R���g���[���ǉ�
bool ButtonCtrl::add(c8* caption, s32 id, s32 x, s32 y, s32 w, s32 h, bool center, c8* help)
{
	name = caption;
	mess = help;
	cx = x;
	cy = y;
	cw = w;
	ch = h;
	if(help)
	{
		hx = (WINDOW_WIDTH - strlen(help)*8)/2;
		hy = WINDOW_HEIGHT - 20;
	}
	ctrlID = id;
	bCenter = center;

	initialize("ButtonCtrl");

	return true;
}

//ID�擾
s32 ButtonCtrl::getID()
{
	return ctrlID;
}

//�e�L�X�g�擾
c8* ButtonCtrl::getName()
{
	return name;
}

//�R���g���[���\��
s32 ButtonCtrl::draw(mouseData m_data, bool bFocus)
{
	s32 state = 0;
	s32 len;
	s32 sx = 4;					//����
	s32 sy = (ch - 16) / 2;		//������
	s32 retState = 0;				//�}�E�X���{�^����ɂ��邩�ǂ���
	
	//�J���[		  ����		�ʏ�		�}�E�X�I��  �N���b�N  
	SColor back[4] = {0xFF999999, 0xFF00FF00, 0xFF33FF33, 0xFF66FF66};
	SColor top[4]  = {0xFF333333, 0xFF006600, 0xFF339933, 0xFF66CC66};
	SColor font[4] = {0xFFCCCCCC, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	SColor help    =  0xFFFFFFFF;
	SColor sub     =  0xFF000000;

	//���O�`�F�b�N
	if(name == NULL || strlen(name) == 0)
		return -1;
	len = strlen(name);

	//������
	if(bCenter)
		sx = (cw - len*8)/2;	//������

	//�\���̏ꍇ�͕\������
	if(getVisible())
	{
		//�L�����ǂ���
		if(getEnable())
		{
			//�t�H�[�J�X�����邩�ǂ���
			if(bFocus)
			{
				//�ڍׂ��擾����
				state = getState(m_data, ctrlID, cx,cy,cw,ch);

				//�w���v���b�Z�[�W��\������
				if(mess && strlen(mess) > 0 && state >= 2)
					Printf(driver,hx,hy,0,help,mess);
			}
			else state = 1;
		}

		//�{�^���쐬
//		FillRect(driver, cx,cy,cw,ch, 0xFF404040);			//���n
//		FillRect(driver, cx,cy,cw-1,ch-1, 0xFFFFFFFF);		//���n
//		FillRect(driver, cx+1,cy+1,cw-2,ch-2, 0xFF808080);			//���n
//		FillRect(driver, cx+1,cy+1,cw-3,ch-3, 0xFFD4D0C8);			//���n
//		Printf(driver,cx+sx,cy+sy,0,0xFF000000,name);		//�e�L�X�g

		FillRect(driver, cx,cy,cw,ch, back[state]);			//���n
		FillRect(driver, cx+1,cy+1,cw-2,ch-2, top[state]);	//�\��
		Printf(driver,cx+sx,cy+sy,0,font[state],name);		//�e�L�X�g
	}

	//�}�E�X�����邩�ǂ���
	if(state > 1)
		retState = 1;

	return retState;
}

//�ʒu�擾
s32 ButtonCtrl::getX()
{
	return cx;
}

s32 ButtonCtrl::getY()
{
	return cy;
}

//------------------------------------------------------------------------------
//���b�Z�[�W�{�b�N�X
//------------------------------------------------------------------------------
MessageCtrl::MessageCtrl()
{
	init();
}

MessageCtrl::~MessageCtrl()
{

}

//������
void MessageCtrl::init()
{
//	cCaption = NULL;
	memset(cCaption, 0x00, sizeof(cCaption));
//	cText    = NULL;
	memset(cText, 0x00, sizeof(cText));
	userID   = 0;

	//�E�C���h�E
	cw = 288;
	ch = 128;
	cx = (WINDOW_WIDTH  - cw)/2;
	cy = (WINDOW_HEIGHT - ch)/2;

	//�{�^��
	width  = 80;
	height = 20;
	space  = 10;
}

//���b�Z�[�W�{�b�N�X�ǉ�
bool MessageCtrl::add(c8* text, c8* caption, u32 type, s32 uid)
{
	if(text == NULL || type > M_RETRYCANCEL)
		return false;

	init();

//	cText    = text;
	sprintf(cText, text);
//	cCaption = caption;
	sprintf(cCaption, caption);
	cType    = type;
	userID   = uid;

	initialize("MessageCtrl");

	return true;
}

//���b�Z�[�W�{�b�N�X����
void MessageCtrl::remove()
{
	init();
}

//���b�Z�[�W�{�b�N�X��\�����邩�ǂ���
bool MessageCtrl::isShow()
{
	if(cText == NULL || strlen(cText) == 0)
		return false;

	return true;
}

//���b�Z�[�W�{�b�N�X��`��
void MessageCtrl::draw(mouseData m_data)
{
	s32 id = UID_OK;
	s32 x = cx;
	s32 y = cy;
	s32 w = cw;
	s32 h = ch;

	s32 bw = width;		//�{�^����
	s32 bh = height;	//�{�^����
	s32 bs = space;		//�{�^���X�y�[�X

	//�J���[		    
	SColor back[2] = {0xFF006600, 0xFF000000};
	SColor top[3]  = {0xFF000000, 0xFF006600, 0xFF000000};
	SColor font[2] = {0xFFFFFF00, 0xFFFFFFFF};

	if(cText == NULL || strlen(cText) == 0)
		return;

	//���[�U�[ID���g�p���邩�ǂ���
	if(userID != 0)
		id = userID;

	//���n
	FillRect2(driver, x,y,w,h,back[0], back[1],back[1],back[0]);
	//���
	FillRect2(driver, x+1,y+1,w-2,h-2,top[0], top[1], top[1], top[0]);
	//�^�C�g���o�[
	FillRect(driver, x+2,y+2,w-4,16,top[2]);
	//�^�C�g��
	Printf(driver,x+8,y+2,0,font[0],cCaption);
	//���b�Z�[�W
	Printf(driver, x+8,y+24,0,font[1], cText);

	switch(cType)
	{
		case M_OK:
			btn[0].add("OK", id, (w-bw)/2+x,y+h-height-8,bw,bh,true);
			btn[1].init();
			btn[2].init();
			break;
		case M_OKCANCEL:
			btn[0].add("OK", id, (w-bs)/2+x-bw,y+h-height-8,bw,bh,true);
			btn[1].add("�L�����Z��", UID_NO, (w+bs)/2+x,y+h-height-8,bw,bh,true);
			btn[2].init();
			break;
		case M_YESNO:
			btn[0].add("�͂�", id, (w-bs)/2+x-bw,y+h-height-8,bw,bh,true);
			btn[1].add("������", UID_NO, (w+bs)/2+x,y+h-height-8,bw,bh,true);
			btn[2].init();
			break;
	}

	btn[0].draw(m_data, true);
	btn[1].draw(m_data, true);
	btn[2].draw(m_data, true);
}

//------------------------------------------------------------------------------
//���[�U�[GUI
//------------------------------------------------------------------------------
UserGUI::UserGUI()
{
	init();
}

UserGUI::~UserGUI()
{
	init();
}

//������
void UserGUI::init()
{
	if(btnCnt > 0)
		free(btn);

	btnCnt = 0;
	btnState = false;
}

//�L����/�������̐ݒ�
void UserGUI::setEnable(s32 num, bool state)
{
	if(this == NULL)
		return;

	btn[num].setEnable(state);
}

//�\��/��\���̐ݒ�
void UserGUI::setVisible(s32 num, bool state)
{
	if(this == NULL)
		return;

	btn[num].setVisible(state);
}

//�{�^���p�e�L�X�g�̐ݒ�
void UserGUI::setText(s32 num, c8* caption)
{
	if(this == NULL)
		return;

	btn[num].setText(caption);
}

//���b�Z�[�W�{�b�N�X�̏���
void UserGUI::messRemove()
{
	if(this == NULL)
		return;

	mess.remove();
}

//�{�^���̒ǉ�
s32 UserGUI::button(c8* caption, s32 id, s32 x, s32 y, s32 w, s32 h, bool center, c8* help)
{
	if(this == NULL)
		return -1;

	//���e�m�F:�����E���ꏊ�͒ǉ������Ȃ�
	s32 i;
	if(btnCnt > 0)
	{
		for(i=0;i<btnCnt;i++)
		{
			if(strcmp(btn[i].getName(), caption) == 0 && 
				btn[i].getX() == x && btn[i].getY() == y)
				return i;// -1;
		}
	}

	//���������蓖��
	if(btnCnt == 0)
		btn = (ButtonCtrl*)malloc(sizeof(ButtonCtrl));
	//�������Ċ��蓖��
	else 
		btn = (ButtonCtrl*)realloc(btn, sizeof(ButtonCtrl) * (btnCnt+1));

	if(btn == NULL)
		return -1;

	btn[btnCnt].add(caption, id,x,y,w,h,center, help);
	btnCnt++;
	
	return btnCnt-1;
}

//���b�Z�[�W�{�b�N�X�̒ǉ�
bool UserGUI::message(c8* text, c8* caption, s32 type, s32 uid)
{
	if(this == NULL)
		return false;

	return mess.add(text, caption, type, uid);
}

//GUI���R���g���[���D�悩�ǂ����i���b�Z�[�W�{�b�N�X�Ȃǁj
//true�̏ꍇ�́A�Q�[���ł̓}�E�X��L�[�{�[�h�̑���͖����ɂ���
bool UserGUI::hasFocus()
{
	bool result;

	result = mess.isShow();

	if(result == false)
		result = onMouse();

	return result;
}

//GUI�{�^����Ƀ}�E�X�����邩�ǂ���
//true�̏ꍇ�́A�Q�[���ł̓}�E�X��L�[�{�[�h�̑���͖����ɂ���
bool UserGUI::onMouse()
{
	return btnState;
}

//GUI�̕`��
void UserGUI::drawAll()
{
	if(this == NULL)
		return;

	s32 i;
	bool bFocus = true;

	if(mess.isShow())
		bFocus = false;

	btnState = false;
	for(i=0;i<btnCnt;i++)
	{
		if(btn[i].draw(guiMouse, bFocus) == 1)
			btnState = true;
	}

	mess.draw(guiMouse);
}

//�C�x���g����
void UserGUI::OnEvent(const SEvent& event)
{
	if(this == NULL)
		return;

	if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		switch(event.MouseInput.Event)
		{
			case EMIE_LMOUSE_PRESSED_DOWN:
				guiMouse.state = 1;
				guiMouse.press.X = event.MouseInput.X;
				guiMouse.press.Y = event.MouseInput.Y;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				guiMouse.state = 0;
				guiMouse.release.X = event.MouseInput.X;
				guiMouse.release.Y = event.MouseInput.Y;
				break;
			case EMIE_MOUSE_WHEEL:
				//���]
				if(event.MouseInput.Wheel == 1){}
				//����]
				else if(event.MouseInput.Wheel == -1){}
				break;
			case EMIE_MOUSE_MOVED:
				guiMouse.move.X = event.MouseInput.X;
				guiMouse.move.Y = event.MouseInput.Y;
				break;
			default:
				break;
		}
	}
}
