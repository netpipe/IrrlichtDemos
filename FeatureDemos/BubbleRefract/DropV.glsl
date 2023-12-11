uniform mat4 mWorld;
uniform vec4 CamPos;
uniform float TexMul, AlphaAdjust;
varying vec4 GlassPos;
varying vec2 TexCoord;
varying float VAlpha;

void main()
{
	VAlpha = gl_Color.a * AlphaAdjust;

    TexCoord = gl_MultiTexCoord0.xy;
    GlassPos = ftransform();
	
    gl_Position = GlassPos;
}