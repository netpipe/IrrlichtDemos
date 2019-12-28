uniform sampler2D RTTexture;
uniform sampler2D NormalMap;
uniform vec4 TintColour;

varying vec3 WorldView;
varying vec4 GlassPos;
varying vec3 LightDir, VNormal;
varying vec4 VCol;
varying vec3 ViewNormal;


void main()
{
    vec4 projCoord = GlassPos / GlassPos.w;
    projCoord += vec4(1.0);
    projCoord *= 0.5;

    vec3 Normal = VNormal; 
    vec2 TexOff = ViewNormal.xy;    
    projCoord.xy += (TexOff.xy / 5.0);
    projCoord.xy = clamp(projCoord.xy, 0.001, 0.999);
    
    vec4 refTex = texture2D(RTTexture,projCoord.xy);
    
    vec3 ViewDir = normalize(WorldView);
    vec3 refCoords = reflect(ViewDir, Normal);
    vec4 reflectTex = texture2D(RTTexture,refCoords.xy);
    
    float facing = max(dot(ViewDir, Normal), 0.0);
    float diff = max(dot(LightDir,Normal),0.0);
    vec3 reflect = normalize(diff * Normal - LightDir);
    float spec = dot(reflect, ViewDir) * 4.0;  
    spec = clamp(pow(clamp(spec,0.0,1.0),4.0),0.0,1.0);
 
    gl_FragColor = mix(reflectTex, refTex * VCol, facing);
    gl_FragColor += spec * VCol; 
}
