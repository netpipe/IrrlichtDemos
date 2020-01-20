//Interpolates the vertex normal across the texture fragment
varying vec3 normal;
//Interpolate the texel coordinate in eye space.
varying vec3 ec_pos;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	ec_pos = gl_ModelViewMatrix * gl_Vertex;
	
	//Perform fixed transformation for the vertex
	gl_Position = ftransform();		
}