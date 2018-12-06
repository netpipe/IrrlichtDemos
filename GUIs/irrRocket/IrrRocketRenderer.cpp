/*
  Copyright (C) 2011 Daniel Sudmann

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

  Daniel Sudmann suddani@googlemail.com
*/
#include "IrrRocketRenderer.h"
#include <IVideoDriver.h>
#include <SMeshBuffer.h>

IrrRocketRenderer::IrrRocketRenderer(irr::video::IVideoDriver* driver) : Driver(driver)
{
    //ctor
    //ScissorEnabled = false;

    Material2D.Lighting = false;
    Material2D.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    Material2D.ZWriteEnable = false;
    Material2D.ZBuffer = true;

    irr::scene::SMeshBuffer* b = new irr::scene::SMeshBuffer();
    b->Vertices.push_back(irr::video::S3DVertex(-0.5, -0.5, 1, 0, 0, -1, irr::video::SColor(255,255,0,0), 0, 0));
    b->Vertices.push_back(irr::video::S3DVertex(-0.5,  0.5, 1, 0, 0, -1, irr::video::SColor(255,255,0,0), 0, 0));
    b->Vertices.push_back(irr::video::S3DVertex( 0.5,  0.5, 1, 0, 0, -1, irr::video::SColor(255,255,0,0), 0, 0));
    b->Vertices.push_back(irr::video::S3DVertex( 0.5, -0.5, 1, 0, 0, -1, irr::video::SColor(255,255,0,0), 0, 0));

    b->Indices.push_back(0);
    b->Indices.push_back(2);
    b->Indices.push_back(1);

    b->Indices.push_back(0);
    b->Indices.push_back(3);
    b->Indices.push_back(2);

    b->Material.ZWriteEnable = true;
    b->Material.ZBuffer = true;
    b->Material.ColorMask = irr::video::ECM_NONE;
    b->Material.Lighting = false;
    b->Material.MaterialType = irr::video::EMT_SOLID;
    ScissorBuffer = b;

    //ScissorScreen = Driver->addRenderTargetTexture(irr::core::dimension2du(Driver->getScreenSize().Width, Driver->getScreenSize().Height), "#RocketScissor");
}

IrrRocketRenderer::~IrrRocketRenderer()
{
    //dtor
    ScissorBuffer->drop();
    //Driver->removeTexture(ScissorScreen);
}
void IrrRocketRenderer::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
    irr::core::vector3df scaleSet(1024.f/Driver->getViewPort().getWidth(), 768.f/Driver->getViewPort().getHeight(), 1);

    irr::core::array<irr::video::S3DVertex> Vertices;
    for (irr::u32 i=0;i<num_vertices;++i)
    {
        Vertices.push_back(irr::video::S3DVertex(vertices[i].position.x, vertices[i].position.y, 0, 0, 0, 1, irr::video::SColor(vertices[i].colour.alpha, vertices[i].colour.red, vertices[i].colour.green, vertices[i].colour.blue), vertices[i].tex_coord.x, vertices[i].tex_coord.y));
    }
    irr::core::array<irr::u16> Indices;
    for (irr::u32 i=0;i<num_indices;i+=3)
    {
        Indices.push_back(indices[i]);
        Indices.push_back(indices[i+2]);
        Indices.push_back(indices[i+1]);
    }

    Material2D.setTexture(0, (irr::video::ITexture*)texture);
    Driver->setMaterial(Material2D);

    irr::core::matrix4 transform;
    transform.setTranslation(irr::core::vector3df(translation.x, translation.y, 0));
    Driver->setTransform(irr::video::ETS_WORLD, transform);

    Driver->drawIndexedTriangleList(&Vertices[0], num_vertices, &Indices[0], (int)(((float)num_indices)/3.f));
    //Driver->draw2DVertexPrimitiveList(&Vertices[0], Vertices.size(), indices, num_indices/3);
}

Rocket::Core::CompiledGeometryHandle IrrRocketRenderer::CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture)
{
    irr::scene::SMeshBuffer* buffer = new irr::scene::SMeshBuffer();
    for (irr::u32 i=0;i<num_vertices;++i)
    {
        buffer->Vertices.push_back(irr::video::S3DVertex(vertices[i].position.x, vertices[i].position.y, 0, 0, 0, 1, irr::video::SColor(vertices[i].colour.alpha, vertices[i].colour.red, vertices[i].colour.green, vertices[i].colour.blue), vertices[i].tex_coord.x, vertices[i].tex_coord.y));
    }
    for (irr::u32 i=0;i<num_indices;i+=3)
    {
        buffer->Indices.push_back(indices[i]);
        buffer->Indices.push_back(indices[i+2]);
        buffer->Indices.push_back(indices[i+1]);
    }

    buffer->Material.setTexture(0, (irr::video::ITexture*)texture);

    return (Rocket::Core::CompiledGeometryHandle)buffer;
}

