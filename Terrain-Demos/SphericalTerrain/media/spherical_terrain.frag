uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D alphaDetailMap;
uniform float texScaleFactor;

varying float normalToCam;
varying vec4 ambientColor;
varying vec3 outNormal;
varying vec4 outWorldPos;

const float rimStart = 0.15f;
const float rimEnd = 1.0f;
const float rimMultiplier = 0.65f;
vec3  rimColor = vec3(1.0f, 1.0f, 1.0f);
	
void main (void)
{
	vec3 weight = abs(outNormal);
	weight /= (weight.x+weight.y+weight.z);
	
	vec4 wPos = outWorldPos * texScaleFactor;
	
    vec4 col1XY = texture2D(texture1, wPos.xy);
	vec4 col1XZ = texture2D(texture1, wPos.xz);
	vec4 col1YZ = texture2D(texture1, wPos.yz);
	
    vec4 col2XY = texture2D(texture2, wPos.xy);
	vec4 col2XZ = texture2D(texture2, wPos.xz);
	vec4 col2YZ = texture2D(texture2, wPos.yz);

    vec4 col3XY = texture2D(texture3, wPos.xy);
	vec4 col3XZ = texture2D(texture3, wPos.xz);
	vec4 col3YZ = texture2D(texture3, wPos.yz);
	
	vec4 colAlphaDetailMap = texture2D(alphaDetailMap, gl_TexCoord[0].xy);

	if (colAlphaDetailMap.a < 0.5) discard;
	
	vec4 col = col1XY * weight.z + col1XZ * weight.y + col1YZ * weight.x;
	col.rgb *= colAlphaDetailMap.r;
	
	vec4 col2 = col2XY * weight.z + col2XZ * weight.y + col2YZ * weight.x;
	col2.rgb *= colAlphaDetailMap.g;
	
	vec4 col3 = col3XY * weight.z + col3XZ * weight.y + col3YZ * weight.x;
	col3.rgb *= colAlphaDetailMap.b;
	
	col += col2;
	col += col3;
	
	float rim = smoothstep(rimStart, rimEnd, normalToCam) * rimMultiplier;
	col += vec4(rimColor * rim, 0.0);

	col *= ambientColor + gl_Color;
	
    gl_FragColor = col;
}
