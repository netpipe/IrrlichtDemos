uniform float distanceScale;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vViewVec;

void main(void)
{
   gl_Position = ftransform();
   vTexCoord = vec2(gl_MultiTexCoord0);

   // Eye-space lighting
   vNormal = gl_NormalMatrix * gl_Normal;

   // We multiply with distance scale in the vertex shader
   // instead of the fragment shader to improve performance.
   vViewVec = -vec3(distanceScale * gl_ModelViewMatrix * gl_Vertex);

}