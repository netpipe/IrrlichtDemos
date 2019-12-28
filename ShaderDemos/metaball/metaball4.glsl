uniform sampler2D RTTexture;
uniform vec4 TintColour;

varying vec3 WorldView;
varying vec4 GlassPos;
varying vec2 TexCoord;
varying vec3 LightDir, VNormal, Tangent, Binormal;
varying vec4 VCol;

void main()
{
    vec4 projCoord = GlassPos / GlassPos.w;
    projCoord += vec4(1.0);
    projCoord *= 0.5;
    vec2 norTex = TexCoord.xy;

   vec3 Normal = VNormal;
    vec2 TexOff = Normal.xz * 2.0;
   TexOff -= vec2(1.0,1.0);
   
    projCoord.xy += (TexOff.xy / 5.0);
    projCoord.xy = clamp(projCoord.xy, 0.001, 0.999);
   
    vec4 refTex = texture2D(RTTexture,projCoord.xy);
   
   
    vec3 ViewDir = normalize(WorldView);
   
    float facing = max(dot(ViewDir, Normal), 0.0);
    float diff = max(dot(LightDir,Normal),0.0);
    vec3 reflect = normalize(diff * Normal - LightDir);
    float spec = dot(reflect, ViewDir) * 4.0;   
    spec = clamp(pow(spec,2.0),0.0,2.0);
 
    gl_FragColor = refTex * VCol * facing;
    gl_FragColor += spec * VCol;
}
