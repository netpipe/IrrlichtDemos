uniform sampler2D texture0; //sceneBuffer
uniform float SampleDist;
uniform float SampleStrength;

//static const float samples[10] =

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
   float samples[10];
	samples[0]= -0.08;
	samples[1]= -0.05;
	samples[2]= -0.03;
	samples[3]= -0.02;
	samples[4]= -0.01;
	samples[5]= 0.01;
	samples[6]= 0.02;
	samples[7]= 0.03;
	samples[8]= 0.05;
	samples[9]= 0.08;

   // Vector from pixel to the center of the screen
   vec2 dir = 0.5-gl_TexCoord[0].xy;

   // Distance from pixel to the center (distant pixels have stronger effect)
   float dist = sqrt(dir.x*dir.x+dir.y*dir.y);

   // Now that we have dist, we can normlize vector
   dir = normalize(dir);

   // Save the color to be used later
   vec4 color = texture2D(texture0, gl_TexCoord[0].xy);
   
   // Average the pixels going along the vector
   vec4 sum = color;
   for (int i=0; i<10; i++)
   {
      sum += texture2D(texture0, gl_TexCoord[0].xy+dir*samples[i]*SampleDist);
   }
   sum /= 11.0;

   // Calculate amount of blur based on
   // distance and a strength parameter
   // We need 0 <= t <= 1
   float t = saturate(dist*SampleStrength);

   //Blend the original color with the averaged pixels
   gl_FragColor = mix(color, sum, t);
}
