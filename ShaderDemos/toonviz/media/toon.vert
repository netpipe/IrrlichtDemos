varying vec3 vNormal;
varying vec3 vVertex;

void main(void)
{
   gl_Position = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
   vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
   vNormal = normalize(gl_NormalMatrix * gl_Normal);
}

