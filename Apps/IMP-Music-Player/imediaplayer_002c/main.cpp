
/// IMP by: jorgerosa ( jorge.bigarte@gmail.com )
/// http://sites.google.com/site/jorgerosaportfolio

/// IN CODE::BLOCKS: (Should compile and start from your Irrlicht "examples" folder), else...
//  EDIT, TO MATCH IN YOUR SYSTEM:
//  "Project" --> "Build options" --> "Linker settings"
//  "Project" --> "Build options" --> "Search directories" --> "Compiler" AND "Linker" TABs
//  ALSO TAKE A LOOK AT:
//  "Project" --> "Properties" TAB

#include "main.h"

// MUST include "XEffects.h", and add all the XEffects ".cpp" files to your project/makefile:
// #include "XEffects.h"


/// Define Global Variables:
// scene::ICameraSceneNode* camera[2] = {0,0};
IrrlichtDevice *device = 0;      // Create Device
video::IVideoDriver* driver = 0; // Create Driver
scene::ISceneManager* smgr = 0;  // Create Scene Manager
core::stringw MessageText = "MessageText";
core::stringw Caption = "CaptionText";




/*
void showWindow() //create a window for the fun of it
{
   device->getGUIEnvironment()->addWindow(core::rect<s32>(100,45,400,480),false,L"Load Media",0,0x10000);
}


class MyEventReceiver : public IEventReceiver
{


virtual bool OnEvent(const SEvent& event)
{
   if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown == false)
   {
      if(OnKeyUp(event.KeyInput.Key)){ };
   }
}

bool OnKeyUp(irr::EKEY_CODE keyCode)
{
   if(keyCode == irr::KEY_KEY_L){ showWindow(); };
}


}; // END OF class MyEventReceiver : public IEventReceiver
*/
















/// Main:
int main(int argc, char** argv)
{



    /// Starting ... (Will use OpenGL only)
	// create device and exit if creation failed:
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32, false, false, false);
	if (device == 0){ return 1; } // If OpenGL doesn´t exist

	video::IVideoDriver* driver = device->getVideoDriver();     // Create Driver
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment(); // For GUI texts
	scene::ISceneManager* smgr = device->getSceneManager();     // Create Scene Manager

	/// Create Events Receiver: (keyboard)
	// MyEventReceiver receiver; // Class must be created before.
    // device->setEventReceiver(&receiver); // Add Event Receiver.

    // Initialise the EffectHandler:
	// EffectHandler* effect = new EffectHandler(device, driver->getScreenSize(), true, true, true);

	// Initialise the EffectHandler:
	// EffectHandler* effect = new EffectHandler(device, driver->getScreenSize(), true, true, true);




	/// Load all needed files:
	device->getFileSystem()->addZipFileArchive("media/world.zip");


    /// Create SKYBOX: (Not needed in this project)
//	ISceneNode* sky = smgr->addSkyBoxSceneNode(
//	driver->getTexture("sky_up.jpg"),
//	driver->getTexture("sky_down.jpg"),
//	driver->getTexture("sky_left.jpg"),
//	driver->getTexture("sky_right.jpg"),
//	driver->getTexture("sky_front.jpg"),
//	driver->getTexture("sky_back.jpg"));
    // Animate SKYBOX:
//	ISceneNodeAnimator* sky_animation = smgr->createRotationAnimator(vector3df(0, 0.04f, 0));
//    sky->addAnimator(sky_animation);
//    sky_animation->drop();






    /// Create (Loads) STAGE:
	// scene::IAnimatedMesh* mesh = smgr->getMesh("cena.x"); // For an animated node
	scene::IMeshSceneNode* node1 = smgr->addMeshSceneNode(smgr->getMesh("cena.obj")->getMesh(0));
	node1->setPosition(vector3df(0, 100, 0));
	node1->setScale(core::vector3df(1,1,1));
	node1->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
    node1->setMaterialFlag(video::EMF_FOG_ENABLE, true); // Node is affected by FOG?
    node1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?

    for(u32 i=0; i < node1->getMaterialCount(); ++i){ node1->getMaterial(i).Lighting = true; }
    // node1->addShadowVolumeSceneNode();
	// smgr->setShadowColor(video::SColor(150,0,0,0));
	// effect->addShadowToNode(node1, EFT_NONE);




