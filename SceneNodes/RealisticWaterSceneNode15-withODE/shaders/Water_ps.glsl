//
// Structure definitions
//

struct PS_OUTPUT {
    vec4 Color;
};

struct PS_INPUT {
    vec4 Position;
    vec2 BumpMapTexCoord;
    vec4 RefractionMapTexCoord;
    vec4 ReflectionMapTexCoord;
    vec3 Position3D;
};


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

//
// Function declarations
//

PS_OUTPUT xlat_main( in PS_INPUT Input );

//
// Function definitions
//

PS_OUTPUT xlat_main( in PS_INPUT Input ) {
    vec4 bumpColor;
    vec2 perturbation;
    vec2 ProjectedRefractionTexCoords;
    vec4 refractiveColor;
    vec2 ProjectedReflectionTexCoords;
    vec4 reflectiveColor;
    vec3 eyeVector;
    vec3 normalVector = vec3( 0.000000, 1.00000, 0.000000);
    float fresnelTerm;
    vec4 combinedColor;
    PS_OUTPUT Output;

    bumpColor = texture2D( WaterBump, Input.BumpMapTexCoord);
    perturbation = (WaveHeight * (bumpColor.xy  - 0.500000));
    ProjectedRefractionTexCoords.x  = (((Input.RefractionMapTexCoord.x  / Input.RefractionMapTexCoord.w ) / 2.00000) + 0.500000);
    ProjectedRefractionTexCoords.y  = (((( Input.RefractionMapTexCoord.y  ) / Input.RefractionMapTexCoord.w ) / 2.00000) + 0.500000);
    refractiveColor = texture2D( RefractionMap, (ProjectedRefractionTexCoords + perturbation));
    ProjectedReflectionTexCoords.x  = (((Input.ReflectionMapTexCoord.x  / Input.ReflectionMapTexCoord.w ) / 2.00000) + 0.500000);
    ProjectedReflectionTexCoords.y  = (((( Input.ReflectionMapTexCoord.y  ) / Input.ReflectionMapTexCoord.w ) / 2.00000) + 0.500000);
    reflectiveColor = texture2D( ReflectionMap, (ProjectedReflectionTexCoords + perturbation));
    eyeVector = normalize( (CameraPosition - Input.Position3D) );
    fresnelTerm = max( dot( eyeVector, normalVector), 0.000000);
    combinedColor = mix( refractiveColor, (reflectiveColor * (1.00000 - fresnelTerm)), vec4( 0.250000));
    Output.Color = ((ColorBlendFactor * WaterColor) + ((1.00000 - ColorBlendFactor) * combinedColor));
    return Output;
}


//
// Translator's entry point
//
void main() {
    PS_OUTPUT xlat_retVal;
    PS_INPUT xlat_temp_Input;
    xlat_temp_Input.Position = vec4( gl_FragCoord);
    xlat_temp_Input.BumpMapTexCoord = vec2( gl_TexCoord[0]);
    xlat_temp_Input.RefractionMapTexCoord = vec4( gl_TexCoord[1]);
    xlat_temp_Input.ReflectionMapTexCoord = vec4( gl_TexCoord[2]);
    xlat_temp_Input.Position3D = vec3( gl_TexCoord[3]);

    xlat_retVal = xlat_main( xlat_temp_Input);

    gl_FragData[0] = vec4( xlat_retVal.Color);
}


