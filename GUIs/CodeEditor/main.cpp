/** Code editor example, by Christian Clavet

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

*/
#include "irrlicht.h"
#include "codeEdit/CGUIEditBoxIRB.h"

#ifdef NDEBUG
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

	IrrlichtDevice *device;

//	if (!device)
//		return;

	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;


// Enumerations for the gui elements
enum GUI_ELEMENTS
{
	MENU_QUIT = 200,
	MENU_UNDO,
	MENU_REDO,
	MENU_CUT,
	MENU_PASTE,
	MENU_COPY,
	MENU_DELETE,
	MENU_LINECOUNT,
	MENU_ENGLISH,
	MENU_FRENCH,
};

// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
	IrrlichtDevice *device;
	CGUIEditBoxIRB * codeEditor;
	IGUIContextMenu * menu;
};

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

			case EGET_SCROLL_BAR_CHANGED:
				break;

			case EGET_BUTTON_CLICKED:
				break;

			case gui::EGET_MENU_ITEM_SELECTED:
				{
					IGUIContextMenu* menu = (IGUIContextMenu*)event.GUIEvent.Caller;
					s32 item = menu->getItemCommandId(menu->getSelectedItem());
					switch (item)
					{
						case MENU_QUIT:
							Context.device->closeDevice();
							break;
						case MENU_UNDO:
							Context.codeEditor->undo();
							break;
						case MENU_REDO:
							Context.codeEditor->redo();
							break;
						case MENU_CUT:
							Context.codeEditor->cut();
							break;
						case MENU_COPY:
							Context.codeEditor->copy();
							break;
						case MENU_PASTE:
							Context.codeEditor->paste();
							break;
						case MENU_DELETE:
							Context.codeEditor->deleteText();
							break;
						case MENU_LINECOUNT:
							{
								bool visible = Context.codeEditor->isLineCountDisplayed();
								Context.codeEditor->setDisplayLineCount(!visible);
							}
							break;
						case MENU_ENGLISH:
							{
								// Change the text of the menu in English
								Context.menu->setItemText(0,L"File");
								Context.menu->setItemText(1,L"Edit");
								Context.menu->setItemText(2,L"Display");

								IGUIContextMenu * submenu;
								submenu = Context.menu->getSubMenu(0);
								submenu->setItemText(0,L"Quit");

								submenu = Context.menu->getSubMenu(1);
								submenu->setItemText(0,L"Cut");
								submenu->setItemText(1,L"Copy");
								submenu->setItemText(2,L"Paste");
								submenu->setItemText(3,L"Delete");
								submenu->setItemText(5,L"Undo");
								submenu->setItemText(6,L"Redo");

								submenu = Context.menu->getSubMenu(2);
								submenu->setItemText(0,L"Toggle linecount display");
								// Change the checked state of the language
								submenu->setItemChecked(1,true);
								submenu->setItemChecked(2,false);

								// Set the elements text of the codeEditor in english
								Context.codeEditor->setElementText(Context.codeEditor->CM_CUT,L"Cut");
								Context.codeEditor->setElementText(Context.codeEditor->CM_COPY,L"Copy");
								Context.codeEditor->setElementText(Context.codeEditor->CM_DELETE,L"Delete");
								Context.codeEditor->setElementText(Context.codeEditor->CM_PASTE,L"Paste");
								Context.codeEditor->setElementText(Context.codeEditor->CM_UNDO,L"Undo");
								Context.codeEditor->setElementText(Context.codeEditor->CM_REDO,L"Redo");
								Context.codeEditor->setElementText(Context.codeEditor->BT_LINECOUNT,
									L"Toggle linecount display");


							}
							break;
						case MENU_FRENCH:
							{
								// Change the text of the menus in French
								Context.menu->setItemText(0,L"Fichier");
//								Context.menu->setItemText(1,L"Édition");
								Context.menu->setItemText(2,L"Affichage");

								IGUIContextMenu * submenu;
								submenu = Context.menu->getSubMenu(0);
								submenu->setItemText(0,L"Quitter");

								submenu = Context.menu->getSubMenu(1);
								submenu->setItemText(0,L"Couper");
								submenu->setItemText(1,L"Copier");
								submenu->setItemText(2,L"Coller");
								submenu->setItemText(3,L"Effacer");
								submenu->setItemText(5,L"Annuler");
								submenu->setItemText(6,L"Refaire");

								submenu = Context.menu->getSubMenu(2);
								submenu->setItemText(0,L"Basculer affichage des lignes");
								// Change the checked state of the language
								submenu->setItemChecked(1,false);
								submenu->setItemChecked(2,true);

								// Set the elements text of the codeEditor in french
								Context.codeEditor->setElementText(Context.codeEditor->CM_CUT,L"Couper");
								Context.codeEditor->setElementText(Context.codeEditor->CM_COPY,L"Copier");
								Context.codeEditor->setElementText(Context.codeEditor->CM_DELETE,L"Effacer");
								Context.codeEditor->setElementText(Context.codeEditor->CM_PASTE,L"Coller");
								Context.codeEditor->setElementText(Context.codeEditor->CM_UNDO,L"Annuler");
								Context.codeEditor->setElementText(Context.codeEditor->CM_REDO,L"Refaire");
								Context.codeEditor->setElementText(Context.codeEditor->BT_LINECOUNT,
									L"Basculer affichage des lignes");

							}
							break;

					}
					break;
				}


			default:
				break;
			}
		}

		return false;
	}

