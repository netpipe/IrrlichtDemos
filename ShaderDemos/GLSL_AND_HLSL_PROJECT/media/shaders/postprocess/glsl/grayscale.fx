// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform sampler2D texture0;
uniform float BaseGray;

void main()
{
	vec3 texColor=texture2D(texture0, gl_TexCoord[0].xy).rgb;
	
	// digital ITU R 709: Y = 0.2126 R + 0.7152 G + 0.0722 B 
	// digital ITU R 601: Y = 0.2990 R + 0.5870 G + 0.1140 B
    float gray = BaseGray+dot(texColor, vec3(0.2990, 0.5870, 0.1140));
	gl_FragColor=vec4(gray.xxx, 1.0);;
}