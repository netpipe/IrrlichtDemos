// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform sampler2D texture0;
uniform sampler2D texture1; // NormalTex
uniform float EffectStrength;

void main()
{
	vec4 normalCol = 2.0*(texture2D(texture1, gl_TexCoord[0].xy*2.5)-0.5);
	gl_FragColor = texture2D(texture0, gl_TexCoord[0].xy+normalCol.xy*EffectStrength);
}