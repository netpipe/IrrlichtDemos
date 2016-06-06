//
// Structure definitions
//

struct VS_OUTPUT {
    vec4 Position;
    vec2 BumpMapTexCoord;
    vec4 RefractionMapTexCoord;
    vec4 ReflectionMapTexCoord;
    vec3 Position3D;
};

struct VS_INPUT {
    vec4 Position;
    vec4 Color;
    vec2 TexCoord0;
};


//
// Global variable definitions
//

uniform float Time;
uniform mat4 View;
uniform float WaveLength;
uniform vec2 WindDirection;
uniform float WindForce;
uniform mat4 WorldReflectionViewProj;
uniform mat4 WorldViewProj;

//
// Function declarations
//

VS_OUTPUT xlat_main( in VS_INPUT Input );

//
// Function definitions
//

VS_OUTPUT xlat_main( in VS_INPUT Input ) {
    VS_OUTPUT Output;

    Output.Position = ( WorldViewProj * Input.Position );
    Output.BumpMapTexCoord = ((Input.TexCoord0 / WaveLength) + ((Time * WindForce) * WindDirection));
    Output.RefractionMapTexCoord = ( WorldViewProj * Input.Position );
    Output.ReflectionMapTexCoord = ( WorldReflectionViewProj * Input.Position);
    Output.Position3D = vec3( ( View * Input.Position ));
    return Output;
}


//
// Translator's entry point
//
void main() {
    VS_OUTPUT xlat_retVal;
    VS_INPUT xlat_temp_Input;
    xlat_temp_Input.Position = vec4( gl_Vertex);
    xlat_temp_Input.Color = vec4( gl_Color);
    xlat_temp_Input.TexCoord0 = vec2( gl_MultiTexCoord0);

    xlat_retVal = xlat_main( xlat_temp_Input);

    gl_Position = vec4( xlat_retVal.Position);
    gl_TexCoord[0] = vec4( xlat_retVal.BumpMapTexCoord, 0.0, 0.0);
    gl_TexCoord[1] = vec4( xlat_retVal.RefractionMapTexCoord);
    gl_TexCoord[2] = vec4( xlat_retVal.ReflectionMapTexCoord);
    gl_TexCoord[3] = vec4( xlat_retVal.Position3D, 0.0);
}


