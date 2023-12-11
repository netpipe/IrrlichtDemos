// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform sampler2D texture0; //sceneBuffer
uniform float NumTilesX;
uniform float NumTilesY;
uniform float Threshhold;

void main ()
{
	vec3 edgeCol=vec3(0.6, 0.6, 0.6);

	vec2 texSize = vec2(1.0/NumTilesX, 1.0/NumTilesY);
	vec2 baseCoord = gl_TexCoord[0].xy-mod(gl_TexCoord[0].xy, texSize);
    vec2 centerCoord = baseCoord+texSize/2.0;
    vec2 stCoord = (gl_TexCoord[0].xy-baseCoord)/texSize;
    vec4 col0 = vec4(1.0-edgeCol ,1.0);
    float threshholdB = 1.0-Threshhold;
    
    vec4 col1 = vec4(0.0);
    vec4 col2 = vec4(0.0);
    if (stCoord.x > stCoord.y) 
    { 
		col1 = col0; 
	}
    if (stCoord.x > threshholdB) 
    { 
		col2 = col1; 
    }
    if (stCoord.y > threshholdB) 
    { 
		col2 = col1; 
    }
    vec4 bottomCol = col2;
    
    col1 = vec4(0.0);
    col2 = vec4(0.0);
    if (stCoord.x > stCoord.y) 
    { 
		col1 = col0; 
    }
    if (stCoord.x < Threshhold) 
    { 
		col2 = col1; 
    }
    if (stCoord.y < Threshhold) 
    { 
		col2 = col1; 
    }
    vec4 topCol = col2;
    
    vec4 tileColor = texture2D(texture0, centerCoord);
    vec4 finalColor = tileColor+topCol-bottomCol;
    gl_FragColor = finalColor;
}
