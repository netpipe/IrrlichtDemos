
#include "gui.h"

mouseData guiMouse;

//ユーザーイベント送信
void postEvent(s32 eventType, s32 eventId)
{
	SEvent event;
	event.EventType = EET_USER_EVENT;
	event.UserEvent.UserData1 = eventType;
	event.UserEvent.UserData2 = eventId;

	device->postEventFromUser(event);
}

//マウス情報消去
void mouseClear()
{
	guiMouse.state     =  0;
	guiMouse.press.X   = -1;
	guiMouse.press.Y   = -1;
	guiMouse.release.X = -1;
	guiMouse.release.Y = -1;
}

//マウスの状態取得
s32 getState(mouseData m_data, s32 ctrlID, s32 x, s32 y, s32 w, s32 h)
{
	s32 state = 1;

	//マウスがコントロール内
	if( x <= m_data.move.X && m_data.move.X <= x+w &&
		y <= m_data.move.Y && m_data.move.Y <= y+h)
	{
		//左ボタンが押されているか
		if(m_data.state == 1)
		{
			//押した場所がコントロール内か
			if( x <= m_data.press.X && m_data.press.X <= x+w &&
				y <= m_data.press.Y && m_data.press.Y <= y+h)
			{
				state = 3;
			}
		}
		//押されていない
		else 
		{
			//押して離した場所が同一のコントロールの場合
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
//ボタンコントロールクラス
//------------------------------------------------------------------------------
ButtonCtrl::ButtonCtrl()
{
	ButtonCtrl::init();
}

ButtonCtrl::~ButtonCtrl()
{

}

//コントロール初期化
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

//キャプション変更
void ButtonCtrl::setText(c8* caption)
{
	name = caption;
}

//コントロール追加
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

//ID取得
s32 ButtonCtrl::getID()
{
	return ctrlID;
}

//テキスト取得
c8* ButtonCtrl::getName()
{
	return name;
}

//コントロール表示
s32 ButtonCtrl::draw(mouseData m_data, bool bFocus)
{
	s32 state = 0;
	s32 len;
	s32 sx = 4;					//左寄せ
	s32 sy = (ch - 16) / 2;		//中央寄せ
	s32 retState = 0;				//マウスがボタン上にあるかどうか
	
	//カラー		  無効		通常		マウスオン  クリック  
	SColor back[4] = {0xFF999999, 0xFF00FF00, 0xFF33FF33, 0xFF66FF66};
	SColor top[4]  = {0xFF333333, 0xFF006600, 0xFF339933, 0xFF66CC66};
	SColor font[4] = {0xFFCCCCCC, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	SColor help    =  0xFFFFFFFF;
	SColor sub     =  0xFF000000;

	//名前チェック
	if(name == NULL || strlen(name) == 0)
		return -1;
	len = strlen(name);

	//中央寄せ
	if(bCenter)
		sx = (cw - len*8)/2;	//中央寄せ

	//表示可の場合は表示する
	if(getVisible())
	{
		//有効かどうか
		if(getEnable())
		{
			//フォーカスがあるかどうか
			if(bFocus)
			{
				//詳細を取得する
				state = getState(m_data, ctrlID, cx,cy,cw,ch);

				//ヘルプメッセージを表示する
				if(mess && strlen(mess) > 0 && state >= 2)
					Printf(driver,hx,hy,0,help,mess);
			}
			else state = 1;
		}

		//ボタン作成
//		FillRect(driver, cx,cy,cw,ch, 0xFF404040);			//下地
//		FillRect(driver, cx,cy,cw-1,ch-1, 0xFFFFFFFF);		//下地
//		FillRect(driver, cx+1,cy+1,cw-2,ch-2, 0xFF808080);			//下地
//		FillRect(driver, cx+1,cy+1,cw-3,ch-3, 0xFFD4D0C8);			//下地
//		Printf(driver,cx+sx,cy+sy,0,0xFF000000,name);		//テキスト

		FillRect(driver, cx,cy,cw,ch, back[state]);			//下地
		FillRect(driver, cx+1,cy+1,cw-2,ch-2, top[state]);	//表面
		Printf(driver,cx+sx,cy+sy,0,font[state],name);		//テキスト
	}

	//マウスがあるかどうか
	if(state > 1)
		retState = 1;

	return retState;
}

//位置取得
s32 ButtonCtrl::getX()
{
	return cx;
}

s32 ButtonCtrl::getY()
{
	return cy;
}

//------------------------------------------------------------------------------
//メッセージボックス
//------------------------------------------------------------------------------
MessageCtrl::MessageCtrl()
{
	init();
}

MessageCtrl::~MessageCtrl()
{

}

//初期化
void MessageCtrl::init()
{
//	cCaption = NULL;
	memset(cCaption, 0x00, sizeof(cCaption));
//	cText    = NULL;
	memset(cText, 0x00, sizeof(cText));
	userID   = 0;

	//ウインドウ
	cw = 288;
	ch = 128;
	cx = (WINDOW_WIDTH  - cw)/2;
	cy = (WINDOW_HEIGHT - ch)/2;

	//ボタン
	width  = 80;
	height = 20;
	space  = 10;
}

//メッセージボックス追加
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

//メッセージボックス消去
void MessageCtrl::remove()
{
	init();
}

//メッセージボックスを表示するかどうか
bool MessageCtrl::isShow()
{
	if(cText == NULL || strlen(cText) == 0)
		return false;

	return true;
}

//メッセージボックスを描画
void MessageCtrl::draw(mouseData m_data)
{
	s32 id = UID_OK;
	s32 x = cx;
	s32 y = cy;
	s32 w = cw;
	s32 h = ch;

	s32 bw = width;		//ボタン幅
	s32 bh = height;	//ボタン高
	s32 bs = space;		//ボタンスペース

	//カラー		    
	SColor back[2] = {0xFF006600, 0xFF000000};
	SColor top[3]  = {0xFF000000, 0xFF006600, 0xFF000000};
	SColor font[2] = {0xFFFFFF00, 0xFFFFFFFF};

	if(cText == NULL || strlen(cText) == 0)
		return;

	//ユーザーIDを使用するかどうか
	if(userID != 0)
		id = userID;

	//下地
	FillRect2(driver, x,y,w,h,back[0], back[1],back[1],back[0]);
	//上面
	FillRect2(driver, x+1,y+1,w-2,h-2,top[0], top[1], top[1], top[0]);
	//タイトルバー
	FillRect(driver, x+2,y+2,w-4,16,top[2]);
	//タイトル
	Printf(driver,x+8,y+2,0,font[0],cCaption);
	//メッセージ
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
			btn[1].add("キャンセル", UID_NO, (w+bs)/2+x,y+h-height-8,bw,bh,true);
			btn[2].init();
			break;
		case M_YESNO:
			btn[0].add("はい", id, (w-bs)/2+x-bw,y+h-height-8,bw,bh,true);
			btn[1].add("いいえ", UID_NO, (w+bs)/2+x,y+h-height-8,bw,bh,true);
			btn[2].init();
			break;
	}

	btn[0].draw(m_data, true);
	btn[1].draw(m_data, true);
	btn[2].draw(m_data, true);
}

//------------------------------------------------------------------------------
//ユーザーGUI
//------------------------------------------------------------------------------
UserGUI::UserGUI()
{
	init();
}

UserGUI::~UserGUI()
{
	init();
}

//初期化
void UserGUI::init()
{
	if(btnCnt > 0)
		free(btn);

	btnCnt = 0;
	btnState = false;
}

//有効化/無効化の設定
void UserGUI::setEnable(s32 num, bool state)
{
	if(this == NULL)
		return;

	btn[num].setEnable(state);
}

//表示/非表示の設定
void UserGUI::setVisible(s32 num, bool state)
{
	if(this == NULL)
		return;

	btn[num].setVisible(state);
}

//ボタン用テキストの設定
void UserGUI::setText(s32 num, c8* caption)
{
	if(this == NULL)
		return;

	btn[num].setText(caption);
}

//メッセージボックスの消去
void UserGUI::messRemove()
{
	if(this == NULL)
		return;

	mess.remove();
}

//ボタンの追加
s32 UserGUI::button(c8* caption, s32 id, s32 x, s32 y, s32 w, s32 h, bool center, c8* help)
{
	if(this == NULL)
		return -1;

	//内容確認:同名・同場所は追加させない
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

	//メモリ割り当て
	if(btnCnt == 0)
		btn = (ButtonCtrl*)malloc(sizeof(ButtonCtrl));
	//メモリ再割り当て
	else 
		btn = (ButtonCtrl*)realloc(btn, sizeof(ButtonCtrl) * (btnCnt+1));

	if(btn == NULL)
		return -1;

	btn[btnCnt].add(caption, id,x,y,w,h,center, help);
	btnCnt++;
	
	return btnCnt-1;
}

//メッセージボックスの追加
bool UserGUI::message(c8* text, c8* caption, s32 type, s32 uid)
{
	if(this == NULL)
		return false;

	return mess.add(text, caption, type, uid);
}

//GUIがコントロール優先かどうか（メッセージボックスなど）
//trueの場合は、ゲームではマウスやキーボードの操作は無効にする
bool UserGUI::hasFocus()
{
	bool result;

	result = mess.isShow();

	if(result == false)
		result = onMouse();

	return result;
}

//GUIボタン上にマウスがあるかどうか
//trueの場合は、ゲームではマウスやキーボードの操作は無効にする
bool UserGUI::onMouse()
{
	return btnState;
}

//GUIの描画
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

//イベント処理
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
				//上回転
				if(event.MouseInput.Wheel == 1){}
				//下回転
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
