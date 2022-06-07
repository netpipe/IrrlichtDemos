/** Example 008 SpecialFX

This tutorials describes how to do special effects. It shows how to use stencil
buffer shadows, the particle system, billboards, dynamic light, and the water
surface scene node.

We start like in some tutorials before. Please note that this time, the
'shadows' flag in createDevice() is set to true, for we want to have a dynamic
shadow casted from an animated character. If this example runs too slow,
set it to false. The Irrlicht Engine checks if your hardware doesn't support
the stencil buffer, and disables shadows by itself, but just in case the demo
runs slow on your hardware.
*/

#include <irrlicht.h>
#include <iostream>
#include "driverChoice.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{
	// ask if user would like shadows
	//char i;
	//printf("Please press 'y' if you want to use realtime shadows.\n");

	//std::cin >> i;

	const bool shadows = 1;//(i == 'y');

	// ask user for driver
//	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
//	if (driverType==video::EDT_COUNT)
//		return 1;


	/*
	Create device and exit if creation failed. We make the stencil flag
	optional to avoid slow screen modes for runs without shadows.
	*/

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480),
		16, false, shadows);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
//	driver->setFog(video::SColor(0,138,125,81), video::EFT_FOG_LINEAR, 250, 1000, .003f, true, false);



	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(core::vector3df(-50,50,-150));
	camera->setFarValue(10000.0f); // this increase a shadow visible range.

	/*
	For our environment, we load a .3ds file. It is a small room I modelled
	with Anim8or and exported into the 3ds format because the Irrlicht
	Engine does not support the .an8 format. I am a very bad 3d graphic
	artist, and so the texture mapping is not very nice in this model.
	Luckily I am a better programmer than artist, and so the Irrlicht
	Engine is able to create a cool texture mapping for me: Just use the
	mesh manipulator and create a planar texture mapping for the mesh. If
	you want to see the mapping I made with Anim8or, uncomment this line. I
	also did not figure out how to set the material right in Anim8or, it
	has a specular light color which I don't really like. I'll switch it
	off too with this code.
	*/
	device->getFileSystem()->addFileArchive("../../media/map-20kdm2.pk3");

	scene::IAnimatedMesh* roomMesh = smgr->getMesh("20kdm2.bsp");//smgr->getMesh("../../media/room.3ds");
	scene::ISceneNode* room = 0;
	scene::ISceneNode* earth = 0;

	if (roomMesh)
	{
		// The Room mesh doesn't have proper Texture Mapping on the
		// floor, so we can recreate them on runtime
		smgr->getMeshManipulator()->makePlanarTextureMapping(
				roomMesh->getMesh(0), 0.003f);

		/*
		Now for the first exciting thing: If we successfully loaded the
		mesh we need to apply textures to it. Because we want this room
		to be displayed with a very cool material, we have to do a
		little bit more than just set the textures. Instead of only
		loading a color map as usual, we also load a height map which
		is simply a grayscale texture. From this height map, we create
		a normal map which we will set as second texture of the room.
		If you already have a normal map, you could directly set it,
		but I simply didn't find a nice normal map for this texture.
		The normal map texture is being generated by the
		makeNormalMapTexture method of the VideoDriver. The second
		parameter specifies the height of the heightmap. If you set it
		to a bigger value, the map will look more rocky.
		*/

		video::ITexture* normalMap =
			driver->getTexture("../../media/rockwall_height.bmp");

		if (normalMap)
			driver->makeNormalMapTexture(normalMap, 9.0f);
/*
		// The Normal Map and the displacement map/height map in the alpha channel
		video::ITexture* normalMap =
			driver->getTexture("../../media/rockwall_NRM.tga");
*/
		/*
		But just setting color and normal map is not everything. The
		material we want to use needs some additional informations per
		vertex like tangents and binormals. Because we are too lazy to
		calculate that information now, we let Irrlicht do this for us.
		That's why we call IMeshManipulator::createMeshWithTangents().
		It creates a mesh copy with tangents and binormals from another
		mesh. After we've done that, we simply create a standard
		mesh scene node with this mesh copy, set color and normal map
		and adjust some other material settings. Note that we set
		EMF_FOG_ENABLE to true to enable fog in the room.
		*/


		scene::IMesh* tangentMesh = smgr->getMeshManipulator()->
				createMeshWithTangents(roomMesh->getMesh(0));

		room = smgr->addOctreeSceneNode(tangentMesh, 0, -1, 1024);
		//->addMeshSceneNode(tangentMesh);
		room->setMaterialTexture(0,
				driver->getTexture("../../media/rockwall.bmp"));
		room->setMaterialTexture(1, normalMap);

		// Stones don't glitter..
		room->getMaterial(0).SpecularColor.set(0,0,0,0);
		room->getMaterial(0).Shininess = 0.f;

		//room->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		room->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
		//room->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
		// adjust height for parallax effect
		room->getMaterial(0).MaterialTypeParam = 1.f / 64.f;
room->setMaterialFlag(video::EMF_LIGHTING, true);
		// drop mesh because we created it with a create.. call.
		tangentMesh->drop();
	}
	/*
	Now, for the first special effect: Animated water. It works like this:
	The WaterSurfaceSceneNode takes a mesh as input and makes it wave like
	a water surface. And if we let this scene node use a nice material like
	the EMT_REFLECTION_2_LAYER, it looks really cool. We are doing this
	with the next few lines of code. As input mesh, we create a hill plane
	mesh, without hills. But any other mesh could be used for this, you
	could even use the room.3ds (which would look really strange) if you
	want to.
	*/

	scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh( "myHill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<u32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));

	scene::ISceneNode* node = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	node->setPosition(core::vector3df(0,7,0));

	node->setMaterialTexture(0, driver->getTexture("../../media/stones.jpg"));
	node->setMaterialTexture(1, driver->getTexture("../../media/water.jpg"));

	//node->setMaterialType(video::EMT_REFLECTION_2_LAYER);
//node->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
node->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
	/*
	The second special effect is very basic, I bet you saw it already in
	some Irrlicht Engine demos: A transparent billboard combined with a
	dynamic light. We simply create a light scene node, let it fly around,
	and to make it look more cool, we attach a billboard scene node to it.
	*/

	// create light

	scene::ISceneNode* lnode = smgr->addLightSceneNode(0, core::vector3df(0,150,0),
		video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
	scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyCircleAnimator (core::vector3df(0,150,0),250.0f);
	lnode->addAnimator(anim);
	anim->drop();
//	 lnode->setParent(camera);
	// video::ELT_SPOT;

		lnode = smgr->addLightSceneNode(0, core::vector3df(0,250,0),
		video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
	scene::ISceneNodeAnimator* anim2 = 0;
	anim2 = smgr->createFlyCircleAnimator (core::vector3df(100,150,100),250.0f);
	lnode->addAnimator(anim2);
	anim2->drop();

	// attach billboard to light

	node = smgr->addBillboardSceneNode(node, core::dimension2d<f32>(50, 50));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	node->setMaterialTexture(0, driver->getTexture("../../media/particlewhite.bmp"));

	/*
	The next special effect is a lot more interesting: A particle system.
	The particle system in the Irrlicht Engine is quite modular and
	extensible, but yet easy to use. There is a particle system scene node
	into which you can put a particle emitter, which makes particles come out
	of nothing. These emitters are quite flexible and usually have lots of
	parameters like direction, amount, and color of the particles they
	create.

	There are different emitters, for example a point emitter which lets
	particles pop out at a fixed point. If the particle emitters available
	in the engine are not enough for you, you can easily create your own
	ones, you'll simply have to create a class derived from the
	IParticleEmitter interface and attach it to the particle system using
	setEmitter(). In this example we create a box particle emitter, which
	creates particles randomly inside a box. The parameters define the box,
	direction of the particles, minimal and maximal new particles per
	second, color, and minimal and maximal lifetime of the particles.

	Because only with emitters particle system would be a little bit
	boring, there are particle affectors which modify particles while
	they fly around. Affectors can be added to a particle system for
	simulating additional effects like gravity or wind.
	The particle affector we use in this example is an affector which
	modifies the color of the particles: It lets them fade out. Like the
	particle emitters, additional particle affectors can also be
	implemented by you, simply derive a class from IParticleAffector and
	add it with addAffector().

	After we set a nice material to the particle system, we have a cool
	looking camp fire. By adjusting material, texture, particle emitter,
	and affector parameters, it is also easily possible to create smoke,
	rain, explosions, snow, and so on.
	*/

	// create a particle system

	scene::IParticleSystemSceneNode* ps =
		smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7), // emitter size
		core::vector3df(0.0f,0.06f,0.0f),   // initial direction
		80,100,                             // emit rate
		video::SColor(0,255,255,255),       // darkest color
		video::SColor(0,255,255,255),       // brightest color
		800,2000,0,                         // min and max age, angle
		core::dimension2df(10.f,10.f),         // min size
		core::dimension2df(20.f,20.f));        // max size

	ps->setEmitter(em); // this grabs the emitter
	em->drop(); // so we can drop it here without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

	ps->addAffector(paf); // same goes for the affector
	paf->drop();

	ps->setPosition(core::vector3df(-70,60,40));
	ps->setScale(core::vector3df(2,2,2));
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("../../media/fire.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	/*
	Next we add a volumetric light node, which adds a glowing fake area light to
	the scene. Like with the billboards and particle systems we also assign a
	texture for the desired effect, though this time we'll use a texture animator
	to create the illusion of a magical glowing area effect.
	*/
//	scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(0, -1,
//				32,                              // Subdivisions on U axis
//				32,                              // Subdivisions on V axis
//				video::SColor(0, 255, 255, 255), // foot color
//				video::SColor(0, 0, 0, 0));      // tail color
//
//	if (n)
//	{
//		n->setScale(core::vector3df(56.0f, 56.0f, 56.0f));
//		n->setPosition(core::vector3df(-120,50,40));
//
//		// load textures for animation
//		core::array<video::ITexture*> textures;
//		for (s32 g=7; g > 0; --g)
//		{
//			core::stringc tmp;
//			tmp = "../../media/portal";
//			tmp += g;
//			tmp += ".bmp";
//			video::ITexture* t = driver->getTexture( tmp.c_str() );
//			textures.push_back(t);
//		}
//
//		// create texture animator
//		scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(textures, 150);
//
//		// add the animator
//		n->addAnimator(glow);
//
//		// drop the animator because it was created with a create() function
//		glow->drop();
//	}

	/*
	As our last special effect, we want a dynamic shadow be casted from an
	animated character. For this we load a DirectX .x model and place it
	into our world. For creating the shadow, we simply need to call
	addShadowVolumeSceneNode(). The color of shadows is only adjustable
	globally for all shadows, by calling ISceneManager::setShadowColor().
	Voila, here is our dynamic shadow.

	Because the character is a little bit too small for this scene, we make
	it bigger using setScale(). And because the character is lighted by a
	dynamic light, we need to normalize the normals to make the lighting on
	it correct. This is always necessary if the scale of a dynamic lighted
	model is not (1,1,1). Otherwise it would get too dark or too bright
	because the normals will be scaled too.
	*/

	// add animated character

	mesh = smgr->getMesh("../../media/dwarf.x");
	scene::IAnimatedMeshSceneNode* anode = 0;

	anode = smgr->addAnimatedMeshSceneNode(mesh);
	anode->setPosition(core::vector3df(-50,20,-60));
	anode->setAnimationSpeed(15);

	// add shadow
	anode->addShadowVolumeSceneNode();
	smgr->setShadowColor(video::SColor(150,0,0,0));
	//smgr->setOptimization(scene::ESV_NONE);

//IShadowVolumeSceneNode::setOptimization set to ESV_NONE

	// make the model a little bit bigger and normalize its normals
	// because of the scaling, for correct lighting
	anode->setScale(core::vector3df(2,2,2));
	anode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	/*
	Finally we simply have to draw everything, that's all.
	*/

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	s32 lastFPS = -1;

	room->setPosition(core::vector3df(-200,-200,-200));

//https://irrlicht.sourceforge.io/forum/viewtopic.php?p=266998&hilit=flashlight#p266998
	scene::ILightSceneNode* flashlight = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f, 0.6f, 0.7f, 0.0f), 100.0f);

