uniform sampler2D ShadowMapSampler;
uniform vec4 LightColour;

void main() 
{
    gl_FragColor = texture2D(ShadowMapSampler,gl_TexCoord[0]*0.2 + vec2(0.5,0));
}