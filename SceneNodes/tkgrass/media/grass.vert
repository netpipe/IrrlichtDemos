varying float realDist;

void main()
{
	vec4 pos;
	// Billboard the quads.
	// The view matrix gives us our right and up vectors.
	gl_Position = gl_ModelViewProjectionMatrix * vec4(
	
	( gl_Vertex.x * vec3(gl_ModelViewProjectionMatrix[0][0],gl_ModelViewProjectionMatrix[1][0] , gl_ModelViewProjectionMatrix[2][0] ) +
		gl_Vertex.y * vec3(gl_ModelViewProjectionMatrix[0][1], gl_ModelViewProjectionMatrix[1][1] , gl_ModelViewProjectionMatrix[2][1]) ),
	1.0);
	//gl_Position = gl_ModelViewProjectionMatrix;// * gl_Vertex;
	
	//gl_Position=ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	realDist = distance(gl_Position.xyz,gl_ModelViewMatrixInverse[3]);
}
