
/*

Based on code from:
	- OpenGL Shading Language (Randi J. Rost)
	- http://www.lighthouse3d.com/opengl/glsl/index.php?pointlight

*/

varying vec2 texCoord02D;

// Lighting

varying vec3 ecPos;

// Point light
varying vec4 diffuse;
varying vec4 ambient;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;
varying float dist;

void main(void)
{
    vec3 eye = vec3(0.0,0.0,1.0);
    
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    ecPos = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 lightVec = vec3(gl_LightSource[0].position)/gl_LightSource[0].position.w - ecPos;
    lightDir = normalize(lightVec);
    dist = length(lightVec);
    
    halfVector = normalize(lightDir + eye);
	
	ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
	
    gl_Position = ftransform();
    texCoord02D = vec2(gl_MultiTexCoord0);
}

