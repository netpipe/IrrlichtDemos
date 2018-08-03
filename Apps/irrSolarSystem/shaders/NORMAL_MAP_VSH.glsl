//Vertex
uniform vec3 fvLightPosition1;
uniform vec3 fvLightPosition2;
uniform vec3 fvLightPosition3;
uniform vec3 fvLightPosition4;

uniform float fLightStrength1;
uniform float fLightStrength2;
uniform float fLightStrength3;
uniform float fLightStrength4;

uniform mat4 matWorldInverse;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection1;
varying vec3 LightDirection2;
varying vec3 LightDirection3;
varying vec3 LightDirection4;
varying vec4 LightDistMultiplier;
   
float getLengthSQR (vec3 vec)
{
return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
} 
   
void main( void )
{
   
   mat4 LightTransform= gl_ModelViewMatrix;
   LightTransform=LightTransform*matWorldInverse;
     
   gl_Position = ftransform();
   Texcoord    = gl_MultiTexCoord0.xy;
   
   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;
   vec4 fvLightPos1 = LightTransform * vec4(fvLightPosition1,1.0);
   vec4 fvLightPos2 = LightTransform * vec4(fvLightPosition2,1.0);
   vec4 fvLightPos3 = LightTransform * vec4(fvLightPosition3,1.0);
   vec4 fvLightPos4 = LightTransform * vec4(fvLightPosition4,1.0);
   
   vec3 fvViewDirection  =  - fvObjectPosition.xyz;
   
   vec3 fvLightDirection1 = (fvLightPos1.xyz - fvObjectPosition.xyz);
   vec3 fvLightDirection2 = (fvLightPos2.xyz - fvObjectPosition.xyz);
   vec3 fvLightDirection3 = (fvLightPos3.xyz - fvObjectPosition.xyz);
   vec3 fvLightDirection4 = (fvLightPos4.xyz - fvObjectPosition.xyz);
   
   LightDistMultiplier[0]=1.0/(getLengthSQR (fvLightDirection1)/(fLightStrength1*10000.0));
   LightDistMultiplier[1]=1.0/(getLengthSQR (fvLightDirection2)/(fLightStrength2*10000.0));
   LightDistMultiplier[2]=1.0/(getLengthSQR (fvLightDirection3)/(fLightStrength3*10000.0));
   LightDistMultiplier[3]=1.0/(getLengthSQR (fvLightDirection4)/(fLightStrength4*10000.0));
     
   vec3 fvNormal         = gl_NormalMatrix * gl_Normal;
   
   vec3 fvTangent   = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);
   vec3 fvBinormal =cross(fvTangent,gl_Normal);  
   fvTangent=gl_NormalMatrix*cross(fvBinormal,gl_Normal);
   fvBinormal=gl_NormalMatrix*fvBinormal;

     
   ViewDirection.x  = dot( fvTangent, fvViewDirection );
   ViewDirection.y  = dot( fvBinormal, fvViewDirection );
   ViewDirection.z  = dot( fvNormal, fvViewDirection );
   
   LightDirection1.x  = dot( fvTangent, fvLightDirection1.xyz );
   LightDirection1.y  = dot( fvBinormal, fvLightDirection1.xyz );
   LightDirection1.z  = dot( fvNormal, fvLightDirection1.xyz );
   
   LightDirection2.x  = dot( fvTangent, fvLightDirection2.xyz );
   LightDirection2.y  = dot( fvBinormal, fvLightDirection2.xyz );
   LightDirection2.z  = dot( fvNormal, fvLightDirection2.xyz );
   
   LightDirection3.x  = dot( fvTangent, fvLightDirection3.xyz );
   LightDirection3.y  = dot( fvBinormal, fvLightDirection3.xyz );
   LightDirection3.z  = dot( fvNormal, fvLightDirection3.xyz );
   
   LightDirection4.x  = dot( fvTangent, fvLightDirection4.xyz );
   LightDirection4.y  = dot( fvBinormal, fvLightDirection4.xyz );
   LightDirection4.z  = dot( fvNormal, fvLightDirection4.xyz );
   
};