//uniform vec3 mLightDir = vec3(0,1,1);
uniform mat4 mWorldViewProj;
uniform mat4 mWorld : World; 

void main() 
{
 	vec3 mLightDir = vec3(0,1,1);
	vec3 normal;
	vec4 diffuse;
	float LightColor;
	float normalDist;
	
	/* first transform the normal into eye space and normalize the result */
	//Output.Normal = mul(Input.Normal,mWorld);
	//normal = mul(gl_Normal,mWorld);
	normal = normalize(gl_Normal);
	normalDist = gl_Normal.y;
	
	/* now normalize the light's direction. Note that according to the
	OpenGL specification, the light is stored in eye space. Also since 
	we're talking about a directional light, the position field is actually 
	direction */
	mLightDir = normalize(mLightDir);
	
	/* compute the cos of the angle between the normal and lights direction. 
	The light is directional so the direction is constant for every vertex.
	Since these two are normalized the cosine is the dot product. We also 
	need to clamp the result to the [0,1] range. */
	LightColor = max(dot(normal, mLightDir), 0.0);
	
	/* Compute the diffuse term */
	
	if (gl_Vertex.y > 268) {
		normalDist = normal.y/2;
	}else{
		normalDist = normal.y;
	}
	
	gl_FrontColor = LightColor;
	gl_TexCoord[0] = gl_MultiTexCoord0 * gl_TextureMatrix[0];
	gl_TexCoord[1] = vec4(normalDist,0,0,0);
	
	gl_Position = ftransform();
}