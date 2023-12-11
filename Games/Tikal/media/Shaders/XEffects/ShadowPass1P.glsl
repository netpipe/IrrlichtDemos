void main() 
{
	float depth = gl_TexCoord[0].z / gl_TexCoord[0].x;
	float mulDepth = depth * 256.0 + 2.0;
	float flooredDepth = floor(mulDepth);
		
    gl_FragColor = vec4(flooredDepth / 256.0, mulDepth - flooredDepth, 0.0, 0.0);
}