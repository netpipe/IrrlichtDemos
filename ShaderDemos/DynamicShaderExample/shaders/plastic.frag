uniform vec4 Color;
varying vec3 tnorm;
varying vec3 lightVec;

void main(void)
{
   float v = 0.5 * (1.0 + dot(normalize(lightVec), tnorm));
   //v = pow(v,2.0);
   gl_FragColor = v * Color;
}