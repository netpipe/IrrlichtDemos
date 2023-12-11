#include "StdAfx.h"
#include "game.h"

void CGame::menu()
{device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(512, 256), 32, false, false,false, this);

		driver = device->getVideoDriver();
		guiEnv = device->getGUIEnvironment();

		createWindow();

		fontVeranda16 = guiEnv->getFont("fontVeranda_16.jpg");

		if (fontVeranda16)
			guiEnv->getSkin()->setFont(fontVeranda16);

		guiEnv->getSkin()->setColor(gui::EGDC_HIGH_LIGHT, video::SColor(255, 30, 30, 70));
		guiEnv->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,37,118,171));

		while(device->run())
		{
			driver->beginScene(true, true, video::SColor(255,0,0,0));
			guiEnv->drawAll();
			driver->endScene();
		}

		device->drop();
}//////////////////////
void CGame::createWindow()
	{
		windowBackground =				guiEnv->addImage(core::rect<int>(0,0,512,256));
			windowBackground->setImage(driver->getTexture("menuBackground.jpg"));

		fullScreenCheck = guiEnv->addCheckBox(true, core::rect<int>(60,110,175,140), 0, 1, L"Full Screen");

		resolutionListBox = guiEnv->addListBox( core::rect<int>(25,150,232,220) );
			lowRes = resolutionListBox->addItem(L"1024x768");
			highRes = resolutionListBox->addItem(L"1280x1024");
			resolutionListBox->setSelected(highRes);

			driverBox = guiEnv->addListBox( core::rect<int>(25,80,220,110) );
			opengl = driverBox->addItem(L"OpenGL");
			//directx = driverBox->addItem(L"DIRECTX8");
			driverBox->setSelected(opengl);

		startButton = guiEnv->addButton(core::rect<int>(265,170,490,230), 0, 100, L"Start");
			startButton->setImage(driver->getTexture("startButton.jpg"), irr::core::rect<int>(0,0,224,59));
	}

////////////////////////////
void CGame::Init()
{
	// Init the Irrlicht engine
	device = createDevice(driverType, dimension2d<u32>(resX, resY), 32, fullscreen, true,true, this);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();




	// add the camera
	camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(0, 50, 0));

}




void CGame::Start()
{

	driver->setFog(video::SColor(0,255,255,255), EFT_FOG_LINEAR, 0.0f, 0.0f,0.001f, true, true);


//////////load graphics zip//////////////////
device->getFileSystem()->addZipFileArchive("gfx.omar");
////////force 32 bit gfx//////////
driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
// add skybox
	smgr->addSkyBoxSceneNode(driver->getTexture("top.jpg"),driver->getTexture("down.jpg"),driver->getTexture("right.jpg"),driver->getTexture("left.jpg"),driver->getTexture("front.jpg"),driver->getTexture("back.jpg"));

//terrain///////////////////////////
 video::ITexture* waternorm = driver->getTexture("water_b.jpg");
		driver->makeNormalMapTexture(waternorm, 5.0f);


IImage* terraintexture = driver->createImageFromFile("tex.jpg");
	IImage* terrainheightmap = driver->createImageFromFile("terrtexture.jpg");

terrain = smgr->addTerrainMesh("terrain",
		terraintexture,								//for texturing
		terrainheightmap,							//heightmap
		core::dimension2d< f32 >(30.0f, 30.0f),		//Stretchsize
		300.0f);									//Maxheight

scene::ISceneNode* light2 =
		smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f, 0.5f, 0.3f, 0.0f), 2000.0f);



terrainnode=smgr->addOctTreeSceneNode(terrain);
terrainnode->setMaterialTexture(0,driver->getTexture("tex.jpg"));
	terrainnode->setMaterialTexture(1,waternorm);
	terrainnode->setPosition(vector3df(-1000,-500,-1000));
	terrainnode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
/////////water
scene::ISceneNode* waternode = 0;
scene::IAnimatedMesh* waterm = 0;

waternode = smgr->addAnimatedMeshSceneNode(waterm);
waterm = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(40,40),
		core::dimension2d<u32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));



waternode = smgr->addWaterSurfaceSceneNode(waterm->getMesh(0),  3.0f, 300.0f, 30.0f);
	waternode->setPosition(core::vector3df(500,-320,500));
waternode->setScale(core::vector3df(10,1,10));

	waternode->setMaterialTexture(0,driver->getTexture("water.jpg"));
