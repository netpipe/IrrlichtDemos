// Copyright (C) 2005 Cestmir "CZestmyr" Houska
// This file uses the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h
/*

ISceneNode* fire = new CFireSceneNode(
    smgr->getRootSceneNode(), //Parent
    smgr, //Scene Manager
    device->getTimer(), //ITimer
    "cool.tga", //Fadeout texture
    SColor(255, 255, 128, 0), //Starting color
    2, //Fadeout color (2 - red, 1 - green, 0 - blue)
    0.95f, //Fadeout rate (0.0f - 1.0f -> the higher, the less the
             //color fades into the fadeout color)
    64, //Texture X size
    64, //Texture Y size
    30, //Width
    70, //Height
    20, //Update speed
    -1, //id
    vector3df(0.0f, 20.0f, 0.0f), //Position
    vector3df(0.0f, 0.0f, 0.0f) //Rotation
);


fire->drop();

*/


#ifndef _C_FIRE_SCENE_NODE_H_1674_
#define _C_FIRE_SCENE_NODE_H_1674_

#include "irrlicht.h"

namespace irr{

namespace scene{

u8 m(u8 a, f32 b){
    if (b < 1.0f) b = 1.0f;
    if (a > b) a = (u8)(a/b);
    return a;
}

u8 avg(u8& a, u8& b, u8& c){
    u32 ret = 0;
    ret += a;
    ret += b;
    ret += c;
    ret /= 3;
    return (u8)ret;
}

void eraseTexture(video::ITexture* texture){
    u8* pixels;
    pixels = (u8*)texture->lock();
    s32 w = texture->getSize().Width;
    s32 h = texture->getSize().Height;
    for (s32 i = 0; i < w; i++){
        for (s32 j = 0; j < h; j++){
            s32 index = (i * h + j) * 4;
            u8 rnd = (u8)(rand() * 255);
            pixels[index] = 0;
            pixels[index + 1] = 0;
            pixels[index + 2] = 0;
        }
    }
    texture->unlock();
}

class CFireSceneNode: public ISceneNode{
    public:
        CFireSceneNode(ISceneNode* parent, ISceneManager* mgr, ITimer* tim,
            c8* coolingTextureFilename, const video::SColor& color = video::SColor(255,255,192,0),
            s32 fadeTo = 2, f32 fadeRate = 0.95f, s32 textureX = 64, s32 textureY = 128,
            f32 width = 10.0f, f32 height = 20.0f, u32 timeStep = 50, s32 id = -1,
            const core::vector3df &pos = core::vector3df(0, 0, 0),
            const core::vector3df &rot = core::vector3df(0, 0, 0)):
        ISceneNode(parent, mgr, id, pos, rot), timer(tim), texX(textureX),
        texY(textureY), step(timeStep), timer1(0), coolOffset(0), fadeto(fadeTo),
        faderate(fadeRate)
        {
            driver = SceneManager->getVideoDriver();
            flameTexture = driver->addTexture(core::dimension2d<s32>(texX, texY), "", video::ECF_R8G8B8);
            coolingTexture = driver->getTexture(coolingTextureFilename);
            eraseTexture(flameTexture);

            f32 minX = - width / 2;
            f32 maxX = width / 2;
            f32 minZ = minX;
            f32 maxZ = maxX;

//!            AutomaticCullingEnabled = false;

            boundingBox = core::aabbox3d<f32>(pos.X - maxX, pos.Y, pos.Z - maxZ,
                                              pos.X + maxX, pos.Y + height, pos.Z + maxZ);

            plane1 = new video::S3DVertex[4];
            plane2 = new video::S3DVertex[4];
            indices = new u16[12];

            video::SColor white = video::SColor(255, 255, 255, 255);
            red = color.getRed();
            blue = color.getBlue();
            green = color.getGreen();

            material.Lighting = false;
            material.Wireframe = false;
//!            material.Texture1 = flameTexture;
            material.BackfaceCulling = false;
            material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;

            plane1[0] = video::S3DVertex(0, 0, minZ, 0, 0, 0, white, 0.1f, 0.95f);
            plane1[1] = video::S3DVertex(0, height, minZ, 0, 0, 0, white, 0.1f, 0.1f);
            plane1[2] = video::S3DVertex(0, height, maxZ, 0, 0, 0, white, 0.95f, 0.1f);
            plane1[3] = video::S3DVertex(0, 0, maxZ, 0, 0, 0, white, 0.95f, 0.95f);
            plane2[0] = video::S3DVertex(minX, 0, 0, 0, 0, 0, white, 0.1f, 0.95f);
            plane2[1] = video::S3DVertex(minX, height, 0, 0, 0, 0, white, 0.1f, 0.1f);
            plane2[2] = video::S3DVertex(maxX, height, 0, 0, 0, 0, white, 0.95f, 0.1f);
            plane2[3] = video::S3DVertex(maxX, 0, 0, 0, 0, 0, white, 0.95f, 0.95f);
            indices[0] = 0; indices[1] = 1; indices[2] = 2;
            indices[3] = 0; indices[4] = 2; indices[5] = 3;
        }

