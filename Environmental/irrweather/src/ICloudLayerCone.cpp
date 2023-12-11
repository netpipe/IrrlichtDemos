// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and most of the code belongs to Pazystamo from the Irrlicht forums.
// Original code from Pazystamo's ATMOSphere

#include "ICloudLayerCone.h"
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <IVideoDriver.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

ICloudLayerCone::ICloudLayerCone(irr::video::ITexture* const tex,irr::scene::ISceneNode* const parent, irr::scene::ISceneManager* const mgr,
    irr::s32 faces, irr::s32 id)
      : irr::scene::ISceneNode(parent, mgr, id)
{
    //AutomaticCullingEnabled = false;
    irr::video::SMaterial mat;
    mat.Lighting = false;
    //mat.Wireframe = true;
    mat.ZBuffer = false;
    //mat.ZWriteEnable = false;
    mat.setFlag(irr::video::EMF_BILINEAR_FILTER,true);
    //mat.NormalizeNormals=true;
    //mat.AnisotropicFilter=true;
    //mat.GouraudShading=false;
    //mat.TrilinearFilter = true;
    //mat.BackfaceCulling = false;
    face=faces;
    Vertices = new irr::video::S3DVertex[face+1];
    indices = new irr::u16[face*3];
    Material = mat;
    Material.setTexture(0,tex);
    irr::f64 angle = 0.0f;           //start positions
    irr::f64 angle2 = 360.0f/face;   //angle to add
    vert=0;                          //vertex nr
    irr::s32 nr = -3;                //indices nr
    //top vertex
    Vertices[0]=irr::video::S3DVertex(0.0f, 100.0f, -0.0f,
     0.0568988f, 0.688538f, -0.722965f,
      irr::video::SColor(255,255,255,255), 0.5f, 0.5f);

    f32 deltaAngle = 2*irr::core::PI/faces;
    for (irr::u16 n=1;n<face+1;++n)
    {
        vert=vert+1;
        nr=nr+3;                        //indices number
        irr::f64 x=cos(angle*0.017453292519943295769236907684886f)*1000;//vertice x coord
        irr::f64 z=sin(angle*0.017453292519943295769236907684886f)*1000;//vertice z coord
        Vertices[vert]=irr::video::S3DVertex(x, 5, z,
           0, -1, 0,
           irr::video::SColor(255,255,255,255), (cos(deltaAngle*vert)+0.2)*1.5, (sin(deltaAngle*vert)+0.2)*1.5);
        angle=angle+angle2;
        //connects face
        indices[nr]=0;                  //top vertex
        indices[nr+1]=vert;             //bottom vertex
        indices[nr+2]=vert+1;           //next bottom vertex
    }
    indices[nr+2]=1;                 //connect last bottom vertex with first
    textureDimensions = vector2df(1024,1024);

    Material.TextureLayer[0].TextureWrapU = ETC_MIRROR;
    Material.TextureLayer[0].TextureWrapV = ETC_MIRROR;

    FirstRun = true;
}

void ICloudLayerCone::OnRegisterSceneNode()
{
      if(IsVisible)
         SceneManager->registerNodeForRendering(this,ESNRP_SKY_BOX);
      ISceneNode::OnRegisterSceneNode();
}

void ICloudLayerCone::OnAnimate(u32 timeMs)
{
    random = timeMs;

    if(FirstRun)
    {
        FirstRun = false;

        applyPerlinTexture();
    }
}

void ICloudLayerCone::render()
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
    irr::scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

    if (!camera || !driver)
        return;

    irr::core::matrix4 mat;
    mat.setTranslation(camera->getAbsolutePosition());

    //attach node to camera
    driver->setTransform(video::ETS_WORLD, mat);

    //don't attach camera
    //driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    driver->setMaterial(Material);

    //update uv maping
    /*for (int i=1;i<face+1;++i)
    {
        Vertices[i].TCoords=core::vector2d< irr::f32 >(uvX,0.98f);
    }
    Vertices[0].TCoords=core::vector2d< irr::f32 >(uvX,0.01f);*/
    driver->drawIndexedTriangleList(&Vertices[0],vert+1,&indices[0],face);
}

const irr::core::aabbox3d<irr::f32>& ICloudLayerCone::getBoundingBox() const
{
      return Box;
}

irr::u32 ICloudLayerCone::getMaterialCount() const
{
      return 1;
}

irr::video::SMaterial& ICloudLayerCone::getMaterial(irr::u32 i)
{
      return Material;
}

//change sky texture
irr::video::SMaterial& ICloudLayerCone::setMaterial(irr::video::ITexture* const tex)
{
      Material.setTexture(0, tex);
      return Material;
}


f32 ICloudLayerCone::findnoise2(f32 x, f32 y)
{
	u32 n = (u32)x+(u32)y*random;
	n = (n<<13)^n;
	int nn = (n*(n*n*60493+19990303)+1376312589)&0x7fffffff;

	return 1.0-((f32)nn/1073741824.0);
}


