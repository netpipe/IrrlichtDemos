// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform float Speed;
uniform float Speed2;
uniform float Pulse;
uniform float PulseE;
uniform float CenterX;
uniform float CenterY;
uniform float ElapsedTime;

void main()
{
	vec2 position;
	position.xy = sign(gl_Vertex.xy);
	gl_Position = vec4(position.xy, 0.0, 1.0);
	
	position.xy *= -1.0;
	gl_TexCoord[0].st = 0.5*(1.0-position.xy);
	
    float r = ElapsedTime*Speed;
    vec2 cs = vec2(cos(r), sin(r));
    r = 2.0*(pow(0.5*(sin(Speed2*ElapsedTime)+1.0), PulseE)-0.5);
    r = 1.0 + Pulse*r;

    vec2 ctr = vec2(CenterX, CenterY);
    vec2 t = r*(gl_TexCoord[0].st-ctr);
    gl_TexCoord[0].st = (((vec2((t.x*cs.x - t.y*cs.y),(t.x*cs.y + t.y*cs.x))))+ctr);
}