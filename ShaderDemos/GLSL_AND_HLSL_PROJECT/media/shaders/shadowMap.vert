uniform mat4 mWorldViewProj;

void main() 
{
	gl_TexCoord[0] = (mWorldViewProj * vec4(gl_Vertex.x,gl_Vertex.y,gl_Vertex.z,1));
	gl_Position = ftransform();
}