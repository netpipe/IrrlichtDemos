#version 130

varying vec3 worldPos;
varying vec3 norm;
varying vec3 vColor;

uniform mat4 normMat;

uniform vec3 camPos;

uniform vec3 ambience;

uniform vec3 shadowColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	bool lighting;
	
	float shininess;
};

struct pointLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float radius;
	
	vec3 position;
	
	vec3 attenuation;
};

struct dirLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 direction;
};

struct spotLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float radius;
	
	vec3 position;
};

uniform Material material;

uniform pointLight plights[32];
uniform int numplights;

uniform dirLight dlights[32];
uniform int numdlights;

uniform spotLight slights[16];
uniform int numslights;


vec3 calculatePointLight(pointLight light, vec3 normal);
vec3 calculateDirLight(dirLight light, vec3 normal);
vec3 calculateSpotLight(spotLight light, vec3 normal);


void main()
{
	vec3 normal = normalize(mat3(normMat) * norm);
	
	vec3 color = vec3(0.0f, 0.0f, 0.0f);
	
	if(material.lighting) {
		for(int i = 0; i < numplights; i++) {
			color = calculatePointLight(plights[i], normal);
		}
		
		for(int i = 0; i < numdlights; i++) {
			color += calculateDirLight(dlights[i], normal);
		}
		
		for(int i = 0; i < numslights; i++) {
			color += calculateSpotLight(slights[i], normal);
		}
	}
	else {
		color = vColor;
	}
	
	gl_FragColor = vec4(ambience + color, 1.0f);
}

vec3 calculatePointLight(pointLight light, vec3 normal)
{
	float dist = length(light.position - worldPos);
	if(dist > light.radius) {
		//return vec3(0.0f, 0.0f, 0.0f);
	}
	
	vec3 viewDir = normalize(camPos - worldPos);
	vec3 lightDir = normalize(light.position - worldPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	vec3 ambient = light.ambient * 
				   vColor * 
				   material.ambient;
    vec3 diffuse = light.diffuse * 
				   vColor * 
				   max(dot(normal, lightDir), 0.0f) * 
				   material.diffuse;
	vec3 specular = light.specular * 
					vColor * 
					pow(max(dot(normal, halfwayDir), 0.0f), material.shininess) * 
					material.specular;
	
    float attenuation = min(1.0f / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist), 1.0f);
	
	return (/*attenuation */ (ambient + diffuse + specular));
}

vec3 calculateDirLight(dirLight light, vec3 normal)
{
	vec3 viewDir = normalize(camPos - worldPos);
	vec3 lightDir = normalize(-light.direction);
	vec3 halfwayDir = normalize(lightDir + viewDir);
        
    vec3 ambient = light.ambient * 
				   vColor * 
				   material.ambient;
    vec3 diffuse = light.diffuse * 
				   vColor * 
				   max(dot(normal, lightDir), 0.0f) * 
				   material.diffuse;
	vec3 specular = light.specular * 
					vColor * 
					pow(max(dot(normal, halfwayDir), 0.0f), material.shininess) * 
					material.specular;
	
	return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(spotLight light, vec3 normal)
{
	return vec3(0.0f, 0.0f, 0.0f);
}
