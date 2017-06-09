uniform vec3 fvEyePosition;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
varying vec3 Normal;
   
void main( void )
{
   gl_Position = ftransform();
   Texcoord    = gl_MultiTexCoord0.xy;
    
   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;
   
   ViewDirection  = fvEyePosition - fvObjectPosition.xyz;
   LightDirection = (gl_LightSource[0].position).xyz - fvObjectPosition.xyz;
   Normal         = gl_NormalMatrix * gl_Normal;
   
}