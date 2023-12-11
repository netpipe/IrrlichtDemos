uniform sampler2D ColorMapSampler;

void main() 
{
	float alpha = texture2D(ColorMapSampler, gl_TexCoord[1].xy).a;

    gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
}