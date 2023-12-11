uniform sampler2D ColorMapSampler;

vec2 offsetArray[7];

void main() 
{		
	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(0.0, -1.5 / float(SCREENY));
	offsetArray[2] = vec2(0.0, 1.5 / float(SCREENY));
	offsetArray[3] = vec2(0.0, -2.5 / float(SCREENY));
	offsetArray[4] = vec2(0.0, 2.5 / float(SCREENY));
	offsetArray[5] = vec2(0.0, -4.5 / float(SCREENY));
	offsetArray[6] = vec2(0.0, 4.5 / float(SCREENY));

	vec4 BlurCol = vec4(0.0, 0.0, 0.0, 0.0);

	for(int i = 0;i < 7;++i)
		BlurCol += texture2D(ColorMapSampler, clamp(gl_TexCoord[0].xy + offsetArray[i] * 3.0, vec2(0.001, 0.01), vec2(0.999, 0.999)));
	
	BlurCol /= 7.0;
	
	gl_FragColor = BlurCol;
}