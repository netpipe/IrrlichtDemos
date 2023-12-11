#define MAX_JOINT_NUM 55
#define MAX_LIGHT_NUM 8

#define verCol gl_LightSource[5].spotExponent
#define ecPos gl_LightSource[1].halfVector
#define Normal gl_LightSource[1].spotDirection
#define LightDir gl_LightSource[0].spotDirection
#define NDotL gl_LightSource[0].spotExponent
#define Dist gl_LightSource[4].spotExponent 

uniform mat4 JointTransform[MAX_JOINT_NUM];

void main() 
{
	mat4 ModelTransform = gl_ModelViewProjectionMatrix;
	 
	verCol = int(gl_Color.r * 255.9);
	mat4 vertTran = JointTransform[verCol - 1];
	
	verCol = int(gl_Color.g * 255.9);
	if(verCol > 0)
		vertTran += JointTransform[verCol - 1];

	verCol = int(gl_Color.b * 255.9);
	if(verCol > 0)
		vertTran += JointTransform[verCol - 1];
		
	verCol = int(gl_Color.a * 255.9);
	if(verCol > 0)
		vertTran += JointTransform[verCol - 1];
		
	ecPos = gl_ModelViewMatrix * vertTran * gl_Vertex;
	
	Normal = normalize(gl_NormalMatrix * mat3(vertTran) * gl_Normal);
	
	gl_FrontColor = vec4(0,0,0,0);

	for(int i = 0;i < MAX_LIGHT_NUM;i++)
	{
		LightDir = vec3(gl_LightSource[i].position-ecPos);
		NDotL = max(dot(Normal,normalize(LightDir)),0.0);
	
		Dist = length(LightDir);
		NDotL *= 1.0 / (gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation * Dist);
				
		gl_FrontColor += gl_LightSource[i].diffuse * NDotL;
	}	
	
	gl_FrontColor = clamp(gl_FrontColor,0.3,1.0);

	ModelTransform *= vertTran;
		
	gl_Position = ModelTransform * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	/* 
	// Reflections.
	vec3 r = reflect( ecPos.xyz , Normal );
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	gl_TexCoord[1].s = r.x/m + 0.5;
	gl_TexCoord[1].t = r.y/m + 0.5;
	*/
}