private:
	SAppContext & Context;
};

void main_loop(){
		device->run();

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
}

int main()
{
	#ifdef __EMSCRIPTEN__
	// Create a opengl device
	device =
		createDevice( video::EDT_OGLES2, dimension2d<u32>(640, 480), 32,
			false, false, false, 0);
	#else
		device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 32,
			false, false, false, 0);

	#endif

//	if (!device)
//		return;

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	// Font and skin setup (file arial10.xml and arial100.png are loaded)
	guienv->createSkin(EGST_WINDOWS_METALLIC);
	IGUISkin*  skin = guienv->getSkin();
	skin->setColor(EGDC_HIGH_LIGHT ,video::SColor(192,222,237,243));
	skin->setColor(EGDC_BUTTON_TEXT  ,video::SColor(255,30,30,30));
	skin->setColor(EGDC_HIGH_LIGHT_TEXT ,video::SColor(255,30,30,30));
	skin->setColor(EGDC_3D_DARK_SHADOW  ,video::SColor(255,56,56,56));
	skin->setColor(EGDC_3D_SHADOW  ,video::SColor(255,85,85,85));
	skin->setColor(EGDC_3D_FACE  ,video::SColor(255,170,170,170));
	IGUIFont*  font = guienv->getFont("media/editor/fonts/arial10.xml");
	if (font)
      skin->setFont(font);

	device->setWindowCaption(L"Code editor example, using IRRlicht 1.7.2");

	// Create the menus for the example with it default language (English)
	IGUIContextMenu * menu = guienv->addMenu(0,-1);
	menu->addItem(L"File",0,true,true);
	menu->addItem(L"Edit",1,true,true);
	menu->addItem(L"Display",2,true,true);

	IGUIContextMenu * file_submenu;
    file_submenu = menu->getSubMenu(0);
	file_submenu->addItem(L"Quit",MENU_QUIT);

	IGUIContextMenu * edit_submenu;
    edit_submenu = menu->getSubMenu(1);
	edit_submenu->addItem(L"Cut",MENU_CUT);
	edit_submenu->addItem(L"Copy",MENU_COPY);
	edit_submenu->addItem(L"Paste",MENU_PASTE);
	edit_submenu->addItem(L"Delete",MENU_PASTE);
	edit_submenu->addSeparator();
	edit_submenu->addItem(L"Undo",MENU_UNDO);
	edit_submenu->addItem(L"Redo",MENU_REDO);

	// Define the example text of the Code editor box
	stringw exampletext = L"// Welcome to the code editor example\n//This example use the c++ style of code to highlight\n\nStrings:\"Here is some text\"\n";
	exampletext += L"C++ Keywords: this, if, else, break, switch, case, while\n\n";
	exampletext += L"custom keywords highlight: Hybrid, CuteAlien, Bitplane\n";
	exampletext += L"custom group highlight 1: (Text is colored between the parenthesis)\n";
	exampletext += L"custom group highlight 2: Feature(This is a demo of a feature and should work on multiple lines of text.)\n";
	exampletext += L"custom group highlight 2: Feature(Single line text)\n";
	exampletext += L"custom line highlight: remark: This line will be highlighted.";

	// Init the Custom GUI
	CGUIEditBoxIRB * codeEditor = new CGUIEditBoxIRB(exampletext.c_str(),true,true,guienv,guienv->getRootGUIElement(),-1,rect<s32>(20,60,600,400),device);
//CGUIEditBoxIRB* codeEditor = new CGUIEditBoxIRB(exampletext.c_str(),
//                       true,
//					   true,
//                       guienv,
//                       -1,
//                       25,
//                       //myRect(10,40,driver->getScreenSize().Width-220,driver->getScreenSize().Height-260),
//					   rect<s32>(20,60,600,400),
//					   device);
	// Recolor the background with pure white
	codeEditor->setBackgroundColor(SColor(255,240,240,240));
	// Recolor the linecount GUI element
	codeEditor->setLineCountColors(SColor(255,32,32,32),SColor(200,64,120,180),SColor(255,200,200,224));
	// Recolor the selection colors
	codeEditor->setSelectionColors(SColor(180,0,96,64),SColor(255,255,255,255),SColor(180,0,128,96));

	// Allow the code editor to use syntax highlighting based on C++ keywords
	codeEditor->addCppKeywords();

	// Allow the code editor to use syntax highlighting based on LUA keywords
	//codeEditor->addLUAKeywords();

	// Define a basic custom dictionnary
	// Define custom "Group" keywords
	codeEditor->addGroupKeyword("(",")",SColor(255,100,100,0),false,true);
	codeEditor->addGroupKeyword("Feature(",")",SColor(255,200,100,0),true,true);
	// Define a custom "line" keyword
	codeEditor->addLineKeyword("remark",SColor(255,0,128,128),true);
	// Define custom keywords
	codeEditor->addKeyword("Hybrid",SColor(255,80,100,200),true);
	codeEditor->addKeyword("CuteAlien",SColor(255,80,100,200),true);
	codeEditor->addKeyword("Bitplane",SColor(255,80,100,200),true);


	// Set the pointers so the event manager can manipulate their objects
	SAppContext context;
	context.device = device;
	context.codeEditor = codeEditor;
	context.menu = menu;


	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);

	// Main loop

	#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
#endif // __EMSCRIPTEN__


}
