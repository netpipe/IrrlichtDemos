
uniform float fFurLength;
uniform float fUVScale;
uniform vec3  vGravity;
uniform float fLayer; // 0 to 1 for the level

uniform vec4 vLightDir; 

//textures
uniform sampler2D FurTexture;
uniform sampler2D ColourTexture;

varying vec3 normal;

mat4 mWorldViewProj = gl_ModelViewProjectionMatrix;
mat4 mWorld = gl_ModelViewMatrix;

vec3 vNormal = gl_Normal;
vec4 vVertex = gl_Vertex;

void fragMain()
{
	vec4 diffuseTexture = texture2D( FurTexture,  vec2(gl_TexCoord[0]) ); // Fur Texture - alpha is VERY IMPORTANT!
	vec4 ColourTexture  = texture2D( ColourTexture, vec2(gl_TexCoord[1]) ); // Colour Texture for patterns
  
	vec4 FinalColour = ColourTexture;
	FinalColour.a = diffuseTexture;
	

	vec4 ambient = vec4(0.3, 0.3, 0.3, 0.0);
	ambient = ambient * FinalColour;
	vec4 diffuse = FinalColour;
	FinalColour = ambient + diffuse * dot(vec3(vLightDir), normal);

	if(fFurLength ==0)
	{
	    FinalColour.a = 1;
	}
	else
	{
	    FinalColour.a  = diffuseTexture;
	}
	
	gl_FragColor = FinalColour;

}


void vertMain()
{
	vec3 P = vVertex.xyz + (vNormal * fFurLength);
	vNormal = normalize( vNormal * mat3(mWorld));
	
	vGravity = mul(vGravity, mat3(mWorld));
	float k = pow(fLayer, 3.0);

	P = P + vGravity*k;

	gl_TexCoord[0] = gl_MultiTexCoord0 * fUVScale;
	gl_TexCoord[1] = gl_MultiTexCoord0;

	gl_Position = mWorldViewProj * vec4(P,1.0);
    normal = vNormal;
} 
