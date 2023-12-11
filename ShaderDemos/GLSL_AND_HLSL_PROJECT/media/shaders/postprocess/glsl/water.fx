// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html
// time based displacement

uniform sampler2D texture0;
uniform sampler2D texture1; // NormalTex
uniform float ElapsedTime;
uniform float EffectStrength;
uniform float Speed;

void main()
{
	vec4 normalCol = 2*(texture2D(texture1, gl_TexCoord[0].xy+ElapsedTime*Speed)-0.5);
	gl_FragColor = texture2D(texture0, gl_TexCoord[0].xy+normalCol.xy*EffectStrength);
}