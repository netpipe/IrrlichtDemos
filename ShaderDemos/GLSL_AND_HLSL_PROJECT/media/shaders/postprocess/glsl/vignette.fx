uniform sampler2D texture0;
uniform float EdgeDarkness;

void main(void)
{    
    vec2 tc = gl_TexCoord[0].xy;
    vec4 finalCol = texture2D(texture0, gl_TexCoord[0].xy);
    
    tc -= 0.5;
    float vignette = 1.0 - (dot(tc, tc) * EdgeDarkness);
       
    gl_FragColor = finalCol * vignette;
}