#version 130
uniform sampler2D textures;
varying vec2 outUV;

varying vec2 leftTextureCoordinate;
varying vec2 rightTextureCoordinate;

varying vec2 secLeftTextureCoordinate;
varying vec2 secRightTextureCoordinate;

out vec4 fragmentColor;
void main()
{

vec3 fragmentColorH = texture2D(textures, outUV).rgb * 0.2270270270;
     fragmentColorH += texture2D(textures, leftTextureCoordinate).rgb * 0.3162162162;
     fragmentColorH += texture2D(textures, rightTextureCoordinate).rgb * 0.3162162162;
     fragmentColorH += texture2D(textures, secLeftTextureCoordinate).rgb * 0.0702702703;
     fragmentColorH += texture2D(textures, secRightTextureCoordinate).rgb * 0.0702702703;

//	gl_FragColor=texture2D(textures,outUV);
	fragmentColor = vec4(fragmentColorH, 1.0);
}
