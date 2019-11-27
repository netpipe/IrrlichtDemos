// Multiple Camera controller functions
// To be included in constructing camera rigs

// current functions are based (and active) on the EventReceiver (To be separated so it support multiple camera rigs at the same time

class Rig : public IEventReceiver
{
      private :
      vector3df target,targetvector; // the Camera Targets
      core::vector3d<f32> RelativeRotation; // this is the relative rotation based on the mouse movement
      float movespeed[6], strafespeed[6], jumpspeed[6]; // the speed variable on the rig 
      int speedaction; //0=run, 1=run, 2=crouch, 3=crawl
      int lastmove; // to store the last used move (run or walk)
      float tiltangle[6], tiltspeed[6]; // store the tilt info for actions
      float crouch[6],cspeed[6]; // store the crouch info for actions
      float walkamplitude; // define the wave amplitude for the walksimulator
      float targetdistance; // define the target distance for the camera
      float maxup,maxdown;
      int action, action1, action2, crouchstate; // states toggle for different actions
      u32 lastAnimationTime;
      f32 timeDiff, lastSprintTime, timeLimit;
      core::position2d<f32> cursor; // Gui cursor 
      float feetcounter, desiredcrouch, currentspeed, currentstrafespeed, oldspeed;
      float tiltAngle, tiltSpeed; // working variables for the tilt actions
      bool slidefront,slideback,slideleft,slideright;
     

