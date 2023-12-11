

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vViewVec;

void main(void)
{

   // use the distance to decide how much blur we want
   float dist = length(vViewVec);

   gl_FragColor = vec4(dist);//light,dist
}