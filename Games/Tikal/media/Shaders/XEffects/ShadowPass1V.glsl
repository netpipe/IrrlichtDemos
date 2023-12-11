uniform mat4 mWorldViewProj;
uniform float MaxD;

void main()
{
    vec4 tPos = mWorldViewProj * gl_Vertex;
    gl_Position = tPos;
    gl_TexCoord[0] = tPos;
	gl_TexCoord[0].x = MaxD; 
	
	gl_TexCoord[1] = gl_MultiTexCoord0;
}