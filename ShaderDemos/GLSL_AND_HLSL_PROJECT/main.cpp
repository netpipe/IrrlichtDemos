
 // Written by Jacques Andre Pretorius "Vectrotek" in the Republic of South Africa in 2016..
 // E-MAIL: pretoriusjacq@gmail.com
 // Station: Irrlicht 1.8.3 (standard); Visual C++ 7.10 (release) ; Windows XP ; DX9.c ; Nvidia 9600 GT

 // This program uses either GLSL or HLSL to render two objects, one Skinned and one Non - Skinned..
 // It illustrates how Irrlicht sends information to both GLSL and HLSL shaders and yes, there are differences..
 // The information sent to the shaders ranges from Images for Texturing, values like the Camera Position
 // and Arrays of Light Ppositions and Colours that start off as Irrlicht Lights, but not rendered by Irrlicht..

 // Before playing with Normal Maps it is important to understand lighting matters completely..
 // Without "Physicaly Based Lighting" and well constructed "Geometric Normals" Normal maps are quite useless..
 // This program tries to show the simplest way to achieve "Correctly Lit Surfaces" using Normal Maps..
 // One important thing that this program does is to "Cyclically Update Tangents and Binormals" for
 // "Skin" or "Bone" animated models such as characters..(this would be cool in the engine itself)
 // Another Important thing to know is that Tangent etc Updating is NOT necessary for objects that
 // that are NOT animated by Skinned (Weighted) Bones. They still need Tangents and Binormals however..
 // Also objects, (Non-Skinned) that are rotated, scaled and translated by Irrlicht itself do not need to undergo this "Per Frame Tangent Updating"..
 // The matrices handles that..

 // Finally, examples of NON-Skin-Animated objects are "*.obj" (Object) from Alias or, among others "*.lwo" (Lightwave Object).
 // Examples of Skinned-Animated Objects are "*.x", "*.md2", "*.dae" (issues) and "*.b3d" using either "Skinned" (Bones) or "Morph" based animation..
 // These are usually Characters..

 // This program is written to work with the "Irrlicht 1.8.3 Standard" DLL (1.8.2 & 1.8.1 should also work)
 // (the included Irrlicht 1.83 is not recompiled under any pipeline or trunk development)..

 #include <irrlicht.h>
 #include <iostream>   // used by "cin" etc..
 //--------------------------------- ---- --- -- -
 using namespace irr;
 #ifdef _MSC_VER
 #pragma comment(lib, "Irrlicht.lib")
 #endif

 // This disables the console, but we need it to choose between GLSL and HLSL..
 // #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")  // This disables the console..

 // #define _WIN32_WINDOWS 0x0410   // Should you like to use the mouse wheel..

 //--------------------------------- ---- --- -- -
 using namespace irr;
 using namespace core;
 using namespace scene;
 using namespace video;
 using namespace gui;
 //--------------------------------- ---- --- -- -
 // Definitions of Image IDs used by the Shaders..
 #define DIFFUSE_LAYER    0   // Diffuse and Clipmap (32 bit).
 #define NORMALS_LAYER    1   // Normals.. “Y-UP” standard Nomral Map..
 #define SPECULAR_LAYER   2   // Specular Image.. Specular is treated as an 8 bit grey scale value..
 #define GLOSS_LAYER      3   // Gloss Map.. PLAECF is used to convert grey scale values into Gloss (Hotspot Tightness)..

 // Images like "Emission" was given up because "Standard Irrlicht" still doesn't support more than 8 images for shaders..
 // It is however possible to cleverly encode these extra layers into the 4 existing images but this example
 // deals with standard 24 and 32 Bit image formats as you'd expect (my earlier stuff used these resulting in overly complex code)..
 // (with encoding many more layers can be squeezed into standard 32 bit images as a work around for the MAX FOUR shader image isue)..

 // #define IMAGEX005                4   // From here on you need an engine recompile
 // #define IMAGEX006                5   //  unless the devs have EIGHT possible images in the next standard release..
 // #define IMAGEX007                6
 // #define IMAGEX008                7

 // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 // * *  THESE ARE IDENTICAL IN "SHADER CODE" (GLSL & HLSL) AND "APP CODE" to simplify coded communication between the twO  * * * * *
 // INTEGER ARRAY to the "FRAGMENT SHADER" (GLSL or HLSL) Index reservations..
 // Note that the Fragment Shader ALSO uses these "Identically Named" defines..
 // Thus you can copy this directly to the shader when you make changes..
 #define MAX_LIGHT_COUNT          4    // This is used in the shader when it declares arrays of Light Related Values..
                                       // It can NEVER be LESS THAN the "ActiveLightCount" variable sent to the shader in the INTEGER ARRAY..
                                       // Understand that this DEFINE determines the SIZE OF THE LIGHT RELATED ARRAYS in the SHADER..
                                       // Also Understand that this DEFINE (in appcode) determines the NUMBER OF IRRLICHT LIGHTS in the appcode....
                                       // We need a "special bond" between "Irrlicht Lights" and "Shaded Lighting" right
                                       // from the start because it will become important in the future..
                                       // Now, the "PassIntArray [IAI_000_ACTIVE_LIGHTCOUNT]" is the NUMBER OF LIGHTS rendered by the shader..
                                       // (this we want to control from our Appcode or Program)..
                                       // Understand that "ActiveLightCount" in the App Code, exists in the PASSED ARRAY, at the [IAI_000_ACTIVE_LIGHTCOUNT] index..
                                       // Clearly "ActiveLightCount" i.e. PassIntArray [IAI_000_ACTIVE_LIGHTCOUNT] CANNOT be MORE than MAX_LIGHT_COUNT.
                                       // Remember that MAX_LIGHT_COUNT is the number of Irrlicht Lights created in the APPCODE but in the Shader it is
                                       // the NUMBER OF LIGHTS as they exist in the Light "Position" and "Colour" etc ARRAYS..
                                       // So.. MAX_LIGHT_COUNT and ActiveLightCount is closely related..
                                       // MAX_LIGHT_COUNT is a #define and "ActiveLightCount" is an Integer Variable..
                                       // "ActiveLightCount" should NEVER exceed MAX_LIGHT_COUNT..
                                       // MAX_LIGHT_COUNT appears in the Appcode AND the Shader and should remain the same defined value..
                                       // "ActiveLightCount" in the appcode exists in the Shader as the "IAI_000_ACTIVE_LIGHTCOUNT" index of the "PassIntArray" array..
                                       // The code between the two "// * * * * * * " lines are Identical in both Appcode and the Shader..
                                       // (keep them identical by copying and pasting between Shader and Appcode)..
                                       // You'll will get it, and when you do, you'll understand the value of sending arrays of Floats and Ints to shaders..

 #define MAX_INTS_INDEX             2  // The beauty of Array Based Value Passing is that we could have quite a lot of values sent from Appcode to the Shader..
                                       // This example show how to do it for both GLSL AND HLSL..
 #define IAI_000_ACTIVE_LIGHTCOUNT  0  // The INDEX in the "PassIntArray" Array for "Active Light Count"..
 #define IAI_001_PLAECF_STATUS      1  // The INDEX in the "PassIntArray" Array for "PLAECF" used or not..

 // FLOAT ARRAY to the "FRAGMENT SHADER" (GLSL or HLSL) Index reservations..
 // Note that the Fragment Shader for BOTH SHADERS ALSO uses these DEFINES..
 // Thus you can copy this directly to the shader when you make changes..
 #define MAX_FLOATS_INDEX            2 // Again, Could be more..
 #define FAI_000_GLOBAL_BRIGHTNESS   0 // The INDEX in the "PassFloatArray" Array holding the controll over GLOBAL LIGHT BRIGHTNESS..
 #define FAI_001_LERP_TO_RAW_DIFFUSE 1 // The INDEX in the "PassFloatArray" Array holding the position of a LERP TO ORIGINAL DIFFUSE..

 // - End of defines that are identical to those in the GLSL & HLSL Fragment Shader (keep identical at all times)..
 // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 // Our VARIABLES.. All placed here and not declared when needed (which has its advantages).
 // While developing we want a "general overview" of what we'll be working with..
 // Once we're ready to release our product we may want to have these declared in a "tighter" way for the compiler
 // to generate cleaner and more optimised machine code..

 //--------------------- ---- --- -- -

 IrrlichtDevice*                 TheIRRNullDevice = 0; // Temporary pointer used to detect the current desktop resolution..
 IrrlichtDevice*                 TheDevice        = 0;
 ISceneManager*                  TheSceneManager;
 core::dimension2d<u32>          TheDeskResolution;  // As you set it in windows..

 // Some Matrices out of many possible combinations..
 // These matrices are what makes "Physically Based Lighting" posiible (see DOOM 2016)
 // under both GLSL and HLSL..
 // They are "numbered" because I had to try HUNDREDS of different combinations..
 // These are the three good ones..

 core::matrix4                   M01World;
 core::matrix4                   M05WorldViewProjection;
 core::matrix4                   M17WorldInverseTranspose;

 // These variables are used by the "Tangent Updater" to decide which meshes are Tan-Updated or not..
 u32                             SceneMeshCount;          // How many meshes are there in the scene?
 IMeshCache*                     TheSceneMeshCache;       // NOT to be assigned CYCLICALLY i.e. at every frame..
 irr::scene::IMeshBuffer*        CurrMeshBuffPtr ;        // If very big, some meshes may have TWO or more Buffers.. We assign this buffer cyclically..
 video::S3DVertexTangents*       CurrTanVertsPtr;         // Current Tangent Vertex Pointer..
 IAnimatedMesh*                  AcquiredAnimeshByIndex;  // Mesh by Index. This can change during runtime!
 IMesh*                          AcquiredImesh;           // This could be Tan Updateable or NOT Tan Updateable..
 // - ANIMATED MODELS (DirectX or BLITZ BASIC etc) -
 scene::IAnimatedMesh*           AniMesh0001_Skinned;     // Gets loaded with animated meshes like "*.b3d", "*.x" and "*.dae"..
 scene::IAnimatedMeshSceneNode*  IScANInode001_Skinned;   // The generally controllable Node is CLOSELY RELATED to the above mesh..
 // - STILL MODELS (ALIAS or DAE etc) -
 scene::IAnimatedMesh*           AniMesh002_NON_Skinned;
 scene::IMesh*                   TanMesh002_NON_Skinned;  // When Tangents and Binormals are added.. (both "Skined" AND "Non Skinned" meshes must get them)..
 scene::ISceneNode*              ISceneNode002_NON_Skinned;
 video::SColor                   BckGrndCol;              // Background Colour when Sky Cube or Dome is not rendered..
 float                           CurrentAnimSpeed;
 float                           CameraFov;
 video::E_DRIVER_TYPE            SelectedDriverType;      // This example shows both GLSL and HLSL..
 s32                             IDSelectedShader = 0;