/*
    /// Create (Loads) GLASS (TRANSPARENT) OBJECTS:
	// scene::ISceneNode* node3 = smgr->addSceneNode(smgr->getMesh("tv.obj"));
	// scene::IAnimatedMesh* node3 = smgr->getMesh("tv.obj");
	scene::IMeshSceneNode* node3 = smgr->addMeshSceneNode(smgr->getMesh("glass.obj")->getMesh(0));
	node3->setPosition(vector3df(0, 100, 0));
	node3->setScale(core::vector3df(1,1,1));
	node3->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
    node3->setMaterialFlag(video::EMF_FOG_ENABLE, false); // Node is affected by FOG?
    node3->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true); // Render both sides?

    // node3->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);  // Transparent texture.
	// node3->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);      // Transparent texture.
	// node3->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);        // Transparent texture.
	node3->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);                    // Transparent texture.
	node3->getMaterial(0).TextureLayer[0].Texture = driver->getTexture("glass.png");


    // for(u32 i=0; i < node3->getMaterialCount(); ++i){ node3->getMaterial(i).Lighting = false; }
    // node2->addShadowVolumeSceneNode();
	// smgr->setShadowColor(video::SColor(150,0,0,0));
	// effect->addShadowToNode(node2, EFT_NONE);
*/




	/// Create (Loads) TV:
	// scene::ISceneNode* node2 = smgr->addSceneNode(smgr->getMesh("tv.obj"));
	// scene::IAnimatedMesh* node2 = smgr->getMesh("tv.obj");
	scene::IMeshSceneNode* node2 = smgr->addMeshSceneNode(smgr->getMesh("tv.obj")->getMesh(0));
	node2->setPosition(vector3df(0, 100, 0));
	node2->setScale(core::vector3df(1,1,1));
	node2->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
    node2->setMaterialFlag(video::EMF_FOG_ENABLE, false); // Node is affected by FOG?
    node2->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?

    for(u32 i=0; i < node2->getMaterialCount(); ++i){ node2->getMaterial(i).Lighting = true; }
    // node2->addShadowVolumeSceneNode();
	// smgr->setShadowColor(video::SColor(150,0,0,0));
	// effect->addShadowToNode(node2, EFT_NONE);




    /// VideoClip for TV, uses: createTextureAnimator ( http://irrlicht.sourceforge.net/docu/example008.html )
	if (node2)
    {
    // Load textures for animation
    core::array<video::ITexture*> textures;
    for (s32 n=1; n < 8; n++)
    {
    core::stringc tmp;
    tmp = "tv/"; tmp += n; tmp += ".jpg"; // Path to images: "tv" folder, included in the "world.zip" file
    video::ITexture* t = driver->getTexture( tmp.c_str() );
    textures.push_back(t);
    }
    // Create texture animator
    scene::ISceneNodeAnimator* tv = smgr->createTextureAnimator(textures, 500); // time between images (textures)
    // Add the animator
    node2->addAnimator(tv);
    // Drop the animator because it was created with a create() function
    tv->drop();
    }









    /// Create FOG: (E_FOG_TYPE ---> EFT_FOG_EXP or EFT_FOG_LINEAR or EFT_FOG_EXP2)
    // SYNTAX: driver->setFog(SColor, E_FOG_TYPE, f32, f32, f32, bool, bool);
    driver->setFog(video::SColor(0,0.0,0.0,0.0), video::EFT_FOG_LINEAR, 250, 1000, .003f, true, false);
    // node->setMaterialFlag(video::EMF_FOG_ENABLE, true);


    /// Create main LIGHT:
    // smgr->setAmbientLight(video::SColorf(1.0,1.0,1.0,0.0)); // Default
    smgr->setAmbientLight(video::SColorf(0.7,0.7,0.7,0.0));
    // Set a global ambient color:
	// effect->setAmbientColor(SColor(255, 100, 100, 100));


	// Adding shadow lights parameters - addShadowLight(): --- VSM Shadow Mapping ---
	// 1st parameter: specifies the shadow map resolution: power of two between 512 and 2048, maybe larger depending on your quality.
	// 2nd parameter: is the light position.
	// 3rd parameter: is the (look at) target.
	// 4th parameter: is the light color.
	// 5th parameter: the nearValue and farValue, be sure to set them appropriate to the current scene.
	// 6th parameter: is the FOV (Field of view).
	// effect->addShadowLight(SShadowLight(512, vector3df(0,280,-1200),  vector3df(0,0,1200), SColor(255,255,255,255), 0, 1000, 60 * DEGTORAD));

    // flashingLights();



    // SYNTAX --> http://irrlicht.sourceforge.net/docu/classirr_1_1video_1_1_s_colorf.html
    // SYNTAX --> SColorf (f32  r, f32  g, f32  b, f32  a=1.0f)

    /// Create small LIGHTS: (6 per column)
    int column = 0; // Column number
    scene::ILightSceneNode* light[column][6]; // Lights Array
    scene::ISceneNode* bill[column][6];  // Bills Array

    // Lights coordinates arrays, per column:
    irr::f32 aX[48] = { // X:
     258, 258, 258, 258, 258, 258,   // Column 1
     210, 210, 210, 210, 210, 210,   // Column 2
     158, 158, 158, 158, 158, 158,   // Column 3
     110, 110, 110, 110, 110, 110,   // Column 4
    -112,-112,-112,-112,-112,-112,   // Column 5
    -161,-161,-161,-161,-161,-161,   // Column 6
    -213,-213,-213,-213,-213,-213,   // Column 7
    -261,-261,-261,-261,-261,-261 }; // Column 8

    irr::f32 aY[48] = { // Y:
    243, 225, 207, 189, 171, 153,    // Column 1
    243, 225, 207, 189, 171, 153,    // Column 2
    243, 225, 207, 189, 171, 153,    // Column 3
    243, 225, 207, 189, 171, 153,    // Column 4
    243, 225, 207, 189, 171, 153,    // Column 5
    243, 225, 207, 189, 171, 153,    // Column 6
    243, 225, 207, 189, 171, 153,    // Column 7
    243, 225, 207, 189, 171, 153 };  // Column 8

    irr::f32 aZ[48] = { // Z:
    -593,-592,-590,-589,-588,-587,   // Column 1
    -593,-592,-590,-589,-588,-587,   // Column 2
    -593,-592,-590,-589,-588,-587,   // Column 3
    -593,-592,-590,-589,-588,-587,   // Column 4
    -593,-592,-590,-589,-588,-587,   // Column 5
    -593,-592,-590,-589,-588,-587,   // Column 6
    -593,-592,-590,-589,-588,-587,   // Column 7
    -593,-592,-590,-589,-588,-587 }; // Column 8



    // Add 6 lights per column loop:
