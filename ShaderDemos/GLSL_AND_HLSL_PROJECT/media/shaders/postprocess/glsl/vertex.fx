void main()
{
	vec2 position;
	position.xy = sign(gl_Vertex.xy);
	gl_Position = vec4(position.xy, 0.0, 1.0);
	
	position.xy *= -1.0;
	gl_TexCoord[0].st = 0.5*(1.0-position.xy);
}
