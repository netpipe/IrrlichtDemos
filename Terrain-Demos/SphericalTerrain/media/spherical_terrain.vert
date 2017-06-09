uniform mat4 mWorldViewProj;
uniform mat4 mInvWorld;
uniform mat4 mTransWorld;
uniform vec4 mAmbientColor;
uniform vec3 mCamPos;
uniform vec3 mLightPos;
uniform vec4 mLightColor;

varying float normalToCam;
varying vec4 ambientColor;
varying vec3 outNormal;
varying vec4 outWorldPos;

void main(void)
{
	gl_Position = mWorldViewProj * gl_Vertex;

	ambientColor = mAmbientColor;

	vec4 normal = vec4(gl_Normal, 0.0);
	normal = mInvWorld * normal;
	normal = normalize(normal);
	
	outNormal = normal;
	
	vec4 worldpos = gl_Vertex * mTransWorld;
	
	outWorldPos = worldpos;
	
	vec4 lightVector = worldpos - vec4(mLightPos,1.0);
	lightVector = normalize(lightVector);
	
	float tmp2 = dot(-lightVector, normal);
	
	vec4 tmp = mLightColor * tmp2;
	 
	normalToCam = 1.0 - dot(normal, normalize(mCamPos - worldpos.xyz));

	gl_FrontColor = gl_BackColor = vec4(tmp.rgb, 0.0);

	gl_TexCoord[0] = gl_MultiTexCoord0;
}