//    for ( int i=0; i<48; i++ ){ // Loop trought lights (6 Lights X 8 columns = 48 Lights)
//	light[column][i] = smgr->addLightSceneNode(0, core::vector3df(aX[i],aY[i],aZ[i]), video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 50.0f);
//	bill[column][i] = smgr->addBillboardSceneNode(light[column][i], core::dimension2d<f32>(30, 30)); // Attach billboard to light.
//	bill[column][i]->setMaterialFlag(video::EMF_LIGHTING, false); // Bill Node is affected by LIGHT?
//	bill[column][i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
//	bill[column][i]->setMaterialTexture(0, driver->getTexture("estrela.png"));
//    }


/*
    // Add 6 lights per column loop:
    for ( int i=0; i<48; i++ ){ // Loop trought lights (6 Lights X 8 columns = 48 Lights)
	light[column][i] = smgr->addLightSceneNode(0, core::vector3df(aX[i],aY[i],aZ[i]), video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 50.0f);
	bill[column][i] = smgr->addBillboardSceneNode(light[column][i], core::dimension2d<f32>(20, 20)); // Attach billboard to light.
	bill[column][i]->setMaterialFlag(video::EMF_LIGHTING, false); // Bill Node is affected by LIGHT?
	bill[column][i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	bill[column][i]->setMaterialTexture(0, driver->getTexture("estrela.png"));
    }
*/



