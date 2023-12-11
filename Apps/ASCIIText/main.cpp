//WIPSTILL
#include <irrlicht.h>

#include "figlet.h"

//#include "selfextract.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif
#include <stdio.h>
#include <wctype.h>
#include <sstream>
#include <iostream>
#include <wchar.h>

IGUIEditBox *editbox1;


//char* WideCharToChar(LPCWSTR lpWideCharStr)
//{
//      size_t length = wcslen(lpWideCharStr);
//      ++length;
//      char* lpszConvertedStr = new char[length];
//      int nReturnVal =  WideCharToMultiByte( CP_OEMCP, 0, lpWideCharStr, -1,lpszConvertedStr, (int)length, NULL, NULL );
//      //If not succeed delete pointer
//      if( 0 == nReturnVal)
//      {
//            delete[] lpszConvertedStr;
//            lpszConvertedStr = NULL;
//      }
//      return lpszConvertedStr;
//}


// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
	IrrlichtDevice *device;
	s32				counter;
	IGUIListBox*	listbox;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR
};

/*
	Set the skin transparency by changing the alpha values of all skin-colors
*/
void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
	for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
	{
		video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
}


class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			/*
			If a scrollbar changed its scroll position, and it is
			'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR),
			then we change the transparency of all gui elements. This is an
			easy task: There is a skin object, in which all color
			settings are stored. We simply go through all colors
			stored in the skin and change their alpha value.
			*/
			case EGET_SCROLL_BAR_CHANGED:
				if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
				{
					s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					setSkinTransparency(pos, env->getSkin());
				}
				break;


			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					{
					Context.listbox->addItem(L"Window created");
					Context.counter += 30;
					if (Context.counter > 200)
						Context.counter = 0;

					IGUIWindow* window = env->addWindow(
						rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
						false, // modal?
						L"Test window");

					env->addStaticText(L"Please close me",
						rect<s32>(35,35,140,50),
						true, // border?
						false, // wordwrap?
						window);
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					Context.listbox->addItem(L"File open");
					// There are some options for the file open dialog
					// We set the title, make it a modal window, and make sure
					// that the working directory is restored after the dialog
					// is finished.
//wchar_t c;
//  int i=0;
wchar_t test=editbox1->getText();
printf ("string %S",test);
//wchar_t test[] = L"Example sentence to test iswspace\n";
//char *test2="testing";
//string test3 = std::ws(test);
//std::wstring string(test);
//std::wstring ws(test);
//// your new String
//string str(ws.begin(), ws.end());
//  while (test[i])
//  {
//
//    c=test[i];
//    if (iswspace(c)) c = L' ';
//    putwchar (c);
//    i++;
//  }
//   char *str = "This is tutorialspoint.com";
//   wchar_t mb[100];
//   int len;
//
//   len = mblen(NULL, MB_CUR_MAX);
//
//   mbtowc(mb, str, len*strlen(str) );
//
//
//        char *str;
////
//   wchar_t array[] = L"Hello World";
//	wcstombs(str, array, 100);

//
//   	char pmb[] = "Welcome to Programiz.com";
//	wchar_t pwc[100];
//	int len, ret_val;
//
//	/* resets internal conversion state */
//	mbtowc (NULL, NULL, 0);
//	len = strlen(pmb);
//	ret_val = mbtowc(pwc, pmb, strlen(pmb));

    //A std:string  using the char* constructor.
//    std::string str2((char*)test);
//    printf ("string %S",str2.c_str());

//    std::string str("string");
//std::wstring wstr;
//for (size_t i = 0; i < str2.length(); i++)
//    wstr.push_back(str2[i]);


//wstring_t
//char *test2=(char)test;

	char *argv1[]={"appname","-f","./fonts/standard", test,"test"};//rectangular_BFS

	int argc1 = sizeof(argv1) / sizeof(char*) - 1;

    	figlet( argc1, argv1 );
//
//  }




					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;

				default:
					return false;
				}
				break;

			case EGET_FILE_SELECTED:
				{
					// show the event and the selected model filename from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_FILE_SELECTED");
					Context.listbox->addItem(dialog->getFileName());
				}
				break;

			case EGET_DIRECTORY_SELECTED:
				{
					// show the event and the selected directory name from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_DIRECTORY_SELECTED");
					Context.listbox->addItem(dialog->getDirectoryNameW());
				}
				break;

			default:
				break;
			}
		}

		return false;
	}

private:
	SAppContext & Context;
};


int main()
{
	// ask user for driver
//	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
//	if (driverType==video::EDT_COUNT)
//		return 1;

	// create device and exit if creation failed
	IrrlichtDevice * device = createDevice(EDT_OPENGL, core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1; // could not create selected driver.


	device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
	device->setResizable(true);

	video::IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* env = device->getGUIEnvironment();

//	const io::path mediaPath = getExampleMediaPath();
//


	IGUISkin* skin = env->getSkin();
//	IGUIFont* font = env->getFont(mediaPath + "fonthaettenschweiler.bmp");
//	if (font)
//		skin->setFont(font);
//
//	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);


	env->addButton(rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON,
			L"Quit", L"Exits Program");
	env->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
			L"New Window", L"Launches a new Window");
	env->addButton(rect<s32>(10,320,110,320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
			L"File Open", L"Opens a file");

	/*
	Now, we add a static text and a scrollbar, which modifies the
	transparency of all gui elements. We set the maximum value of
	the scrollbar to 255, because that's the maximal value for
	a color value.
	Then we create an other static text and a list box.
	*/

	env->addStaticText(L"Transparent Control:", rect<s32>(150,20,350,40), true);
	IGUIScrollBar* scrollbar = env->addScrollBar(true,
			rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);
	scrollbar->setPos(255);
	setSkinTransparency( scrollbar->getPos(), env->getSkin());

	// set scrollbar position to alpha value of an arbitrary element
	scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());

	env->addStaticText(L"Logging ListBox:", rect<s32>(10,110,350,130), true);
	IGUIListBox * listbox = env->addListBox(rect<s32>(10, 140, 350, 210));
	editbox1 = env->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);


//	env->addImage(driver->getTexture( "irrlichtlogo2.png"),
//			position2d<int>(10,10));
//



	while(device->run() && driver)
	if (device->isWindowActive())
	{
		driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, SColor(0,200,200,200));

		env->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}

/*
**/
