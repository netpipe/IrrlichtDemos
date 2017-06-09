uniform float time;
uniform float cx;
uniform float cy;
uniform float iter;

void main()
{
    vec2 z, c;
    c.x= cx;
    c.y= cy;
    z.x= (gl_TexCoord[0].x - 0.5) * 5.0;//sin(time);
    z.y= (gl_TexCoord[0].y - 0.5) * 5.0;//cos(time);

    int i;
    for(i= 0; i < iter; i++)
    {
        float x= (z.x * z.x - z.y * z.y) + c.x;
        float y= (2* z.x * z.y) + c.y;

        if((x*x + y*y) > 4.0) break;
        z.x= x;
        z.y= y;
    }

    float col= float(i) / float(iter);
    if(i == iter) col= 0.0;

    gl_FragColor = vec4(col/5.0, col, 0.0, 1.0);
}
