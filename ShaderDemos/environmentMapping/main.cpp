#include <irrlicht.h>
#include <iostream>

/**Cylindrical mapping

What is? 
it is a texture projection  system which allows to read a texture as if it was wrapped into a cylinder (or a sphere)

Why cylindrical mapping? 
the reflections on real time graphics have been many times subject to discussion about which is the best
approach. Cube maps, dual paraboloid maps, and spherical projections all share this objective, but with different results.

Cubemaps: require of special support to use 6 textures at a time. Their main advantage is that is very easy to create cubemaps and to 
use them. Their main drawback, the cubemap is a special texture type, which may require diferent set ups from the regular textures.

Dual Paraboloid maps: They are 2 textures at a time. Their main advantage is that, for instance, to generate real time reflections
they only require 2 renders, instead of 6, if we used a cubemap approach, and only 2 regular textures, but this render isn't trivial, 
in the sense that we need a special projection for the dual paraboloids to work, they aren't as much efficient as a cube map would 
be, and use two texture slots, instead of one.

Spherical maps: It is a single texture of an image of a sphere centered on it, in which a spherical projection is applied. Their main
advantage is that it is a single standard texture map, their disadvantages are is that this texture map is bound to a single direction,
i.e. you can't change the reflection direction to get the reflections from another point of view, and it is even harder to sythethise 
a spherical map, and also, involve somewhat complex maths, like square roots

The cylindrical projections, on the other hand, share some benefits of all the above methods, although, with some disadvantages as well,
eventhough, those can be minor if they're done with some careful set ups. They use a single standard texture, they don't have special 
projection needs to render, or perhaps, these can be addressed with ease, and they aren't bound to any direction, so they can be 
projected over world directions, not only the view one, and their maths are somewhat simple, as they require a single normalization
that is done in a 2D vector.

Eventhough it could be appropriate to call this approach "spherical projection" it is closer to a cylinder in the sense that the height 
(the V coordinate of the texture maps) is calculated completely independent of the horizontal coordinate, being able to be 
free in that meaning, and able to reach any height, athough, if it is clamped to the texture space, this can be used as a better 
"spherical maps" projection, given they can point to any direction
*/

inline irr::f32 max(irr::f32 a,irr::f32 b)
{
	return (a >= b ? a : b);
}


//maps a 2D texture coordinate into a 3D vector
irr::core::vector3df uv2xyz(irr::core::vector2df inTexCoord)
{
	irr::core::vector3df vec;
	irr::core::vector2df texCoord;

	//texture space normalization
	texCoord.X = inTexCoord.X-floor(inTexCoord.X);
	texCoord.Y = inTexCoord.Y-floor(inTexCoord.Y);
	
	vec.X = -sin(texCoord.X*irr::core::PI*2.0f)*sin(texCoord.Y*irr::core::PI);
	vec.Y = cos(texCoord.Y*irr::core::PI);
	vec.Z = cos(texCoord.X*irr::core::PI*2.0f)*sin(texCoord.Y*irr::core::PI);

	return vec.normalize();
}

//maps a 3D vector into a texture
irr::core::vector2df xyz2uv(irr::core::vector3df vector)
{
	irr::core::vector2df texCoords;
	irr::core::vector3df vecSQ = vector;

	if(vecSQ.X<0)
		vecSQ.X = -vecSQ.X;
	if(vecSQ.Y<0)
		vecSQ.Y = -vecSQ.Y;
	if(vecSQ.Z<0)
		vecSQ.Z = -vecSQ.Z;
	
	if(vecSQ.X>vecSQ.Y && vecSQ.X>vecSQ.Z)
	{
		if(vector.X>=0)
		{
			texCoords.X = vector.Z/vector.X*0.5f;
			texCoords.Y = -vector.Y/vector.X*0.5f;
		}
		else
		{
			texCoords.X = vector.Z/vector.X*0.5f;
			texCoords.Y = vector.Y/vector.X*0.5f;
		}	
	}
	else
	{
		if(vecSQ.Y>vecSQ.X && vecSQ.Y>vecSQ.Z)
		{
			if(vector.Y>=0)
			{
				texCoords.X = -vector.Z/vector.Y*0.5f;
				texCoords.Y = -vector.X/vector.Y*0.5f;
			}
			else
			{
				texCoords.X = vector.Z/vector.Y*0.5f;
				texCoords.Y = -vector.X/vector.Y*0.5f;			
			}
		}
		else
		{
			if(vector.Z>=0)
			{
				texCoords.X = -vector.X/vector.Z*0.5f;
				texCoords.Y = -vector.Y/vector.Z*0.5f;						
			}
			else
			{
				texCoords.X = -vector.X/vector.Z*0.5f;
				texCoords.Y = vector.Y/vector.Z*0.5f;			
			}
		}
	}

	texCoords += irr::core::vector2df(0.5f,0.5f);

	return texCoords;
}

