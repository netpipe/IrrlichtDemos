
uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;

float unpackFloat(vec4 value)
{
   float extractedDistance = (value.r + value.g / 256.0);
   return extractedDistance;
}

float calcSample(float depth, vec2 texCoords)
{
   vec4 tempDepth = texture2D(DepthMapSampler, texCoords);
   float otherDepth = unpackFloat(tempDepth);
   return min(abs(otherDepth - depth) * 20.0, 1.0);
}

vec2 offsetArray[4];

//vec4 pixelMain ( vec2 Texcoords)
void main()
{
   offsetArray[0] = vec2( 1.5  ,0);
   offsetArray[1] = vec2( -1.5 ,0);
   offsetArray[2] = vec2(0, 1.5 );
   offsetArray[3] = vec2(0, -1.5 );


   float depth = unpackFloat(texture2D(DepthMapSampler, gl_TexCoord[0].xy));
   float outline = (0.0, 0.0, 0.0, 0.0);

   for(int i = 0;i < 4;i++)
      outline += calcSample(depth, offsetArray[i]);

   gl_FragColor = texture2D(ScreenMapSampler, gl_TexCoord[0].xy) * min(1.0 - outline, 1.0);
} 

