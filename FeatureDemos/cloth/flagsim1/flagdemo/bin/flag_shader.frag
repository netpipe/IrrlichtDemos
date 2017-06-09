//*****************************************************************
// file:	flag_shader.frag
// purpose:	textured per vertex 2 sided half lambert diffuse lighting 

uniform sampler2D diffuseMapTexture;
varying vec4 frontColour; 
varying vec4 backColour; 

void main()
{
	vec4 diffuse = texture2D(diffuseMapTexture, gl_TexCoord[0]);
	if(gl_FrontFacing)
    	gl_FragColor = diffuse * frontColour;
    else
    	gl_FragColor = diffuse * backColour;
}