//reads a single pixel from a texture map
irr::video::SColor readTexturemap(irr::core::vector2df texCoord,irr::video::IImage* image)
{
	irr::core::vector2df nTexCoord;
	irr::core::vector2df blend;
	irr::core::vector2di baseCoord;
	irr::core::dimension2du imageResolution = image->getDimension();
	irr::video::SColor color;
	
	nTexCoord.X = texCoord.X-floor(texCoord.X);
	nTexCoord.Y = texCoord.Y-floor(texCoord.Y);

	baseCoord.X = floor(imageResolution.Width*nTexCoord.X);
	baseCoord.Y = floor(imageResolution.Height*nTexCoord.Y);

	blend.X = imageResolution.Width*nTexCoord.X-imageResolution.Width;
	blend.Y = imageResolution.Height*nTexCoord.Y-imageResolution.Height;

	color = image->getPixel(baseCoord.X,baseCoord.Y);
	irr::f32 clampedCoord = baseCoord.X+1 >= imageResolution.Width ? imageResolution.Width-1 : baseCoord.X+1;
	color = color.getInterpolated(image->getPixel(clampedCoord,baseCoord.Y),blend.X);
	clampedCoord = baseCoord.Y+1>=imageResolution.Height?imageResolution.Height-1:baseCoord.Y+1;
	color = color.getInterpolated(image->getPixel(baseCoord.X,clampedCoord),blend.Y);

	return color;
}


//reads a pixel from a cubemap. The images are a vector of 6 images
irr::video::SColor readCubemap(irr::core::vector3df vector,irr::core::array< irr::video::IImage* >& images)
{
	//Order: 0,1 left,right
	//Order: 2,3 down,up
	//Order: 4:5 front,back
	unsigned int facing;

	irr::core::vector3df vecSQ = vector;
	vecSQ.X *= vecSQ.X;
	vecSQ.Y *= vecSQ.Y;
	vecSQ.Z *= vecSQ.Z;

	irr::f32 maximal = max(vecSQ.X,max(vecSQ.Y,vecSQ.Z));

	if(maximal == vecSQ.X)
	{
		facing = (vector.X<0? 0 : 1);
	}
	else
	{
		if(maximal == vecSQ.Y)
		{
			facing = (vector.Y<0? 2 : 3);
		}
		else
		{
			facing = (vector.Z<0? 4 : 5);
		}
	}

	irr::core::vector2df texCoords = xyz2uv(vector);

	return readTexturemap(texCoords,images[facing]);
}

//Generates a skydome out of a cubemap
void cubemap2skydome(irr::video::IImage* outSkydome,irr::core::array< irr::video::IImage* >& inCubemap)
{
	const irr::core::dimension2du skydomeResolution = outSkydome->getDimension();
	unsigned int i,j;
	irr::video::SColor color;
	irr::core::vector3df vector;
	irr::core::vector2df uv;

	for(i=0;i<skydomeResolution.Width;i++)
	{
		for(j=0;j<skydomeResolution.Height;j++)
		{
			uv.X = ((irr::f32)i)/((irr::f32)skydomeResolution.Width+0.5f/(irr::f32)skydomeResolution.Width);
			uv.Y = ((irr::f32)j)/((irr::f32)skydomeResolution.Height+0.5f/(irr::f32)skydomeResolution.Height);
			vector = uv2xyz(uv);
			color = readCubemap(vector,inCubemap);

			outSkydome->setPixel(i,j,color);	
		}
	}
}

//We use a shader, thus we will need a shader callback
class phongCB : public irr::video::IShaderConstantSetCallBack
{
	bool oneTimeInit;
	bool isGL;
	int matWorldViewProjection;
	int matWorld;
	int matViewInverse;
	int mipsCount;
	int baseMap;
	int reflectionMap;
	irr::video::IVideoDriver* driver;
	irr::video::IGPUProgrammingServices* gpu;
	irr::f32 mipmapsCount;

public:

	phongCB()
	{
		oneTimeInit = true;
	}

	void OnSetMaterial(const irr::video::SMaterial& material)
	{
		irr::video::ITexture* texture;

		texture = material.getTexture(1);
		irr::f32 size = max(texture->getSize().Width,texture->getSize().Height);
		mipmapsCount = floor(log(size)/log(2));
	}

	void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::core::matrix4 matrix;
		if(oneTimeInit)
		{
			driver = services->getVideoDriver();
			gpu = driver->getGPUProgrammingServices();
			isGL = driver->getDriverType() == irr::video::EDT_OPENGL;

			if(isGL)
			{
				baseMap = services->getPixelShaderConstantID("baseMap");
				reflectionMap = services->getPixelShaderConstantID("reflectionMap");
			}
			else
			{
				matWorldViewProjection = services->getVertexShaderConstantID("matWorldViewProjection");
				matWorld = services->getVertexShaderConstantID("matWorld");
			}

			matViewInverse = services->getVertexShaderConstantID("matViewInverse");
			mipsCount = services->getPixelShaderConstantID("mipsCount");

			oneTimeInit = false;
		}

