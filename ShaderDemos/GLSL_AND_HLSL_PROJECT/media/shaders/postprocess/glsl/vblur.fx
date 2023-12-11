uniform sampler2D texture0; // this should hold the texture rendered by the horizontal blur pass
uniform float BufferHeight;
 
void main()
{
	float blurSize = 1.0/BufferHeight; 
	vec4 finalCol = vec4(0.0);

	// gauss distribution with mean:0 std:2
	float weight[9];
	weight[0] = 0.0295;
	weight[1] = 0.0673;
	weight[2] =	0.1235;
	weight[3] =	0.1786;
	weight[4] =	0.2022;
	weight[5] = 0.1786;
	weight[6] = 0.1235;
	weight[7] =	0.0673;
	weight[8] =	0.0295;


	// blur in y (vertical)
	// take the samples with the distance blurSize between them
	for (int i=0; i<9; i++)
		finalCol += texture2D(texture0, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y+(i-4)*blurSize))*weight[i];

	gl_FragColor = finalCol;
}

void main7x7()
{
	float blurSize = 1.0/BufferHeight; 
	vec4 finalCol = vec4(0.0);

	// gauss distribution with mean:0 std:1
	float weight[7];
	weight[0]= 0.0050;
	weight[1]= 0.0540;
	weight[2]= 0.2410;
	weight[3]= 0.4000;
	weight[4]= 0.2410;
	weight[5]= 0.0540;
	weight[6]= 0.0050;

	// blur in y (vertical)
	// take the samples with the distance blurSize between them
	for (int i=0; i<7; i++)
		finalCol += texture2D(texture0, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y+(i-3)*blurSize))*weight[i];

	gl_FragColor = finalCol;
}