/*
    // Add 6 lights per column loop:
    for ( int i=0; i<6; i++ ){ // Loop trought lights (6 Lights X 8 columns = 48 Lights)
	light[column][i]->setVisible(false);
	bill[column][i]->setVisible(false);
    }
*/



/*
	// init some nodes for test
	scene::IAnimatedMeshSceneNode* node[99];
	scene::ILightSceneNode* lightz[99];
    scene::ISceneNode* billz[99];

	for (int t=0; t<50; t++)
	{
    float x=rand()%999 -500;
    float y=rand()%999;
    float z=rand()%999 -500;

    lightz[t] = smgr->addLightSceneNode(0, core::vector3df(0,0,0), video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 50.0f);
    lightz[t]->setPosition(core::vector3df(x,y,z));

    billz[t]  = smgr->addBillboardSceneNode(lightz[t], core::dimension2d<f32>(20, 20));
    billz[t]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    billz[t]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
    billz[t]->setMaterialTexture(0, driver->getTexture("estrela.png"));
	}
*/







    /// Create particle system: (BOLINHAS)
	scene::IParticleSystemSceneNode* ps2 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em2 = ps2->createBoxEmitter(
	core::aabbox3d<f32>(0,0,0,30,20,5),  // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                         // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.1f,0.0f),    // initial direction. Y = speed
	200,400,                             // emit rate
	// video::SColor(0,0,0,255),         // darkest color
	video::SColor(0,0,0,0),              // darkest color
	video::SColor(255,255,255,255),      // brightest color
	5000,5000,10,                         // min age, max age, angle
	core::dimension2df(1.0f,1.0f),       // min size
	core::dimension2df(6.0f,6.0f));      // max size

	ps2->setEmitter(em2); // Grabs the emitter
	em2->drop(); // We can drop it here, without deleting it

	// scene::IParticleAffector* paf2 = ps2->createFadeOutParticleAffector(); // Fade out particles
	// ps2->addAffector(paf2); // Same goes for the affector
	// paf2->drop();

	ps2->setPosition(core::vector3df(-550,100,-600)); // x, y, z: profundidade
	// ps2->setScale(core::vector3df(100,100,100));
	ps2->setScale(core::vector3df(40,40,40));
	ps2->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps2->setMaterialTexture(0, driver->getTexture("bolinhas.png"));
	// ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps2->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps2->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps2);








    /// Create particle system: (RIGHT FIRE)
	scene::IParticleSystemSceneNode* ps3 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em3 = ps3->createBoxEmitter(
	core::aabbox3d<f32>(-7,0,-7,7,1,7),  // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                         // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.1f,0.0f),    // initial direction. Y = speed
	90,150,                               // emit rate
	// video::SColor(0,0,0,255),         // darkest color
	video::SColor(0,255,255,255),        // darkest color
	video::SColor(0,255,255,255),        // brightest color
	400,800,0,                          // min age, max age, angle
	core::dimension2df(5.f,5.f),         // min size
	core::dimension2df(10.f,10.f));      // max size

	ps3->setEmitter(em3); // Grabs the emitter
	em3->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf3 = ps3->createFadeOutParticleAffector(); // Fade out particles
	ps3->addAffector(paf3); // Same goes for the affector
	paf3->drop();

	ps3->setPosition(core::vector3df(-315,155,-545)); // x, y, z: profundidade
	// ps3->setScale(core::vector3df(100,100,100));
	ps3->setScale(core::vector3df(1,1,1));
	ps3->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps3->setMaterialTexture(0, driver->getTexture("fire.png"));
	// ps3->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps3->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps3->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps3->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps3);






    /// Create particle system: (LEFT FIRE)
	scene::IParticleSystemSceneNode* ps4 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em4 = ps4->createBoxEmitter(
	core::aabbox3d<f32>(-7,0,-7,7,1,7),  // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                         // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.1f,0.0f),     // initial direction. Y = speed
	90,150,                               // emit rate
	// video::SColor(0,0,0,255),         // darkest color
	video::SColor(0,255,255,255),        // darkest color
	video::SColor(0,255,255,255),        // brightest color
	400,800,0,                          // min age, max age, angle
	core::dimension2df(5.f,5.f),        // min size
	core::dimension2df(10.f,10.f));      // max size

	ps4->setEmitter(em4); // Grabs the emitter
	em4->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf4 = ps4->createFadeOutParticleAffector(); // Fade out particles
	ps4->addAffector(paf4); // Same goes for the affector
	paf4->drop();

	ps4->setPosition(core::vector3df(330,155,-545)); // x, y, z: profundidade
	// ps4->setScale(core::vector3df(100,100,100));
	ps4->setScale(core::vector3df(1,1,1));
	ps4->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps4->setMaterialTexture(0, driver->getTexture("fire.png"));
	// ps4->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps4->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps4->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps4->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps4->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps4);















