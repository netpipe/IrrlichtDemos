uniform float MaxD;
uniform float Timer;
uniform mat4 mWorldViewProj;

void main() 
{
    float timeNow = Timer;
    vec4 Po = gl_Vertex;
    float iny = (Po.y  * 0.5) + timeNow;
    float wiggleX = sin(iny) * 0.5;
    float wiggleY = cos(iny) * 0.5;
    Po.x = Po.x + wiggleX;
    vec4 tPos = mWorldViewProj * Po;

	gl_Position = tPos;
	gl_TexCoord[0] = tPos;
	gl_TexCoord[0].x = MaxD;
}
