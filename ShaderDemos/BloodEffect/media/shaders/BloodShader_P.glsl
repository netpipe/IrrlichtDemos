uniform sampler2D blood;
uniform float time;

varying vec2 texCoords;

void main()
{
  vec2 tex = texCoords;

  tex.x += sin(25.0 * tex.x + time / 100.0 ) / 40.0;
  tex.y -= sin(25.0 * tex.y + time / 100.0 ) / 40.0;

  gl_FragColor = texture2D(blood, tex);
}