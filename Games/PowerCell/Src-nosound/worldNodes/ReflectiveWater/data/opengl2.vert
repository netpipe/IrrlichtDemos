// vertex shader
varying vec3 normal, viewVector, texCoord;
varying vec3 lightVector;
varying float numlights;

void main()
{
	vec4 worldPos;

	normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 tangent = gl_MultiTexCoord1.xyz;
	vec3 binormal = cross(tangent, normal);
	gl_Position = ftransform();

	mat3 TBNMatrix;
	TBNMatrix[0] =  gl_NormalMatrix * tangent;
	TBNMatrix[1] =  gl_NormalMatrix * binormal;
	TBNMatrix[2] =  normal;

	worldPos = gl_ModelViewMatrix * gl_Vertex;
	
	lightVector = vec3(gl_LightSource[0].position - worldPos);
			
	viewVector = vec3(-worldPos) * TBNMatrix ;

	texCoord = gl_MultiTexCoord0.xyz;
}