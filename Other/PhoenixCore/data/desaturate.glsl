#version 120

uniform sampler2D tex;
uniform vec4 tint = vec4(1,1,1,1);

void main (void)
{
	vec4 texel = texture2D(tex,gl_TexCoord[0].st);
	vec4 color = texel * gl_Color;
	
	// desaturate, luminosity forumla
	float lum = (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);
	vec4 basecolor = vec4( lum*tint.r,lum*tint.g,lum*tint.b,color.a );

	vec4 accum;
	float amount = 0.006;
	float exposure = 1.2;
	
		vec4 color0 = texture2D( tex, gl_TexCoord[0].st-vec2(-amount,0) );	
		vec4 color1 = texture2D( tex, gl_TexCoord[0].st-vec2(amount,0) );	
		vec4 color2 = texture2D( tex, gl_TexCoord[0].st-vec2(0,-amount) );	
		vec4 color3 = texture2D( tex, gl_TexCoord[0].st-vec2(0,+amount) );	
	
		//blur
	accum = (texel + color0 + color1 + color2 + color3) / 5;
	
		//make it black and white 
	float lum1 = (0.299 * accum.r) + (0.587 * accum.g) + (0.114 * accum.b);
		//This is bloomed blur 
	color = vec4( lum1*(basecolor.r*exposure),lum1*(basecolor.g*exposure),lum1*(basecolor.b*exposure),color.a );
		//This is normal blur 
	//color = vec4( lum1*tint.r,lum1*tint.g,lum1*tint.b,color.a );
	
    gl_FragColor = color;
}

