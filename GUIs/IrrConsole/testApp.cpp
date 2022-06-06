//#include "IrrConsole/testApp.h"
//=========================================================================
#include "IrrConsole/console.h"
#include "IrrConsole/nodeViewer.h"
using namespace irr;

String name;
IC_Console console;
array<WideString> captionText;
u32 captionHeight;

bool bRunning;

irr::IrrlichtDevice* device;
irr::scene::ISceneNode* sceneNode;
irr::scene::IAnimatedMesh* levelMesh;
irr::scene::IAnimatedMesh* modelMesh;
irr::video::ITexture* modelTexture;
irr::video::ITexture* testTexture;
irr::gui::IGUIFont* captionFont;

void showModel();
void showMap();
void showTestNode();
void hideNode();
void drawCaptions();


bool isRunning()
{
	return bRunning;
}

void setRunning(bool bVal)
{
	bRunning = bVal;
}

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
    {
	if(event.EventType == irr::EET_KEY_INPUT_EVENT )
	{
		if(event.KeyInput.PressedDown)
		{
			if(event.KeyInput.Key == irr::KEY_ESCAPE)
			{
				if(console.isVisible())
				{
					console.setVisible(false);
					return true;
				}
				else
				{
					setRunning(false);
					return true;
				}
			}
			else if(event.KeyInput.Key ==  KEY_F1)
			{
				if(!console.isVisible())
				{
					console.setVisible(true);
					return true;
				}
				else if(!event.KeyInput.Control)
				{
					console.setVisible(false);
					return true;
				}
			}

			if(console.isVisible())
			{
				console.handleKeyPress(event.KeyInput.Char, event.KeyInput.Key,event.KeyInput.Shift, event.KeyInput.Control);
				return true;
			}
		}
	}
	else if(event.EventType == irr::EET_LOG_TEXT_EVENT)
	{
		console.logMessage_ANSI(event.LogEvent.Level,event.LogEvent.Text);
		return true;
	}
	else if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		return console.isVisible();
	}
	return false;
}
};
void showModel()
{
	hideNode();
	if(modelMesh)
	{
		sceneNode = device->getSceneManager()->addAnimatedMeshSceneNode(modelMesh);
		sceneNode->setMaterialTexture(0, modelTexture);
		sceneNode->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		sceneNode->setPosition(vector3df(0,10,80));

        ((irr::scene::IAnimatedMeshSceneNode *)sceneNode)->setAnimationSpeed(20);
		((irr::scene::IAnimatedMeshSceneNode *)sceneNode)->setFrameLoop(0, modelMesh->getFrameCount());
	}
	else
	{
		throw IC_Error(L"Model could not be loaded");
	}
	device->setWindowCaption(L"IrrConsole Demo App [ Showing MD2 Model]");
}
void showMap()
{
	hideNode();
	if(levelMesh)
	{
		//add the quake III level
		sceneNode = device->getSceneManager()->addOctTreeSceneNode(levelMesh->getMesh(0));
		//set lighting to false
		sceneNode->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		//position camera in a suitable place
		sceneNode->setPosition(vector3df(-1300,-144,-1249));
	}
	else
	{
		throw IC_Error(L"Level/Map could not be loaded");
	}
	device->setWindowCaption(L"IrrConsole Demo App [ Showing Quake III Map]");
}

void showTestNode()
{
	hideNode();
	if(testTexture)
	{
		//add a test scene node
		sceneNode = device->getSceneManager()->addDummyTransformationSceneNode();
		//set the texture
		sceneNode->setMaterialTexture(0, testTexture);
		//set lighting to false
		sceneNode->setMaterialFlag(irr::video::EMF_LIGHTING,false);

		//set the model position
		sceneNode->setPosition(vector3df(0,0,30));

		//create a rotation animator
		irr::scene::ISceneNodeAnimator* anim = device->getSceneManager()->createRotationAnimator(vector3df(1,2,1));
		sceneNode->addAnimator(anim);
		anim->drop();

	}
	else
	{
		throw IC_Error(L"Test Texture Could not be loaded");
	}
	device->setWindowCaption(L"IrrConsole Demo App [ Showing Test Node]");
}

void hideNode()
{
	if(sceneNode)
	{
		sceneNode->remove();
		sceneNode = 0;
	}
	device->setWindowCaption(L"IrrConsole Demo App [ Node Hidden ]");
}

