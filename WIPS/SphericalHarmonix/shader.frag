// GI= simple?
//uniform vec3 SH[9];

varying vec3 normal;
varying vec3 pos;

uniform sampler2D SHatlas;
uniform sampler2D SHatlas2;
uniform sampler2D SHatlas3;
uniform sampler2D SHatlas4;

uniform vec3 LightVec;

void main()
{
	vec3 Normal = normalize(normal);/*
	vec3 col = SH[0]+SH[1]*Normal.y+SH[2]*Normal.z+SH[3]*Normal.x+SH[4]*Normal.x*Normal.y+SH[5]*Normal.y*Normal.z+SH[7]*Normal.x*Normal.z+SH[6]*0.743125*Normal.z*Normal.z-0.247708*SH[6]+SH[8]*(Normal.x*Normal.x-Normal.y*Normal.y);
	gl_FragColor = vec4(col,1.0);*/
	float idx = clamp(floor(pos.y+Normal.y*0.25),0.0,12.0);
	float idx2 = min(idx+1.0,12.0);
	float interpolant = clamp(pos.y+Normal.y*0.25-idx,0.0,1.0);
	vec2 TC1 = pos.xz+Normal.xz*0.0044;
	vec2 TC2 = TC1+vec2(mod(idx2,4.0)*0.125,floor(idx2*0.25)*0.125);
	TC1 += vec2(mod(idx,4.0)*0.125,floor(idx*0.25)*0.125);
	vec4 term1 = mix(texture2D(SHatlas,TC1),texture2D(SHatlas,TC2), interpolant);
	TC1.x += 0.5;
	TC2.x += 0.5;
	vec4 term2 = mix(texture2D(SHatlas,TC1),texture2D(SHatlas,TC2), interpolant);
	TC1.y -= 0.5;
	TC2.y -= 0.5;
	vec4 term4 = mix(texture2D(SHatlas,TC1),texture2D(SHatlas,TC2), interpolant);
	TC1.x -= 0.5;
	TC2.x -= 0.5;
	vec4 term3 = mix(texture2D(SHatlas,TC1),texture2D(SHatlas,TC2), interpolant);
	TC1.y += 0.5;
	TC2.y += 0.5;
	vec4 term5 = mix(texture2D(SHatlas2,TC1),texture2D(SHatlas2,TC2), interpolant); // ba = SH6
	TC1.x += 0.5;
	TC2.x += 0.5;
	vec4 term6 = mix(texture2D(SHatlas2,TC1),texture2D(SHatlas2,TC2), interpolant); // r = SH6
	TC1 -= vec2(0.5);
	TC2 -= vec2(0.5);
	vec4 term7 = mix(texture2D(SHatlas2,TC1),texture2D(SHatlas2,TC2), interpolant);
	vec3 col = term1.rgb+vec3(term1.a,term2.rg)*Normal.y+vec3(term2.ba,term3.r)*Normal.z+term3.gba*Normal.x+term4.rgb*Normal.x*Normal.y+vec3(term4.a,term5.rg)*Normal.y*Normal.z+term6.gba*Normal.x*Normal.z+vec3(term5.ba,term6.r)*(0.743125*Normal.z*Normal.z-0.247708)+term7.rgb*(Normal.x*Normal.x-Normal.y*Normal.y);
	TC1.y += 0.5;
	TC2.y += 0.5;
	term1 = mix(texture2D(SHatlas3,TC1),texture2D(SHatlas3,TC2), interpolant);
	TC1.x += 0.5;
	TC2.x += 0.5;
	term2 = mix(texture2D(SHatlas3,TC1),texture2D(SHatlas3,TC2), interpolant);
	TC1.y -= 0.5;
	TC2.y -= 0.5;
	term4 = mix(texture2D(SHatlas3,TC1),texture2D(SHatlas3,TC2), interpolant);
	TC1.x -= 0.5;
	TC2.x -= 0.5;
	term3 = mix(texture2D(SHatlas3,TC1),texture2D(SHatlas3,TC2), interpolant);
	TC1.y += 0.5;
	TC2.y += 0.5;
	term5 = mix(texture2D(SHatlas4,TC1),texture2D(SHatlas4,TC2), interpolant); // ba = SH6
	TC1.x += 0.5;
	TC2.x += 0.5;
	term6 = mix(texture2D(SHatlas4,TC1),texture2D(SHatlas4,TC2), interpolant); // r = SH6
	TC1 -= vec2(0.5);
	TC2 -= vec2(0.5);
	term7 = mix(texture2D(SHatlas4,TC1),texture2D(SHatlas4,TC2), interpolant);
	vec3 otherNormal = term1.rgb+vec3(term1.a,term2.rg)*Normal.y+vec3(term2.ba,term3.r)*Normal.z+term3.gba*Normal.x+term4.rgb*Normal.x*Normal.y+vec3(term4.a,term5.rg)*Normal.y*Normal.z+term6.gba*Normal.x*Normal.z+vec3(term5.ba,term6.r)*(0.743125*Normal.z*Normal.z-0.247708)+term7.rgb*(Normal.x*Normal.x-Normal.y*Normal.y);
	gl_FragColor = vec4(max(dot(reflect(-LightVec,otherNormal),Normal),0.0)*col*2.0+col+max(dot(LightVec,Normal),0.0)*0.375,1.0);
}
