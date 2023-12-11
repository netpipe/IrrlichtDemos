// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1

uniform vec4 fvAmbient;
uniform vec4 fvLight1Color;
uniform vec4 fvLight2Color;
uniform vec4 fvLight3Color;
uniform vec4 fvLight4Color;

uniform float fSpecularPower;
uniform float fSpecularStrength;
uniform float fBumpStrength;

uniform sampler2D baseMap;
uniform sampler2D bumpMap;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection1;
varying vec3 LightDirection2;
varying vec3 LightDirection3;
varying vec3 LightDirection4;
varying vec4 LightDistMultiplier;

void main( void )
{
   
   vec3  fvLightDirection1 = normalize( LightDirection1 );
   vec3  fvLightDirection2 = normalize( LightDirection2 );
   vec3  fvLightDirection3 = normalize( LightDirection3 );
   vec3  fvLightDirection4 = normalize( LightDirection4 );
   
   vec3  fvNormal         = texture2D( bumpMap, Texcoord ).yxz;
   fvNormal.xy*=2.0;
   fvNormal.xy-=1.0;
   
   fvNormal=(vec3(0.0,0.0,1.0)-fvNormal)*fBumpStrength+fvNormal;
   
   fvNormal=normalize(fvNormal);
   
   float fNDotL1           = max(dot(fvNormal, fvLightDirection1),0.0)-0.1; 
   float fNDotL2           = max(dot(fvNormal, fvLightDirection2),0.0)-0.1; 
   float fNDotL3           = max(dot(fvNormal, fvLightDirection3),0.0)-0.1; 
   float fNDotL4           = max(dot(fvNormal, fvLightDirection4),0.0)-0.1; 
   
   vec3  fvReflection1     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection1 ); 
   vec3  fvReflection2     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection2 ); 
   vec3  fvReflection3     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection3 ); 
   vec3  fvReflection4     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection4 ); 
   
   vec3  fvViewDirection  = normalize( ViewDirection );
   
   float fRDotV1          = max( 0.0, dot( fvReflection1, fvViewDirection ) );
   float fRDotV2          = max( 0.0, dot( fvReflection2, fvViewDirection ) );
   float fRDotV3          = max( 0.0, dot( fvReflection3, fvViewDirection ) );
   float fRDotV4          = max( 0.0, dot( fvReflection4, fvViewDirection ) );
   
   vec4  fvBaseColor      = texture2D( baseMap, Texcoord );
   
   vec4  fvTotalAmbient   = fvAmbient * fvBaseColor; 
   
   vec4  fvTotalDiffuse   = fvLight1Color * fNDotL1* fvBaseColor*LightDistMultiplier[0]; 
   vec4  fvTotalSpecular  = fNDotL1*fvLight1Color * ( pow( fRDotV1, fSpecularPower ) )*LightDistMultiplier[0];
   
         fvTotalDiffuse   += fvLight2Color * fNDotL2* fvBaseColor*LightDistMultiplier[1]; 
         fvTotalSpecular  += fNDotL2*fvLight2Color * ( pow( fRDotV2, fSpecularPower ) )*LightDistMultiplier[1];  
   
         fvTotalDiffuse   += fvLight3Color * fNDotL3* fvBaseColor*LightDistMultiplier[2]; 
         fvTotalSpecular  += fNDotL3*fvLight3Color * ( pow( fRDotV3, fSpecularPower ) )*LightDistMultiplier[2];  
         
         fvTotalDiffuse   += fvLight4Color * fNDotL4* fvBaseColor*LightDistMultiplier[3]; 
         fvTotalSpecular  += fNDotL4*fvLight4Color * ( pow( fRDotV4, fSpecularPower ) )*LightDistMultiplier[3]; 
   
   vec4 color=( fvTotalAmbient + fvTotalDiffuse+ (fvTotalSpecular*fSpecularStrength));
   if(color.r>1.0){color.gb+=color.r-1.0;}
   if(color.g>1.0){color.rb+=color.g-1.0;}
   if(color.b>1.0){color.rg+=color.b-1.0;}
   gl_FragColor = color;

}




