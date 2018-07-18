#include "SpheremapGenerator.h"

namespace irr
{
namespace scene
{

SpheremapGenerator::SpheremapGenerator( irr::scene::ISceneManager * SceneManager , const irr::core::dimension2du skydomeResolution )
        : smgr(SceneManager), driver(SceneManager->getVideoDriver()), skydomeRes(skydomeResolution)
{
    Skydome = driver->createImage( irr::video::ECF_A8R8G8B8, skydomeResolution);
}

void SpheremapGenerator::cubemap2skydome( irr::core::array< irr::video::ITexture* > cubemap,  const io::path & relativeRute )
{

    for( u32 i=0; i < cubemap.size(); i++ )
	{
			imagesCubemap.push_back( driver->createImage( cubemap[i], irr::core::vector2di(0,0), cubemap[i]->getSize()) );
	}

    const irr::core::dimension2du skydomeResolution = Skydome->getDimension();
    f32 i,j;
    irr::video::SColor color;
    irr::core::vector3df vector;
    irr::core::vector2df uv;

    for( i = 0; i < skydomeResolution.Width; i++ )
    {
        for( j = 0; j < skydomeResolution.Height; j++ )
        {
            uv.X = i / skydomeResolution.Width + 0.5f / skydomeResolution.Width;
            uv.Y = j / skydomeResolution.Height + 0.5f / skydomeResolution.Height;
            vector = uv2xyz(uv);
            color = readCubemap(vector, imagesCubemap);

            Skydome->setPixel( i , j, color);
        }
    }

    driver->writeImageToFile(Skydome, relativeRute );

    for(u32 i = 0; i < 6; i++ )
    {
        imagesCubemap[i]->drop();
    }

    for(u32 i = 0; i < 6; i++ )
    {
        imagesCubemap.erase(0);
    }
}


void SpheremapGenerator::setSkyDomeResolution(const irr::core::dimension2du Resolution )
{
    Skydome->drop();
    Skydome = driver->createImage( irr::video::ECF_A8R8G8B8, Resolution );
}


irr::core::vector3df SpheremapGenerator::uv2xyz(const irr::core::vector2df inTexCoord)
{
    irr::core::vector3df vec;
    irr::core::vector2df texCoord;

    //texture space normalization
    texCoord.X = inTexCoord.X - floor( inTexCoord.X );
    texCoord.Y = inTexCoord.Y - floor( inTexCoord.Y );

    vec.X = -sin( texCoord.X * irr::core::PI * 2.0f ) * sin( texCoord.Y * irr::core::PI );
    vec.Y = cos( texCoord.Y * irr::core::PI );
    vec.Z = cos( texCoord.X * irr::core::PI * 2.0f ) * sin( texCoord.Y * irr::core::PI );

    return vec.normalize();
}

irr::core::vector2df SpheremapGenerator::xyz2uv(const irr::core::vector3df vector)
{
    irr::core::vector2df texCoords;
    irr::core::vector3df vecSQ = vector;

    if( vecSQ.X < 0 )
        vecSQ.X = -vecSQ.X;
    if( vecSQ.Y < 0 )
        vecSQ.Y = -vecSQ.Y;
    if( vecSQ.Z < 0 )
        vecSQ.Z = -vecSQ.Z;

    if( vecSQ.X > vecSQ.Y && vecSQ.X > vecSQ.Z )
    {
        if( vector.X >= 0 )
        {
            texCoords.X = vector.Z / vector.X * 0.5f;
            texCoords.Y = -vector.Y / vector.X * 0.5f;
        }
        else
        {
            texCoords.X = vector.Z / vector.X * 0.5f;
            texCoords.Y = vector.Y / vector.X * 0.5f;
        }
    }
    else
    {
        if( vecSQ.Y > vecSQ.X && vecSQ.Y > vecSQ.Z )
        {
            if( vector.Y >= 0 )
            {
                texCoords.X = -vector.Z / vector.Y * 0.5f;
                texCoords.Y = -vector.X / vector.Y * 0.5f;
            }
            else
            {
                texCoords.X = vector.Z / vector.Y * 0.5f;
                texCoords.Y = -vector.X / vector.Y * 0.5f;
            }
        }
        else
        {
            if( vector.Z >= 0 )
            {
                texCoords.X = -vector.X / vector.Z * 0.5f;
                texCoords.Y = -vector.Y / vector.Z * 0.5f;
            }
            else
            {
                texCoords.X = -vector.X / vector.Z * 0.5f;
                texCoords.Y = vector.Y / vector.Z * 0.5f;
            }
        }
    }

    texCoords += irr::core::vector2df(0.5f , 0.5f);

    return texCoords;
}


irr::video::SColor SpheremapGenerator::readTexturemap(const irr::core::vector2df texCoord, irr::video::IImage* image)
{
    irr::core::vector2df nTexCoord;
    irr::core::vector2df blend;
    irr::core::vector2di baseCoord;
    f32 height = image->getDimension().Height;
    f32 width = image->getDimension().Width;
    irr::video::SColor color;

    nTexCoord.X = texCoord.X - floor( texCoord.X );
    nTexCoord.Y = texCoord.Y - floor( texCoord.Y );

    baseCoord.X = floor( width * nTexCoord.X );
    baseCoord.Y = floor( height * nTexCoord.Y );

    blend.X = width * nTexCoord.X - width;
    blend.Y = height * nTexCoord.Y - height;

    color = image->getPixel( baseCoord.X, baseCoord.Y );

    irr::f32 clampedCoord = baseCoord.X + 1 >= width ? width - 1 : baseCoord.X + 1;

    color = color.getInterpolated( image->getPixel( clampedCoord , baseCoord.Y ), blend.X);

    clampedCoord = baseCoord.Y + 1 >= height ? height - 1 : baseCoord.Y + 1;

    color = color.getInterpolated( image->getPixel( baseCoord.X , clampedCoord ), blend.Y );

    return color;
}

irr::video::SColor SpheremapGenerator::readCubemap(const irr::core::vector3df vector, irr::core::array< irr::video::IImage* >& images)
{
    //Order: 0,1 left,right
    //Order: 2,3 down,up
    //Order: 4:5 front,back
    unsigned int facing;

    irr::core::vector3df vecSQ = vector;
    vecSQ.X *= vecSQ.X;
    vecSQ.Y *= vecSQ.Y;
    vecSQ.Z *= vecSQ.Z;

    irr::f32 maximal = maxi( vecSQ.X, maxi( vecSQ.Y, vecSQ.Z ) );

    if( maximal == vecSQ.X )
    {
            facing = ( vector.X < 0? 0 : 1 );
    }
    else
    {
        if(maximal == vecSQ.Y)
        {
                facing = ( vector.Y < 0? 2 : 3 );
        }
        else
        {
            facing = ( vector.Z < 0? 4 : 5 );
        }
    }

    irr::core::vector2df texCoords = xyz2uv( vector );
    return readTexturemap(texCoords,images[facing]);
}

}
}