waternode->setMaterialFlag(EMF_LIGHTING, false);
	waternode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);


///player

	 video::ITexture* planenorm = driver->getTexture("plane_b.jpg");
		driver->makeNormalMapTexture(planenorm, 5.0f);

	IAnimatedMesh* planem = smgr->getMesh("smallcube2.3ds");
		IMesh* mesh = smgr->getMeshManipulator()->createMeshWithTangents(		planem->getMesh(0));
 playernode = smgr->addMeshSceneNode( mesh,camera );
//	playernode->setMaterialFlag(EMF_LIGHTING, false);

playernode->setScale(core::vector3df(1,1,1));
playernode->setPosition(core::vector3df(0,-30,40));
playernode->setRotation(core::vector3df(0,0,0));
playernode->setMaterialTexture(1,planenorm);
	playernode->setMaterialTexture(0,	driver->getTexture("crate.jpg"));
playernode->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	playernode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
playernode->	getMaterial(0).EmissiveColor.set(0,0,0,0);
//////////////////
scene::ITriangleSelector* selectorterr = 0;

if (terrainnode)
{


		selectorterr = smgr->createTriangleSelector(
          terrain->getMesh(0), terrainnode);
		terrainnode->setTriangleSelector(selectorterr);
	selectorterr->drop();

	}
scene::ISceneNodeAnimator* animterr =    smgr->createCollisionResponseAnimator(
		selectorterr, camera, core::vector3df(30,50,30),
		core::vector3df(0,0,0),
		core::vector3df(0,50,0));
	camera->addAnimator(animterr);
	animterr->drop();
	device->getCursorControl()->setVisible(false);

	//////////////////////

enemynode = smgr->addMeshSceneNode( mesh);
	enemynode->setMaterialFlag(EMF_LIGHTING, false);

enemynode->setScale(core::vector3df(1,1,1));
enemynode->setPosition(core::vector3df(0,-30,40));
enemynode->setRotation(core::vector3df(0,0,0));

	enemynode->setMaterialTexture(0,	driver->getTexture("crate.jpg"));
	enemynode->setMaterialTexture(1,planenorm);
	enemynode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
	wchar_t h1[1024];
		swprintf(h1, 1024, L"enemy 1 health (:%d)",health);
ITextSceneNode* eh =smgr->addTextSceneNode(guienv->getBuiltInFont(), h1,		video::SColor(255,255,255,255), enemynode);
health=100;
	// add light 1 (nearly red)
	scene::ILightSceneNode* light1 = 	smgr->addLightSceneNode(enemynode, core::vector3df(0,0,0), 	video::SColorf(1.0f, 0.5f, 0.3f, 0.0f), 200.0f);



// attach billboard to the light
// add particle systemscene::
	IParticleSystemSceneNode* ps = 	smgr->addParticleSystemSceneNode(false, light1);
	ps->setParticleSize(core::dimension2d<f32>(30.0f, 30.0f));
