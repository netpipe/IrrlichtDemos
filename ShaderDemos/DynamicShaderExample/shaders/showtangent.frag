varying vec2 TexCoord;
varying vec3 Normal;
varying vec3 Binormal;
varying vec3 Tangent;

void main( void )
{
   vec4 color = vec4( 0.0, 0.0, 0.0, 0.0 );
   
   //color.xy  = TexCoord;
   //color.xyz = Normal;
   //color.xyz = Binormal;
   color.xyz = Tangent;   
   
   gl_FragColor = color;
   
}