		if(isGL)
		{
			matrix = driver->getTransform(irr::video::ETS_VIEW);
			matrix.makeInverse();
			services->setVertexShaderConstant(matViewInverse,matrix.pointer(),16);
			int sampler=0;
			services->setPixelShaderConstant(baseMap,&sampler,1);
			sampler=1;
			services->setPixelShaderConstant(reflectionMap,&sampler,1);
			services->setPixelShaderConstant(mipsCount,&mipmapsCount,1);
		}
		else
		{
			matrix = driver->getTransform(irr::video::ETS_PROJECTION);
			matrix *= driver->getTransform(irr::video::ETS_VIEW);
			matrix *= driver->getTransform(irr::video::ETS_WORLD);

			services->setVertexShaderConstant(matWorldViewProjection,matrix.pointer(),16);

			matrix = driver->getTransform(irr::video::ETS_VIEW);
			matrix.makeInverse();
			services->setVertexShaderConstant(matViewInverse,matrix.pointer(),16);

			matrix = driver->getTransform(irr::video::ETS_WORLD);
			services->setVertexShaderConstant(matWorld,matrix.pointer(),16);

			services->setPixelShaderConstant(mipsCount,&mipmapsCount,1);
		}
	}
};

int main(void)
{
	//Declaration of callbacks...
	phongCB* phongCallback = new phongCB();

	//Declaration of the device and the parameters...
	irr::IrrlichtDevice* device;
	irr::SIrrlichtCreationParameters prm;

	int value;

	std::cout<<"Pick driver 1:OpenGL 2:Direct3D9"<<std::endl;
	std::cin>>value;

	prm.DriverType = value == 1? irr::video::EDT_OPENGL : irr::video::EDT_DIRECT3D9;
	prm.Fullscreen = false;
	prm.WindowSize = irr::core::dimension2du(800,600);
	prm.AntiAlias = 0;
	prm.Bits = 32;
	prm.Stencilbuffer = false;//No hard edges shadows...

	//Engine initialization! :D
	device = createDeviceEx(prm);

	if(device)
	{
		irr::video::IVideoDriver* driver = device->getVideoDriver();
		irr::scene::ISceneManager* manager = device->getSceneManager();
		
		irr::core::array< irr::video::ITexture* > cubemap;

		cubemap.push_back(driver->getTexture("./data/irrlicht2_lf.jpg"));
		cubemap.push_back(driver->getTexture("./data/irrlicht2_rt.jpg"));
		cubemap.push_back(driver->getTexture("./data/irrlicht2_dn.jpg"));
		cubemap.push_back(driver->getTexture("./data/irrlicht2_up.jpg"));
		cubemap.push_back(driver->getTexture("./data/irrlicht2_ft.jpg"));
		cubemap.push_back(driver->getTexture("./data/irrlicht2_bk.jpg"));

		irr::core::array< irr::video::IImage* > imagesCubemap;
		
		for(int i=0;i<cubemap.size();i++)
		{
			imagesCubemap.push_back(driver->createImage(cubemap[i],irr::core::vector2di(0,0),cubemap[i]->getSize()));
		}

		irr::video::IImage* skydome = driver->createImage(irr::video::ECF_A8R8G8B8,irr::core::dimension2du(2048,1024));

		//Render skydome!
		cubemap2skydome(skydome,imagesCubemap);
		driver->writeImageToFile(skydome,L"sample.jpg");

		//removing the unnecesary textures
		driver->removeAllTextures();

		//releasing the images...
		for(int i=0;i<6;i++)
		{
			imagesCubemap[i]->drop();
		}

		//Adding the newly created image as texture...
		irr::video::ITexture* skydomeTex = driver->addTexture("DinamicSkydome.bmp",skydome);
		skydomeTex->regenerateMipMapLevels();

		//This image is not necesary anymore, so it is safe to release it as well.
		skydome->drop();

		//shader compilation.
		irr::s32 shaderIndex;

		if(prm.DriverType == irr::video::EDT_DIRECT3D9)
		{
			shaderIndex = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
			"./data/shader.hlsl",
			"vs_main",
			irr::video::EVST_VS_3_0,
			"./data/shader.hlsl",
			"ps_main",
			irr::video::EPST_PS_3_0,
			phongCallback,
			irr::video::EMT_SOLID
			);
		}
		else
		{
			shaderIndex = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
			"./data/shaderV.glsl",
			"vs_main",
			irr::video::EVST_VS_3_0,
			"./data/shaderP.glsl",
			"ps_main",
			irr::video::EPST_PS_3_0,
			phongCallback,
			irr::video::EMT_SOLID
			);		
		}

		//scene generation
		manager->addSkyDomeSceneNode(skydomeTex);
		irr::scene::ICameraSceneNode* camera =
        manager->addCameraSceneNodeFPS();

		//Add some spheres...
		irr::scene::ISceneNode* root = manager->addEmptySceneNode();
		irr::scene::ISceneNodeAnimator* anim = manager->createRotationAnimator(irr::core::vector3df(0,0.3,0));
		root->addAnimator(anim);
		anim->drop();

		for(int i=0;i<12;i++)
		{
			irr::scene::ISceneNode* sphere = manager->addSphereSceneNode(10.0f);
			sphere->setPosition(irr::core::vector3df(sin(2*i*irr::core::PI/12.0f)*50,0.0f,cos(2*i*irr::core::PI/12.0f)*50.0f));
			sphere->getMaterial(0).setTexture(0,driver->getTexture("./data/rockwall_height.bmp"));
			sphere->getMaterial(0).setTexture(1,skydomeTex);
			sphere->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaderIndex;
			sphere->getMaterial(0).TextureLayer[1].TrilinearFilter = true;
			sphere->getMaterial(0).TextureLayer[1].TextureWrapV = irr::video::ETC_CLAMP;
			sphere->setParent(root);
		}

		irr::scene::ISceneNode* root2 = manager->addEmptySceneNode();
		irr::scene::ISceneNodeAnimator* anim2 = manager->createRotationAnimator(irr::core::vector3df(0.3,0,0));
		root2->addAnimator(anim2);
		anim2->drop();

		for(int i=0;i<18;i++)
		{
			irr::scene::ISceneNode* sphere = manager->addSphereSceneNode(10.0f);
			sphere->setPosition(irr::core::vector3df(sin(2*i*irr::core::PI/12.0f)*60,0.0f,cos(2*i*irr::core::PI/12.0f)*60.0f));
			sphere->getMaterial(0).setTexture(0,driver->getTexture("./data/rockwall_height.bmp"));
			sphere->getMaterial(0).setTexture(1,skydomeTex);
			sphere->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaderIndex;
			sphere->getMaterial(0).TextureLayer[1].TrilinearFilter = true;
			sphere->getMaterial(0).TextureLayer[1].TextureWrapV = irr::video::ETC_CLAMP;
			sphere->setParent(root2);
		}

		irr::scene::ISceneNode* root3 = manager->addEmptySceneNode();
		irr::scene::ISceneNodeAnimator* anim3 = manager->createRotationAnimator(irr::core::vector3df(0,0,0.3));
		root3->addAnimator(anim3);
		anim3->drop();

		for(int i=0;i<24;i++)
		{
			irr::scene::ISceneNode* sphere = manager->addSphereSceneNode(10.0f);
			sphere->setPosition(irr::core::vector3df(sin(2*i*irr::core::PI/12.0f)*80,0.0f,cos(2*i*irr::core::PI/12.0f)*80.0f));
			sphere->getMaterial(0).setTexture(0,driver->getTexture("./data/rockwall_height.bmp"));
			sphere->getMaterial(0).setTexture(1,skydomeTex);
			sphere->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaderIndex;
			sphere->getMaterial(0).TextureLayer[1].TrilinearFilter = true;
			sphere->getMaterial(0).TextureLayer[1].TextureWrapV = irr::video::ETC_CLAMP;
			sphere->setParent(root3);
		}

		root2->setParent(root);
		root3->setParent(root2);

		//Add a dwarf...
		irr::scene::IAnimatedMesh* dwarf = manager->getMesh("./data/dwarf.x");
		irr::scene::ISceneNode* dwarfNode = manager->addAnimatedMeshSceneNode(dwarf);
		for(int i=0;i<dwarfNode->getMaterialCount();i++)
		{
			dwarfNode->getMaterial(i).setTexture(1,skydomeTex);
			dwarfNode->getMaterial(i).MaterialType = (irr::video::E_MATERIAL_TYPE)shaderIndex;
			dwarfNode->getMaterial(i).TextureLayer[1].TrilinearFilter = true;
			dwarfNode->getMaterial(i).TextureLayer[1].TextureWrapV = irr::video::ETC_CLAMP;
		}

		irr::s32 lastFPS = -1;

		while(device->run())
		{
			const irr::s32 fps = driver->getFPS();
			driver->beginScene();
			manager->drawAll();
			irr::core::stringw str = L"Irrlicht Engine - Environment Mapping Demo [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
			driver->endScene();
		}

		device->drop();
	}

	//Deleting our objects...
	phongCallback->drop();

	return 0;
}