uniform sampler2D RTTexture;
uniform sampler2D NormalMap;
uniform vec4 TintColour;

varying vec4 GlassPos;
varying vec2 TexCoord;
varying float VAlpha;

void main()
{
    vec4 projCoord = GlassPos / GlassPos.w;
    projCoord += vec4(1.0);
    projCoord *= 0.5;
    vec2 norTex = TexCoord.xy;

	vec4 NorMap = texture2D(NormalMap,norTex);
    vec3 Normal = NorMap.xyz;
    
    vec2 TexOff = Normal.xy * 2.0;
    TexOff -= vec2(1.0,1.0); 
    
    projCoord.xy += ((TexOff.xy - vec2(0.5,0.5)) / 25.0);
    projCoord.xy = clamp(projCoord.xy, 0.001, 0.999);
    
    vec4 refTex = texture2D(RTTexture,projCoord.xy);
    
    gl_FragColor = refTex * TintColour;
    gl_FragColor.a = NorMap.a * VAlpha;
}