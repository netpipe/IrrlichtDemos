//*****************************************************************
// file:	flag_shader.vert
// purpose:	textured per vertex 2 sided half lambert diffuse lighting 

uniform int  glight;
uniform vec4 gAmbient;			
uniform vec4 gDiffuse;	
uniform vec4 gTranslucency;

varying vec4 frontColour; 	// color for front face
varying vec4 backColour; 	// color for back face			

//*****************************************************************

void main()
{
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec3 N = gl_NormalMatrix * gl_Normal;	
	vec3 L =  gl_LightSource[glight].position;
	vec3 V = -(normalize(gl_ModelViewMatrix * gl_Vertex)).xyz;
	
	float lambert = 0.5 * dot(N,L) + 0.5;
	float blambert = 1.0 - lambert;
	vec4 diffuse = gl_LightSource[glight].diffuse * gDiffuse;
	vec4 translucency = gl_LightSource[glight].diffuse * gTranslucency * max(0,dot(L,-V));
	frontColour =  diffuse * ( gAmbient + lambert * lambert) + translucency;
	backColour = diffuse * ( gAmbient + blambert * blambert) + translucency;
}

//*****************************************************************