// video::E_GPU_SHADING_LANGUAGE   ShaderLanguage;          // GLSL or HLSL determines what is done..
                                                          // A CUSTOM Recompiled Irrlicht DLL handles my GLSL fine but when
                                                          // I try HLSL it has problems.. Why? (I'd appreciate an answer)..
 bool                            TanUpdStatus;            // Do we update the Tangents & Binormals in realtime or not..
                                                          // For non skinned (boned) models we need NOT update them..
                                                          // If you look at the "Tan Updater Code" later then you'll see that it MAKES ITS OWN DECISION..
                                                          // Irrlicht handles rotated, moved and scaled (non-skinned) objects O.K.
 s32                             ActiveLightCount;        // This value limits the amount of lights rendered by the shader..
                                                          // It may NEVER be more than the "MAX_LIGHT_COUNT" #define..
                                                          // The reason for having this as a variable here and not as a #define is to show that we
                                                          // could easily control this value during ingame action like an explosion
                                                          // or gun mussle flashes that would emit light for a few frames only..
                                                          // Do understand why it should not exceed the value of the #define MAX_LIGHT_COUNT..
 core::vector3df                 CameraPosition;          // Apart from being used internally by the renderer this is ALSO sent to our Fragment Shaders..
                                                          // Without this we cannot correclty render Diffuse and Specular Lighting in the Shaders..
 // - L I G H T S -
 // For good practice we manage our lights under the Irrlicht system..
 // We don't use Irrlicht to render their effects because our GPU Shader is invoked..
 // However, changes made to these lights would then find their way into the shaders..
 scene::ILightSceneNode*         IRRLightARRAY [MAX_LIGHT_COUNT]; // MAX_LIGHT_COUNT is defined in the shaders too!
                                                                  // You see, the size of this array
                                                                  // is pretty constant, but we may want
                                                                  // to actually render LESS lights during
                                                                  // during ingame action..
                                                                  // We should NEVER try to render MORE than MAX_LIGHT_COUNT..
 core::vector3df  LightsPosArray    [MAX_LIGHT_COUNT];  // We can send Arrays of lights to the shaders..
 core::vector3df  LightsColArray    [MAX_LIGHT_COUNT];
 f32              PassFloatArray    [MAX_FLOATS_INDEX]; // MAKE SURE these values are the same for their counterparts in Shader Code..
 s32              PassIntArray      [MAX_INTS_INDEX];
 bool             ImageAssignedStatus;                  // Set to true once we've sent the image refrences for the first time..
                                                        // so we dont need to send it at each frame and waste time..

 void ResetShaderArrays (); // Reset all the values kept in Shader Arrays to their DEFAULTS..

 class MyEventReceiver : public IEventReceiver
  {public:
   MyEventReceiver()
    {for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i) KeyIsDown[i] = false;
    }
   virtual bool OnEvent(const SEvent& CurrEvent)
    {if(CurrEvent.EventType == irr::EET_KEY_INPUT_EVENT)
      {
       KeyIsDown[CurrEvent.KeyInput.Key] = CurrEvent.KeyInput.PressedDown;
       if(!CurrEvent.KeyInput.PressedDown)
        {switch(CurrEvent.KeyInput.Key)
          {
           /*  // Change values by key presses for fun.. This is a good place for you to start testing different values..
           case irr::KEY_KEY_1:
           IRRLightARRAY [0]->setPosition(vector3df (CameraPosition.X , CameraPosition.Y , CameraPosition.Z) );
           break;
           case irr::KEY_KEY_2:
           IRRLightARRAY [1]->setPosition(vector3df (CameraPosition.X , CameraPosition.Y , CameraPosition.Z) );
           break;
           */
          }
        }
      }
     return false;
    }
   // Check whether a key is being HELD DOWN like Game Controll Switches..
   // "Clicked" keypresses (like typing) are handled differently..
   virtual bool IsKeyDown(EKEY_CODE keyCode) const {return KeyIsDown[keyCode];}
   private:
   bool KeyIsDown[KEY_KEY_CODES_COUNT];
  };

 //

 // For both GLSL and HLSL..
 class OurShaderCallBackClass : public video::IShaderConstantSetCallBack
  {public:
   virtual void OnSetConstants(video::IMaterialRendererServices* TheMATRENDServices, s32 userData)
    {video::IVideoDriver* TheDriver = TheMATRENDServices->getVideoDriver();
     s32 TexAddresses [8]; // 4 is current max without a Irrlicht Engine recompile.. (8 is max with a recompile, but unlike GLSL, D3D don't like MY recompiles)
     TexAddresses [DIFFUSE_LAYER]   = DIFFUSE_LAYER; // The Index and the Values are IDENTICAL fo0r both GPU languages..
     TexAddresses [NORMALS_LAYER]   = NORMALS_LAYER;
     TexAddresses [SPECULAR_LAYER]  = SPECULAR_LAYER;
     TexAddresses [GLOSS_LAYER]     = GLOSS_LAYER;

     if ( SelectedDriverType == EDT_OPENGL)
      {//
       M01World = TheDriver->getTransform(video::ETS_WORLD); // For GLSL this is all we need!
       TheMATRENDServices->setVertexShaderConstant("M01World", M01World.pointer(), 16);

       // We don't have to send images at each and every frame! But we could if we wanted to..
       if (ImageAssignedStatus == 0) // Unless ofcourse we want to change these images during ingame action.
        {TheMATRENDServices->setPixelShaderConstant("DiffuseMapSample",   (s32*)(&TexAddresses [DIFFUSE_LAYER]), 1);
         // TheMATRENDServices->setPixelShaderConstant("DiffuseMapSample",   (s32*)(&TexAddresses [NORMALS_LAYER]), 1);   // SEE HOW THIS IS POSSIBLE UNDER GLSL but NOT HLSL....
         TheMATRENDServices->setPixelShaderConstant("NormalMapSample",    (s32*)(&TexAddresses [NORMALS_LAYER]), 1);
         TheMATRENDServices->setPixelShaderConstant("SpecularMapSample",  (s32*)(&TexAddresses [SPECULAR_LAYER]), 1);
         TheMATRENDServices->setPixelShaderConstant("GlossMapSample",     (s32*)(&TexAddresses [GLOSS_LAYER]), 1);
        }
       ImageAssignedStatus = 1; // So we don't load these values again (unless explicitly want to change image like Quake II's dammage diffuses)..

       // Here we have our old MAX 4 Image thing solved to a degree except that for HLSL, an engine recompilation has serious issues. (GLSL is O.K. however).
       // This is probably due to the DX API by most of the Irrlicht developers being a different version to my DX API!!
       // HOW DO WE USE "SAMPLER CUBE" ??
       // Jimmy's Cubemap Fix works fine for now..
       // Apparently it does work under some version of the engine..
       // We'd obviously want to send this at each frame..
       TheMATRENDServices->setPixelShaderConstant("CameraPos", &CameraPosition.X, 3);    // see how we have the ".x" to indicate the first in an array..
       TheMATRENDServices->setPixelShaderConstant("LightsPosArray[0]", (f32*)(&LightsPosArray[0].X), (ActiveLightCount * 3));
       // see how we have the "[0].X" indicating [0],
       TheMATRENDServices->setPixelShaderConstant("LightsColArray[0]", (f32*)(&LightsColArray[0].X), (ActiveLightCount * 3));
       // SEE HOW the "INDEX NUMBER" in the "STRINGED NAME" IS "LightPos001[0]"! In the Shader it is declared as "LightPos001[16]"!
       // Under GLSL This solves a LOT of problems..
       // Also see how the "Count" value is "Max Array Index" * "vector3df's Float ItemCount" which is THREE..
       // This essentially allows us to use "Loops" in the shader for our "Many Lights"..
       TheMATRENDServices->setPixelShaderConstant("PassFloatArray[0]", (f32*)(&PassFloatArray [0]), MAX_FLOATS_INDEX); // 24
       TheMATRENDServices->setPixelShaderConstant("PassIntArray[0]", (s32*)(&PassIntArray [0]), MAX_INTS_INDEX);  // was and probably is still 7..
       // ===============================
       // THE_GLITCH.. You can ignore the above!!
      }//

     if (SelectedDriverType == EDT_DIRECT3D9)
      {//

       // We handle matrices for HLSL slightly different to the way we do in GLSL..
       // This may have something to do with a small and insignificant Tangents issue under HLSL..
       M01World = TheDriver->getTransform(video::ETS_WORLD);
       M05WorldViewProjection =   TheDriver->getTransform(video::ETS_PROJECTION);
       M05WorldViewProjection *=  TheDriver->getTransform(video::ETS_VIEW);
       M05WorldViewProjection *=  TheDriver->getTransform(video::ETS_WORLD);
       M17WorldInverseTranspose = M01World.getTransposed(); // For Irrlicht this replaces the commonly used "world matrix!"
       M17WorldInverseTranspose.makeInverse();
       // You may (under "pipeline") have to use.. MyMatrix = services->getVertexShaderConstantID("my_matrix"); etc..
       // (I think Irrlicht would do well having both methods available to coders!)
       TheMATRENDServices->setVertexShaderConstant("M01World", M01World.pointer(), 16);
       TheMATRENDServices->setVertexShaderConstant("M05WorldViewProjection",   M05WorldViewProjection.pointer(), 16);
       TheMATRENDServices->setVertexShaderConstant("M17WorldInverseTranspose", M17WorldInverseTranspose.pointer(), 16);

       // IMPORTANT! See how we have "LightsPosArray" in HLSL and "LightsPosArray[0]" in GLSL..
       // Also, see how both "(f32*)" and "reinterpret_cast<f32*>" works fine.. (which is faster?)..
       TheMATRENDServices->setPixelShaderConstant("LightsPosArray", (f32*)(&LightsPosArray[0].X), (ActiveLightCount * 3));
       TheMATRENDServices->setPixelShaderConstant("LightsColArray", (f32*)(&LightsColArray[0].X), (ActiveLightCount * 3));

       // Again, have a good look at the "stringed" array-name and see how with HLSL it does NOT have the "[0]" included like in GLSL..
       TheMATRENDServices->setPixelShaderConstant("PassFloatArray", (f32*)(&PassFloatArray [0]), MAX_FLOATS_INDEX);
       TheMATRENDServices->setPixelShaderConstant("PassIntArray", (s32*)(&PassIntArray [0]), MAX_INTS_INDEX);

       // Under Irrlicht HLSL, images are sent WITHOUT HUMAN INTERVENTION!!!
       // This is very cool, but it does not allow us to change the image sent to the shader on the fly like we can do in GLSL!!
       // It is useful to change the image to simulate, say, damage by changing the normal map..

       // Irrlict does the following AUTOMATICALLY (probably based on your Texture Addresses) which is good but, again, deprives us of the ability to control this ourselves!
       // We want control over this like we have under GLSL..
       // It remains a mystery to me..

       if (ImageAssignedStatus == 0) // Unless ofcourse we want to change these images during ingame action (which we cant do in HLSL!!!).
        {// TheMATRENDServices->setPixelShaderConstant("DiffuseMapSample",  (s32*)(&TexAddresses [DIFFUSE_LAYER]), 1); // Same as GLSL..
         // TheMATRENDServices->setPixelShaderConstant("NormalMapSample",   (s32*)(&TexAddresses [NORMALS_LAYER]), 1);
         // TheMATRENDServices->setPixelShaderConstant("SpecularMapSample", (s32*)(&TexAddresses [SPECULAR_LAYER]), 1);  // OVERRIDEN!!!
         // TheMATRENDServices->setPixelShaderConstant("GlossMapSample",    (s32*)(&TexAddresses [GLOSS_LAYER]), 1);  // OVERRIDEN!!!
        }
       ImageAssignedStatus = 1; // So don't load these image values again (unless changeing a shader image)..

       // Non-Image Related Variables sent to the shader works as expected though..
       // TheMATRENDServices->setPixelShaderConstant("CameraPosFRAG", reinterpret_cast<f32*>(&CameraPosition), 3);   // This could also have worked..
       TheMATRENDServices->setPixelShaderConstant("CameraPosFRAG", (f32*)(&CameraPosition), 3);

      }//

     float AppShaderCommand = 0;
    }
  };
 //----------------------------------------------------------- ------ ----- ---- --- -- -




 int main()
  {// Determine your windows screen resolution..
   TheIRRNullDevice    = createDevice(video::EDT_NULL);    // Null Device for us to get our windows resolution..
   TheDeskResolution   = TheIRRNullDevice->getVideoModeList()->getDesktopResolution();    // As set by you in windows setup..
   TheIRRNullDevice->drop();  // We got the desktop resolution, so drop the Null Device..
   MyEventReceiver TheReceiver; // This receiver is the one assigned in the CREATE DEVICE function..
   printf("Select the DriverType:\n"\
          " (a) OpenGL \n (b) Direct3D  \n (otherKey) exit\n\n");
   char inkeypress;
   std::cin >> inkeypress;
   switch(inkeypress)
    {case 'a': SelectedDriverType = video::EDT_OPENGL;    break;
     case 'b': SelectedDriverType = video::EDT_DIRECT3D9; break;
     default: return 1;
    }
   TheDevice = createDevice(SelectedDriverType, TheDeskResolution, 32, false, false, false, &TheReceiver);
   if (TheDevice == 0) return 1;
   video::IVideoDriver* TheDriver = TheDevice->getVideoDriver();
   scene::ISceneManager* TheSceneManager = TheDevice->getSceneManager();
   video::IGPUProgrammingServices* TheGPUProgrammingServices = TheDriver->getGPUProgrammingServices();   // Whether GLSL or HLSL..
   //  - VARIABLE INITIALISATION -
   // even coded into your shaders..
   // BckGrndCol = SColor( 255 , 207 , 211 , 192 );

   BckGrndCol = SColor( 255 , 0 , 0 , 0 );
   TanUpdStatus = true;  // Turn this off to see how skinned animation could mess with tangents and thus the final normals..
                         // There is a lot that can be said about this...
   ActiveLightCount = 4; // NEVER to exceed "MAX_LIGHT_COUNT"..(try smaller values to see less lights)

   CurrentAnimSpeed = 10.0;  // Animating your objects in Max or Blender etc is a big subject not covered here.. (see "Realtime Rant" posted elsewhere) ..

   ResetShaderArrays ();     // Here resetting means "Set Defaults"..(see the definition further down)..
   // Add the camera controls.. "WASD" or "ARROW KEYS"..
   SKeyMap keyMap[8];
   keyMap[0].Action  = EKA_MOVE_FORWARD;  keyMap[0].KeyCode = KEY_UP;   // Arrow Keys AND "WASD"..
   keyMap[1].Action  = EKA_MOVE_FORWARD;  keyMap[1].KeyCode = KEY_KEY_W;
   keyMap[2].Action  = EKA_MOVE_BACKWARD; keyMap[2].KeyCode = KEY_DOWN;
   keyMap[3].Action  = EKA_MOVE_BACKWARD; keyMap[3].KeyCode = KEY_KEY_S;
   keyMap[4].Action  = EKA_STRAFE_LEFT;   keyMap[4].KeyCode = KEY_LEFT;
   keyMap[5].Action  = EKA_STRAFE_LEFT;   keyMap[5].KeyCode = KEY_KEY_A;
   keyMap[6].Action  = EKA_STRAFE_RIGHT;  keyMap[6].KeyCode = KEY_RIGHT;
   keyMap[7].Action  = EKA_STRAFE_RIGHT;  keyMap[7].KeyCode = KEY_KEY_D;

   // Static and Realtime TEXT on screen becomes very usefull during development, so I left this here..
   IGUIEnvironment* OurGUIEnvironment = TheDevice->getGUIEnvironment();
   // These are the variables used to return "static" and "realtime" info via the GUI to the rendered screen..
   // Some values (no change) are updated once outside of the main loop and some (changing) are updated at every Nth frame..
   u16 StartGuiID =  100 ;         // 100 to make space for other possible eventualities..
   u16 RunningGuiID = StartGuiID;  // Used in text values "assigned once" and in text "changed cyclically" during the animation..
   u16 ItemYPosA =  10 ;           // The Starting Y Position of our TextBoxes..
   u16 ItemYPosB =  20 ;           // The Starting Y Extent of our Textboxes..
   u16 ItemWidth =  400 ;          // This is the Width of the Textboxes..
   u32 ItemCount =  4 ;            // Be sure not to exceed this value as used in the array definition below..
   // Note how the pointer doesnt get acessed again for the later setting of text, but rather the ID assigned to the item..
   IGUIStaticText* TGuiItemArray [ 4 ];  // Should be the same as "ItemCount" above..
   //=========================================================
   // - Create the text areas for our GUI Items -
   // This happens ONCE and not cyclically..
   // The updating of the text can happen once, before or cyclically during animation..
   for (u16 GuiItI = 0 ; GuiItI < ItemCount ; GuiItI++)
    {TGuiItemArray [GuiItI] = OurGUIEnvironment->addStaticText(L"---",
                                                               rect<s32>(10,ItemYPosA, 600,ItemYPosB),
                                                               false,
                                                               false,
                                                               0,
                                                               RunningGuiID,
                                                               false);
     RunningGuiID++;
     ItemYPosA += 10;   // Incrementing Textbox Coordinates..
     ItemYPosB += 10;
    }

   // Handle our "On Screen Text"..
   IGUIElement* OurStTxt;
   u16 CurrentGuiUI = StartGuiID;
   u32 StartLiveGuiUI ;
   //===========================================
   OurStTxt = TheDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId (CurrentGuiUI);
   if (OurStTxt) {stringw OurStr = L" Text placed only once (not cyclic) ";
   OurStTxt->setText(OurStr.c_str());}
   CurrentGuiUI++;
   CurrentGuiUI++;

   OurStTxt = TheDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId (CurrentGuiUI);
   if (OurStTxt) {stringw OurStr = L"  ";
   OurStTxt->setText(OurStr.c_str());}
   StartLiveGuiUI = CurrentGuiUI;  // This is where we start setting the In-Animation Loop Values..
   //===========================================
   // In Blender the most managageable size for a model is 20 X 20 so we have the
   // camera position and speed match this..
   scene::ICameraSceneNode* TheCamera
     = TheSceneManager->addCameraSceneNodeFPS(0,
                                              200,
                                              0.0100,
                                              -1,
                                              keyMap,
                                              8);
   TheCamera->setPosition(core::vector3df(0.0,0.0,50.0));
   TheCamera->setTarget(core::vector3df(0.0, 0.0, 0.0)); // This changes unseen when a FPS camera is mouse-aimed and moved around..
   CameraFov = 80.0;  // The bigger this angle is, the biggger the 3D objects "appear"..
   TheCamera->setFOV(CameraFov * 0.017453);
   // We dont need Mipmapping for our skycube..
   TheDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
   /*  // Uncomment this for the "SkyBox" keeping in mind that the images "MUST" exist in our "Data/Media Directory"..
   ISceneNode* TheSKYBOXIscenenode
     = TheSceneManager
     ->addSkyBoxSceneNode(TheDriver->getTexture("Data/0000_SKY_BOXES/0003_ENV_A/Y_POSITIVE.png"),       // "TOP"
                          TheDriver->getTexture("Data/0000_SKY_BOXES/0003_ENV_A/Y_NEGATIVE.png"),       // "DOWN"
                          TheDriver->getTexture("Data/0000_SKY_BOXES/0003_ENV_A/X_NEGATIVE.png"),       // "LEFT"
                          TheDriver->getTexture("Data/0000_SKY_BOXES/0003_ENV_A/X_POSITIVE.png"),       // "RIGHT"
                          TheDriver->getTexture("Data/0000_SKY_BOXES/0003_ENV_A/Z_NEGATIVE.png"),       // "FRONT"
                          TheDriver->getTexture("Data/0000_SKY_BOXES/0003_ENV_A/Z_POSITIVE.png"));      // "BACK"
   //*/

   // Because we disabled mipmapping for skycube images we need to now re-enable them for our other images..
   TheDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
   TheDevice->getCursorControl()->setVisible(false);

   // LIGHTS (we try to have "Irrlicht Lights" talk to our shaders)..
   // Initialisation (could have been a for loop)..
   IRRLightARRAY [0] = TheSceneManager->addLightSceneNode(0, vector3df( 0 , 0 , 0 ), SColorf( 1.0f , 1.0f , 1.0f ), 1.0f, 1); // We have Light 001 always at Camera Position..
   IRRLightARRAY [1] = TheSceneManager->addLightSceneNode(0, vector3df( 0 , 0 , 0 ), SColorf( 1.0f , 1.0f , 1.0f ), 1.0f, 2);
   IRRLightARRAY [2] = TheSceneManager->addLightSceneNode(0, vector3df( 0 , 0 , 0 ), SColorf( 1.0f , 1.0f , 1.0f ), 1.0f, 3);
   IRRLightARRAY [3] = TheSceneManager->addLightSceneNode(0, vector3df( 0 , 0 , 0 ), SColorf( 1.0f , 1.0f , 1.0f ), 1.0f, 4); // Do we need to "increment" the ID?
   // O.K. weve created FOUR Lights (we can do hundreds), now set their data..
   // See how some items are set at NODE LEVEL where others are set via Getting the DATA..
   // LIGHTS INITIALISATION (GETS SET IN RUNTIME TOO!!!)..
   // We also update this in realtime in the main loop..
   IRRLightARRAY [0]->setPosition(vector3df ( 0.0 ,  0.0 ,     300.0 )); // This gets overridden as we have it "hooked to the camera"..
   IRRLightARRAY [1]->setPosition(vector3df (  0.0 , -300.0 , 0.0 ) );
   IRRLightARRAY [2]->setPosition(vector3df (  -300.0 , 0.0 , 0.0 ) );
   IRRLightARRAY [3]->setPosition(vector3df (  300.0 , 0.0 , 0.0 ) );

   // We will not use these lights in the same way as it is used by irrlicht.
   // We will however have these lights in their Irrlicht form i.e. "ILightSceneNode" because we'd like
   // to have our shader and Irrlicht work closely together so that any possible future developemnt
   // with shaders could be easier.. (some day, someone could even have these shaders hardcoded into the engine :)
   // We'd also like to be able to contol these light's animation etc like we can do with other "Scenenodes"..
   // In other words our shader will read the information from these Irrlicht lights and act accoringly
   // using, for instance, methods like "getLightType()"..
   // Irrlicht could then be seamlessly used with the Shader in, for instance, a "SpotLight" (yet to be coded)..
   // For now, our shader, when it sees that this light is a "Point Light", will know what to do with it..
   // We'd have "getLightData().Position"..

   // Here we do the actual "Loading" of our Shader Programs depending on which Driver Type we had selected i.e. GLSL or HLSL..
   //                                             OPEN GL
   if (SelectedDriverType == EDT_OPENGL)
    {if (TheGPUProgrammingServices)
      {OurShaderCallBackClass* OurCallback = new OurShaderCallBackClass();
//       ShaderLanguage = video::EGSL_DEFAULT; // Meaning HLSL or GLSL.. (cg is not supported but an engine recompile could bring it back)..
                                             // Though not supported anymore CG is still very usefull for certain platforms..
       IDSelectedShader = TheGPUProgrammingServices
                 ->addHighLevelShaderMaterialFromFiles("Shaders/GLSL_002_VERTEX_PROGRAM.vert",  // Vertex Program Filename and Directory Location..
                                                       "main",                                  // Main Vertex Function Name
                                                       EVST_VS_2_0,                             // Vertex Program Profile..
                                                       "Shaders/GLSL_001_FRAGMENT_SHADER.frag", // Fragment Shader Filename.. (with GLSL it it seems mandatory to have them in separate files)..
                                                       "main",                                  // Main Fragment function name..(with GLSL other names seems to cause problems?)
                                                       EPST_PS_2_0,                             // Could be 3_0 Fragment Shader Profile but NO PROBLEM WITH GLSL (in HLSL we have an issue)..
                                                       OurCallback,
                                                       EMT_TRANSPARENT_ALPHA_CHANNEL_REF,       // This is what makes the "Clip Mapping" possible  "Mmmmonster kill!!"..
                                                       0
//                                                       ShaderLanguage                           // As we had selected in the console..
                                                      );
       // For some reason I couldn't get both VERTEX PROGRAM and FRAGMENT SHADER in ONE FILE..
       // (something to do with a function HAVING TO BE NAMED "main")?..
       OurCallback->drop();
      }
    }
   //                                             DIRECT 3D
   if (SelectedDriverType == EDT_DIRECT3D9)
   {if (TheGPUProgrammingServices)
     {OurShaderCallBackClass* OurCallback = new OurShaderCallBackClass();
//      ShaderLanguage = video::EGSL_DEFAULT; // Meaning HLSL or GLSL..
      IDSelectedShader = TheGPUProgrammingServices
                  ->addHighLevelShaderMaterialFromFiles("Shaders/HLSL_0043_WORKING.hlsl",
                                                        "vertexMain",
                                                        EVST_VS_2_0,  // Seems Fine..
                                                        "Shaders/HLSL_0043_WORKING.hlsl", // Ja.. Same name..
                                                        "pixelMain",
                                                        EPST_PS_3_0,  // ARITHMETIC SLOTS ISSUE (exceeds max registers) WITH "2_0"..(see similar note at GLSL version)..
                                                        OurCallback,
                                                        video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF,  // CLIPMAPPING..
                                                        0
//                                                        ShaderLanguage
);
      OurCallback->drop();
     }
   }
  //
  TheDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
  //  - FIRST OBJECT -
  //  AniMesh0001_Skinned = TheSceneManager ->getMesh("Data/0013_FUSION_BALL/X_001.x"); // A good test model..
  AniMesh0001_Skinned = TheSceneManager ->getMesh("Data/0110_DOOM_BOSS/X_001.x");
  ((ISkinnedMesh*)AniMesh0001_Skinned)->convertMeshToTangents();  // Must do!..

   IScANInode001_Skinned = TheSceneManager->addAnimatedMeshSceneNode(AniMesh0001_Skinned);
   IScANInode001_Skinned->setAnimationSpeed (CurrentAnimSpeed) ;

   // Frameloop..
   // IScANInode001_Skinned->setFrameLoop  ( 0.0   +  1.0  ,  IScANInode001_Skinned -> getEndFrame () -  0.0 );  // Character animation matters..

   IScANInode001_Skinned->setFrameLoop  ( 5.0   + 0.0  ,  IScANInode001_Skinned -> getEndFrame () -  5.0 );
   IScANInode001_Skinned->setPosition(core::vector3df(0.0 ,0.0 ,0.0));
   IScANInode001_Skinned->setRotation(core::vector3df( 0.0 , 180.0 , 0.0 ));
   IScANInode001_Skinned->setScale(core::vector3df(1.0 , 1.0 ,1.0));
   IScANInode001_Skinned->setAutomaticCulling(EAC_FRUSTUM_BOX); // If we don't do this then it would seem culling is dependent on a Single Plane which
                                                                // has its normal vector and position based on the camera direction and position..

                                                                // Now.. Would this be usefull if we divided a big level object into smaller cubes treated
                                                                // as separate objects? (not as cool as true BSP)..
   // IScANInode001_Skinned->setAutomaticCulling(EAC_FRUSTUM_BOX || EAC_OCC_QUERY); // Could this combination have been possible? (does occlusion_query really work?)..
   // Wouldn't it be cool if we had "EAC_FRUSTUM_BUFF_BOX"? It is becoming easier to assign faces to buffers in Creative Content Apps..
   // There is a method in which this can be done using Blender, Deep Explorer and Frag Motion..
   IScANInode001_Skinned->setVisible(true);
   IScANInode001_Skinned->getMaterial( 0 ).setTexture(DIFFUSE_LAYER,  TheDriver->getTexture("Data/0110_DOOM_BOSS/DIFFUSE_MAP_A.png")); // ACDC produced a smaller PNG than Deep Explorer..
   IScANInode001_Skinned->getMaterial( 0 ).setTexture(NORMALS_LAYER,  TheDriver->getTexture("Data/0110_DOOM_BOSS/NORMAL_MAP_A.png"));
   IScANInode001_Skinned->getMaterial( 0 ).setTexture(SPECULAR_LAYER, TheDriver->getTexture("Data/0110_DOOM_BOSS/SPECULAR_MAP_A.png"));
   IScANInode001_Skinned->getMaterial( 0 ).setTexture(GLOSS_LAYER,    TheDriver->getTexture("Data/0110_DOOM_BOSS/GLOSS_MAP_A.png"));

   // File Formats and which Software Titles can save to which format is quite an interesting subject..
   // An example of how one would use this knowledge is a PNG file that exists in 8 bit mode (very small)..
   // Photoshop does not save PNG's in 8 bit mode.. Deep Exploration doesn't either..
   // But.. If you open a TGA (in 8 bit form from Photoshop) and save it as a png from ACDC then you get an 8 bit PNG
   // which is much smaller than a 24 bit png of the same file (lossless)..
   // Now, Irrlicht handles All these file formats beautifully!!

   // Keep experimenting with different image formats and titles because there is a lot that is not common knowledge..
   // The smaller the file the better and there are many undocumented methods to make an image file smaller..

   // For more layers see "Engine Recompiling" matters concering Custom Irrlicht versions..
   // I understand that all cool "Pipeline" and "SVN" things find their way into Irrlicht releases?

   IScANInode001_Skinned->setMaterialFlag(video::EMF_LIGHTING, false);
   IScANInode001_Skinned->setMaterialType((video::E_MATERIAL_TYPE) IDSelectedShader);
   scene::ISceneNodeAnimator* TheAnimator = TheSceneManager->createRotationAnimator(core::vector3df(0.0 ,0.0, 0.0));
   IScANInode001_Skinned->addAnimator(TheAnimator);

   // Important Note:
   // When your Alias Wavefront "*.obj" refers to more than one material, then
   // it becomes necessary to include the Material "*.mtl" file in your DATA directory!
   // Irrlicht sees that there are references to more than one material and it becomes
   // necessary to have the material file available for Irrlicht to read.
   // This creates an issue inside of irrlicht when it encounters references within
   // the material file to "Bumpmaps" (which are actually different to Normal Maps).
   // The way Irrlicht renders these bumps is NOT turned off by invoking your shaders resulting
   // in strange bump effects.
   // We solve this by simply "commenting out" the Bumpmap references
   // like this..
   //      map_Kd DIFFUSE_MAP_C.tga
   //      # map_bump NORMAL_MAP_C.tga
   //      map_kS SPECULAR_MAP_C.tga
   // See how "#" is the comment-line initiating character in Alias Wavefront's *.mtl" or "*.obj"..

   //  - SECOND (Non Skinned) OBJECT -

   AniMesh002_NON_Skinned = TheSceneManager ->getMesh("Data/0109_BADGER/OBJ_001.obj");  // Quake 4 model you've never seen rendered this way!..
   TanMesh002_NON_Skinned = TheSceneManager->getMeshManipulator() ->createMeshWithTangents(AniMesh002_NON_Skinned, false, false, false, true );  // RECALC NORMALS..
   ISceneNode002_NON_Skinned = 0; // Like "NULL"-ing a new pointer?
   ISceneNode002_NON_Skinned = TheSceneManager->addMeshSceneNode(TanMesh002_NON_Skinned);

   // Material 000..
   ISceneNode002_NON_Skinned->getMaterial( 0 ).setTexture(DIFFUSE_LAYER,  TheDriver->getTexture("Data/0109_BADGER/DIFFUSE_MAP_A.png"));
   ISceneNode002_NON_Skinned->getMaterial( 0 ).setTexture(NORMALS_LAYER,  TheDriver->getTexture("Data/0109_BADGER/NORMAL_MAP_A.png"));
   ISceneNode002_NON_Skinned->getMaterial( 0 ).setTexture(SPECULAR_LAYER, TheDriver->getTexture("Data/0109_BADGER/SPECULAR_MAP_A.png"));
   ISceneNode002_NON_Skinned->getMaterial( 0 ).setTexture(GLOSS_LAYER,    TheDriver->getTexture("Data/0109_BADGER/GLOSS_MAP_A.png"));
   // Still to be properly implemented in Standard Irrlicht Releases..
   // ISceneNode002_NON_Skinned->getMaterial( 0 ).setTexture(CUBE_MAP_LAYER, TheDriver->getTexture("Data/MAIN_SKY_IMAGE.tga"));

   // Material 001.. With Alias's “*.obj” when using MORE THAN ONE texture, then the “*.mtl” file MUST exist along with the “*.obj” file..
   // Also disable all metion of “bump” or “bump_map” in the material file as Irrlicht uses it to mess with your normal calculations..

   ISceneNode002_NON_Skinned->setMaterialFlag(video::EMF_LIGHTING, false);         // Once this happens then irrlicht SHOULD NOT touch the lighting-rendering process for the material..
   ISceneNode002_NON_Skinned->setMaterialType(E_MATERIAL_TYPE(IDSelectedShader)); // Invoke your Shader.. All Irrlicht Rendering should IMMEDIATELY be TURNBED OFF..
   ISceneNode002_NON_Skinned->setScale(vector3df( 1.0 , 1.0 , 1.0 ));
   ISceneNode002_NON_Skinned->setRotation(core::vector3df( 0.0 , 0.0 , 0.0 ));
   ISceneNode002_NON_Skinned->setPosition(vector3df( 40.0 , 0.0 , 0.0 ));
   ISceneNode002_NON_Skinned->setAutomaticCulling(EAC_FRUSTUM_BOX); // See earlier culling note..
   ISceneNode002_NON_Skinned->setVisible(true);

   //------------------------------------------------------------------------------------------- ---- --- -- - -
   // Variables concerned with "LIVE TANGENT UPDATES"..
   // We assign this "SceneMeshCount" value during the main render loop aswell
   // as things can be added or removed during run-time..
   // This part of the Tangent Updater do not get called at each frame as it is an “Initialisation”..
   // *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
   SceneMeshCount = TheSceneManager ->getMeshCache()->getMeshCount();
   irr::scene::IMeshBuffer* TempMeshBuffer;
   video::S3DVertexTangents* TempVertTansPTR;
   TempMeshBuffer    = TheSceneManager->getMeshCache()->getMeshByIndex(0)->getMeshBuffer(0);
   TempVertTansPTR   = (video::S3DVertexTangents*)TempMeshBuffer->getVertices();
   TheSceneMeshCache = TheSceneManager->getMeshCache();
   S3DVertexTangents VertexTriA;
   S3DVertexTangents VertexTriB;
   S3DVertexTangents VertexTriC;
   // *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *

   int lastFPS = -1;

   // MAIN CYCLIC LOOP..
   while(TheDevice->run())
    {if (TheDevice->isWindowActive())
      {// REALTIME LIGHTS UPDATE..
       // This would happen cyclically during the game or simulation..
       // Some would be Muzzle flashes and others would be explosions while yet others would be static level lights..
       // We do this in here instead of the Fragment Shader because we wantto treat the Shader Lights as Irrlicht Lights....
       IRRLightARRAY [0]->setPosition(vector3df(CameraPosition) ); // We update this "cyclically" in the Realtime Loop..
       IRRLightARRAY [1]->setPosition(vector3df (  0.0 , -300.0 , 0.0 ) );        // They could have been animated!..
       IRRLightARRAY [2]->setPosition(vector3df (  -300.0 , 0.0 , 0.0 ) );
       IRRLightARRAY [3]->setPosition(vector3df (  300.0 , 0.0 , 0.0 ) );


       IRRLightARRAY [0]->getLightData().DiffuseColor = SColorf( 0.7f , 0.7f , 0.7f );  // Our "Camera Light" Grey-ish..
       // IRRLightARRAY [0]->getLightData().DiffuseColor = SColorf( 0.0f , 0.0f , 0.0f ); // If you want the Camera Light Disabled (i.e. Darkened to zero)..
       IRRLightARRAY [1]->getLightData().DiffuseColor = SColorf( 0.9f , 0.4f , 0.0f );  // Red-ish..
       IRRLightARRAY [2]->getLightData().DiffuseColor = SColorf( 0.3f , 0.7f , 0.7f );  // Blue-ish..
       IRRLightARRAY [3]->getLightData().DiffuseColor = SColorf( 0.6f , 0.6f , 0.3f );  // Yellow-ish..

       // Use grey lights to show "helio free" diffuse colour (from all lights)..
       // IRRLightARRAY [1]->getLightData().DiffuseColor = SColorf( 0.3f , 0.3f , 0.3f );  // 33.3..% Grey..
       // IRRLightARRAY [2]->getLightData().DiffuseColor = SColorf( 0.3f , 0.3f , 0.3f );  // 33.3..% Grey..
       // IRRLightARRAY [3]->getLightData().DiffuseColor = SColorf( 0.3f , 0.3f , 0.3f );  // 33.3..% Grey..

       // Darken all lights except the camera lighT....
       // IRRLightARRAY [1]->getLightData().DiffuseColor = SColorf( 0.0f , 0.0f , 0.0f );
       // IRRLightARRAY [2]->getLightData().DiffuseColor = SColorf( 0.0f , 0.0f , 0.0f );
       // IRRLightARRAY [3]->getLightData().DiffuseColor = SColorf( 0.0f , 0.0f , 0.0f );

       // Try different configurations above by un-commenting certain lines....


       // We do have an option here to do this without Irrlicht lights, but this method is simply "more complete"
       // and shows a reasonably seamless existence between Irrlicht Lights and Shader Lights..
       // This places Irrlicht Light Attributes into an "float" array sothat we can feed the array into the shader in real-time..
       // We could hypothetically have Thousands of lights fed to the shaders but even GPUs have limits..
       for (int Li = 0; Li < ActiveLightCount ; Li ++) // Active Lights kept to a minimum. (Light Pool.. future stuff..)
        {// Get all POSITIONS to a form we can feed the Shaders as arrays..
         // This could also work but we might want to treat things as nodes at the end of the day..
         // Up to you..(whichever is faster?)
         LightsPosArray [Li] = IRRLightARRAY [Li]->getLightData().Position; // An "Array Load Stride" of 3 is possible here so we do X,Y and Z in one move..Cool!
         // Get all Colours to a form we can feed the Shaders as arrays..
         // We can't just load them in one move like above because were dealing with "Scolorf" which is FOUR Floats, and we want as a set of "Three Floats"..
         // Also note that Alpha (fourth value) in my opinion has nothing to do with lights under Physically Based Shading..
         LightsColArray [Li].X = IRRLightARRAY [Li]->getLightData().DiffuseColor.r;
         LightsColArray [Li].Y = IRRLightARRAY [Li]->getLightData().DiffuseColor.g;
         LightsColArray [Li].Z = IRRLightARRAY [Li]->getLightData().DiffuseColor.b; // We skip Light alpha or .w..
         // MOre things like Type, Attenuation, and Spot Light Cone Agnles can go here later..
        }

       // Allready initialised, but we CAN control this in Real-Time..
                                                            // "Lerping" towards Original Mapped Colour is a cool way to emulate "Ambience"..
       PassIntArray   [IAI_000_ACTIVE_LIGHTCOUNT]   = 4;    // We use FOUR lights but we could use MANY MORE..
       PassIntArray   [IAI_001_PLAECF_STATUS]       = 1;

       PassFloatArray [FAI_000_GLOBAL_BRIGHTNESS]   = 	1.25; // Sent to the shader via the "float" array and multiplied to all Light Intensities..
                                                             // Try different values to see it work!
       PassFloatArray [FAI_001_LERP_TO_RAW_DIFFUSE] = 0.1;   // not used in the shaders, but shows that we could..

       IScANInode001_Skinned->setAnimationSpeed(CurrentAnimSpeed) ;  // SECOND TIME FOR REALTIME CONTROLL..

       CurrentGuiUI = StartLiveGuiUI; // So that we dont render static text at each frame..
       // CurrentGuiUI++;
       OurStTxt = TheDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId (CurrentGuiUI);
       if (OurStTxt)
        {stringw OurStr = L" Text updated Cyclically: ";
         OurStTxt->setText(OurStr.c_str());
        }
       CurrentGuiUI++;
       // ready for next one..
       // "GeeXLab" has a nice colour picker from which you can copy Float or Int RGB values after selecting from a pallette..
       TheDriver->beginScene(true, true, BckGrndCol);
       // Swap the order if you want text to be blocked by rendered things..
       TheSceneManager->drawAll();
       OurGUIEnvironment->drawAll(); // Render our GUI elements..
       TheDriver->endScene(); // Irrlicht brilliantly does all Openg GL and D3D things depending on the selected API..
       CameraPosition = TheDevice->getSceneManager()->getActiveCamera()->getAbsolutePosition();
       //================================================
       SceneMeshCount = TheSceneManager ->getMeshCache()->getMeshCount();
       //  PLEASE OPTIMIZE THIS or simply do it as irrlicht does it internally....
       if (TanUpdStatus == true) // globally updated for all Animated "Skinned" meshes..(not for NON-Skinned items..)
        {// SceneMeshCount..
         for (u32 SMeshI = 0; SMeshI < SceneMeshCount; SMeshI++)
          {// start loop "SMESHI"..
           u16 TheMeshType = TheSceneMeshCache->getMeshByIndex(SMeshI)->getMeshType();
           // We want to recalculate Tangents for Skinned Meshes only so we must determine which ones are "Skinned"..
           if (TheMeshType == EAMT_SKINNED)
            {// start procedure: Only "Skinned" meshes..
             AcquiredAnimeshByIndex = TheSceneMeshCache->getMeshByIndex(SMeshI);
             AcquiredImesh = AcquiredAnimeshByIndex ;
             u32 TheMBufferCount = AcquiredImesh->getMeshBufferCount();
             for (u32 MBuffI = 0 ; MBuffI < TheMBufferCount ; MBuffI++)
              {// start Buffer Loop (some large nodels may have more than one buffer)..
               // It would be cool if Buffers could also be considered for Frustum Culling..
               // Not relevant here but Frustum culling currently only works with complete objects..
               CurrMeshBuffPtr = AcquiredImesh->getMeshBuffer(0);  // WE MUST ALSO LOOP BUFFERS..
               CurrTanVertsPtr  = (video::S3DVertexTangents*)CurrMeshBuffPtr->getVertices(); // Many Buffers for Many Meshes..
               // O.K. to do declare these here multiple times ?
               u16* TheINDEXPtr =    TheSceneMeshCache->getMeshByIndex(SMeshI)->getMeshBuffer(0)->getIndices();
               u32 TheIndexCount = (u32)TheSceneMeshCache->getMeshByIndex(SMeshI)->getMeshBuffer(0)->getIndexCount();
               for (u32 IndexII = 0; IndexII < TheIndexCount; IndexII+=3)
                {// start Index Loop..  // Get all all three of our triangle vertices..
                 VertexTriA = CurrTanVertsPtr[TheINDEXPtr[IndexII]];
                 VertexTriB = CurrTanVertsPtr[TheINDEXPtr[IndexII+1]];
                 VertexTriC = CurrTanVertsPtr[TheINDEXPtr[IndexII+2]];
                 // Here we get the THREE POINTS XYZ Positions for the TRIANGLE..
                 f32 TAX = VertexTriA.Pos.X;   f32 TAY = VertexTriA.Pos.Y; f32 TAZ = VertexTriA.Pos.Z;
                 f32 TBX = VertexTriB.Pos.X;   f32 TBY = VertexTriB.Pos.Y; f32 TBZ = VertexTriB.Pos.Z;
                 f32 TCX = VertexTriC.Pos.X;   f32 TCY = VertexTriC.Pos.Y; f32 TCZ = VertexTriC.Pos.Z;
                 // Here we get the UV Coordinates for each of the three Points.
                 f32 TAU = VertexTriA.TCoords.X; f32 TAV = VertexTriA.TCoords.Y;
                 f32 TBU = VertexTriB.TCoords.X; f32 TBV = VertexTriB.TCoords.Y;
                 f32 TCU = VertexTriC.TCoords.X; f32 TCV = VertexTriC.TCoords.Y;
                 // We introduce THREE new "Delta Vectors" which will eventually become "Triangle Edges"..
                 // This is a special "recipe" using "Triangle Points" and "UV Coordinates"..
                 // "Subtraction" hence "Delta Vectors"..
                 f32 DV1X = TBX - TAX ; f32 DV1Y = TBU - TAU ; f32 DV1Z = TBV - TAV;
                 f32 DV2X = TCX - TAX ; f32 DV2Y = TCU - TAU ; f32 DV2Z = TCV - TAV;
                 f32 DV3X = TBY - TAY ; f32 DV3Y = TBU - TAU ; f32 DV3Z = TBV - TAV;
                 f32 DV4X = TCY - TAY ; f32 DV4Y = TCU - TAU ; f32 DV4Z = TCV - TAV;
                 f32 DV5X = TBZ - TAZ ; f32 DV5Y = TBU - TAU ; f32 DV5Z = TBV - TAV;
                 f32 DV6X = TCZ - TAZ ; f32 DV6Y = TCU - TAU ; f32 DV6Z = TCV - TAV;
                 // Now we introduce THREE "Cross Products". Cross Product A, Cross Product B and Cross Product C.
                 f32 CAX = (DV1Y * DV2Z) - (DV2Y * DV1Z); f32 CAY = (DV1Z * DV2X) - (DV2Z * DV1X);
                 f32 CAZ = (DV1X * DV2Y) - (DV2X * DV1Y); f32 CBX = (DV3Y * DV4Z) - (DV4Y * DV3Z);
                 f32 CBY = (DV3Z * DV4X) - (DV4Z * DV3X); f32 CBZ = (DV3X * DV4Y) - (DV4X * DV3Y);
                 f32 CCX = (DV5Y * DV6Z) - (DV6Y * DV5Z); f32 CCY = (DV5Z * DV6X) - (DV6Z * DV5X);
                 f32 CCZ = (DV5X * DV6Y) - (DV6X * DV5Y);
                 // Calculate our TANGENT..
                 f32 TanX = (CAY / CAX);  f32 TanY = (CBY / CBX);  f32 TanZ = (CCY / CCX);
                 // ..and our BINORMAL..
                 // In this implementation the act of subtraction to get the Delta Vectors effectively
                 // makes getting Binormals elswehere (like shaders) impossible!!
                 f32 BinX = (CAZ / CAX);  f32 BinY = (CBZ / CBX);  f32 BinZ = (CCZ / CCX);
                 // Now we replace the Static Tangents/Binormals with our animated ones..
                 CurrTanVertsPtr[TheINDEXPtr[IndexII]].Tangent.X = -TanX; // Negatives are the result of numerous tests..
                 CurrTanVertsPtr[TheINDEXPtr[IndexII]].Tangent.Y = -TanY;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII]].Tangent.Z = -TanZ;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+1]].Tangent.X = -TanX;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+1]].Tangent.Y = -TanY;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+1]].Tangent.Z = -TanZ;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+2]].Tangent.X = -TanX;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+2]].Tangent.Y = -TanY;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+2]].Tangent.Z = -TanZ;
                 // Binormals are NOT done in the shader by crossing Tangents with Normals..
                 CurrTanVertsPtr[TheINDEXPtr[IndexII]].Binormal.X = BinX;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII]].Binormal.Y = BinY;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII]].Binormal.Z = BinZ;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+1]].Binormal.X = BinX;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+1]].Binormal.Y = BinY;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+1]].Binormal.Z = BinZ;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+2]].Binormal.X = BinX;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+2]].Binormal.Y = BinY;
                 CurrTanVertsPtr[TheINDEXPtr[IndexII+2]].Binormal.Z = BinZ;
                }  // end Index Loop..
              }  // end Buffer Loop
            } // End Only "Skinned" meshes..
          } // end loop "SMESHI"
        }  // End user determined conditional Tangents & Binormal Update..

       // END of Tangent and Binormals Updating..

       int fps = TheDriver->getFPS();
       if (lastFPS != fps)
        {core::stringw str = L"Irrlicht 'HLSL' or 'GLSL'     J.Pretorius 2015 [";
         str += TheDriver->getName();
         str += "] FPS:";
         str += fps;
         TheDevice->setWindowCaption(str.c_str());
         lastFPS = fps;
        }
      }
    }
   TheDevice->drop();
   return 0;
  }

 void ResetShaderArrays ()  // Scene Specific for this demo, but availabe during runtime..
  {// Integers.. (in shaders I'm not sure we can pass Booleans which is what some of these actually are)..
   PassIntArray   [IAI_000_ACTIVE_LIGHTCOUNT]   = 4;     // With this we control a "For Loop" in the shaders..
   PassIntArray   [IAI_001_PLAECF_STATUS]       = 1;     // On by default..
   // Floats..
   PassFloatArray [FAI_000_GLOBAL_BRIGHTNESS]   = 1.0;   // Just an example of how we'd send a single Floating Point Value as one element in an array..
   PassFloatArray [FAI_001_LERP_TO_RAW_DIFFUSE] = 0.1;
   // We need global brightness here..
  }



//   END...

