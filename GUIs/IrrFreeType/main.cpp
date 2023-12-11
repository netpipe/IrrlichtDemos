/*
 * Irrlicht FreeType Demo
 */

#ifdef WIN32
#include <windows.h>
#endif
#include <irrlicht.h>
#include "CGUITTFont.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "freetype221.lib")

#define	SIZE	24

IrrlichtDevice *Device;
s32 cnt = 0;
s32 lang = 0;
IGUIListBox* listbox =  0;
IGUIListBox* lstLang = 0;
IGUIStaticText* txtTrans = 0;
IGUIStaticText* txtLog = 0;
IGUIButton *btnQuit = 0;
IGUIButton *btnNew = 0;
IGUIButton *btnFile = 0;
IGUICheckBox *chkAA = 0;
IGUICheckBox *chkTrans = 0;

CGUITTFont *fonts[6],*font,*font2;
IGUISkin* skin;
/*
 *	Japanese Texts
 */
wchar_t	jtxtTrans[] = {0x900f,0x660e,0x5ea6,0x8a2d,0x5b9a,0x003a,0};
wchar_t	jtxtQuit[] = {0x7d42,0x308f,0x308b,0};
wchar_t	jtxtNew[] = {0x65b0,0x898f,0x30a6,0x30a3,0x30f3,0x30c9,0x30a6,0};
wchar_t	jtxtFile[] = {0x30d5,0x30a1,0x30a4,0x30eb,0x3092,0x958b,0x304f,0};
wchar_t	jtxtLog[] = {0x64cd,0x4f5c,0x30ed,0x30b0,0};
wchar_t	jtxtAA[] = {0x30a2,0x30f3,0x30c1,0x30a8,0x30a4,0x30ea,0x30a2,0x30b9,0};
wchar_t	jtxtTfont[] = {0x900f,0x904e,0x30d5,0x30a9,0x30f3,0x30c8,0};
wchar_t	jtxtHello[] = {0x3053,0x3093,0x306b,0x3061,0x306f,0x30c8,0x30a5,0x30eb,0x30fc,0x30bf,0x30a4,0x30d7,0};

void ChangeCaption(s32 newlang){
	lang = newlang;
	switch(lang){
	case	0:	// English
		txtTrans->setText(L"Transparency:");
		btnQuit->setText(L"Quit");
		btnNew->setText(L"New Window");
		btnFile->setText(L"Open File");
		txtLog->setText(L"Logging ListBox:");
		chkAA->setText(L"AntiAlias:");
		chkTrans->setText(L"Transparent Font");
		break;
	case	1:	// Japanese
		txtTrans->setText(jtxtTrans);
		btnQuit->setText(jtxtQuit);
		btnNew->setText(jtxtNew);
		btnFile->setText(jtxtFile);
		txtLog->setText(jtxtLog);
		chkAA->setText(jtxtAA);
		chkTrans->setText(jtxtTfont);
		break;
	}
}

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			case EGET_SCROLL_BAR_CHANGED:
				if (id == 104)
				{
					s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();

					for (s32 i=0; i<EGDC_COUNT ; ++i)
					{
						SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
						col.setAlpha(pos);
						env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
					}

				}
				break;

			case EGET_BUTTON_CLICKED:

				if (id == 101)
				{
					Device->closeDevice();
					return true;
				}

				if (id == 102)
				{
					listbox->addItem(L"Window created");
					cnt += 30;
					if (cnt > 200)
						cnt = 0;

					IGUIWindow* window = env->addWindow(
						rect<s32>(100 + cnt, 100 + cnt, 700 + cnt, 700 + cnt),
						false, // modal?
						L"Test window");

					env->addStaticText(L"Please close me\n asdfasdfasdfsda/n fsdafsdaf",
						rect<s32>(35,35,540,50),
						true, // border?
						false, // wordwrap?
						window);

					return true;
				}

				if (id == 103)
				{
					listbox->addItem(L"File open");
					env->addFileOpenDialog(L"Please choose a file.");
					return true;
				}

				break;
			case EGET_CHECKBOX_CHANGED:
				if (id == 110)
				{
					bool flg = !font->AntiAlias;
					for (int i = 0;i < 6;i++){
						fonts[i]->AntiAlias = flg;
					}
					return true;
				}
				if (id == 111)
				{
//					bool flg = !font->TransParency;
					for (int i = 0;i < 6;i++){
			//			fonts[i]->TransParency = flg;
					}
					return true;
				}
				break;
			case EGET_LISTBOX_CHANGED:
			case EGET_LISTBOX_SELECTED_AGAIN:
				if (id == 120){
					int sel = lstLang->getSelected();
					font = fonts[sel * 2];
					font2 = fonts[sel * 2 + 1];
					//skin->setFont(font);
					if (sel == 2){
						ChangeCaption(1);
					} else {
						ChangeCaption(0);
					}
				}
				break;
			}
		}

		return false;
	}
};

