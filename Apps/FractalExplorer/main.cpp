/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

class MyEventReceiver : public IEventReceiver
{
public:
        virtual bool OnEvent(const SEvent& event)
        {
                if (event.EventType == irr::EET_KEY_INPUT_EVENT)
                    KeyIsDown[event.KeyInput.Key]= event.KeyInput.PressedDown;

                return false;
        }

        virtual bool IsKeyDown(EKEY_CODE keyCode)
        {
                if(KeyIsDown[keyCode])
                {
                    KeyIsDown[keyCode]= false;
                    return true;
                }
                return false;
        }

        MyEventReceiver()
        {
                for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
                        KeyIsDown[i] = false;
        }

private:
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

class testCallback : public IShaderConstantSetCallBack
{
    public:
        virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
        {
            float time= Timer->getTime() / 10000.0;
            services->setVertexShaderConstant("time", &time, 1);
            services->setVertexShaderConstant("cx", &Cx, 1);
            services->setVertexShaderConstant("cy", &Cy, 1);
            services->setVertexShaderConstant("iter", &Iter, 1);
        }

        void setIter(int iter)
        {
            Iter= iter;
        }

        int getIter() const
        {
            return Iter;
        }

        void setCx(float cx)
        {
            Cx= cx;
        }

        void setCy(float cy)
        {
            Cy= cy;
        }

        float getCx() const
        {
            return Cx;
        }

        float getCy() const
        {
            return Cy;
        }

        testCallback(ITimer* timer)
        {
            Timer= timer;
            Cx= 0.0;
            Cy= 0.75;
            Iter= 20;
        }

        private:
            ITimer* Timer;
            float Cx, Cy;
            float Iter;
};

int main()
{
    IrrlichtDevice* dev0= createDevice(EDT_NULL);
    dimension2d<u32> screenRes= dev0->getVideoModeList()->getDesktopResolution();
    int screenDepth= dev0->getVideoModeList()->getDesktopDepth();
    dev0->closeDevice();

    IrrlichtDevice* dev= createDevice(EDT_OPENGL, screenRes, screenDepth, true);
    ISceneManager* smgr= dev->getSceneManager();
    IVideoDriver* video= dev->getVideoDriver();
    MyEventReceiver* rec= new MyEventReceiver;
    dev->setEventReceiver(rec);
    dev->setWindowCaption(L"Fractal Explorer");

    testCallback* callback= new testCallback(dev->getTimer());
    E_MATERIAL_TYPE EMT_FRACTAL;
    EMT_FRACTAL= (E_MATERIAL_TYPE)video->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        "fractal.vert", "main", EVST_VS_3_0,
		"fractal.frag", "main", EPST_PS_3_0,
		callback);

    smgr->addCameraSceneNodeFPS();
    ISceneNode* cube= smgr->addCubeSceneNode(1800);
    cube->setPosition(vector3df(0, 0, 1500));
    cube->setMaterialFlag(EMF_LIGHTING, false);
    cube->setMaterialType(EMT_FRACTAL);

    int screenCount= 0;
    bool drawInfo= true;

    while(dev->run())
    {
        if(rec->IsKeyDown(KEY_ADD)) callback->setIter(callback->getIter() + 1);
        else if(rec->IsKeyDown(KEY_SUBTRACT)) callback->setIter(callback->getIter() - 1);
        else if(rec->IsKeyDown(KEY_KEY_Q)) callback->setCx(callback->getCx() + 0.002);
        else if(rec->IsKeyDown(KEY_KEY_A)) callback->setCx(callback->getCx() - 0.002);
        else if(rec->IsKeyDown(KEY_KEY_W)) callback->setCy(callback->getCy() + 0.002);
        else if(rec->IsKeyDown(KEY_KEY_S)) callback->setCy(callback->getCy() - 0.002);
        else if(rec->IsKeyDown(KEY_F1)) drawInfo= !drawInfo;
        else if(rec->IsKeyDown(KEY_SPACE))
        {
            callback->setIter(20);
            callback->setCx(0);
            callback->setCy(0.75);
        }
        else if(rec->IsKeyDown(KEY_RETURN))
        {
            stringc name;
            name+= "screenshot";
            name+= screenCount;
            name+= ".png";
            video->writeImageToFile(video->createScreenShot(), name.c_str());
            screenCount++;
        }
        else if(rec->IsKeyDown(KEY_ESCAPE)) dev->closeDevice();

        video->beginScene();
        smgr->drawAll();

        if(drawInfo)
        {
            stringw text;
            text+= "FPS: ";
            text+= video->getFPS();
            text+= "\n\nIterations: ";
            text+= callback->getIter();
            text+= "    (use + or -)\nCx: ";
            text+= callback->getCx();
            text+= "    (use Q or A)\nCy: ";
            text+= callback->getCy();
            text+= "    (use W or D)\n\nHit Enter to create a screenshot\nHit Space to set default values\nPress F1 to hide/show this info\nPress Escape to exit";

            dev->getGUIEnvironment()->getSkin()->getFont()->draw(text, rect<s32>(20,20,500,500), SColor(255,255,255,255));
        }
        video->endScene();
    }


    return 0;
}
