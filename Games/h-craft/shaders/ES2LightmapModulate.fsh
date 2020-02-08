precision mediump float;

uniform sampler2D uTextureUnit0;
uniform sampler2D uTextureUnit1;

varying vec2 vTextureCoord0;
varying vec2 vTextureCoord1;
varying vec4 vVertexColor;
varying vec4 vSpecularColor;

void main()
{
	vec4 Color0 = texture2D(uTextureUnit0, vTextureCoord0);
	vec4 Color1 = texture2D(uTextureUnit1, vTextureCoord1);

	gl_FragColor = (Color0 * Color1 * 2.0) * vVertexColor + vSpecularColor;
}
