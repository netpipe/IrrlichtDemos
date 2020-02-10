
varying vec2 texCoord02D;

void main(void)
{
    gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);
    
    texCoord02D = vec2(gl_MultiTexCoord0);
}

