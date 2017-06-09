uniform vec4 fvAmbient;
uniform vec4 fvSpecular;
uniform vec4 fvDiffuse;
uniform float fSpecularPower;

uniform sampler2D baseMap;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
varying vec3 Normal;

void main( void )
{
   vec3  fvLightDirection = normalize( LightDirection );
   vec3  fvNormal         = normalize( Normal );
   float fNDotL           = dot( fvNormal, fvLightDirection ); 
   
   vec3  fvReflection     = normalize( ( ( 2.0 * fvNormal ) * fNDotL ) - fvLightDirection ); 
   vec3  fvViewDirection  = normalize( ViewDirection );
   float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );
   
   vec4  fvBaseColor      = texture2D( baseMap, Texcoord );
   
   
   vec4  fvTotalAmbient   = gl_LightSource[0].ambient * fvBaseColor; 
   vec4  fvTotalDiffuse   = gl_LightSource[0].diffuse * fNDotL * fvBaseColor; 
   vec4  fvTotalSpecular  = gl_LightSource[0].specular * ( pow( fRDotV, fSpecularPower ) );

   gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular );
       
}