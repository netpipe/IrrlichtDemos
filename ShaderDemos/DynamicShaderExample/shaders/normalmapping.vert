uniform vec3 fvEyePosition;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
   
void main( void )
{

   vec3 rm_Binormal = gl_MultiTexCoord2.xyz;
   vec3 rm_Tangent = gl_MultiTexCoord1.xyz;

   gl_Position = ftransform();
   Texcoord    = gl_MultiTexCoord0.xy;
    
   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;
   
   vec3 fvViewDirection  = fvEyePosition - fvObjectPosition.xyz;
   vec3 fvLightDirection = gl_LightSource[0].position.xyz - fvObjectPosition.xyz;
     
   vec3 fvNormal         = gl_NormalMatrix * gl_Normal;
   vec3 fvBinormal       = gl_NormalMatrix * rm_Binormal;
   vec3 fvTangent        = gl_NormalMatrix * rm_Tangent;
      
   ViewDirection.x  = dot( fvTangent, fvViewDirection );
   ViewDirection.y  = dot( fvBinormal, fvViewDirection );
   ViewDirection.z  = dot( fvNormal, fvViewDirection );
   
   LightDirection.x  = dot( fvTangent, fvLightDirection.xyz );
   LightDirection.y  = dot( fvBinormal, fvLightDirection.xyz );
   LightDirection.z  = dot( fvNormal, fvLightDirection.xyz );
   
}