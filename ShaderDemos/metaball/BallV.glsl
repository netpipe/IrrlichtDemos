uniform mat4 mWorld;
uniform vec4 CamPos;
uniform vec3 Ball1, Ball2, Ball3;
varying vec4 GlassPos;
varying vec3 WorldView;
varying vec3 LightDir, VNormal;
varying vec4 VCol;
varying vec3 ViewNormal;

#define Ball1Col vec4(1,0,0,1)
#define Ball2Col vec4(0,1,0,1)
#define Ball3Col vec4(0,0,1,1)
#define Threshold 0.8

float valueFunc(vec3 position, vec3 center)
{
    float Value = Threshold;

    if(position != center)
    {
        vec3 diffVec = position - center;
        Value = 1.0 / dot(diffVec,diffVec);
    }

    return Value;
}

float getValue(vec3 position)
{
	return valueFunc(position, Ball1.xyz) + valueFunc(position, Ball2.xyz) + valueFunc(position, Ball3.xyz);
}

void main()
{
	GlassPos = ftransform();
	vec3 VPos = gl_Vertex.xyz;

	float Ball1Value = valueFunc(VPos, Ball1.xyz);
	float Ball2Value = valueFunc(VPos, Ball2.xyz);
	float Ball3Value = valueFunc(VPos, Ball3.xyz);
	float ValueTotal = Ball1Value + Ball2Value + Ball3Value;

	vec3 Norm = normalize((	normalize(VPos - Ball1.xyz) * Ball1Value
						+ 	normalize(VPos - Ball2.xyz) * Ball2Value		
						+ 	normalize(VPos - Ball3.xyz) * Ball3Value) / ValueTotal);
						
	VNormal = normalize(vec3(mWorld * vec4(Norm.xyz,0.0)).xyz);
	ViewNormal = normalize(gl_NormalMatrix * Norm);
	
	VCol = Ball1Value * Ball1Col;
	VCol += Ball2Value * Ball2Col;
	VCol += Ball3Value * Ball3Col;
	VCol /= ValueTotal;

	vec3 LightPos = vec3(10,10,10);
	LightDir = normalize(LightPos);
	vec3 PosW = (mWorld * gl_Vertex).xyz;
	WorldView = CamPos.xyz - PosW;

	gl_Position = GlassPos;
}