f32 ICloudLayerCone::interpolate1(f32 a, f32 b, f32 x)
{
	f32 ft = x*irr::core::PI;
	f32 f = (1.0 - cos(ft))*0.5;

	return a*(1.0 - f) + b*f;
}


f32 ICloudLayerCone::noise(f32 x, f32 y)
{
	f32 floorx = (f32)((u32)x);//This is kinda a cheap way to floor a double integer.
	f32 floory = (f32)((u32)y);

	return interpolate1(
			interpolate1(findnoise2(floorx, floory), findnoise2(floorx + 1, floory), x - floorx),
			interpolate1(findnoise2(floorx, floory + 1), findnoise2(floorx + 1, floory + 1), x - floorx),
			y - floory);//Here we use y-floory, to get the 2nd dimension.
}


void ICloudLayerCone::generatePerlinData(u32* data, ITexture* const texture,
    irr::f32 zoom, irr::f32 persistence, irr::u16 octaves, const SColor& baseColor, irr::f32 mixAmount)
{
    for(u32 y = 0; y < textureDimensions.X; ++y)
	{
		for(u32 x = 0; x < textureDimensions.X; ++x)
		{
			f32 getnoise = 0;

			for(u32 a = 0; a < octaves; ++a)
			{
				f32 frequency = pow(2, a);//This increases the frequency with every loop of the octave.
				f32 amplitude = pow(persistence, a);//This decreases the amplitude with every loop of the octave.

				//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
				//It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1
				//printf("EXP CURVE: %.2f\n", cloudExpCurve(2));
				getnoise += noise(((f32)x)*frequency/zoom,((f32)y)/zoom*frequency)*amplitude;
			}

			int color = (u32)((getnoise*128.0) + 128.0);//Convert to 0-256 values.

			if(color > 255)
				color = 255;

			if(color < 0)
				color = 0;

			/*image->setPixel(x, y, irr::video::SColor(
					255,
					(int)((r/255.0)*(double)color),
					(int)((g/255.0)*(double)color),
					(int)((b/255.0)*(double)color)));*/

            irr::video::SColor finalColor = irr::video::SColor(color,
                (u32)((baseColor.getRed()/255.0)*(f32)color),
                (u32)((baseColor.getGreen()/255.0)*(f32)color),
                (u32)((baseColor.getBlue()/255.0)*(f32)color));

            if(mixAmount > 0.0f && mixAmount < 1.0f)
                data[y * texture->getSize().Width + x] = SColor(data[y * texture->getSize().Width + x]).getInterpolated(finalColor, mixAmount).color;

            else
                data[y * texture->getSize().Width + x] = finalColor.color;

            //printf("CLOUD EXP: %.2f\n", cloudExpCurve(getnoise));

            //data[y * texture->getSize().Width + x] += SColor(cloudExpCurve(getnoise)).color;
		}

	}
}


void ICloudLayerCone::generatePerlinTexture(irr::video::ITexture* const texture)
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

    irr::f32 zoom = 18;
    irr::f32 persistence = 0.1/2;
    irr::u16 octaves = 1;

	u32 r = 200, g = 200, b = 200;

	const SColor baseColor = SColor(255,r,g,b);

    u32* data = (u32*)texture->lock();

    if(!data)
    {
        texture->unlock();
        return;
    }

	generatePerlinData(data, texture, zoom, persistence, octaves, baseColor, 1.0f);

    zoom = 6;
    persistence = 0.1/2;
    octaves = 1;
	generatePerlinData(data, texture, zoom, persistence, octaves, baseColor, 0.5f);

	zoom = 40;
    persistence = 0.1/2;
    octaves = 1;
	generatePerlinData(data, texture, zoom, persistence, octaves, baseColor, 0.5f);

	for(u32 y = 0; y < textureDimensions.Y; ++y)
	{
		for(u32 x = 0; x < textureDimensions.X; ++x)
		{
		    SColor finalColor = SColor(data[y * texture->getSize().Width + x]);
			finalColor.setAlpha(finalColor.getAlpha()<180 ? finalColor.getAlpha()*0.2:finalColor.getAlpha());
            data[y * texture->getSize().Width + x] = finalColor.color;
		}

	}


    texture->regenerateMipMapLevels();
	texture->unlock();
}


void ICloudLayerCone::applyPerlinTexture()
{
    irr::video::ITexture* texture = SceneManager->getVideoDriver()->addTexture(irr::core::dimension2du(textureDimensions.X, textureDimensions.Y),
        "NoiseTexture", ECF_A8R8G8B8);

    generatePerlinTexture(texture);

    setMaterial(texture);
}


ICloudLayerCone::~ICloudLayerCone()
{
    delete Vertices;
    delete indices;
}
