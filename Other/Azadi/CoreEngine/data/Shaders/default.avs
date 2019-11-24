varying vec2 Texcoord;
varying vec4 Color;

void main( void )
{
    gl_Position = ftransform();
    Texcoord    = gl_MultiTexCoord0.xy;
	Color		= gl_Color;
}
