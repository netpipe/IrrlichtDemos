uniform int terrainScale;
varying float posY;

void main(void)
{
	float scale = (float(terrainScale) * 0.05);
			
	posY = gl_Vertex.y;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec4 vertex = gl_Vertex;
	vertex.y = -scale;
	gl_Position = gl_ModelViewProjectionMatrix * vertex;

	gl_FogFragCoord = gl_Position.z;
}
