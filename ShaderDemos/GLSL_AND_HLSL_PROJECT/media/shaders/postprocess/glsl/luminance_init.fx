uniform sampler2D texture0; //sceneBuffer
uniform float BufferWidth;
uniform float BufferHeight;

// digital ITU R 709: Y = 0.2126f R + 0.7152f G + 0.0722f B 
// digital ITU R 601: Y = 0.2990f R + 0.5870f G + 0.1140f B
const vec3 ITU_R_709 = vec3(0.2126f, 0.7152f, 0.0722f);

void main ()
{
	vec2 texSize = vec2(1.0/BufferWidth, 1.0/BufferHeight);
	vec4 texCol = vec4(0.0f);
	float average = 0.0f;
    for (int v=0; v<2; v++)
	{
		for (int u=0; u<2; u++)
		{
			texCol = texture2D(texture0, gl_TexCoord[0].xy+texSize*vec2(u, v));
            
            // There are a number of ways we can try and convert the RGB value into
            // a single luminance value:
            
            // 1. Do a very simple mathematical average:
            //float lumValue = dot(texCol.rgb, float3(0.33f, 0.33f, 0.33f));
            
            // 2. Perform a more accurately weighted average:
            //float lumValue = dot(texCol.rgb, ITU_R_709);
            
            // 3. Take the maximum value of the incoming, same as computing the
            //    brightness/value for an HSV/HSB conversion:
            //float lumValue = max(texCol.r, max(texCol.g, texCol.b));
            
            // 4. Compute the luminance component as per the HSL colour space:
            float lumValue = 0.5f*(max(texCol.r, max(texCol.g, texCol.b))+min(texCol.r, min(texCol.g,texCol.b)));
            
            // 5. Use the magnitude of the colour
            //float lumValue = length(texCol.rgb);	
			
			average += lumValue;		
		}
	}
    average *= 0.25f;
    gl_FragColor = vec4(average, 0.0, 0.0, 1.0);	
}
