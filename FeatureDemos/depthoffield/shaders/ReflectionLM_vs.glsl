uniform mat4 WorldReflectionViewProj;
uniform mat4 WorldViewProj;

void main() {
    gl_Position = vec4( gl_Vertex * WorldViewProj );
    gl_TexCoord[0] = vec4( gl_MultiTexCoord0, 0.0, 0.0);
    gl_TexCoord[1] = vec4( gl_MultiTexCoord0, 0.0, 0.0);
    gl_TexCoord[2] = vec4( gl_MultiTexCoord1, 0.0, 0.0);
    gl_TexCoord[3] = vec4( gl_Vertex * WorldReflectionViewProj );
}
