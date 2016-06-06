struct VS_OUTPUT 
{
    vec4 Position;
    vec4 ShadowMapSamplingPos;
    vec4 MVar;
    vec2 TexCoords;
    vec2 TexCoords2;
};

uniform float MaxD;
uniform float ShadDark;
uniform mat4 mWorldViewProj;
uniform mat4 mWorldViewProj2;
uniform float EnableLighting;

VS_OUTPUT vertexMain( in vec3 Position, in vec2 TexCoords, in vec2 TexCoords2) 
{
    VS_OUTPUT OUT;

    OUT.Position = (mWorldViewProj * vec4( Position.x , Position.y , Position.z , 1.00000)  );
    OUT.ShadowMapSamplingPos = ( mWorldViewProj2 * vec4( Position.x , Position.y , Position.z , 1.00000));
	
    OUT.MVar.x = (OUT.ShadowMapSamplingPos.z / MaxD) - 0.01;
	
	if(EnableLighting > 0.5)
	{
		vec3 PosE = vec4(gl_ModelViewMatrix * gl_Vertex).xyz;
		vec3 lightDir = gl_LightSource[0].position.xyz - PosE;
		vec3 normal = gl_NormalMatrix * gl_Normal;
		OUT.MVar.y = max(dot(normalize(lightDir),normalize(normal)),0.1);
	}
	else
	{	
		OUT.MVar.y = 1.0;
	}	
	
    OUT.MVar.z  = MaxD;

	OUT.MVar.w  = (ShadDark);
		
    OUT.TexCoords = TexCoords;
    OUT.TexCoords2 = TexCoords2;
	
	return OUT;
}

void main() 
{
    VS_OUTPUT xlat_retVal;

    xlat_retVal = vertexMain( vec3(gl_Vertex), vec2(gl_MultiTexCoord0), vec2(gl_MultiTexCoord1));

    gl_Position = vec4( xlat_retVal.Position);
    gl_TexCoord[0] = vec4( xlat_retVal.ShadowMapSamplingPos);
    gl_TexCoord[1] = vec4( xlat_retVal.MVar);
    gl_TexCoord[2] = vec4( xlat_retVal.TexCoords, 0.0, 0.0);
    gl_TexCoord[3] = vec4( xlat_retVal.TexCoords2, 0.0, 0.0);
}

