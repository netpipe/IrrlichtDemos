#version 130

//layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;

varying vec3 worldPos;
varying vec3 norm;
varying vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * gl_Vertex;
	
	worldPos = vec3(model * gl_Vertex);
	norm = gl_Normal;
	vColor = vec3(gl_Color);
}
