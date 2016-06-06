struct VS_OUTPUT 
{
    vec4 Position;
    vec4 Color;
};

uniform mat4 mWorldViewProj;

VS_OUTPUT vertexMain( in vec3 Position ) 
{
    vec4 hpos;
    VS_OUTPUT OUT;

    hpos = (mWorldViewProj * vec4( Position.x , Position.y , Position.z , 1.00000) );
    OUT.Color = hpos;
    OUT.Position = hpos;
    return OUT;
}

void main() 
{
    VS_OUTPUT xlat_retVal;

    xlat_retVal = vertexMain( vec3(gl_Vertex));

    gl_Position = vec4( xlat_retVal.Position);
    gl_TexCoord[0] = vec4( xlat_retVal.Color);
}