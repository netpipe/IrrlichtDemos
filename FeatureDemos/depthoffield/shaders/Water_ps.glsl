//
// Global variable definitions
//

uniform vec3 CameraPosition;
uniform float ColorBlendFactor;
uniform sampler2D ReflectionMap;
uniform sampler2D RefractionMap;
uniform sampler2D WaterBump;
uniform vec4 WaterColor;
uniform float WaveHeight;
uniform float reflectivity;
uniform float MaxD;
varying vec4 Position;

//
// Translator's entry point
//
void main() {
    vec2 perturbation = vec2(WaveHeight * (texture2D( WaterBump, gl_TexCoord[0]).xy  - 0.500000));
    vec4 refractiveColor = texture2D( RefractionMap, (vec2((((gl_TexCoord[1].x  / gl_TexCoord[1].w ) / 2.00000) + 0.500000), (((( gl_TexCoord[1].y  ) / gl_TexCoord[1].w ) / 2.00000) + 0.500000)) + perturbation));
    vec4 reflectiveColor = texture2D( ReflectionMap, (vec2( (((gl_TexCoord[2].x  / gl_TexCoord[2].w ) / 2.00000) + 0.500000), (((( gl_TexCoord[2].y  ) / gl_TexCoord[2].w ) / 2.00000) + 0.500000)) + perturbation));
    float depth = Position.z /gl_DepthRange.far;
    float mulDepth = depth * 256.0 + 2.0;
    float flooredDepth = floor(mulDepth);

    gl_FragData[0] = (ColorBlendFactor * WaterColor) + (1.00000 - ColorBlendFactor) * mix( refractiveColor, reflectiveColor, vec4(reflectivity));
}