void IrrRocketRenderer::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
    irr::core::vector3df scaleSet(1024.f/Driver->getViewPort().getWidth(), 768.f/Driver->getViewPort().getHeight(), 1);

    irr::scene::SMeshBuffer* buffer = (irr::scene::SMeshBuffer*)geometry;

    irr::core::matrix4 transform;
    transform.setTranslation(irr::core::vector3df(translation.x, translation.y, 0));
    Driver->setTransform(irr::video::ETS_WORLD, transform);

    Material2D.setTexture(0, buffer->Material.getTexture(0));
    Driver->setMaterial(Material2D);

    Driver->drawMeshBuffer(buffer);
}

void IrrRocketRenderer::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
    ((irr::scene::SMeshBuffer*)geometry)->drop();
}

void IrrRocketRenderer::EnableScissorRegion(bool enable)
{
    //ScissorEnabled = enable;
    if (enable)
    {

        //Driver->setViewPort(ScissorArea);

        Material2D.ZBuffer = true;

        //Driver->setRenderTarget(ScissorScreen, false, true);

        Driver->setMaterial(ScissorBuffer->getMaterial());

        Driver->setTransform(irr::video::ETS_WORLD, ScissorTransform[0]);
        Driver->drawMeshBuffer(ScissorBuffer);

        Driver->setTransform(irr::video::ETS_WORLD, ScissorTransform[1]);
        Driver->drawMeshBuffer(ScissorBuffer);

        Driver->setTransform(irr::video::ETS_WORLD, ScissorTransform[2]);
        Driver->drawMeshBuffer(ScissorBuffer);

        Driver->setTransform(irr::video::ETS_WORLD, ScissorTransform[3]);
        Driver->drawMeshBuffer(ScissorBuffer);

        //Driver->setRenderTarget(0, false, false);

    }
    else
    {
        //Driver->setViewPort(irr::core::rect<irr::s32>(0,0,Driver->getScreenSize().Width,Driver->getScreenSize().Height));

        Material2D.ZBuffer = false;

        //Driver->setRenderTarget(0, false, true);
    }
}

void IrrRocketRenderer::SetScissorRegion(int x, int y, int width, int height)
{
    //printf("Scissor: %i %i %i %i\n", x, y, width, height);
    //ScissorArea = irr::core::rect<irr::s32>(x,y,x+width,y+height);

    ScissorTransform[0].setTranslation(irr::core::vector3df(x+width/2.f,y/2.f,0));
    ScissorTransform[0].setScale(irr::core::vector3df(width, y, 1));

    ScissorTransform[1].setTranslation(irr::core::vector3df(x+width/2.f,y+height+(Driver->getScreenSize().Height-y-height)/2.f,0));
    ScissorTransform[1].setScale(irr::core::vector3df(width, (Driver->getScreenSize().Height-y-height), 1));

    ScissorTransform[2].setTranslation(irr::core::vector3df(x/2.f,y+height/2.f,0));
    ScissorTransform[2].setScale(irr::core::vector3df(x, Driver->getScreenSize().Height, 1));

    ScissorTransform[3].setTranslation(irr::core::vector3df(x+width+(Driver->getScreenSize().Width-x-width)/2.f,y+height/2.f,0));
    ScissorTransform[3].setScale(irr::core::vector3df(Driver->getScreenSize().Width-x-width, Driver->getScreenSize().Height, 1));

    //EnableScissorRegion(true);
}

bool IrrRocketRenderer::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
    irr::video::ITexture* texture = Driver->getTexture(source.CString());
    texture_handle = (Rocket::Core::TextureHandle)texture;
    if (texture)
    {
        texture_dimensions.x = texture->getSize().Width;
        texture_dimensions.y = texture->getSize().Height;
        return true;
    }
    return false;
}


/*
GenerateTexture() is used by the font system to convert raw pixel data to a texture.
The texture_handle parameter is a reference to a Rocket::Core::TextureHandle type to be set,as in LoadTexture().
The raw pixel data is given in source; this is an array of unsigned, 8-bit values in RGBA order.
It is laid out in tightly-packed rows, so is exactly (source_dimensions.x * source_dimensions.y * 4) bytes in size.
The source_dimensions variable is set to the dimensions of the raw texture data.
*/
irr::u32 RocketTextureID = 0;
bool IrrRocketRenderer::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
    printf("Generate Texture....");
    irr::core::stringc RocketTextureName = "#Rocket::Core::TextureHandle::";
    RocketTextureName.append(RocketTextureID++);
    irr::video::ITexture* texture = Driver->addTexture(irr::core::dimension2du(source_dimensions.x, source_dimensions.y), RocketTextureName.c_str());
    if (texture)
    {
        void* data = texture->lock();
        memcpy(data, source, source_dimensions.x*source_dimensions.y*4);
        texture->unlock();
        texture->regenerateMipMapLevels();
        texture_handle = (Rocket::Core::TextureHandle)texture;
        printf("DONE\n");
        return true;
    }
    printf("FAILED\n");
    return false;
}

void IrrRocketRenderer::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
    if (texture_handle)
        Driver->removeTexture((irr::video::ITexture*)texture_handle);
}
