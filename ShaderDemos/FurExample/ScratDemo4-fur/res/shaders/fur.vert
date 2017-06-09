
uniform float fFurLength;
uniform float fUVScale;
uniform float fLayer; // 0 to 1 for the level
uniform vec4  vGravity; 

uniform vec4 vLightDir; 

//textures
uniform sampler2D FurTexture;
uniform sampler2D ColourTexture;

varying vec3 normal;

mat4 mWorldViewProj= gl_ModelViewProjectionMatrix;
mat4 mWorld = gl_ModelViewMatrix;

void main()
{
	vec4 oNormal = vec4(gl_Normal.x, gl_Normal.y, gl_Normal.z, 1.0);
	vec4 vVertex = gl_Vertex;

	vec3 P = vVertex.xyz + (oNormal.xyz * ((fFurLength)*10));
	vec3 vNormal = normalize((oNormal * mWorld).xyz);
	
	// Additional Gravit/Force Code
	vec3 vGravity2 = (vGravity * mWorld ).xyz;
	//float pp = 3.14 * 0.5 * Layer; // depth paramete
	//float A = dot(normal, vGravity);  //A is the angle between surface normal and gravity vector
	float k = pow(fLayer, 3.0); // We use the pow function, so that only the tips of the hairs bend
	                           // As layer goes from 0 to 1, so by using pow(..) function is still 
	                           // goes form 0 to 1, but it increases faster! exponentially
	P = P + vGravity2*k;
	// End Gravity Force Addit Code


	gl_TexCoord[0] = gl_MultiTexCoord0 * fUVScale;
	gl_TexCoord[1] = gl_MultiTexCoord0;
	// UVScale??  Well we scale the fur texture alpha coords so this effects the fur thickness
	// thinness, we don't change our T1 value as this is our actual texture!
	
	//OUT.HPOS = P;
	gl_Position = mWorldViewProj* vec4(P,1.0);
    normal = vNormal;
} 
