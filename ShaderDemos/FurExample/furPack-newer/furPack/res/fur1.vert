uniform float fFurLength; 
uniform float fUVScale; 
uniform float fLayer; // 0 to 1 for the level 
uniform vec3  vGravity; 

uniform vec4 vLightDir; 

//textures 
uniform sampler2D FurTexture; 
uniform sampler2D ColourTexture; 

varying vec3 normal; 

void main() 
{ 
   mat4 mWorldViewProj= gl_ModelViewProjectionMatrix; 
   mat4 mWorld = gl_ModelViewMatrix; 

   vec4 oNormal = vec4(gl_Normal.x, gl_Normal.y, gl_Normal.z, 1.0); 
   vec4 vVertex = gl_Vertex; 
   
   vec3 P = vVertex.xyz + (oNormal.xyz * ((fFurLength)*10)); 
   vec3 vNormal = normalize((oNormal * mWorld).xyz); 

   vec4 vGravity4 = vec4(vGravity.x, vGravity.y, vGravity.z, 1.0); 
   vec3 vGrav = (vGravity4 * mWorld).xyz; 
   float k = pow(fLayer, 3.0); 

   P = P + vGravity*k; 

   gl_TexCoord[0] = gl_MultiTexCoord0 * fUVScale; 
   gl_TexCoord[1] = gl_MultiTexCoord0; 

   gl_Position = mWorldViewProj* vec4(P,1.0); 
   normal = vNormal; 
}