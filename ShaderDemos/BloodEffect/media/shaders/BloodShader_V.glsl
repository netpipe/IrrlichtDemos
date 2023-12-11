varying vec2 texCoords;

void main()
{
  gl_Position = ftransform();
  texCoords   = vec2(gl_MultiTexCoord0);
}