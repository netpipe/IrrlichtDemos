uniform float Time;
uniform mat4 View;
uniform float WaveLength;
uniform vec2 WindDirection;
uniform float WindForce;
uniform mat4 WorldReflectionViewProj;
uniform mat4 WorldViewProj;
varying vec4 Position;

void main() {
    Position = vec4( WorldViewProj * gl_Vertex );
    gl_Position = Position;
    gl_TexCoord[0] = vec4( ((gl_MultiTexCoord0 / WaveLength) + ((Time * WindForce) * WindDirection)), 0.0, 0.0);
    gl_TexCoord[1] = vec4( WorldViewProj * gl_Vertex);
    gl_TexCoord[2] = vec4( WorldReflectionViewProj * gl_Vertex);
    gl_TexCoord[3] = vec4( View * gl_Vertex);
}


