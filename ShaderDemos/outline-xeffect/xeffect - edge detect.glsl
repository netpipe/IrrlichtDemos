/*

Author: B@z
   effect->enableDepthPass(true);

   // Load the dwarf mesh.
   IAnimatedMesh* dwarfmesh = smgr->getMesh("media/dwarf.x");
   
   // We create 9 dwarves arranged in a grid in a nested loop.
   for(int g = 0;g < 3;g++)
   {
      for(int v = 0;v < 3;v++)
      {
         // Add a dwarf animated mesh scene node.
         IAnimatedMeshSceneNode* dwarf = smgr->addAnimatedMeshSceneNode(dwarfmesh);
         dwarf->setAnimationSpeed(25);
         dwarf->setScale(vector3df(0.05f, 0.05f, 0.05f));
         dwarf->setPosition(vector3df(g * 4.5f, 0.5f, v * 3.5f + 1.0f));

         // Disable the lighting here too, XEffects overlay will handle it.
         dwarf->getMaterial(0).Lighting = false;
         dwarf->getMaterial(1).Lighting = false;

         // If its the first row add some effects.
         if(g == 0)
         {
            dwarf->setMaterialTexture(0,driver->getTexture("media/red.bmp"));
            effect->addEffectToNode(dwarf, (E_EFFECT_TYPE)(v + 1));
         }

         effect->addNodeToDepthPass(dwarf);
         // Apply a shadow to the dwarf, notice we are applying it to all of them
         // including the ones that have effects applied.
         //effect->addShadowToNode(dwarf, filterType);
      }
   }

   // Set the background clear color to orange.
   effect->setClearColour(SColor(255, 250, 100, 0));

   // Add some post processing effects, a very subtle bloom here.
   core::stringc shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
   
effect->addPostProcessingEffectFromFile(core::stringc("shaders/EdgeDetect") + shaderExt);[quote][/quote]

//Oh sorry there is a bug in XEffects and depth buffer.
//Put the line:
//driver->setRenderTarget(0, false, false);
*/
uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;

float unpackFloat(vec4 value)
{
   float extractedDistance = (value.r + value.g / 256.0);
   return extractedDistance;
}

float calcSample(float depth, vec2 texCoords)
{
   vec4 tempDepth = texture2D(DepthMapSampler, texCoords);
   float otherDepth = unpackFloat(tempDepth);
   return min(abs(otherDepth - depth) * 20.0, 1.0);
}

vec2 offsetArray[4];

//vec4 pixelMain ( vec2 Texcoords)
void main()
{
   offsetArray[0] = vec2( 1.5 / float(SCREENX),0);
   offsetArray[1] = vec2( -1.5 / float(SCREENX),0);
   offsetArray[2] = vec2(0, 1.5 / float(SCREENY));
   offsetArray[3] = vec2(0, -1.5 / float(SCREENY));


   float depth = unpackFloat(texture2D(DepthMapSampler, gl_TexCoord[0].xy));
   float outline = vec4(0.0, 0.0, 0.0, 0.0);

   for(int i = 0;i < 4;i++)
      outline += calcSample(depth, gl_TexCoord[0] + offsetArray[i]);

   gl_FragColor = texture2D(ScreenMapSampler, gl_TexCoord[0].xy) * min(1.0 - outline, 1.0);
} 



/*

//HLSL version  Author : BlindSide

sampler2D ScreenMapSampler : register(s1);
sampler2D DepthMapSampler : register(s2);

float unpackFloat(const float4 value)
{
   float extractedDistance = (value.r + (value.g / 256.0));
   return extractedDistance;
}

float calcSample(float depth, float2 texCoords)
{
   float4 tempDepth = tex2D(DepthMapSampler, texCoords);
   float otherDepth = unpackFloat(tempDepth);
   return min(abs(otherDepth - depth) * 20.0, 1.0);
}

float4 pixelMain (
float2 Texcoords    : TEXCOORD0   
) : COLOR0
{
   const float2 offsetArray[4] =
   {
      float2( 1.5 / SCREENX,0),
      float2( -1.5 / SCREENX,0),
      float2(0, 1.5 / SCREENY),
      float2(0, -1.5 / SCREENY)
   };

   float depth = unpackFloat(tex2D(DepthMapSampler, Texcoords.xy));
   float outline = float4(0.0, 0.0, 0.0, 0.0);

   for(int i = 0;i < 4;i++)
      outline += calcSample(depth, Texcoords + offsetArray[i]);

   return tex2D(ScreenMapSampler, Texcoords.xy) * min(1.0 - outline, 1.0);
} 


*/
