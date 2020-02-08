#define MAX_LIGHTS 4

/* Attributes */

attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
attribute vec4 inVertexColor;
attribute vec2 inTexCoord0;

/* Uniforms */

uniform mat4 uWVPMatrix;
uniform mat4 uWVMatrix;
uniform mat4 uNMatrix;
uniform mat4 uTMatrix0;

uniform vec4 uGlobalAmbient;
uniform vec4 uMaterialAmbient;
uniform vec4 uMaterialDiffuse;
uniform vec4 uMaterialEmissive;
uniform vec4 uMaterialSpecular;
uniform float uMaterialShininess;

uniform int uLightCount;
uniform vec3 uLightPosition[MAX_LIGHTS];
uniform float uLightAttenuation[MAX_LIGHTS];
uniform vec4 uLightDiffuse[MAX_LIGHTS];
uniform vec4 uLightSpecular[MAX_LIGHTS];

/* Varyings */

varying vec2 vTextureCoord0;
varying vec4 vVertexColor;
varying vec4 vSpecularColor;

void pointLightDiffuse(in int index, in vec3 position, in vec3 normal, inout vec4 diffuse)
{
	vec3 L = uLightPosition[index] - position;
	vec3 Lnorm = normalize(L);

	float NdotL = dot(normal, Lnorm);
	if (NdotL > 0.0)
	{
		float D = length(L)+0.01;	
		float Attenuation = 1.0 / (uLightAttenuation[index] * D);	
		diffuse += uLightDiffuse[index] * NdotL * Attenuation;
	}
}

void pointLightSpecular(in int index, in vec3 position, in vec3 normal, in vec3 e, inout vec4 specular)
{
	vec3 L = uLightPosition[index] - position;
	vec3 Lnorm = normalize(L);

	float NdotL = dot(normal, Lnorm);

	if (NdotL > 0.0)
	{
		float D = length(L)+0.01;	
		float Attenuation = 1.0 / (uLightAttenuation[index] * D);	

		vec3 HalfVector = normalize(Lnorm + e);
		float NdotH = max(0.0, dot(normal, HalfVector));

		float SpecularFactor = pow(NdotH, uMaterialShininess);
		specular += uLightSpecular[index] * SpecularFactor * Attenuation;
	}
}

void main()
{
	gl_Position = uWVPMatrix * vec4(inVertexPosition, 1.0);
	
	vTextureCoord0 = inTexCoord0;
	vVertexColor = inVertexColor.bgra;
	vSpecularColor = vec4(0.0, 0.0, 0.0, 0.0);

	if (uLightCount > 0)
	{
		vec3 Position = (uWVMatrix * vec4(inVertexPosition, 1.0)).xyz;	
		vec3 Normal = normalize((uNMatrix * vec4(inVertexNormal, 0.0)).xyz);
		vec3 E = normalize(-Position); 

		vec4 Diffuse = vec4(0.0, 0.0, 0.0, 1.0);

		pointLightDiffuse(0, Position, Normal, Diffuse);
		pointLightDiffuse(1, Position, Normal, Diffuse);
		pointLightSpecular(2, Position, Normal, E, vSpecularColor);
		if ( uLightCount == 4 )
			pointLightSpecular(3, Position, Normal, E, vSpecularColor);
		
		vVertexColor *= clamp(Diffuse, 0.0, 1.0) * uMaterialDiffuse;
		vVertexColor += uMaterialEmissive;
		vVertexColor += uGlobalAmbient * uMaterialAmbient;
		vVertexColor = clamp(vVertexColor, 0.0, 1.0);			
		
		vSpecularColor *= uMaterialSpecular;
	}
}