void drawCaptions()
{
	static const irr::video::SColor fontColor(200,200,200,200);
	dimension2d<u32> screenDim = device->getVideoDriver()->getScreenSize();
	rect<s32> lineRect(10,10,screenDim.Width, screenDim.Height);
	for(u32 i = 0; i < captionText.size(); i++)
	{
		captionFont->draw(captionText[i].c_str(),lineRect,fontColor);
		lineRect.UpperLeftCorner.Y += captionHeight;
		lineRect.LowerRightCorner.Y += captionHeight;
	}
}
void IC_TestParser()
{
	//TODO: for testing only
	WideString cmdLine = L"echo \"Hello World\" du\\de h\\\"o\\\"w";
	array<WideString> args;
	WideString cmdName = L"";
	IC_CmdLineParser parser(cmdLine);
	parser.parse(cmdName,args);
	cout<<"Command Line : ["<<cmdLine<<"]"<<endl;
	cout<<"Command Name : ["<<cmdName<<"]"<<endl;
	cout<<"Arguments : ["<<args.size()<<"]"<<endl;
	for(u32 x = 0; x < args.size(); x++)
	{
		cout<<" arg["<<x<<"]==>{"<<args[x]<<"}"<<endl;
	}
}

bool run()
{

	irr::video::IVideoDriver* videoDriver = device->getVideoDriver();
	irr::scene::ISceneManager* sceneManager = device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	s32 fps = videoDriver->getFPS();
	u32 deltaMillis = 0;
	if(fps > 0)
	{
		deltaMillis  = (u32) (1000.0f / (f32)fps);
	}

	videoDriver->beginScene(true,true,irr::video::SColor(200,10,10,100));
		sceneManager->drawAll();
		guienv->drawAll();
		console.renderConsole(guienv,videoDriver,deltaMillis);

		//if console is not visible
		//we draw the caption text
		if(!console.isVisible())
		{
			drawCaptions();
		}
	videoDriver->endScene();
device->sleep(5,0);
	return true;
}



int main()
{
	//initialize the app
	   	MyEventReceiver receiver;

	irr::video::E_DRIVER_TYPE driverType = irr::video::EDT_OPENGL;
	s32 bitsPerPixel = 32;
	irr::core::dimension2du screenDim(800,600);
	bool fullscreen = false;
	bool stencil = true;
	bool vsync = false;
	device = irr::createDevice(driverType, screenDim, bitsPerPixel, fullscreen,stencil,vsync, &receiver);

	//load the caption font
	//captionFont = device->getGUIEnvironment()->getFont("data/font/caption.bmp");
//	if(!captionFont)
	//{
		captionFont = device->getGUIEnvironment()->getBuiltInFont();
	//}
	//initialize the caption array
	captionText.push_back(L"IrrConsole : Quake Style Drop Down Console Demo For Irrlicht");
	captionText.push_back(L"Author : Saurav Mohapatra (mohaps@gmail.com)");
	captionText.push_back(L"");
	captionText.push_back(L"HELP TEXT:");
	captionText.push_back(L"============================================================");
	captionText.push_back(L"     Press the F1 key to toggle console");
	captionText.push_back(L"     Use the UP/DN arrow keys to access command history");
	captionText.push_back(L"     Type commands at the prompt");
	captionText.push_back(L"     To execute a command put \\ in front");
	captionText.push_back(L"     e.g. \\list");
	captionText.push_back(L"     use the \"list\" or \"help\" commands to find out more");
	captionText.push_back(L"     to start try the command \"\\help show_node\");");
	captionText.push_back(L"============================================================");
	captionText.push_back(L" ");
	captionText.push_back(L"TO QUIT THIS DEMO PRESS ESCAPE KEY");


	//initialize the caption line height
	captionHeight = captionFont->getDimension(L"X").Height + 2;
	//initialize the console
	//this is how you alter some of the config params
	console.getConfig().dimensionRatios.Y = 0.8f;
	//now initialize
	console.initializeConsole(device->getGUIEnvironment(),screenDim);

	//register common commands
	console.loadDefaultCommands(device);

	//register custom commands
	// we register the show node command
//	console.registerCommand(new TestCommand_SHOWNODE(this));
	// we register the hide node command
//	console.registerCommand(new TestCommand_HIDENODE(this));

	//we do some app specific init her
	//to save time we preload the mesh, map and texture required for this demo

	//for the texture
	testTexture = device->getVideoDriver()->getTexture("../../media/wall.jpg");

	//for the model
	modelMesh = device->getSceneManager()->getMesh("../../media/faerie.md2");
	modelTexture = device->getVideoDriver()->getTexture("../../media/faerie2.bmp");

	//for the map
	device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");
	levelMesh = device->getSceneManager()->getMesh("20kdm2.bsp");

	//init the scene node to 0
	sceneNode = 0;

	device->getSceneManager()->addCameraSceneNodeFPS();

	//for starters we have displayed the quake III level
	//showModel();
    showMap();

	//init();

	setRunning(true);
	while(device && device->run() && isRunning())
	{
		if(!run())
		{
			setRunning(false);
		}
	}

}