      public :
      vector3df CamRot, CamPos;
      vector3df direction; // directional vector of the camera
      core::stringw rigAction, lastAction; 
      bool firstupdate;
      bool gamemode;
      bool runmode, crouchmode, sprintmode, exhausted, pressed;
      bool smoothmode, tiltmode, walksimmode, slidemode;
      static scene::ICameraSceneNode *camera; 
      static scene::ISceneNode *feetnode;
      static scene::ISceneNode *headnode;
      static IrrlichtDevice * rigdevice;

////////////////////////////////////////////////////////////////////////////////
// Function return the current mode the rig is
// True or False if the rig is paused (for GUI settings)
////////////////////////////////////////////////////////////////////////////////           
      bool getRigMode()
      {return gamemode;}
      
////////////////////////////////////////////////////////////////////////////////
// Function return the camera pointer so we can add additionnal things on it
// Like the Collision response Animator, get info, etc.
////////////////////////////////////////////////////////////////////////////////           
      scene::ICameraSceneNode * getCamera()
      {return camera;}

////////////////////////////////////////////////////////////////////////////////
// Function return the camera pointer so we can add additionnal things on it
// Like the Collision response Animator, get info, etc.
////////////////////////////////////////////////////////////////////////////////           
      float getRigTargetDistance()
      {return targetdistance;}

////////////////////////////////////////////////////////////////////////////////
// Function return the camera pointer so we can add additionnal things on it
// Like the Collision response Animator, get info, etc.
////////////////////////////////////////////////////////////////////////////////           
      void setRigTargetDistance(float distance)
      {targetdistance=distance;}

////////////////////////////////////////////////////////////////////////////////
// This define the limit in case of falling outside the level
// Will position the rig at the UP max 
////////////////////////////////////////////////////////////////////////////////           
      void setRigSceneUpDownMax(float up, float down)
      {maxup=up; maxdown=down;}

////////////////////////////////////////////////////////////////////////////////
// Function to enable/disable certain rig functions
// Walksim mode, Smooth acceleration, tilt on strafe, smooth slide when stop
////////////////////////////////////////////////////////////////////////////////      
      void changemode(bool wsm, bool smm, bool tlm, bool slm)
      { walksimmode=wsm; smoothmode=smm; tiltmode=tlm; slidemode=slm;
      }
////////////////////////////////////////////////////////////////////////////////
// Function to set the speed of movements
// modes are:
// 0=Run, 1=Walk, 2=Crouch, 3=Crawl
////////////////////////////////////////////////////////////////////////////////  
      void setRigSpeed(int mode, float move, float strafe, float jump)
      {movespeed[mode]=move; strafespeed[mode]=strafe; jumpspeed[mode]=jump;}
////////////////////////////////////////////////////////////////////////////////
// Function to set tilt angle for types of actions
// modes are:
// 0=Tilt on strafe, 1=Head Tilt 
////////////////////////////////////////////////////////////////////////////////  
      void setRigTilt(int mode, float tilt, float speed)
      {tiltangle[mode]=tilt; tiltspeed[mode]=speed;}
////////////////////////////////////////////////////////////////////////////////
// Function set the crouch position for different actions  
// 0=crouch, 1=Crawl
////////////////////////////////////////////////////////////////////////////////  
      void setRigCrouch(int mode, float value, float speed)
      {crouch[mode]=value; cspeed[mode]=speed;} 
////////////////////////////////////////////////////////////////////////////////
// Set the walk simulator amplitude for fine tuning of the movement 
// Default=1.0f 
////////////////////////////////////////////////////////////////////////////////  
      void setRigWalk(float amplitude)
      {walkamplitude=amplitude;}
////////////////////////////////////////////////////////////////////////////////
// Function get the current speed of the player  
// 
////////////////////////////////////////////////////////////////////////////////  
      void rigstate(stringw status=L"still")
      {
         rigAction=status;
      }  
////////////////////////////////////////////////////////////////////////////////
// Function to return the rig state in a text string (temporary)
// This will need to be changed to something more convenient
////////////////////////////////////////////////////////////////////////////////
      stringw getRigState()
      {return rigAction;}
////////////////////////////////////////////////////////////////////////////////
// Event receiver with all the function attributed to the rig
// MAJOR change will be required there, need to be able to set the keys
////////////////////////////////////////////////////////////////////////////////
     bool OnEvent(const SEvent& event) 
    { 
         //data data;
         //float movespeed, strafespeed, jumpspeed; // tge speed variable on the rig     
         // Keyboard, mouse handling   
         // To do: Use the SKeymap class and their action (add new actions also)
         //        Use a parameter instead of a full class and take a paramenter for EventReceiver.
         //        Put back the Event receiver in place only for other game function (ESC->quit, TAB->menu etc)
         
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_ESCAPE && 
            !event.KeyInput.PressedDown) 
            { 
             rigdevice->closeDevice(); 
             return true; 
            }  
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_TAB && 
            !event.KeyInput.PressedDown) 
            { 
             gui::ICursorControl* cursorcontrol;                           
             if (!gamemode) 
                {gamemode=true; 
                cursorcontrol=rigdevice->getCursorControl();
                cursor=cursorcontrol->getRelativePosition();
                } else 
                {gamemode=false;
                cursorcontrol=rigdevice->getCursorControl();
                cursorcontrol->setPosition(cursor);
                }
                
             return true; 
            }     
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_SPACE) 
            { 
             if (event.KeyInput.PressedDown) {action2=5;rigstate(L"Jumping");} else {action2=0;}
             //printf ("Jump! \n");
             return true;
            }
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_KEY_W) 
            { 
             if (event.KeyInput.PressedDown) {action=1; rigstate(L"moving forward");} else {action=0;if (slidemode) slidefront=true; else slidefront=false;}
             return true;  
            } 
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_KEY_S) 
            { 
             if (event.KeyInput.PressedDown) {action=2; rigstate(L"moving backward");} else {action=0;if (slidemode) slideback=true; else slideback=false;}
             return true;
            } 
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_KEY_D) 
            { 
             if (event.KeyInput.PressedDown) 
             {  action1=3;
                if (tiltmode) 
                   {tiltAngle=-tiltangle[0]*(strafespeed[speedaction]/3); tiltSpeed=tiltspeed[0];}
                    rigstate(L"strafe left");
             } else {action1=0; tiltAngle=0; if (slidemode) slideleft=true; else slideleft=false;}
             return true;
            } 
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_KEY_A) 
            {      
             if (event.KeyInput.PressedDown) 
             {action1=4;
               if (tiltmode) {tiltAngle=tiltangle[0]*(strafespeed[speedaction]/3);tiltSpeed=tiltspeed[0];}
               rigstate(L"strafe right");
             } else {action1=0; tiltAngle=0; if (slidemode) slideright=true; else slideright=false;}
             return true;
            }
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_KEY_Q) 
            {      
             if (event.KeyInput.PressedDown) 
             {  tiltAngle=tiltangle[1];  tiltSpeed=tiltspeed[1]; rigstate(L"tilt left");}
             else 
             {  tiltAngle=0;} 
             return true;
            }
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_KEY_E) 
            {      
             if (event.KeyInput.PressedDown) 
             { tiltAngle=-tiltangle[1]; tiltSpeed=tiltspeed[1]; rigstate(L"tilt right"); }
             else 
             { tiltAngle=0;} 
             return true;
            }
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_KEY_C) 
            {      
             if (event.KeyInput.PressedDown) 
                 { if (crouchstate==0) { desiredcrouch=crouch[0]; speedaction=2; rigstate(L"Crouch");}
                   if (crouchstate==1) { desiredcrouch=crouch[1]; speedaction=3; rigstate(L"Crawl");}
                   if (crouchstate==2) { desiredcrouch=0.0f; speedaction=lastmove; rigstate("Back up");}
                    
                 } else {crouchstate+=1; if (crouchstate>2) crouchstate=0;}
                 return true;
             }
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_CONTROL) 
            {      
             if (event.KeyInput.PressedDown) 
                 { if (!crouchmode || crouchstate==2) 
                   {desiredcrouch=crouch[0]; crouchstate=1; crouchmode=true; speedaction=2; rigstate(L"Crouch");}
                   else 
                   {desiredcrouch=0.0f; crouchstate=0; crouchmode=false; speedaction=lastmove; rigstate("Back up");}
                 } 
                 return true;
             }
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_SHIFT ) 
            {      
             if (event.KeyInput.PressedDown) 
               {// This will increase the speed or make it back to default.
                // To do: input theses as parameters.
                 if (!sprintmode && !exhausted) {oldspeed=movespeed[speedaction]; movespeed[speedaction]*=2; timeLimit=2000.0f; sprintmode=true; lastSprintTime=rigdevice->getTimer()->getRealTime();}
                 if (!sprintmode && exhausted) {timeLimit=9000.0f; sprintmode=true; lastSprintTime=rigdevice->getTimer()->getRealTime();}
                 if (sprintmode && exhausted) {movespeed[speedaction]=oldspeed;}
               } else {sprintmode=false; movespeed[speedaction]=oldspeed;}
             return true; 
            } 
         if(event.EventType == EET_KEY_INPUT_EVENT && 
            event.KeyInput.Key == KEY_CAPITAL ) 
            {  
             //printf ("Current Crouch state: %i\n",crouchstate);    
             if (event.KeyInput.PressedDown) 
             // Change the speed, walk or run, check if the player is crouched first
             if (runmode) 
                 {if (crouchstate==0) 
                      {rigstate("Run mode"); lastmove=0; speedaction=0; runmode=false;}
                 } 
             else 
                 {if (crouchstate==0)
                      {rigstate("Walk mode"); lastmove=1; speedaction=1; runmode=true;}
                 }
             return true;
            }
         return false; 
      } 
      
