// some useful functions for terrain demo

#include <irrlicht.h>
using namespace irr;

#include"ShTlTerrainSceneNode.h"
#include"random.h"

void makeRandomHills(ShTlTerrainSceneNode* node, s32 hillCount, s32 maxRadius)
{
    double pi = 3.1415926535;
    
    // make hills
    for(int n=0; n<hillCount; n++)
    {
        s32 radius = rnd(1, maxRadius);
        f32 height = rnd(1, radius);
        height = height/10;
        core::vector2di pos;
        pos.X = rnd(-radius, node->getSize().Width-radius);
        pos.Y = rnd(-radius, node->getSize().Height-radius);
        
        // decide if add or substract
        if(rnd(1)==1) height = -height;
        
        // create hill using sinus and distance to center
        for(int j=0; j<=radius*2; j++)
            for(int i=0; i<=radius*2; i++)
            {
                f32 distance = core::vector2d<s32>(i-radius, j-radius).getLength();
                if(distance <= radius)
                {
                    f32 v = radius;
                    v = v - distance;
                    v = v / radius;
                    v = v * pi / 2;
                    v = sin(v);
                    if(pos.X+i>=0 && pos.X+i<node->getSize().Width+1 && pos.Y+j>=0 && pos.Y+j<node->getSize().Height+1)
                        node->setHeight(pos.X+i, pos.Y+j, node->getHeight(pos.X+i, pos.Y+j)+v*height*node->getTileSize());
                }
            }
    }
}



void randomizeColor(ShTlTerrainSceneNode* node)
{
    // apply random shades of green color ower terrain
    for(u32 j=0; j<node->getSize().Height+1; j++)
        for(u32 i=0; i<node->getSize().Width+1; i++)
        {
            u32 g = rnd(80,170);
            u32 r = rnd(g-40,g+10);
            u32 b = rnd(r-40,r-20);
            
            node->setColor(i,j, video::SColor(255,r,g,b));
        }
    
    // modify color depending on height
    for(u32 j=0; j<node->getSize().Height+1; j++)
        for(u32 i=0; i<node->getSize().Width+1; i++)
        {
            f32 hk = node->getHeight(i,j);
            if(hk > 2) hk =2;
            if(hk < -2) hk = -2;
            hk = hk + 2;
            hk = hk / 4;
            
            f32 kh = 1.0f - hk;
            
            video::SColor color1(255,50,150,50);
            video::SColor color2(255,200,200,120);
            
            u32 r = (u32)(color1.getRed()*kh + color2.getRed()*hk);
            u32 g = (u32)(color1.getGreen()*kh + color2.getGreen()*hk);
            u32 b = (u32)(color1.getBlue()*kh + color2.getBlue()*hk);
            
            color1 = video::SColor(255,r,g,b);
            color2 = node->getColor(i,j);
            
            hk = 0.6f;
            kh = 1.0f - hk;
            
            r = (u32)(color1.getRed()*kh + color2.getRed()*hk);
            g = (u32)(color1.getGreen()*kh + color2.getGreen()*hk);
            b = (u32)(color1.getBlue()*kh + color2.getBlue()*hk);
            
            node->setColor(i,j, video::SColor(255,r,g,b));
        }
}



void randomizeUV(ShTlTerrainSceneNode* node)
{
    for(u32 j=0; j<node->getSize().Height; j++)
        for(u32 i=0; i<node->getSize().Width; i++)
        {
            s32 r = rnd(4);
            if(r == 0)
                node->setTileUV(i,j, core::vector2d<f32>(0,0.5),
                    core::vector2d<f32>(0,0),
                    core::vector2d<f32>(0.5,0),
                    core::vector2d<f32>(0.5,0.5));
            
            if(r == 1)
                node->setTileUV(i,j, core::vector2d<f32>(0.5,0.5),
                    core::vector2d<f32>(0.5,0),
                    core::vector2d<f32>(1,0),
                    core::vector2d<f32>(1,0.5));
            
            if(r == 2)
                node->setTileUV(i,j, core::vector2d<f32>(0,1),
                    core::vector2d<f32>(0,0.5),
                    core::vector2d<f32>(0.5,0.5),
                    core::vector2d<f32>(0.5,1));
            
            if(r == 3)
                node->setTileUV(i,j, core::vector2d<f32>(0.5,1),
                    core::vector2d<f32>(0.5,0.5),
                    core::vector2d<f32>(1,0.5),
                    core::vector2d<f32>(1,1));
        }
}
