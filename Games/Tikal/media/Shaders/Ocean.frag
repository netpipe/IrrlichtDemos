//====================================================================//
// Ocean.frag                                                         //
// Project Tikal                                                      //
// Ocean Fragment Shader v1.0, september 2009                         //
// reflection funcion is based in the SIO2 demo from sio2.g0dsoft.com //
// everything else by Andres J. Porfirio ... andresjesse@yahoo.com.br //
//====================================================================//

//!alphaMap for the ocean (terrain heighmap)
uniform sampler2D t0;
//!reflection texture
uniform sampler2D t1;
//!water normalmap
uniform sampler2D NormalMap;

varying vec4 waterpos;
varying vec2 bumpCoord0;
varying vec2 originalCoord;


vec4 reflection()
{
	vec4 projCoord = waterpos / waterpos.w;
	projCoord += vec4(1.0);
	projCoord *= 0.5;
	projCoord = clamp(projCoord, 0.001, 0.999);
	
	projCoord.y = 1.0 - projCoord.y;
	
	vec4 tnmap = texture2D(NormalMap, bumpCoord0);
	return texture2D(t1, vec2(projCoord) + tnmap.xz*0.15 - vec2(0.08,0.15));
}

void main(void)
{
    vec4 alphaMap = texture2D(t0, originalCoord);
    vec4 ref=reflection()+vec4(0,0,0,1);
    
    gl_FragColor = ref * vec4(1,1,1,(0.12-alphaMap.b)*6);
}
