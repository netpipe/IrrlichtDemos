uniform mat4 mWorldViewProj;
uniform mat4 mInvWorld;
uniform mat4 mTransWorld;
uniform vec3 mLightPos;
uniform vec4 mLightColor;

uniform vec3 mLight2Pos;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
varying vec3 Normal;

varying vec3 rm_Binormal;
varying vec3 rm_Tangent;

void main(void)
{
   gl_Position = mWorldViewProj * gl_Vertex;
   
   vec4 normal = vec4(gl_Normal, 0.0);
   normal = mInvWorld * normal;
   normal = normalize(normal);
   
   vec4 worldpos = gl_Vertex * mTransWorld;
   
   vec4 lightVector = worldpos - vec4(mLightPos,1.0);
   lightVector = normalize(lightVector);
   
   float tmp2 = dot(-lightVector, normal);
   
   vec4 tmp = mLightColor * tmp2;
   gl_FrontColor = gl_BackColor = vec4(tmp.x, tmp.y, tmp.z, 0.0);
   
   
   
   gl_TexCoord[0] = gl_MultiTexCoord0;
   
   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;
   
   Texcoord    = gl_MultiTexCoord0.xy;
   ViewDirection  = mLightPos - fvObjectPosition.xyz;
   LightDirection = mLight2Pos - fvObjectPosition.xyz;
   Normal         = gl_NormalMatrix * gl_Normal;

   rm_Tangent = gl_MultiTexCoord1;
   rm_Binormal = gl_MultiTexCoord2;
   
   vec3 fvNormal         = gl_NormalMatrix * gl_Normal;
   vec3 fvBinormal       = gl_NormalMatrix * rm_Binormal;
   vec3 fvTangent        = gl_NormalMatrix * rm_Tangent;

   vec3 fvViewDirection  = mLightPos - fvObjectPosition.xyz;
   vec3 fvLightDirection = mLight2Pos - fvObjectPosition.xyz;

   ViewDirection.x  = dot( fvTangent, fvViewDirection );
   ViewDirection.y  = dot( fvBinormal, fvViewDirection );
   ViewDirection.z  = dot( fvNormal, fvViewDirection );

   LightDirection.x  = dot( fvTangent, fvLightDirection.xyz );
   LightDirection.y  = dot( fvBinormal, fvLightDirection.xyz );
   LightDirection.z  = dot( fvNormal, fvLightDirection.xyz );
   
   

} 