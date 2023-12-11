void main() 
{
	gl_Position = vec4(gl_Vertex.x, gl_Vertex.y, 0.0, 1.0);
	gl_TexCoord[0].x = 0.5 * (1.0 + gl_Vertex.x - (1.0 / float(SCREENX)));
	gl_TexCoord[0].y = 0.5 * (1.0 + gl_Vertex.y - (1.0 / float(SCREENY)));
}