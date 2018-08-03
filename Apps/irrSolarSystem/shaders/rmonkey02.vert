uniform mat4 mWorldViewProj;
uniform mat4 mInvWorld;
uniform mat4 mTransWorld;
uniform vec3 mLightPos;
uniform vec4 mLightColor;

uniform vec3 mLight2Pos;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
varying vec3 Normal;

varying vec3 rm_Binormal;
varying vec3 rm_Tangent;

varying vec4 vTexCoords00;
varying vec4 vTexCoords01;
varying vec4 vTexCoords02;
varying vec4 vTexCoords10;
varying vec4 vTexCoords12;
varying vec4 vTexCoords20;
varying vec4 vTexCoords21;
varying vec4 vTexCoords22;

void main(void)
{

   //
   //  Calculate normalmap values here
   //

   float TextureSize;
   TextureSize = 512.0;
   vec4 Pos = mWorldViewProj * gl_Vertex;

   // Clean up inaccuracies
   //Pos.xy = sign(Pos.xy);

   //gl_Position = vec4(Pos.xy, 0, 1);
   // Image-space
   vec4 BaseTexCoord;
   BaseTexCoord.x = 0.5 * (1.0 + Pos.x);
   BaseTexCoord.y = 0.5 * (1.0 - Pos.y);
   BaseTexCoord.zw = vec2(0.0, 1.0);
   
   float vOffset = (1.0 / TextureSize);
   
   vTexCoords00 = BaseTexCoord + vec4(-vOffset, -vOffset, 0.0, 0.0);
   vTexCoords01 = BaseTexCoord + vec4( 0.0,     -vOffset, 0.0, 0.0);
   vTexCoords02 = BaseTexCoord + vec4( vOffset, -vOffset, 0.0, 0.0);

   vTexCoords10 = BaseTexCoord + vec4(-vOffset,  0.0, 0.0, 0.0);
   vTexCoords12 = BaseTexCoord + vec4( vOffset,  0.0, 0.0, 0.0);
   
   vTexCoords20 = BaseTexCoord + vec4(-vOffset,  vOffset, 0.0, 0.0);
   vTexCoords21 = BaseTexCoord + vec4( 0.0,      vOffset, 0.0, 0.0);
   vTexCoords22 = BaseTexCoord + vec4( vOffset,  vOffset, 0.0, 0.0);   


   //
   //  More code
   //



   gl_Position = mWorldViewProj * gl_Vertex;
    //gl_Position = ftransform();
   
   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;
   
   Texcoord    = gl_MultiTexCoord0.xy;
   ViewDirection  = mLightPos - fvObjectPosition.xyz;
   LightDirection = mLight2Pos - fvObjectPosition.xyz;
   Normal         = gl_NormalMatrix * gl_Normal;
 
   rm_Tangent = gl_MultiTexCoord1.xyz;
   rm_Binormal = gl_MultiTexCoord2.xyz;   
   
   vec3 fvNormal         = gl_NormalMatrix * gl_Normal;
   vec3 fvBinormal       = gl_NormalMatrix * rm_Binormal;
   vec3 fvTangent        = gl_NormalMatrix * rm_Tangent;   

   vec3 fvViewDirection  = mLightPos - fvObjectPosition.xyz;
   vec3 fvLightDirection = mLight2Pos - fvObjectPosition.xyz;

   ViewDirection.x  = dot( fvTangent, fvViewDirection );
   ViewDirection.y  = dot( fvBinormal, fvViewDirection );
   ViewDirection.z  = dot( fvNormal, fvViewDirection );

   LightDirection.x  = dot( fvTangent, fvLightDirection.xyz );
   LightDirection.y  = dot( fvBinormal, fvLightDirection.xyz );
   LightDirection.z  = dot( fvNormal, fvLightDirection.xyz );   
   
   
   
   
} 