////////////////////////////////////////////////////////////////////////////////
// Time based animation function
// This will animate the rig and calculate the time
// This "animate function" do the Rig assembly
////////////////////////////////////////////////////////////////////////////////
      void OnAnimate(u32 TimeMS)
      { // function to animate and update the rig and camera
        //
        //
        if (!gamemode)
        {
            //if (!tiltTest(camera,tiltAngle)) 
            timeDiff = (f32) (TimeMS - lastAnimationTime); // Find the difference in time between each call.
            lastAnimationTime=TimeMS;
            //printf("The time diff is: %f\n",timeDiff);
            MouseRotate (camera,TimeMS);
            KeysMove(camera,camera,smoothmode,0,walksimmode,false);
            doCrouch(); doTimedAction(TimeMS,timeLimit);
            if ((currentspeed==0) && (action2==0)) rigstate("still");
            // If player jumped over the level
            if (camera->getPosition().Y < maxdown) {camera->setPosition(CamPos+vector3df(0,maxup,0));
                                                 camera->setTarget(vector3df(0,0,0)); 
                                                 RelativeRotation.X=30.0f;RelativeRotation.Y=0.0f;
                                                 camera->updateAbsolutePosition();}
            // TODO: Make the below code work to update the other animators
            // Need to update animators there. Working but seem to block the jump
            core::list<ISceneNodeAnimator*>::ConstIterator ait = camera->getAnimators().begin();
            for (; ait != camera->getAnimators().end(); ++ait)
            (*ait)->animateNode(camera,TimeMS);   
            camera->updateAbsolutePosition();
            } else {}
            
      }
        
