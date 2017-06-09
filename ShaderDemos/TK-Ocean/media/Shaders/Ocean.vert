//====================================================//
// Ocean.vert                                         //
// Project Tikal                                      //
// Ocean Vertex Shader v1.0, september 2009           //
// by Andres J. Porfirio ... andresjesse@yahoo.com.br //
//====================================================//


//!device->getTimer()->getTime()/1000.0f;
uniform float waterTime;

varying vec4 waterpos;
varying vec2 bumpCoord0;
varying vec2 originalCoord;

void main(void)
{
	waterpos = ftransform();

	originalCoord = gl_MultiTexCoord0;

	bumpCoord0.xy = gl_MultiTexCoord0.xy * 10 + waterTime * vec2(-0.01,-0.05);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

