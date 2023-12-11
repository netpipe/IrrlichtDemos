
//
// Function declarations
//

vec4 pixelMain( in vec2 UV );

//
// Function definitions
//

vec4 pixelMain(  in vec2 UV ) {
    float sv;
    float tv;

    sv = (5.80000 * pow( max( 0.000000, UV.x ), 16.0000));
    tv = max( 0.000000, UV.y );
    return vec4( (sv * mix( vec3( 1.00000, 1.00000, 1.00000), vec3( 1.00000, 0.700000, 0.500000), vec3( tv))), 1.00000);
}


//
// User varying
//
varying vec4 xlat_varying_TEXCOORD0;

//
// Translator's entry point
//
void main() {
    vec4 xlat_retVal;

    xlat_retVal = pixelMain(  vec2(xlat_varying_TEXCOORD0));

    gl_FragColor = vec4( xlat_retVal);
}


 