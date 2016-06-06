uniform sampler2D RTTexture;
uniform sampler2D NormalMap;
uniform vec4 TintColour;

varying vec3 WorldView;
varying vec4 GlassPos;
varying vec2 TexCoord;
varying vec3 LightDir, VNormal, Tangent, Binormal;


void main()
{
    vec4 projCoord = GlassPos / GlassPos.w;
    projCoord += vec4(1.0);
    projCoord *= 0.5;
    vec2 norTex = TexCoord.xy;

    vec3 Normal = texture2D(NormalMap,norTex).xyz;
    
    vec2 TexOff = Normal.xy * 2.0;
    TexOff -= vec2(1.0,1.0); 
    
    projCoord.xy += ((TexOff.xy - vec2(0.5,0.5)) / 25.0);
    projCoord.xy = clamp(projCoord.xy, 0.001, 0.999);
    
    vec4 refTex = texture2D(RTTexture,projCoord.xy);
    
    Normal -= vec3(0.5,0.5,0.5);
    Normal = normalize(	Normal.x * Tangent -
						Normal.y * Binormal + 
						Normal.z * VNormal);
    
    vec3 ViewDir = normalize(WorldView);
    
    //float facing = max(dot(ViewDir, Normal), 0.0);
    float diff = max(dot(LightDir,Normal),0.0);
    vec3 reflect = normalize(diff * Normal - LightDir);
    //float spec = dot(reflect, ViewDir);
    float spec = clamp(dot(normalize(abs(LightDir-ViewDir)),Normal),0.0,1.0);   
    spec = clamp(pow(spec,16.0),0.0,1.0);
 
    gl_FragColor = refTex * TintColour;
    gl_FragColor += spec; 
}