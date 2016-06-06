uniform float MaxD;

void main() 
{
	float depth = gl_TexCoord[0].z / MaxD;
	float mulDepth = depth * 256.0f;
	float flooredDepth = floor(mulDepth);
    gl_FragColor = vec4(flooredDepth / 256.0f, (mulDepth - flooredDepth),0,0);
}