////////////////////////////////////////////////////////////////////////////////
// Get the mouse information X,Y to update the rotation of the node
// The rotation calculated is applied to the cam TARGET
////////////////////////////////////////////////////////////////////////////////
      void MouseRotate(scene::ICameraSceneNode *cam, u32 TimeMS)
      // Animate the camera using the information coming from the inputs
      // (Mouse/Keyboard input at the moment
      //
      {
        gui::ICursorControl* cursorcontrol;
        core::position2d<f32> centercursor;
        //
        cursorcontrol=rigdevice->getCursorControl(); // Capture the mouse cursor
        core::position2d<f32> cursorpos;
        // Move the position
        core::vector3df pos = cam->getPosition();
        direction = vector3df(0,0,targetdistance); // First direction vector 1000 unit in front of the local player axis
       if (firstupdate) // First time use of this setup- Init the camera rig
        {cursorcontrol->setPosition(0.5f,0.5f);
          centercursor=cursorcontrol->getRelativePosition();
          RelativeRotation.X=10;
          RelativeRotation.Y=180;
          firstupdate=false;
          currentspeed=0.0f; desiredcrouch=0; 
          currentstrafespeed=0.0f; tiltAngle=0; tiltSpeed=0;
          // Setup the default speed
          oldspeed=0;
          slidefront=false;
          slideback=false;
          slideleft=false;
          slideright=false;
          action=0;
          action1=0;
          action2=0;
          lastAnimationTime=TimeMS;
          rigAction=L"still"; lastAction=rigAction; pressed=false;
          runmode=false; crouchmode=false; crouchstate=0; sprintmode=false; exhausted=false; timeLimit=0;
          //RelativeRotation.X=-45;
        }
       cursorpos = cursorcontrol->getRelativePosition(); // Get the mouse position
       core::matrix4 mat,mat1; // Define the matrix to be used for finding directional vectors
       if (!core::equals(cursorpos.X, centercursor.X) || 
          !core::equals(cursorpos.Y, centercursor.Y))
          {
          RelativeRotation.X *= -1.0f;
          RelativeRotation.Y *= -1.0f;
          RelativeRotation.X = core::clamp(RelativeRotation.X + (0.5f - cursorpos.Y) * 100.0f,-88.0f,+88.0f);
          RelativeRotation.Y += (0.5f - cursorpos.X) * 100;
          RelativeRotation.X *= -1.0f;
          RelativeRotation.Y *= -1.0f;
          cursorcontrol->setPosition(0.5f,0.5f);
          centercursor=cursorcontrol->getRelativePosition();
          //printf("Relative rotation=%f\n",camera->getRotation().Y);
          camera->setRotation(vector3df(RelativeRotation.X,RelativeRotation.Y,RelativeRotation.Z));
          if (RelativeRotation.Y > 359.0f) RelativeRotation.Y -= 360.0f;
          //if (RelativeRotation.Y < -359.0f) RelativeRotation.Y += 360.0f;
          if (RelativeRotation.Y < -0.0f) RelativeRotation.Y += 360.0f;
          // input the angle of rotation into the transformation matrix
          // TODO - Try to calculate the upvector of the cam to find the new base rotation then add the relative one to it.
          mat.setRotationDegrees(core::vector3df(RelativeRotation.X,RelativeRotation.Y,0));
          mat.transformVect(direction); // Find the direction vector from the matrix
          // Rotate the UPVector of the camera for the Z local axis (this now work)
          vector3df upv = vector3df(0,1,0);
          RelativeRotation.Z=Interpolate(RelativeRotation.Z,tiltAngle,tiltSpeed); // tiltValue
          //RelativeRotation.Z=tiltAngle;
          upv.rotateXYBy(RelativeRotation.Z,vector3df(0,0,0)); // Local Axis rotation
          upv.rotateXZBy(-RelativeRotation.Y,vector3df(0,0,0)); // Current camera rotation Y
          upv.rotateXZBy(-RelativeRotation.X,vector3df(0,0,0)); // Current camera rotation X
          cam->setUpVector(upv); //setup the UPvector
      }
      cam->setTarget(pos+direction); // Set the new target
      cam->updateAbsolutePosition();  
    }
    
