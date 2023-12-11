varying float posY;
uniform int terrainScale;

void main(void)
{
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_FogFragCoord = gl_Position.z;
}
