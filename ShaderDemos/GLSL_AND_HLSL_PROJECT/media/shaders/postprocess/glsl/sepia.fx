// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform sampler2D texture0;
uniform float Toning;
uniform float Desaturation;

void main()
{
	vec3 PaperTone=vec3(1,0.9,0.6);
	vec3 StainTone=vec3(0.2,0.05,0);
	
	vec3 texColor = PaperTone * texture2D(texture0, gl_TexCoord[0].xy).rgb;
	
	// digital ITU R 709: Y = 0.2126 R + 0.7152 G + 0.0722 B 
	// digital ITU R 601: Y = 0.2990 R + 0.5870 G + 0.1140 B
    float gray = dot(texColor, vec3(0.2990, 0.5870, 0.1140));
    vec3 muted = mix(texColor, gray.xxx, Desaturation);
    vec3 sepia = mix(StainTone, PaperTone, gray);
    vec3 result = mix(muted, sepia, Toning);
    
    gl_FragColor = vec4(result,1.0);
}
