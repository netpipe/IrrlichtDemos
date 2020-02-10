
uniform sampler2D texture0;
uniform float UseTexture0;

varying vec2 texCoord02D;

// Lighting

varying vec3 ecPos;
uniform float Saturation; // Hmm is this the correct term?

// Hemisphere lighting
uniform vec3 SunPos;
uniform vec3 SkyCol;
uniform vec3 GroundCol;

// Point light
varying vec4 diffuse;
varying vec4 ambient;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;
varying float dist;

void main(void)
{
	vec3 n;
	vec4 col = vec4(0.0);
	
	n = normalize(normal);
	
	// dot product between normal and normalized lightdir
	float NdotL = max(dot(n,normalize(lightDir)),0.0);
	
	if (NdotL > 0.0)
	{
		float attenuation = 1.0 / (gl_LightSource[0].constantAttenuation +
			gl_LightSource[0].linearAttenuation * dist +
			gl_LightSource[0].quadraticAttenuation * dist * dist);
		
		col += attenuation * (diffuse * NdotL + ambient);
		
		col += attenuation * gl_FrontMaterial.specular * gl_LightSource[0].specular *
			pow(
				max(dot(n,normalize(halfVector)), 0.0), // NdotHV
				gl_FrontMaterial.shininess);
	}
	
	// Hemisphere lighting
	
    vec4 hemiColour = vec4(
    	mix(GroundCol, SkyCol, 0.5 + 0.5 * dot(n, normalize(SunPos - ecPos))),
    		1.0
    		);
	
	gl_FragColor = col + hemiColour;
	gl_FragColor *= Saturation;
	
	if (UseTexture0 > 0.0)
	{
		 gl_FragColor *= texture2D(texture0,texCoord02D);
	}
}
