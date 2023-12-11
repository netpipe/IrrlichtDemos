// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform sampler2D texture0; //sceneBuffer
uniform sampler2D texture1; //noiseTex
uniform float ElapsedTime;
uniform float Speed1;
uniform float Speed2;
uniform float ScratchIntensity;
uniform float IS;

void main ()
{
    float scanLine = (ElapsedTime*Speed1);
    float side = (ElapsedTime*Speed2);
    vec4 texCol = texture2D(texture0, gl_TexCoord[0].xy);
    vec2 noise = vec2(gl_TexCoord[0].x+side, scanLine);
    float scratch = texture2D(texture1, noise).x;
    scratch = 2.0f*(scratch - ScratchIntensity)/IS;
    scratch = 1.0-abs(1.0-scratch);
    scratch = max(0.0, scratch);
    gl_FragColor = texCol+vec4(scratch.xxx, 0.0);
}