//	smgr->addLightSceneNode(flashlight);
                video::SLight flashlightData;
            //    flashlightData.Direction= camera->getTarget();//camera->getRotation();
                flashlightData.Direction= camera->getRotation();
                flashlightData.OuterCone= 40;
                flashlightData.InnerCone = 20;
//                flashlightData.OuterCone = (float)cos(flashlightData.OuterCone*3.141615926f/180.0f);
//                flashlightData.InnerCone = (float)cos(flashlightData.InnerCone*3.141615926f/180.0f);
                flashlightData.Position= camera->getPosition();
                flashlightData.Falloff= 5;
               // flashlightData.CastShadows=1;
                flashlightData.Type= video::ELT_SPOT;
                flashlight->setLightData(flashlightData);
                flashlight->setRadius(80);
            //    flashlight->setParent(camera);
            // flashlight->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            flashlight->setVisible(1);

        //        lnode->setLightData(flashlightData);

	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene();


//flashlight->updateAbsolutePosition();
//                lnode->setPosition(camera->getPosition());
//                lnode->setRotation(camera->getRotation());
//                flashlightData.Position= camera->getPosition();
//			//	flashlightData.Direction= camera->getTarget();//camera->getRotation();
//				flashlightData.Direction= camera->getRotation();
//                flashlight->setLightData(flashlightData);
// flashlight->setPosition(camera->getPosition());
// flashlight->setRotation(camera->getRotation());


		smgr->drawAll();


		driver->endScene();

		const s32 fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"Irrlicht Engine - SpecialFX example [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
		device->sleep(30);
	}

	device->drop();

	return 0;
}

/*
**/