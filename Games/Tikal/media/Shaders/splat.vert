varying vec2 texCoord;
varying vec4 diffuse,ambientGlobal,ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;

//============
struct VS_OUTPUT 
{
	vec4 Position;
	vec4 ShadowMapSamplingPos;
	vec4 MVar;
	vec2 TexCoords;
};

uniform float MaxD, MAPRES;
uniform vec3 LightPos;
uniform mat4 mWorldViewProj;
uniform mat4 mWorldViewProj2;

VS_OUTPUT vertexMain( in vec3 Position, in vec2 TexCoords) 
{
	VS_OUTPUT OUT;

	OUT.Position = (mWorldViewProj * vec4(Position.x, Position.y, Position.z, 1.0));
	OUT.ShadowMapSamplingPos = (mWorldViewProj2 * vec4(Position.x, Position.y, Position.z, 1.0));

	vec3 lightDir = normalize(LightPos - Position);
	
	OUT.MVar.x = OUT.ShadowMapSamplingPos.z;
	OUT.MVar.y = dot(normalize(gl_Normal.xyz), lightDir);
	OUT.MVar.z = MaxD;
	OUT.MVar.w = 1.0 / MAPRES;
		
	OUT.TexCoords = TexCoords;

	return OUT;
}
//==========

void main()
{	
	texCoord=gl_MultiTexCoord0;
	vec4 ecPos;
	vec3 aux;
	
	normal = normalize(gl_NormalMatrix * gl_Normal);
		
	ecPos = gl_ModelViewMatrix * gl_Vertex;
	aux = vec3(gl_LightSource[0].position-ecPos);
	lightDir = normalize(aux);
	dist = length(aux);
	
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);
		
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
			
	//gl_Position = ftransform();
	
	VS_OUTPUT vOut = vertexMain(gl_Vertex.xyz, gl_MultiTexCoord0.xy);

	gl_Position = vOut.Position;
	gl_TexCoord[0] = vOut.ShadowMapSamplingPos;
	gl_TexCoord[1] = vOut.MVar;
	gl_TexCoord[2].xy = vOut.TexCoords.xy;
} 

		