// create and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(	core::aabbox3d<f32>(-3,0,-3,3,1,3), 	core::vector3df(0.0f,0.03f,0.0f),	80,100, 	video::SColor(0,255,255,255), video::SColor(0,255,255,255),	800,1500);ps->setEmitter(em);em->drop();
// create and set affector
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();ps->addAffector(paf);paf->drop();

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);ps->setMaterialTexture(0, driver->getTexture("fire.jpg"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	/// player stuff

vector3df prevrneg=camera->getRotation();
/*
//////sounds////////
AudioDevicePtr audiereDevice;
	OutputStreamPtr streamboom;
	audiereDevice = OpenDevice();


    streamboom = OpenSound(audiereDevice.get(), "Expomar.wav", false  );



    streamboom->setRepeat(false);
    streamboom->setVolume(1.0f); // 50% volume
////////
	 audiereDevice = OpenDevice();
   	OutputStreamPtr streamg;
    streamg = OpenSound(audiereDevice.get(), "fire1.wav", false  );

    streamg->setRepeat(false);
    streamg->setVolume(1.0f); // 50% volume

/////////
	OutputStreamPtr streamb;


    streamb = OpenSound(audiereDevice.get(), "hit.wav", true);


    streamb->setRepeat(false );
    streamb->setVolume(1.0f); // 50% volume

	/////////////////////////
	*/
	prevn1rot =enemynode->getRotation();
while(device->run()){


	driver->beginScene(true, true, video::SColor(0,220,220,255));

	//house gen
	Housegen(30);

	//tree gen
	Treegen(60);

	Cloudgen(80);
	//does player functions

	// render the scene
	smgr->drawAll();

	player(playernode,camera);
	enemy(enemynode,camera);


	///////moveplayer code
irr::core::vector3df vel=vector3df(0,0,10);
		irr::core::matrix4 m;
   m.setRotationDegrees(camera->getRotation());
   m.transformVect(vel);
    camera->setPosition(camera ->getPosition() + vel);
	/////////////////////



core::vector3df rneg = camera->getRotation();
	core::vector3df nodepos4rot=playernode->getRotation();

	//rneg.Y*-1
	int camy=(rneg.Y-prevrneg.Y);

	int rotadd=	nodepos4rot.Y-camy;
		//	node->setRotation(r);

core::vector3df blah = camera->getRotation();
	prevrneg= (blah);

int zrot=nodepos4rot.Z+rotadd;

		if (zrot<35 & zrot>-35)
				{
				playernode->setRotation(core::vector3df(nodepos4rot.X,nodepos4rot.Y,zrot));

				}
////////////////shoot stuff

core::position2d<s32> midscreen;
		vector3df node1pos = enemynode->getPosition();
midscreen =smgr->getSceneCollisionManager()-> getScreenCoordinatesFrom3DPosition( node1pos,camera);

	if (fired==true){
if (midscreen.Y>0 & midscreen.Y<600& midscreen.X>0 & midscreen.X<600){
//	streamb->play();

	health=health-1;
printf("hit");
	if(health==0){

      die=true;
	}
}
		}


 if (fired==true)
 {
 scene::IBillboardSceneNode* bill = smgr->addBillboardSceneNode();
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	bill->setMaterialTexture(0, driver->getTexture("fire.jpg"));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setSize(core::dimension2d<f32>(200.0f, 200.0f));

core::line3d<f32> line;
		line.start = camera->getPosition();
		line.end = line.start +
         (camera->getTarget() - line.start).normalize() * -3000.0f;

eh->setText(h1);

//streamg->play();
printf("fired");
scene::ISceneNodeAnimator* fly = smgr->createFlyStraightAnimator (line.start,line.end,300);
bill->addAnimator(fly);
	fly->drop();

scene::ISceneNodeAnimator* animb = smgr->createDeleteAnimator(300);
	bill->addAnimator(animb);
	animb->drop();
	fired=false;
}


///////////////
//		vector3df node1pos = enemynode->getPosition();
int node1y=node1pos.Y;
	vector3df node1rot = enemynode->getRotation();
int node1roty=node1rot.Y;
int node1rotz=node1rot.Z;
int node1rotx=node1rot.X;
		//////

///////////plane death
int node1x=node1pos.X;
int node1z=node1pos.Z;
	if (die==true){

			if (node1y>-250){
   node1y= node1y-5;
   node1roty= node1roty+5;
   node1rotz= node1rotz+5;
   node1rotx= node1rotx+5;
  enemynode->setPosition(core::vector3df(node1x,node1y,node1z));
	 enemynode->setRotation(core::vector3df(node1rotx,node1roty,node1rotz));
/////////


//	streamboom->play();
	}
	}
		////////////




	driver->endScene();



	// Draw fps counter
	int fps = driver->getFPS();
	if (lastFPS != fps)
	{
		wchar_t tmp[1024];
		swprintf(tmp, 1024, L"Newton Example [fps:%d] [triangles:%d]",
		fps, driver->getPrimitiveCountDrawn());
		device->setWindowCaption(tmp);
		lastFPS = fps;
	}
}
}
void CGame::Finish()
{


	device->drop();
}

//////////////////////////////////////////////////////////////////////////
// Irrlicht event receiver
//////////////////////////////////////////////////////////////////////////

bool CGame::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_KEY_INPUT_EVENT &&
      event.KeyInput.Key == KEY_SPACE &&
      event.KeyInput.PressedDown == true)
	{
	fired=true;
	}
	/////////////menu event recievr
