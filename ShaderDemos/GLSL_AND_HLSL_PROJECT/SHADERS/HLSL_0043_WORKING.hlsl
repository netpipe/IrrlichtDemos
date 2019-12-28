
 // Written by Jacques Pretorius 2015..
 // This is a very simple "PHYSICALLY BASED" D3D HLSL Shader that renders
 // "Physically Correct" anything you throw at it!
 // New functionality like "Arrays of Passed Information" like light parameters have some usefull spinoffs.. 
 //--------------------------------- --- -- -

                                                                                                       
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
                                       // So.. MAX_LIGHT_COUNT and ActiveLightCount is closely related..
                                       // MAX_LIGHT_COUNT is a #define and "ActiveLightCount" is an Integer Variable..  
                                       // "ActiveLightCount" should NEVER exceed MAX_LIGHT_COUNT..
                                       // MAX_LIGHT_COUNT appears in the Appcode AND the Shader and should remain the same defined value..
                                       // "ActiveLightCount" in the appcode exists in the Shader as the "IAI_000_ACTIVE_LIGHTCOUNT" index of the "PassIntArray" array..
                                       // The code between the two "// * * * * * * " lines are Identical in both Appcode and the Shader..
                                       // (keep them identical by copying and pasting between Shader and Appcode)..
                                       // You'll will get it, and when you do, you'll understand the value of sending arrays of Floats and Ints to shaders..

 #define MAX_INTS_INDEX    2           // The beauty of Array Based Value Passing is that we could have quite a lot of values sent from Appcode to the Shader..
                                       // This example show how to do it for boith GLSL AND HLSL..
 #define IAI_000_ACTIVE_LIGHTCOUNT  0  // The INDEX in the "PassIntArray" Array for "Active Light Count"..
 #define IAI_001_PLAECF_STATUS      1  // The INDEX in the "PassIntArray" Array for "PLAECF" used or not..
 

 // FLOAT ARRAY to the "FRAGMENT SHADER" (GLSL or HLSL) Index reservations..
 // Note that the Fragment Shader for BOTH SHADERS ALSO uses these DEFINES..
 // Thus you can copy this directly to the shader when you make changes..
 #define MAX_FLOATS_INDEX    2          // Again, Could be more..
 #define FAI_000_GLOBAL_BRIGHTNESS   0  // The INDEX in the "PassFloatArray" Array holding the controll over GLOBAL LIGHT BRIGHTNESS..
 #define FAI_001_LERP_TO_RAW_DIFFUSE 1  // The INDEX in the "PassFloatArray" Array holding the position of a LERP TO ORIGINAL DIFFUSE..

 // - End of defines that are identical to those in the GLSL & HLSL Fragment Shader (keep identical at all times)..
 // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

 // From app to shader (the ORDER in which these appear seems unimportant)..
 float4x4  M01World;                 // Numbered because I tried Hundreds of combinations.. I just left it like that..        
 float4x4  M05WorldViewProjection;   // All three of these MAtrices are used..
 float4x4  M17WorldInverseTranspose; // You might find a better mathematical solution, but this is mine..
 sampler2D DiffuseMapSample;               
 sampler2D NormalMapSample; 
 sampler2D SpecularMapSample; 
 sampler2D GlossMapSample;
 
 float3    LightsPosArray[MAX_LIGHT_COUNT];      // Does the initial size of this, in here, impact framerate? NO! 
 float3    LightsColArray[MAX_LIGHT_COUNT];      // The Appcode may feed anything less than or equal to this "MAX_LIGHT_COUNT" into this shader..
 
 float     PassFloatArray    [MAX_FLOATS_INDEX]; // An array of floats sent from the Appcode used in this shader to do various things..
 int       PassIntArray      [MAX_INTS_INDEX];   // An array of floats sent from the Appcode..

 float3    CameraPosFRAG;

 // Data from Irrlicht's Vertex Buffer..
 struct appdata 
  {float3 Position       : POSITION;   // O.K.
   float4 Normal         : NORMAL;
   float4 UVCoords       : TEXCOORD0;  // Can we have A second set of UV COORDS HERE?..
   // These Tangents and Binormals depend on being correctly calculated and correctly updated in your appcode or engine.
   // The appcode does the updates in this case..(only skinned meshes need this update)..

   float4 Tangent        : TEXCOORD1;  // As per Irrlicht HLSL or GLSL..
   float4 Binormal       : TEXCOORD2;  // Which semantics are used for what is a deeper Irrlicht issue..
  };
  
 // Structured Data passed from "Vertex Program" to "Fragment(Pixel) Shader"..
 struct vertexOutput 
  {float4 HPosition             : POSITION;
   float2 UVCoords              : TEXCOORD0;   // We need a SECOND set of UVCoords! (it can be done)..
   float3 PassedWORLDVertPos    : TEXCOORD1;
   float3 WorldITNormal         : TEXCOORD2;
   float3 WorldITTangent        : TEXCOORD3;
   float3 WorldITBinormal       : TEXCOORD4;
  };

 // ===============  MAIN VERTEX PROGRAM  ================= 
 vertexOutput vertexMain(appdata IN)
  {vertexOutput OUT       = (vertexOutput)0;
   OUT.WorldITNormal      = mul(IN.Normal,   M17WorldInverseTranspose).xyz;  // SWAP the params between GLSL ad HLSL ! (be very aware of this issue)..
   OUT.WorldITTangent     = mul(IN.Tangent,  M17WorldInverseTranspose).xyz;  // Happens to NOT be "World" as in so many examples!!.. 
   OUT.WorldITBinormal    = mul(IN.Binormal, M17WorldInverseTranspose).xyz;
   OUT.PassedWORLDVertPos = (mul(float4(IN.Position.xyz,1),M01World).xyz);
   OUT.UVCoords           = IN.UVCoords.xy;
   OUT.HPosition          = mul(float4(IN.Position.xyz,1),M05WorldViewProjection);   // Needed even if not visibly used..
   return OUT;
  }

 // ===============  FRAGMENT SHADER  ================= 
 float4 pixelMain(vertexOutput IN  ) : COLOR 
  {float3 Tn = normalize(IN.WorldITTangent);  // Allready prepared in the Vertex Program (best that way)..
   float3 Bn = normalize(IN.WorldITBinormal); // Allready prepared in the Vertex Program (best that way)..
   float3 NormalisedWorldITNormal = normalize(IN.WorldITNormal);
   float4 MappedDiffuse = tex2D(DiffuseMapSample,IN.UVCoords);  
   float4 MappedNormals  = tex2D(NormalMapSample,IN.UVCoords).rgba;
   float3 UncompressedNormal =  2.0  * (MappedNormals.xyz - float3(0.5,0.5,0.5)); // Old XYZ method..



   // Skip this if your normal map uses RGB instead of just R and G to save domain space for something else....
   // It simply uses the Pythogrean Theorem to get Z on the fly..(imperfect Normal Maps results in small artifacts..
   // Fix these with "Crazy Bump" by loading and resaving)..

   // UncompressedNormal.z = sqrt(-(UncompressedNormal.x*UncompressedNormal.x) - (UncompressedNormal.y*UncompressedNormal.y) + 1.0);

   // This shader don't use the ERM Map's Alpha Channel i.e. "Reflection Mask"..
   float4 MappedSpecular = tex2D(SpecularMapSample,IN.UVCoords); // "float4" means it should be a 32 bit image.. 
   float4 MappedGloss = tex2D(GlossMapSample,IN.UVCoords);
   float3 FinalNormal = normalize(NormalisedWorldITNormal + UncompressedNormal.x * Tn + UncompressedNormal.y * Bn);

   // Diffuse, Specular and Gloss TWEAKABLES..
   // Ideally your original image maps should reflect these tweaks..
   MappedSpecular.xyz *= 1.0;
   MappedGloss.x *= 1.1;
   MappedDiffuse.xyz -= (MappedSpecular.x * 0.3) ;  // SPECIAL ADJUSTMENT:  DIFFUSE MINUS SPECULAR..
                                                    // This is based on the true statement: "Metalic surfaces have LESS diffuse and MORE Specular"
                                                    // If you dont design your diffuse map with these things in mind
                                                    // then tricks like this becomes usefull..

   // DIFFUSE & SPECULAR Calculation..
   // == = == = == = == = == = == = == = == = == = == = == = == = == = == = == = == =
   float3 FinalRGB = float3 (0.0, 0.0, 0.0);
 
   // I added this PLAECF thing for those interested in the futureof CG.. :)  
   // - S P E C U L A R - SHADING..
   // I had added this not because it shows the simplest way of specular calculation, but because it will eventually be used widely..
   // (Rights Reserved) PLAECF Makes Realtime Gloss Mapping possible in a way never implemented before..
   // Internally "Powers" are no more than very fast "Bit-Shifts" in the GPU so..
   // "www.pretoriusjacq@gmail.com" for more information..
   // Do not use this commercially without my written permission.
   // Any other use is fine bar further notice..

   float3 SpecColledFINAL;

   // "Diffuse" is also calculated here..(could have (should have?) been a separate loop though)..
   if (PassIntArray [IAI_001_PLAECF_STATUS] == 1) // PLAECF USED (included, so deal with it..)..
    {// See how the "+=" act as a "Looped Build-Up" of the surface intensity by ALL the light intensities and colour..
     for (int Lii = 0; Lii < PassIntArray [IAI_000_ACTIVE_LIGHTCOUNT] ; Lii++) // .. or here..
      {FinalRGB += (clamp(pow(clamp(dot(normalize(( normalize(CameraPosFRAG - IN.PassedWORLDVertPos) + normalize(LightsPosArray[Lii] - IN.PassedWORLDVertPos))),
                                                    FinalNormal),0.0,1.0), pow(2, (MappedGloss.x * 10.0)) ), 0.0 , 1.0)  // "MappedSpecular.x * 2.0" just amplifies it.. not needed..
                                                    * LightsColArray[Lii] * MappedSpecular.x * 2.0) + (MappedDiffuse.xyz *(max(dot(FinalNormal,         // DIFFUSE PART..(do we want them together???)
                                                    (normalize(LightsPosArray[Lii] - IN.PassedWORLDVertPos))),0.0)
                                                    * LightsColArray[Lii]  * PassFloatArray [FAI_000_GLOBAL_BRIGHTNESS] ));
      }
    }
   if (PassIntArray [IAI_001_PLAECF_STATUS] == 0) // PLAECF NOT USED..
    {for (int Lii = 0; Lii < PassIntArray [IAI_000_ACTIVE_LIGHTCOUNT] ; Lii++) // .. or here..
      {// See how the "+=" act as a "build-up" of the surface intensity by ALL the light intensities and colour..
       FinalRGB += (clamp(pow(clamp(dot(normalize(( normalize(CameraPosFRAG - IN.PassedWORLDVertPos) + normalize(LightsPosArray[Lii] - IN.PassedWORLDVertPos))),
                                                    FinalNormal),0.0,1.0), 7.777), 0.0 , 1.0) // See how PLAECF replaces 7.777 with a Special Formula for the artist..
                                                    * LightsColArray[Lii] * MappedSpecular.x * 2.0) + (MappedDiffuse.xyz *(max(dot(FinalNormal,
                                                    (normalize(LightsPosArray[Lii] - IN.PassedWORLDVertPos))),0.0)
                                                    * LightsColArray[Lii]  * PassFloatArray [FAI_000_GLOBAL_BRIGHTNESS] ));
      }
    }

   // Note why "Emmision" Would be added just before Fog (very last) which we don't do here..
   // We "Lerp" ("before" Fog, not done here) towards Mapped Diffuse to emulate "Ambience" which, frankly, is debatable..
   return lerp (float4 (FinalRGB.xyz , MappedDiffuse.w), float4 (MappedDiffuse.xyz, MappedDiffuse.w), PassFloatArray [FAI_001_LERP_TO_RAW_DIFFUSE]   );  
  }
 // This is as simple as I can get it while still showing the important things..
   
 // There is a lot that can be added like Fog etc, but this basis solves the most important lighting problems..
 // Enjoy! and please comment..





 





 
 
 
 
 
 