////////////////////////////////////////////////////////////////////////////////
// Time based movements, using the keyboard 
// Moves supported: Forward, backward, strafe: left & right, jump
// Most camera rigs function are used in there 
////////////////////////////////////////////////////////////////////////////////
    void KeysMove(scene::ISceneNode *footnode, scene::ICameraSceneNode *cam, 
                  bool smooth=false, float smoothvalue=0.1f, bool walksim=false,
                  bool novertmove=true)
    // Moving part of the code. Theses codes below for the node / camera moving
    // To do: Separate, then apply some parameters (moving node, speed)
    // Also: do the NOvertical movement
    { 
      core::vector3df pos = footnode->getPosition();
      //core::vector3df movedir = direction; // Get the Front/back -- Dependant from the rotate function
      core::vector3df movedir = cam->getTarget()-cam->getPosition(); // Get the Front/back -- Dependant of the camera target.
      if (novertmove) movedir.Y=0.0f;
      movedir.normalize();
      if (smoothvalue==0) smoothvalue=0.1f; //default smoothvalue if 0 is entered
      //if (action) printf("movespeed=%f",movespeed);
      float desiredspeed = movespeed[speedaction];
      float smoothfac = 0;
      if (action==1 || (slidefront==true && smooth)) 
      {            if (smooth)
                   {  if (slidefront) // Released the keyboard, objective is to get to 0
                      {  desiredspeed=0;
                         smoothfac=smoothvalue*3;
                      } else smoothfac=smoothvalue;
                      currentspeed=Interpolate(currentspeed,desiredspeed,smoothfac);
                      if (currentspeed==0) slidefront=false;
                      pos += movedir * ((timeDiff * currentspeed)/15);
                   } else pos+= (movedir * ((timeDiff * movespeed[speedaction])/15));
                   
      }// Move Forward
      
      if (action==2 || (slideback==true && smooth)) 
      {              if (smooth)
                     {  if (slideback) // Released the keyboard, objective is to get to 0
                        {   desiredspeed=0; 
                            smoothfac=smoothvalue*3;
                        } else smoothfac=smoothvalue;
                        currentspeed=Interpolate(currentspeed,-desiredspeed,smoothfac);
                        if (currentspeed==0) slideback=false;
                        pos -= movedir * ((timeDiff * (-currentspeed))/15);
                     } else pos -= (movedir * ((timeDiff * movespeed[speedaction])/15));
                     // Move Backward
      }
      // Walk simulation
      feetnode->setPosition(vector3df(0,0,0));
      if (walksim) {feetnode->setPosition(walksimulator(smooth));
                     //target=vector3df((f32)target.X,(f32)walksimulator(smooth).Y,(f32)target.Z); // Not used
                     }
      // End walk sim
      core::vector3df strafevect = direction;
      strafevect = strafevect.crossProduct(vector3df(0,1,0)); // Get the strafe Vector
      if (novertmove) strafevect.Y=0.0f; // no vertical move implementation
      strafevect.normalize();
      if (action1==3 || (slideleft==true && smooth)) 
      {           if (smooth)
                  {  if (slideleft)
                     {  desiredspeed=0;
                        smoothfac=smoothvalue*3;
                     } else smoothfac=smoothvalue;
                     currentstrafespeed=Interpolate(currentstrafespeed,-desiredspeed,smoothfac);
                     if (currentstrafespeed==0) slideleft=false;
                     pos -= strafevect * ((timeDiff * (-currentstrafespeed))/15);
                  } else pos -= (strafevect * ((timeDiff * strafespeed[speedaction])/15)); 
      } // Strafe left
      if (action1==4 || (slideright==true && smooth))
      {          if (smooth)
                 {  if (slideright)
                    {  desiredspeed=0; 
                       smoothfac=smoothvalue*3;
                    } else smoothfac=smoothvalue;
                    currentstrafespeed=Interpolate(currentstrafespeed,desiredspeed,smoothfac);
                    if (currentstrafespeed==0) slideright=false;
                    pos += strafevect * ((timeDiff * currentstrafespeed) / 15);
                 } else pos += (strafevect * ((timeDiff * strafespeed[speedaction])/15));  
      } // Strafe Right
      if (action2==5) pos += (vector3df(0,1,0) * ((timeDiff * jumpspeed[speedaction]) / 15)); // Jump
      footnode->setPosition(pos); // Set the camera Position
      cam->setTarget(cam->getPosition()+direction); // Set the new target  
      //feetnode->setRotation(vector3df(RelativeRotation.X,RelativeRotation.Y,0));
      cam->updateAbsolutePosition(); 
     }
     
