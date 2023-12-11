// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform sampler2D texture0;
uniform float NumColors;
uniform float Gamma;

void main()
{
	vec4 texCol = texture2D(texture0, gl_TexCoord[0].xy);
	vec3 col0 = texCol.rgb;
	col0 = vec3(pow(float(col0), Gamma));
	col0 = col0*NumColors;
	col0 = floor(col0);
	col0 = col0/NumColors;
	col0 = vec3(pow(float(col0), 1.0f/Gamma));
	gl_FragColor = vec4(col0, texCol.a);
}
