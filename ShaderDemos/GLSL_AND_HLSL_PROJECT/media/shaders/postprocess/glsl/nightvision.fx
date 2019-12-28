// based on http://www.geeks3d.com/20091009/shader-library-night-vision-post-processing-filter-glsl/

uniform sampler2D texture0; //sceneBuffer
uniform sampler2D texture1; //noiseTex
uniform sampler2D texture2; //maskTex
//uniform float ElapsedTime;
uniform float RandomValue;
uniform float LuminanceThreshold;
uniform float ColorAmplification;
uniform float NoiseStrength;
uniform float VisionColorR;
uniform float VisionColorG;
uniform float VisionColorB;

float saturate(float x)
{
 return (x < 0.0) ? 0.0 : (1.0 < x) ? 1.0 : x;
}
 
vec2 saturate(vec2 x)
{
 return vec2(saturate(x[0]),saturate(x[1]));
}
 
vec3 saturate(vec3 x)
{
 return vec3(saturate(x[0]),saturate(x[1]),saturate(x[2]));
}
 
vec4 saturate(vec4 x)
{
 return vec4(saturate(x[0]),saturate(x[1]),saturate(x[2]),saturate(x[3]));
}
 

void main ()
{
	vec4 finalCol;

	// noise
	vec2 noiseCoord;
	noiseCoord.x = RandomValue; // 0.4*sin(ElapsedTime*50.0);
	noiseCoord.y = RandomValue; // 0.4*cos(ElapsedTime*50.0);
	vec3 noiseCol = texture2D(texture1, (gl_TexCoord[0].xy*NoiseStrength) + noiseCoord).rgb;
	
	// light amplification
	vec3 texCol = texture2D(texture0, gl_TexCoord[0].xy + (noiseCol.xy*0.001)).rgb;
	texCol = vec3(saturate((texCol.xyz-LuminanceThreshold)/(ColorAmplification-LuminanceThreshold)));

	// mask
	vec4 maskCol = texture2D(texture2, vec2(gl_TexCoord[0].x, 1.0-gl_TexCoord[0].y));

	// calculate final color
	if (maskCol.a == 1.0)
	{
		finalCol = maskCol;
	}
	else
	{
		finalCol.rgb = (texCol+(noiseCol*0.2))*vec3(VisionColorR, VisionColorG, VisionColorB)*(1.0-maskCol.a);
		finalCol.a = 1.0;
	}
	gl_FragColor = finalCol;
}
