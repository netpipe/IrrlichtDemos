varying vec3 tnorm;
varying vec3 lightVec;

void main(void)
{
   vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
   tnorm = normalize(gl_NormalMatrix * gl_Normal);
   lightVec = normalize(vec3(gl_ModelViewMatrix * gl_LightSource[0].position) - ecPosition);
   
   gl_Position = ftransform();
}