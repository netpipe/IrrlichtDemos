uniform mat4 mWorld;
uniform vec4 CamPos;
varying vec4 GlassPos;
varying vec3 WorldView;
varying vec2 TexCoord;
varying vec3 LightDir, VNormal, Tangent, Binormal;

void main()
{

    TexCoord = gl_MultiTexCoord0.xy * 2.0;
    GlassPos = ftransform();

    vec3 Norm = gl_Normal.xyz;
    VNormal = normalize(vec3(mWorld * vec4(Norm.xyz,0)).xyz);
    Tangent = -vec3(abs(Norm.y) + abs(Norm.z), abs(Norm.x), 0);
    Binormal = cross(Tangent,Norm);  
    
    Tangent = normalize(vec3(mWorld * vec4(Tangent.xyz,0)).xyz);
    Binormal = normalize(vec3(mWorld * vec4(Binormal.xyz,0)).xyz);
    
    Tangent = cross(Binormal,Norm);
    Binormal = Binormal;
    
    vec3 LightPos = vec3(10,10,10);
    
    vec4 PosW = mWorld * gl_Vertex;
    
    LightDir = normalize(LightPos);
    WorldView = CamPos.xyz - PosW.xyz;
	
    gl_Position = GlassPos;
}