////////////////////////////////////////////////////////////////////////////////
// Sine wave walk simulator, used by the KeysMove function (subfunction)
// 
////////////////////////////////////////////////////////////////////////////////
     vector3df walksimulator(bool smooth)
     //recalculate for simulating walk
     {
        if (feetcounter>360) feetcounter=0;
        float current=0;
        
        if (smooth)
          {  if (action==1 || action==2) current=currentspeed;  //smoothed
             if (action1==3 || action1==4)  current=currentstrafespeed;// strafing
             if (current<0) current=-current; 
             feetcounter += ((current*timeDiff)/15)*3.5f;
          } else 
          {  if (action==1 || action==2) feetcounter += ((movespeed[speedaction] * timeDiff)/15)*3.5f; // non smooth
             if (action1==3 || action1==4) feetcounter += ((strafespeed[speedaction]*timeDiff)/15)*3.5f; //strafing;
          }
           //  Base the sin position on the "feetcounter"
           float piover180 = 0.0174532925f;
           float upmove=sin(feetcounter * piover180);
           vector3df walkrotation = vector3df(upmove,0,0);
           walkrotation.rotateXZBy(RelativeRotation.Y,vector3df(0,0,0));
           if (action==1 || action==2 || action1==3 || action1==4) 
           return vector3df(0,((walkamplitude*upmove)*((currentspeed+strafespeed[speedaction]))/2),0);
     }
////////////////////////////////////////////////////////////////////////////////
// Crouch function
// Use the "interpolate" function to animate the movement to the desired
// position
////////////////////////////////////////////////////////////////////////////////
     void doCrouch()
     { 
       float currentcrouch=headnode->getPosition().Y;
       if (currentcrouch<1.0f)   
         { currentcrouch=Interpolate(currentcrouch, desiredcrouch, 2.5f);
           headnode->setPosition(vector3df(headnode->getPosition().Z,currentcrouch,headnode->getPosition().Z));
         } 
     }
////////////////////////////////////////////////////////////////////////////////
// Timed action
// This is used for the SPRINT feature
////////////////////////////////////////////////////////////////////////////////
     void doTimedAction(u32 Timems, f32 limit)
     {
          bool decide=true;
          if ((sprintmode && (currentspeed>0)) || (!sprintmode && exhausted))
          { f32 elapsed = Timems-lastSprintTime;
             //printf("elapsed time: %f \n",elapsed);
             if (elapsed<limit && !exhausted) {rigstate("Sprint");}
             if (elapsed>limit && !exhausted) {rigstate("Sprint Exausted"); printf ("Im exhausted!\n"); movespeed[speedaction]=oldspeed; exhausted=true; elapsed=0; lastSprintTime=Timems;} 
             if (elapsed>limit && exhausted) {printf ("Im not exhausted, now!\n"); exhausted=false; rigstate("Can Sprint now");}
             //{sprintmode=false; if (exhausted) exhausted=false; else exhausted=true; movespeed=oldspeed;}
          }
      }
     
////////////////////////////////////////////////////////////////////////////////
// Testing function
// Check if the interpolation as reached the limit
////////////////////////////////////////////////////////////////////////////////
     bool tiltTest(scene::ICameraSceneNode *cam, float desiredAngle=0)
     { 
       float currentAngle = RelativeRotation.Z;
       if (desiredAngle!=currentAngle) return false; else return true;
     }

////////////////////////////////////////////////////////////////////////////////
// Interpolate motion to animate the rig elements
// Time base animate. Will return the current info to be interpolated
// Was used to interpolate angles at first
// Now do much more (smooth use this)
////////////////////////////////////////////////////////////////////////////////     
     float Interpolate(float currentAngle, float desiredAngle=0.0f, float amount=0.5f)
     { // This function will allow to set the angle we need on the Z axis of the camera for tilt action
       //
       //
          if (amount==0) amount=0.5f;
          if (desiredAngle>currentAngle) 
            {currentAngle+=(amount*timeDiff)/15; 
             if (currentAngle>=desiredAngle) return desiredAngle; else return currentAngle;
             //printf ("Current: %f, Desired: %f\n",currentAngle,desiredAngle);
             } else
           if (desiredAngle<currentAngle) 
            {currentAngle-=(amount*timeDiff)/15; 
             if (currentAngle<=desiredAngle) return desiredAngle; else return currentAngle;
             //printf ("Current: %f, Desired: %f\n",currentAngle,desiredAngle);
            }
           //printf ("Current: %f, Desired: %f\n",currentAngle,desiredAngle); 
           return currentAngle;
     }
