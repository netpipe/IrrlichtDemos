#version 120

uniform sampler2D tex;

void main (void)
{
	vec4 texel = texture2D(tex,gl_TexCoord[0].st);
	vec4 color = texel * gl_Color;
    gl_FragColor = color;
}