if (event.EventType == EET_GUI_EVENT)
		{
			int id = event.GUIEvent.Caller->getID();

			switch(event.GUIEvent.EventType)
			{
			case gui::EGET_BUTTON_CLICKED:

				// *** Triangle Buttons ***

				if (id == 100)
				{
					if ( resolutionListBox->getSelected() == lowRes )
					{
						resX = 1024;
						resY = 768;
					}

					else if ( resolutionListBox->getSelected() == highRes )
					{
						resX = 1280;
						resY = 1024;
					}

					if ( fullScreenCheck->isChecked() == true )
					{
						fullscreen = true;
					}
					else
					{
						fullscreen = false;
					}

					if ( driverBox->getSelected() == opengl)
					{
						driverType = video::EDT_OPENGL;
					}
					if ( driverBox->getSelected() == directx)
					{
						driverType = video::EDT_OPENGL;
					}

					startGame = true;
					device->closeDevice();
				}
			break;
			}

			return true;
		}
//////////////////////////////////




	return false;
}
//////////////////////////
void CGame::Housegen(int max)
{

	if( houseno<max)
{
int cloud_x=rand()%1500;

int cloud_z=rand()%1500;



IAnimatedMesh* housem = smgr->getMesh("house4.3ds");
IMesh* meshhouse = smgr->getMeshManipulator()->createMeshWithTangents(		housem->getMesh(0));
 house = smgr->addMeshSceneNode( meshhouse );
house->setMaterialType(video::EMT_NORMAL_MAP_SOLID);

house->setScale(core::vector3df(500,500,500));
house->setPosition(core::vector3df(cloud_x,-250,cloud_z));
house->setRotation(core::vector3df(0,0,0));
house->setMaterialTexture(0,	driver->getTexture("house.jpg"));

	smgr->setShadowColor(video::SColor(220,0,0,0));

houseno=houseno+1;

}
}
//////////////////////////////


//////////////////////////
void CGame::Treegen(int max)
{

if(treeno<max)
{
int	cloud_x=rand()%2000;

int cloud_z=rand()%2000;

IBillboardSceneNode* tree = smgr->addBillboardSceneNode(0,core::dimension2d<f32>(100, 100));
tree->setPosition(core::vector3df(cloud_x,-200,cloud_z));
tree->setMaterialFlag(video::EMF_LIGHTING, false);
tree->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
 tree->setMaterialTexture(0,	driver->getTexture("trees.jpg"));
 printf("tree generated");

treeno=treeno+1;
}

}

void CGame::Cloudgen(int max)
{

if(treeno<max)
{
int	cloud_x=rand()%2000;
int cloud_y=rand()%2000;
int cloud_z=rand()%2000;

IBillboardSceneNode* tree = smgr->addBillboardSceneNode(0,core::dimension2d<f32>(200, 100));
tree->setPosition(core::vector3df(cloud_x,cloud_y,cloud_z));
tree->setMaterialFlag(video::EMF_LIGHTING, false);
tree->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
 tree->setMaterialTexture(0,	driver->getTexture("cloud.jpg"));
 printf("cloud generated");

treeno=treeno+1;
}

}
//////////////////////////////

void CGame::player(ISceneNode *node,ICameraSceneNode *camera)
{



	///////moveplayer code
irr::core::vector3df vel=vector3df(0,0,10);
		irr::core::matrix4 m;
   m.setRotationDegrees(camera->getRotation());
   m.transformVect(vel);
    camera->setPosition(camera ->getPosition() + vel);
	/////////////////////



}


void CGame::enemy(ISceneNode *node,ICameraSceneNode *camera)
{

	if(die==false){

	///////moveplayer code
irr::core::vector3df vel=vector3df(0,0,5);
		irr::core::matrix4 m;
   m.setRotationDegrees(node->getRotation());
   m.transformVect(vel);
    node->setPosition(node ->getPosition() + vel);
	/////////////////////





/////	enemy seek player code

vector3df up(0,-1,0); //makes enemy plane look right way (works for ver 0.6)
// the up direction during gameplay


// the initial forward direction

// the forward direction during gameplay
vector3df targetpos=camera->getPosition();

		irr::core::matrix4 rotmat;
vector3df node1rota;
rotmat.buildCameraLookAtMatrixLH(node->getPosition(), targetpos, up);

node1rota=rotmat.getRotationDegrees();


	vector3df rotadd1=prevn1rot-node1rota;//calculates the rotaion that should be added
	vector3df rotvec1=node1rota+rotadd1;//calculates final rot thats set to the plane

	node->setRotation(rotvec1); //set enemy rotaion

prevn1rot=node1rota;

}
}
void CGame::sound()
{
////////////// sounds


}