int main()

{
	Device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, false, 0);
	IVideoDriver *Driver = Device->getVideoDriver();
	IGUIEnvironment* env = Device->getGUIEnvironment();

	MyEventReceiver receiver;
	Device->setEventReceiver(&receiver);

	/*
	 *	Load TrueType Font to CGUITTFace
	 */
	CGUITTFace face,face2,face3;
	face.load("kochi-gothic-subst.ttf");
	face2.load("kochi-gothic-subst.ttf");
	face3.load("kochi-gothic-subst.ttf");

	/*
	 *	Create CGUITTFont and Bind with CGUITTFace
	 */
	for (int i = 0;i < 6;i++){
		fonts[i] = new CGUITTFont(env);
	}
	fonts[0]->attach(&face2,12);
	fonts[1]->attach(&face2,24);
	fonts[2]->attach(&face3,12);
	fonts[3]->attach(&face3,24);
	fonts[4]->attach(&face,12);
	fonts[5]->attach(&face,24);
	font = fonts[0];
	font2 = fonts[1];

	//skin = env->getSkin();
	//skin->setFont(font);

	txtTrans = env->addStaticText(L"Transparency:", rect<s32>(150,20,350,40), true);
	btnQuit = env->addButton(rect<s32>(10,210,100,240), 0, 101, L"Quit");
	btnNew = env->addButton(rect<s32>(10,250,100,290), 0, 102, L"New Window");
	btnFile = env->addButton(rect<s32>(10,300,100,340), 0, 103, L"Open File");
	chkAA = env->addCheckBox(font->AntiAlias,rect<s32>(10,350,180,390),0,110,L"AntiAlias");
//	chkTrans = env->addCheckBox(font->TransParency,rect<s32>(190,350,380,390),0,111,L"TransParent Font");
	IGUIScrollBar* scrollbar = env->addScrollBar(true, rect<s32>(150, 45, 350, 60), 0, 104);
	lstLang = env->addListBox(rect<s32>(10, 400, 250, 470),0,120);
	lstLang->addItem(L"Arial");
	lstLang->addItem(L"Times Roman");
	lstLang->addItem(L"Kochi-Gothic(Japanese)");

	scrollbar->setMax(255);

	txtLog = env->addStaticText(L"Logging ListBox:", rect<s32>(50,80,250,100), true);
	listbox = env->addListBox(rect<s32>(50, 110, 550, 580));

	int lastFPS = -1;




listbox->addItem(L"Window created /n fasdfasdf/n   \n sdfsadfsdafsd");
listbox->addItem(L"Window created /n fasdfasdf/n   \n sdfsadfsdafsd");
listbox->addItem(L"Window created /n fasdfasdf/n   \n sdfsadfsdafsd");
listbox->addItem(L"Window created /n fasdfasdf/n   \n sdfsadfsdafsd");

	while(Device->run())
	{
		Driver->beginScene(true, true, SColor(0,64,64,128));
		if (!lang){
			font2->draw(L"Hello TrueType",rect<s32>(0,240,640,240),SColor(255,255,64,64),true);
		} else {
			font2->draw(jtxtHello,rect<s32>(0,240,640,240),SColor(255,255,64,64),true);
		}
		env->drawAll();
		Driver->endScene();

		int fps = Driver->getFPS();
		if (lastFPS != fps)
		{
			wchar_t tmp[1024];
			swprintf(tmp, 1024, L"Irrlicht TrueType Demo (fps:%d)", fps);
			Device->setWindowCaption(tmp);
			lastFPS = fps;
		}
    Device->sleep(20);
	}
	Device->drop();
	for (int i = 0;i < 6;i++){
		fonts[i]->drop();
	}

	return 0;
}
