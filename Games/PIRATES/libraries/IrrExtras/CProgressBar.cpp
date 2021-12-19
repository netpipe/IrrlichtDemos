#include "CProgressBar.h"

void CProgressBar::draw(){

   if(!isVisiblee){ return; };

   driver->beginScene(true, true, SColor(100,0,0,0));

   // if (Text){
   // text = guienv->addStaticText(DisplayText.c_str(), core::rect<s32>(10,50,200,50), false, false); // border? wordwrap?
   // text = guienv->addStaticText(L"Loading...", core::rect<s32>(10,50,200,50), false, false); // border? wordwrap?
   // text->setOverrideColor(video::SColor(255,180,180,255)); // opacity, red, green, blue
   // };

   /// Render Background Image:
   if(backgroundImageTexture != NULL){
   int sw = driver->getScreenSize().Width;
   int sh = driver->getScreenSize().Height;
   driver->draw2DImage(backgroundImageTexture, position2d<s32>(0,0), core::rect<s32>(0,0,sw,sh));
   };

   // if (Border){
    /// Render Borders:
   BorderRect.LowerRightCorner.Y = BorderRect.UpperLeftCorner.Y + 1;
   driver->draw2DRectangle(video::SColor(50,180,180,255), BorderRect, &AbsoluteBorderRect);

   BorderRect.LowerRightCorner.Y = AbsoluteBorderRect.LowerRightCorner.Y;
   BorderRect.LowerRightCorner.X = BorderRect.UpperLeftCorner.X + 1;
   driver->draw2DRectangle(video::SColor(50,180,180,255), BorderRect, &AbsoluteBorderRect);

   BorderRect = AbsoluteBorderRect;
   BorderRect.UpperLeftCorner.X = BorderRect.LowerRightCorner.X - 1;
   driver->draw2DRectangle(video::SColor(50,180,180,255), BorderRect, &AbsoluteBorderRect);

   BorderRect = AbsoluteBorderRect;
   BorderRect.UpperLeftCorner.Y = AbsoluteBorderRect.LowerRightCorner.Y - 1;
   BorderRect.LowerRightCorner.Y = AbsoluteBorderRect.LowerRightCorner.Y;
   driver->draw2DRectangle(video::SColor(50,180,180,255), BorderRect, &AbsoluteBorderRect);

   BorderRect = AbsoluteBorderRect;
   BorderRect.UpperLeftCorner.X += 1;
   // BorderRect.UpperLeftCorner.X += 3;
   // };

   //if(Percent == 0) { return; }
   MaxNum = AbsoluteBorderRect.LowerRightCorner.X - 1;
   PercentNum = (Percent * MaxNum) / 100;
   PercentNum += BorderRect.UpperLeftCorner.X;

   LoadRect = BorderRect;
   LoadRect.UpperLeftCorner.X = BorderRect.UpperLeftCorner.X;
   LoadRect.LowerRightCorner.X = PercentNum;
   LoadRect.LowerRightCorner.Y -= 1;


   /// Render Progress Bar:
   driver->draw2DRectangle(video::SColor(200,180,180,255), LoadRect, &AbsoluteBorderRect); // opacity, red, green, blue

   driver->setMaterial(driver->getMaterial2D()); // In case buffer is distorted
   driver->endScene();

   if(Percent >= 100){ isDone = true; };

};




    /*
    // USAGE EXAMPLE:
    CProgressBar* LoadBar;

    LoadBar = new CProgressBar(device,rect<s32>(0,25,800,50),true,7);

    LoadBar->setPercent(25);
    LoadBar->draw();

    //Load some more stuff
    LoadBar->setPercent(50);
    LoadBar->draw();

    //Load more stuff
    LoadBar->setPercent(100);
    LoadBar->draw();

    LoadBar->setVisible(false);
    */



