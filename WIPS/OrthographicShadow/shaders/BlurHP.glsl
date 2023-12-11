uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;

vec2 offsetArray[5];

void main() 
{
	offsetArray[0] = vec2(0,0);
	offsetArray[1] = vec2(2.5 / SCREENX,0);
	offsetArray[2] = vec2(-2.5 / SCREENX,0);
	offsetArray[3] = vec2(-4.5 / SCREENX,0);
	offsetArray[4] = vec2(4.5 / SCREENX,0);

	vec4 BlurCol = vec4(0);

	for(int i = 0;i < 5;++i)
		BlurCol += texture2D(ColorMapSampler,clamp(gl_TexCoord[0].xy + offsetArray[i] * 3.0,0.001,0.999));
	
	BlurCol /= 5.0;
	
	gl_FragColor = BlurCol;
}