        ~CFireSceneNode(){
            delete[] plane1;
            delete[] plane2;
            delete[] indices;
        }

        void OnPreRender(){
         //   if (IsVisible == true)
                SceneManager->registerNodeForRendering(this);
//!            ISceneNode::OnPreRender();
        }

        void render(){
            video::IVideoDriver* driver = SceneManager->getVideoDriver();
            driver->setMaterial(material);
            driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

            u32 deltaTime = timer->getTime() - prevTime;
            prevTime += deltaTime;
            timer1 += deltaTime;
            if (timer1 > step){
                u8* pixels;
                u8* pixels_cool;
                pixels = (u8*)flameTexture->lock();
                pixels_cool = (u8*)coolingTexture->lock();
                s32 prev_index_minus1;
                s32 prev_index_plus1;
                s32 color_average;
                s32 size_of_a_row = texX * 4;
                s32 pix_count = (texY-1) * size_of_a_row;
                s32 max_index = texY * size_of_a_row;
                s32 cool_index_diff;
                for (s32 j = 1; j < texX - 1; j++){
                    s32 index = j * 4 + pix_count;
                    pixels[index] = blue;
                    pixels[index + 1] = green;
                    pixels[index + 2] = red;
                }
                for (s32 i = 0; i < texY - 1; i++){
                    cool_index_diff = coolOffset * size_of_a_row;
                    s32 column = i * texX;
                    for (s32 j = 1; j < texX - 1; j++){
                        s32 index = (column + j) * 4;
                        s32 cool_index = index + cool_index_diff;
                        cool_index %= max_index;
                        s32 prev_index = index + size_of_a_row;
                        prev_index_minus1 = prev_index-4;
                        prev_index_plus1 = prev_index+4;
                        for (u32 k = 0; k != 3; k++){
                            color_average = m(avg(pixels[prev_index + k], pixels[prev_index_minus1 + k], pixels[prev_index_plus1 + k]), pixels_cool[cool_index + k] / 128.0f);
                            if (k != fadeto) color_average = (s32)(color_average * faderate);
                            pixels[index + k] = color_average;
                        }
                    }
                }
                flameTexture->unlock();
                coolingTexture->unlock();
                pixels_cool = NULL;
                pixels = NULL;
                timer1 %= step;
                coolOffset += 2;
                if (coolOffset >= texY) coolOffset -= texY;
            }
            driver->drawIndexedTriangleList(&plane1[0], 4, &indices[0], 2);
            driver->drawIndexedTriangleList(&plane2[0], 4, &indices[0], 2);
        }

        const core::aabbox3df& getBoundingBox() const {return boundingBox;}

        s32 getMaterialCount(){
            return 1;
        }

        video::SMaterial& getMaterial(s32 i){
            return material;
        }
    private:
        ITimer* timer;
        video::IVideoDriver* driver;
        video::ITexture* flameTexture;
        video::ITexture* coolingTexture;
        s32 texX;
        s32 texY;
        s32 coolOffset;
        video::S3DVertex* plane1;
        video::S3DVertex* plane2;
        u16* indices;
        u32 prevTime;
        u32 step;
        u32 timer1;
        core::aabbox3d<f32> boundingBox;
        video::SMaterial material;
        u8 red;
        u8 green;
        u8 blue;
        u8 fadeto;
        f32 faderate;
};

} // End namespace scene

} // End namespace irr

#endif
