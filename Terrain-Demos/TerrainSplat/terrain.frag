uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Tex3;
uniform sampler2D Alpha;

void main(void)
{
	vec2 texUV = gl_TexCoord[0].xy;
 
	vec4 alpha   = texture2D( Alpha, texUV.xy );

  	vec4 tex0    = texture2D( Tex1, texUV.xy * 32.0 ); // Tile
  	vec4 tex1    = texture2D( Tex3,  texUV.xy * 32.0 ); // Tile
   	vec4 tex2    = texture2D( Tex2, texUV.xy * 32.0 ); // Tile

   	tex0 *= alpha.r; // Red channel
  	tex1 = mix( tex0, tex1, alpha.g ); // Green channel
	vec4 outColor = mix( tex1, tex2, alpha.b ); // Blue channel
	
	gl_FragColor = outColor;
}