/*
    /// Light only for NAU:
    ILightSceneNode* lumiere = smgr->addLightSceneNode(node1, vector3df(200,200,200), SColorf(0.8f, 0.8f, 1.2f, 0.0f), 100.0f);
    lumiere->getLightData().DiffuseColor =  SColorf(0.6, 1.0, 1.0, 1);
    lumiere->getLightData().SpecularColor = SColorf(0.6, 0.0, 0.0, 1);
    node1->getMaterial(0).Shininess = 20.0f;
    node1->getMaterial(0).EmissiveColor = SColor(255,0,255,255);

*/

    // node = smgr->addMeshSceneNode(mesh->getMesh(0));
	// TEXT: Player name

	// smgr->addTextSceneNode(guienv->getBuiltInFont(), L"RONALDO", video::SColor(255,255,255,255), node7, core::vector3df(node7->getPosition().X,node7->getPosition().Y+2,node7->getPosition().Z)); // video::SColor(255,255,255,255)


/*
    /// Create CAMERA:
    scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
    camera->setPosition(core::vector3df(600,60,600));
    camera->setTarget(core::vector3df(0,0,0));
    // camera->setTarget(node1.Position);
    camera->setFarValue(3000.0f);
*/




    /// Camera ---> Keyboard:
    irr::SKeyMap keyMap[5];
    keyMap[0].Action =  irr::EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = irr::KEY_KEY_W;
    keyMap[1].Action =  irr::EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = irr::KEY_KEY_S;
    keyMap[2].Action =  irr::EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = irr::KEY_KEY_A;
    keyMap[3].Action =  irr::EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = irr::KEY_KEY_D;
    keyMap[4].Action =  irr::EKA_JUMP_UP;
    keyMap[4].KeyCode = irr::KEY_SPACE;


    /// Camera:
    irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.1f, -1, keyMap, 5, false, 0.4);
    // irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS();
    camera->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera->setPosition(core::vector3df(0,110,-50));  // Default Camera Position
    camera->setTarget(core::vector3df(0,110,-1000));  // Default Camera Target

    // make our players camera
    // smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));




