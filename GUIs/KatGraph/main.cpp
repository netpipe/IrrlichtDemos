/*******************************************************************************
 ** Copyright (C) 2008-2009 by Katsankat mccauley404@gmail.com                **
 **                                                                           **
 ** This software is provided 'as-is', without any express or implied		  **
 ** warranty.  In no event will the authors be held liable for any damages	  **
 ** arising from the use of this software.					                  **
 **										                                      **
 ** Permission is not granted to anyone to use this software for any purpose, **
 ** including private and commercial applications, and to alter it and		  **
 ** redistribute it.								                          **
 **										                                      **
 ******************************************************************************/

#include "GraphKat.h"

int main()
{
  IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<s32>(280,300));
  if (!device) return 1;

  device->setWindowCaption(L"Graph class");
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr  = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  GraphKat *graph[5];

  graph[0] = new GraphKat(L"Download"    , device, rect<s32>(10,10 , 100,30));
  graph[1] = new GraphKat(L"Upload"      , device, rect<s32>(10,40 , 100,60));
  graph[2] = new GraphKat(L"CPU heat"    , device, rect<s32>(10,70 , 100,90));
  graph[3] = new GraphKat(L"Memory usage", device, rect<s32>(10,100, 100,120));
  graph[4] = new GraphKat(L"stuff"       , device, rect<s32>(10,150, 265,255));

  u32 now=0, then=0, NextGraph=0;

  bool flag = false;

  while(device->run())//
  {
    now = device->getTimer()->getTime();

      //feed them artificially every 120ms
      if (NextGraph < now)
      {
        for (u8 n=0; n<5; n++)
         if (rand()%2==0) // no parallel movement
           graph[n]->addValue(rand()% graph[n]->getMax());

         if (rand()%10==0)
         {
           flag = !flag;
           if (flag) graph[2]->setText(L"** yeeaah **");
           else      graph[2]->setText(L"stuff");
         }

        NextGraph = now + 120;
      }

    if (device->isWindowActive()){
        if (then < now)
        {
        driver->beginScene(true, true, 0);

        // draw each graph
                    graph[0]->drawGreen();
                    graph[1]->drawOrange();
        if (!flag) graph[2]->drawGreen();
        else       graph[2]->drawCrazy();
                    graph[3]->drawBlue();
                    graph[4]->drawOrange();
                    //graph[4]->drawStrange();
                    //graph[4]->draw();

        //smgr->drawAll(); // not needed
        guienv->drawAll(); // not needed if draw text is removed
        driver->endScene();

        then = now + 30;
        }
    device->sleep(10,0);
    }
    else device->sleep(10,0);//device->yield();
  }


  //end of main loop
  for (u8 n=0; n<5; n++)
    delete graph[n];

  device->closeDevice();

  //printf("ok press key to exit\n");
  //getchar();
  return 0;
}





/* - very simple basic example -

#include "GraphKat.h"

int main()
{
  IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<s32>(280,300));
  if (!device) return 1;

  device->setWindowCaption(L"Graph class");
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr  = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  GraphKat* graph = new GraphKat(L"random values", device, rect<s32>(10,10, 140,80));

  u32 now=0, then=0, NextGraph=0;

  while(device->run()) if (device->isWindowActive())
  {
    now = device->getTimer()->getTime();

    if (then < now)
    {
      driver->beginScene(true, true, 0);

      // draw the graph
      graph->drawOrange();

      //feed it artificially every 240 ms
      if (NextGraph < now)
      {
        graph->addValue(rand()% graph->getMax());
        NextGraph = now + 240;
      }

      //smgr->drawAll(); // not needed
      guienv->drawAll(); // not needed if draw text is removed
      driver->endScene();

      then = now + 30;
    }
  }
  else device->yield();

  //end of main loop
  delete graph;
  device->closeDevice();

  return 0;
}

*/

