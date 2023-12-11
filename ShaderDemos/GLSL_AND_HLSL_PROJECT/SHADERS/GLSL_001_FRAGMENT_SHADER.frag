
 // Very Simple Physically Based Lighting Shader..  Jacques Pretorius    email: pretoriusjacq@gmail.com 
 // I'm looking for paying work..


                                                                                                       
                                                                                                       
                                                                                                       
 // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 // * *  THIS IS IDENTICAL IN "SHADER CODE" (GLSL & HLSL) AND "APP CODE" TO SIMPLIFY CODED COMMUNICATION BETWEEN THE TWO  * * * * * 
 // INTEGER ARRAY to the "FRAGMENT SHADER" (GLSL or HLSL) Index reservations..
 // Note that the Fragment Shader ALSO uses these "Identically Named" DEFINES..
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
                                       // So.. MAX_LIGHT_COUNT and ActiveLightCount are closely related..
                                       // MAX_LIGHT_COUNT is a #define and "ActiveLightCount" is an Integer Variable..  
                                       // "ActiveLightCount" should NEVER exceed MAX_LIGHT_COUNT..
                                       // MAX_LIGHT_COUNT appears in the Appcode AND the Shader and should remain the same defined value..
                                       // "ActiveLightCount" in the appcode exists in the Shader as the "IAI_000_ACTIVE_LIGHTCOUNT" index of the "PassIntArray" array..
                                       // The code between the two "// * * * * * * " lines are Identical in both Appcode and the Shader..
                                       // (keep them identical by copying and pasting between Shader and Appcode)..
                                       // You'll will get it, and when you do, you'll understand the value of sending arrays of Floats and Ints to shaders..

 #define MAX_INTS_INDEX    2          // The beauty of Array Based Value Passing is that we could have quite a lot of values sent from Appcode to the Shader..
                                       // This example show how to do it for boith GLSL AND HLSL..
 #define IAI_000_ACTIVE_LIGHTCOUNT  0  // The INDEX in the "PassIntArray" Array for "Active Light Count"..
 #define IAI_001_PLAECF_STATUS      1  // The INDEX in the "PassIntArray" Array for "PLAECF" used or not..
 

 // FLOAT ARRAY to the "FRAGMENT SHADER" (GLSL or HLSL) Index reservations..
 // Note that the Fragment Shader for BOTH SHADERS ALSO uses these DEFINES..
 // Thus you can copy this directly to the shader when you make changes..
 #define MAX_FLOATS_INDEX    2        // Again, Could be more..
 #define FAI_000_GLOBAL_BRIGHTNESS   0   // The INDEX in the "PassFloatArray" Array holding the controll over GLOBAL LIGHT BRIGHTNESS..
 #define FAI_001_LERP_TO_RAW_DIFFUSE 1   // The INDEX in the "PassFloatArray" Array holding the position of a LERP TO ORIGINAL DIFFUSE..


 // - End of defines that are identical to those in the GLSL & HLSL Fragment Shader (keep identical at all times)..
 // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
                                                                                                       


 // FROM VERTEX PROGRAM
 varying vec2 UVCoordsXY;              // Sent to here from the Vertex Program..
 varying vec3 VNormal;                 // Effectively (interpolated?) "Fragment Normal" after passing through the Vertex Progam.
 varying vec3 Tangent;                 // Generated in the User App and passed throught he Vertex Program..
 varying vec3 Binormal;                // Generated in the User App and passed throught he Vertex Program..
 varying vec3 VertexGLPosition;        // GL Internal value passed unseen from the Vertex Program.. 

 // FROM USER APP..
 uniform sampler2D DiffuseMapSample;        // The "ClipMap" is determined by a "Selected Cutoff Range" 
                                            // somewhere between 0.0 and 1.0 in the ALPHA Channel.
                                            // Transparency could also be "Fitted In".. (see the code)
 uniform sampler2D NormalMapSample;         // For the normal map we use only X and Y as we can cleverly deduce Z!!
                                            // The B and A channels arethus open for us to do more things with..
                                            
 uniform sampler2D SpecularMapSample;       // RGB values not influenced by light thus simulating Emission or Self Lighting..
 uniform sampler2D GlossMapSample;                // More than 4 images availabe can be done with a special engine recompile but it
                                            // should eventually find its way to a future standard release of Irrlicht..
 uniform sampler2D Image005;                // Reserved.. 
 uniform sampler2D Image006;                // Reserved.. 
 uniform sampler2D Image007;                // Reserved.. 
 uniform vec3      CameraPos;               // The Position of the camera i.e. the EYE (important enough to not be included in the float array)..
                                            // For demonstrative purposes we have the first light hooked to this camera..

 // Unfortunately we cannot create an array with the size given by another uniform integer value.. so we have
 // to keep track of our array sizes between Appcode and Shadercode (see how we always keep them identical, thanks to Shader syntax being similar to C++ syntax)..
 uniform vec3  LightsPosArray [MAX_LIGHT_COUNT];  // See how, in the Appcode, this Variable is reffered to as "LightsPosArray[0]" specifically GLSL. HLSL is different..
 uniform vec3  LightsColArray [MAX_LIGHT_COUNT];  // Colours..
 uniform float PassFloatArray [MAX_FLOATS_INDEX];  
 uniform int   PassIntArray   [MAX_INTS_INDEX];    
 // vec4          MappedDiffuse;  //Here because we need it in an independent render function..

 // =  M A I N  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
 void main()
  {vec4 MappedDiffuse  = texture2D(DiffuseMapSample,UVCoordsXY);   // ALPHA is the "Clipmap"..(transparency is "Render Order" dependent and NOT used in this example..)
                                                                   // Make sure you understand the difference between "Clipping" and "Transpaency":..
   // MappedDiffuse.xyz  = vec3(0.4, 0.4, 0.4);                   // Override the Diffuse Image with a Given Colour for tests..
   vec4 MappedNormals  = texture2D(NormalMapSample,UVCoordsXY);    // ALPHA is "Droplet Shader Mask"..

   // MappedNormals.xyz = vec3 (0.5, 0.5, 1.0);  // Simulate a perfectly flat Normal Map.. (usually to check out specular effect)

   vec4 MappedSpecular = texture2D(SpecularMapSample,UVCoordsXY);
   vec4 MappedGloss    = texture2D(GlossMapSample,UVCoordsXY);
   
   
   // Diffuse, Specular and Gloss TWEAKABLES..
   // Ideally your original image maps should reflect these tweaks..
   MappedSpecular.xyz *= 1.0;
   MappedGloss.x *= 1.1;
   MappedDiffuse.xyz -= (MappedSpecular.x * 0.3) ;  // SPECIAL ADJUSTMENT:  DIFFUSE MINUS SPECULAR..
                                                     // This is based on the true statement: "Metalic surfaces have LESS diffuse and MORE Specular"
                                                     // If you dont design your diffuse map with these things in mind
                                                     // then tricks like this becomes usefull..
   
													  
   vec3 UnCompressedNormal;
   // We could IGNORE the incoming Z value using Pythagoras's Theorem to aquire Z..(only if we want to save as many image channels as possible)
   float NormalAmp = 2.0 ;   // Default 2.0.. Once you calculate the Z value with the Pythagorean Theorem this could come into play..
   UnCompressedNormal.x = (MappedNormals.x - 0.5) * NormalAmp; // Amplifiation is an intersting concept that acts on the Tan & Bin related to a Normal..
   UnCompressedNormal.y = (MappedNormals.y - 0.5) * NormalAmp;
   UnCompressedNormal.z = (MappedNormals.z - 0.5) * NormalAmp;
   
   // Below is a way to calculate the Z of the normals given X and Y when we need the Blue (Z) channel of the Normal Map for other things (see encoding)..
   // In this example we use plain vanilla Normal maps so we don't use this..
   // UnCompressedNormal.z = sqrt(-(UnCompressedNormal.x * UnCompressedNormal.x) - (UnCompressedNormal.y * UnCompressedNormal.y) + 1.0);



   vec3 FinalNormal;
   // DO NOT TRY TO CALCULATE THE BINORMALS IN THE SHADER..
   // It has to be done in the User App because there are un-obvious uniquenesses that come into play..
   FinalNormal = normalize((UnCompressedNormal.x * Tangent)
                           +(UnCompressedNormal.y * Binormal)
                           +(UnCompressedNormal.z * VNormal) 
                           );
   // D I F F U S E  and S P E C U L A R  Shading..
   vec3      SpecColledFINAL; // Not touched by the Diffuse Map values, it depends on Specular, Gloss and Lights only (based on reflection)..
   vec3      DiffColledFINAL; // Influenced by "Mapped Diffuse Colour"..
   // LightsPosArray[0] = CameraPos;  // Don't do this here, but rather in the User App..
   // Work through all the lights..
   // - D I F F U S E - SHADING..
   for (int LghtI = 0; LghtI < PassIntArray [IAI_000_ACTIVE_LIGHTCOUNT]; LghtI++)
    {// See how the "+=" act as a "Looped Build-Up" of surface intensity over all the light intensities and colour..
     DiffColledFINAL += (max(dot(FinalNormal, (normalize(LightsPosArray[LghtI] - VertexGLPosition))),0.0)
       * LightsColArray[LghtI].xyz * PassFloatArray [FAI_000_GLOBAL_BRIGHTNESS]) ;
     // - S P E C U L A R - SHADING..
     // I had added this not because it shows the simplest way of specular calculation, but because it will eventually be used widely..
     // (Rights Reserved) PLAECF Makes Realtime Gloss Mapping possible in a way never implemented before..
     // Internally "Powers" are no more than very fast "Bit-Shifts" in the GPU so..
     // "www.pretoriusjacq@gmail.com" for more information..
     // Do not use this comercially without my written permission.
     // Any other use is fine bar further notice..
     if (PassIntArray [IAI_001_PLAECF_STATUS] == 1) 
      {SpecColledFINAL +=  ((LightsColArray[LghtI] * PassFloatArray [FAI_000_GLOBAL_BRIGHTNESS]
       * clamp(pow(clamp(dot(normalize((normalize(CameraPos - VertexGLPosition) + normalize(LightsPosArray[LghtI] - VertexGLPosition))),
       FinalNormal),0.0,1.0),
       pow(2.0, (MappedGloss.x * 11.0)) ), 0.0 , 1.0 )) * MappedSpecular.x * 1.0);  // "* 2.0" just amplifies it.. not needed..
      } 
     else // Do not use "PLAECF" which is also cool if you don't want to map "Glossyness" from an image..
          // Glossyness has to date never been mapped from a grey scale image by any softwate title in this manner..
      {// See how the "+=" act as a "build-up" of surface intensity over all the light intensities and colour..
       SpecColledFINAL += (LightsColArray[LghtI] * PassFloatArray [FAI_000_GLOBAL_BRIGHTNESS]
       * clamp(pow(clamp(dot(normalize((normalize(CameraPos - VertexGLPosition) + normalize(LightsPosArray[LghtI] - VertexGLPosition))),
       FinalNormal),0.0,1.0),  MappedNormals.w * 77.777 ), 0.0 , 1.0 ) ) ; 
       // 77.777 is the "Tightness" of the Specular Highlight (See PLAECF)..
       // Very difficult to map on a greyscale image considering the Exponential nature of the curve involved..
      }
    }
   // "Emmision" would now be added quite simply by just "Adding" the RGB values to the final render..
   // We don't do this however because we need all of our 4 available images.. (I trust the Devs will fix this in the next release)
   // It would be done second last because Emission is not influenced by any other physical attribute calculated in this shader..
   // It would however be influenced by "Fog"..

   // Lerping between "Final Diffuse" and "Mapped RGB" is a sensible way to deal with "Ambient Lighting"..   
   gl_FragColor.xyz = lerp(((MappedDiffuse.xyz * DiffColledFINAL.xyz) + (SpecColledFINAL.xyz * MappedSpecular.x * 2.0)), MappedDiffuse.xyz, PassFloatArray[FAI_001_LERP_TO_RAW_DIFFUSE]);

   // There are some software titles that likes to have colour in their specular maps which is fine
   // but it is NOT PHYSICALLY CORRECT..

    // Only FOG should be calculated after all this, but as you can see this shader does not do Fog..
   // (extra cool "Plane Distance" based Fog coming soon) 

   gl_FragColor.w = MappedDiffuse.w; // Our "Clip" mapping (not transparency) The clip-off occurs at Grey values between approx 45% and 55%
                                     // effectively allowing us to use the rest of this "Grey Domain" for other things..
                                     // However in this shader we don't do this..

  }// -- END -- 
  
 // This is as simple as I can get it while still showing the important things..
 // There is really a lot that can be added like Fog and Droplet etc, but this minimal basis solves the most important "Physically Based" lighting problems..
 // Enjoy! and please comment..











