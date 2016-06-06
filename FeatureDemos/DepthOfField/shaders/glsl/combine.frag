uniform float range;
uniform float focus;

uniform sampler2D RT;
uniform sampler2D Distance;
uniform sampler2D Blur0;
uniform sampler2D Blur1;

varying vec2 vTexCoord;


void main(void)
{
   vec4 sharp = texture2D(RT, vTexCoord);
   vec4 blur  = texture2D(Blur1, vTexCoord);
   vec4 dist  = texture2D(Distance, vTexCoord);

   gl_FragColor = mix(sharp, blur, clamp(range * abs(focus - dist.r), 0.0, 1.0));
   //gl_FragColor = dist;
}