////////////////////////////////////////////////////////////////////////////////
// Create a FPS rig and get the DEVICE pointer for the entire class
// This FPS rig is composed of a Camera and 2 empty nodes for controls
////////////////////////////////////////////////////////////////////////////////               
     void createFPSrig(IrrlichtDevice * deviceparm)
     {
      // Creation of the player RIG here
      // NOTE: This will have to be created in the RIG class (some part of it)
      //
      // First pivot for feet for this rig 
      rigdevice=deviceparm;
      scene::ISceneNodeAnimatorCollisionResponse* anim;
      feetnode = rigdevice->getSceneManager()->addEmptySceneNode();
      //Second pivot point where the camera is attached
      headnode = rigdevice->getSceneManager()->addEmptySceneNode(feetnode);
       //printf ("Create a new camera for the scene\n");
      camera = rigdevice->getSceneManager()->addCameraSceneNode(headnode); // Basic camera to be used for the new camera rig.
      // game.camera = smgr->addCameraSceneNodeFPS(0, 120.0f, 220.0f, -1,MyControl.keyMap, 9, true,0.25f); 
      camera->setFarValue(4500.0); // Define the limit of the view (will render less poly)
      printf ("Camera Created?\n");
      camera->setAspectRatio(1.3);  // 1.6 for 16/10 display, enter 1.3 for 4/3 display or windowed
      camera->setFOV(45 * irr::core::DEGTORAD); // Set a standard FOV
      smoothmode=true;
      tiltmode=true;
      walksimmode=true;
      slidemode=true;
      gamemode=false;
      maxup=3200.0f;
      maxdown=-700.0f;
      walkamplitude=1.0f;
      targetdistance=1000.0f;
      // Set the speed for each defined move:
      setRigSpeed(0,5.0f,3.0f,5.0f); // For running
      setRigSpeed(1,2.0f,1.0f,3.0f); // For walking
      setRigSpeed(2,1.5f,0.7f,2.5f); // For Crouch
      setRigSpeed(3,0.5f,0.5f,1.5f); // For Crawl 
      speedaction=0; // set the current mode for running
      lastmove=0;
      // Set the desired angles for the tilt actions;
      setRigTilt(0,4.5f,0.2f); // For Tilt on strafe
      setRigTilt(1,30.0f,3.0f); // For Head Tilt
      // Set the desired value for crouch
      setRigCrouch(0,-30.0f,0.0f); // for Crouch
      setRigCrouch(1,-50.0f,0.0f); // for Crawl
      }
      
};

// Init the static keyword from the class
scene::ICameraSceneNode * Rig::camera=0;
scene::ISceneNode * Rig::feetnode=0;
scene::ISceneNode * Rig::headnode=0;
IrrlichtDevice * Rig::rigdevice=0;

///////////////////////////////////////////////////////////////////////////////
// Define the keymap for the rig. 
// Not yet implemented. At the moment, is not used
///////////////////////////////////////////////////////////////////////////////
class Gamecontroller
{
public :
      SKeyMap keyMap[9];
      Gamecontroller()
      // Initialise the FPS Keymap for a Qwerty keyboard
      // To add: Define 3 types of keyboard: Azerty, Qwerty, and Dvorak mappings
      // To do: This class is not used right now with the camera rig. 
      // Will need to update the camera rig to use it.
      {
      keyMap[0].Action = EKA_MOVE_FORWARD; 
      keyMap[0].KeyCode = KEY_UP; 
      keyMap[1].Action = EKA_MOVE_FORWARD; 
      keyMap[1].KeyCode = KEY_KEY_W; 

      keyMap[2].Action = EKA_MOVE_BACKWARD; 
      keyMap[2].KeyCode = KEY_DOWN; 
      keyMap[3].Action = EKA_MOVE_BACKWARD; 
      keyMap[3].KeyCode = KEY_KEY_S; 

      keyMap[4].Action = EKA_STRAFE_LEFT; 
      keyMap[4].KeyCode = KEY_LEFT; 
      keyMap[5].Action = EKA_STRAFE_LEFT; 
      keyMap[5].KeyCode = KEY_KEY_A; 

      keyMap[6].Action = EKA_STRAFE_RIGHT; 
      keyMap[6].KeyCode = KEY_RIGHT; 
      keyMap[7].Action = EKA_STRAFE_RIGHT; 
      keyMap[7].KeyCode = KEY_KEY_D;  

      keyMap[8].Action = EKA_JUMP_UP; 
      keyMap[8].KeyCode = KEY_SPACE ;
    
     } 
     ~Gamecontroller()
     { delete keyMap;};       
};
