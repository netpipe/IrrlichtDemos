#version 130
uniform sampler2D blurred_textures;
varying vec2 outUV;

const mediump float harrisConstant = 0.04;
void main()
{
mediump vec3 derivativeElements = texture2D(blurred_textures, outUV).rgb;
mediump float derivativeSum = derivativeElements.x + derivativeElements.y;
mediump float harrisIntensity = (derivativeElements.x * derivativeElements.y - (derivativeElements.z * derivativeElements.z)) / (derivativeSum);
gl_FragColor = vec4(vec3(harrisIntensity * 10.0), 1.0);
}