/*
    // Animate Camera:
	ISceneNodeAnimator* camera_animation = smgr->createRotationAnimator(vector3df(0.0f, 0.0f, 0.5f));
    camera->addAnimator(camera_animation);
    camera_animation->drop();
*/

    /// Add GUI image:
	guienv->addImage(driver->getTexture("logotipo.png"),position2d<int>(650,5));
	// guienv->addGUIElement("myButton", IGUIButton(core::rect< s32> (0,0,200,50), 0, 101, wchar_t "mybutton", wchar_t "clickhere")  )

    /// Other Stuff:
	device->getCursorControl()->setVisible(false); // Hide mouse cursor




    irr::f32 al = 0.0;  // for Ambient Light
    bool fadeIn = true; // for Ambient Light

    int speed = 0; // Lights speed


    /// Run all:
	int lastFPS = -1;
	while(device->run())
	{
//		if (device->isWindowActive())
//		{
			driver->beginScene(true, true, video::SColor(0,0,0,0));
			int fps = driver->getFPS();

            /// Camera always target ...
			// camera->setTarget(vector3df(node1->getPosition().X+1.0, node1->getPosition().Y, node1->getPosition().Z));

//			if (lastFPS != fps)
//			{
//				core::stringw str = L"IMP - Irrlicht Media Player [DRIVE: "; str += driver->getName(); str += "] [FPS: "; str += fps; str += "]";
//				device->setWindowCaption(str.c_str());
//				lastFPS = fps;
//			}




////			/// Flashing lights: ( For Multy Colors )
////			// Random Colors:
////			float red =   rand()%2;
////            float green = rand()%2;
////            float blue =  rand()%2;
////            // Apply Random Colors to lights:
////            SLight lights_data;
////            lights_data.DiffuseColor = video::SColorf(red,green,blue,1.0f); // SYNTAX: f32 r, f32 g, f32 b, f32 a = 1.0f
////            // Apply Random Colors to scene:
////            node1->getMaterial(0).DiffuseColor = video::SColor(red,green,blue,1.0f);
////            // Apply Random Colors to Bills:
////            // billCA[b]->getMaterial(0).DiffuseColor = video::SColor(red,green,blue,1.0f);
////
////
////
////
////            if(speed > 10){
////            /// Ambient light: ( Fade in/out )
////            smgr->setAmbientLight(video::SColorf(al,al,al,0.0)); // Default Ambient Light
////            if(al >= 1.0){ fadeIn = false; };
////            if(al <= 0.7){ fadeIn = true; };
////            if(fadeIn){ al+=0.1; } else { al-=0.1; };
////            }
////
////
////
////			if(speed > 10){
////            /// Flashing lights: ( Show/Hide by column )
////			// Reset all lights to hidden:
////			for ( int i=-4; i<48; i++ ){ light[column][i]->setVisible(false); }
////
////
////            // Choose random column to visible:
////            int k = rand()%8; // from (0 to 7)
////
////            if(k == 0){ // 1st column lights loop:
////			for ( int i=-4; i<2; i++ ){ light[column][i]->setVisible(true); }
////            }
////            if(k == 1){ // 2nd column lights loop:
////			for ( int i=2; i<8; i++ ){ light[column][i]->setVisible(true); }
////            }
////            if(k == 2){ // 3rd column lights loop:
////			for ( int i=8; i<14; i++ ){ light[column][i]->setVisible(true); }
////            }
////            if(k == 3){ // 4th column lights loop:
////			for ( int i=14; i<20; i++ ){ light[column][i]->setVisible(true); }
////            }
////            if(k == 4){ // 5th column lights loop:
////			for ( int i=20; i<26; i++ ){ light[column][i]->setVisible(true); }
////            }
////            if(k == 5){ // 6th column lights loop:
////			for ( int i=26; i<32; i++ ){ light[column][i]->setVisible(true); }
////            }
////            if(k == 6){ // 7th column lights loop:
////			for ( int i=32; i<38; i++ ){ light[column][i]->setVisible(true); }
////            }
////            if(k == 7){ // 8th column lights loop:
////			for ( int i=38; i<48; i++ ){ light[column][i]->setVisible(true); }
////            }
////
////			speed = 0;
////			}
////
////            speed ++;








/*
				/// Flashing lights: ( For Multy Colors )
				// Random Colors:
				float r = rand()%2;
                float g = rand()%2;
                float b = rand()%2;
                // Apply Random Colors to lights:
                SLight lights_data;
                lights_data.DiffuseColor = video::SColorf(r,g,b,1.0f); // SYNTAX: f32 r, f32 g, f32 b, f32 a = 1.0f
                // Apply Random Colors to scene:
                node1->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                // Apply Random Colors to Bills:
                for (int b=0; b<6; b++) {
                    billCA[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    billCB[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    billCC[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    billCD[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    billCE[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    billCF[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    billCG[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    billCH[b]->getMaterial(0).DiffuseColor = video::SColor(r,g,b,1.0f);
                    }
                // Random Lights Visibility:
				for (int t=0; t<6; t++) // Number of lights, per column
				{
				int x = rand()%2;
				if(x == 0){
				    lightCA[t]->setVisible(true);
				    lightCB[t]->setVisible(true);
				    lightCC[t]->setVisible(true);
				    lightCD[t]->setVisible(true);
				    lightCE[t]->setVisible(true);
				    lightCF[t]->setVisible(true);
				    lightCG[t]->setVisible(true);
				    lightCH[t]->setVisible(true);
				    };
				if(x == 1){
				    lightCA[t]->setVisible(false);
                    lightCB[t]->setVisible(false);
                    lightCC[t]->setVisible(false);
                    lightCD[t]->setVisible(false);
                    lightCE[t]->setVisible(false);
                    lightCF[t]->setVisible(false);
                    lightCG[t]->setVisible(false);
                    lightCH[t]->setVisible(false);
				    };
                lightCA[t]->setLightData(lights_data);
                lightCB[t]->setLightData(lights_data);
                lightCC[t]->setLightData(lights_data);
                lightCD[t]->setLightData(lights_data);
                lightCE[t]->setLightData(lights_data);
                lightCF[t]->setLightData(lights_data);
                lightCG[t]->setLightData(lights_data);
                lightCH[t]->setLightData(lights_data);
				}
*/

/*
				/// Flashing lights: ( For white colors only )
                // Random Lights Visibility:
				for (int i=0; i<6; i++) // Number of lights, per column
				{
				int x = rand()%2;
				if(x == 0){
				    lightCA[i]->setVisible(true);
				    lightCB[i]->setVisible(true);
				    lightCC[i]->setVisible(true);
				    lightCD[i]->setVisible(true);
				    lightCE[i]->setVisible(true);
				    lightCF[i]->setVisible(true);
				    lightCG[i]->setVisible(true);
				    lightCH[i]->setVisible(true);
				    };
				if(x == 1){
				    lightCA[i]->setVisible(false);
                    lightCB[i]->setVisible(false);
                    lightCC[i]->setVisible(false);
                    lightCD[i]->setVisible(false);
                    lightCE[i]->setVisible(false);
                    lightCF[i]->setVisible(false);
                    lightCG[i]->setVisible(false);
                    lightCH[i]->setVisible(false);
                    };
				};
*/





			// effect->update();  // Shadows
			// flare->render();   // Draw flares
			 guienv->drawAll(); // Draw GUI
			smgr->drawAll();
			// effect->update();  // Shadows  (effect->update() replaces: smgr->drawAll())
			guienv->drawAll();    // Draw GUI
			driver->endScene();
	//	}
//		else
//		{
//        	device->yield();
//		}
	}
//	device->drop();
	return 0;
}



