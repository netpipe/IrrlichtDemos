//uniform vec4 lightDir;
uniform sampler2D Base;

varying vec2 vTexCoord;
varying vec3 vNormalES;
varying vec3 vViewVec;
varying vec3 vLightDir;

void main(void)
{

   vec4 base = texture2D(Base, vTexCoord);
   // Make our base texture a little brighter ...
   // Yes, you're right, this should be done in photoshop rather
   // than in the shader, but I'm too lazy to make a brighter version
   // of this texture or even look for a better one.
   base = sqrt(base);

   // Make the speaker more glossy in the middle
   vec2 l = vTexCoord - 0.5;
   float gloss = clamp(1.0 - 6.0 * dot(l, l), 0.0, 1.0);

   vec3 normal = normalize(vNormalES);
   // Soft diffuse
   vec3 lightDir = normalize(vLightDir);
   float diffuse = 0.5 + 0.5 * dot(lightDir, normal);
   // Standard Phong specular
   float specular = pow(clamp(dot(reflect(-normalize(vViewVec), normal), lightDir.xyz), 0.0, 1.0), 64.0);

   gl_FragColor = 0.8 * diffuse * base + gloss * specular ;


}
