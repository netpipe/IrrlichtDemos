
//Global variables (HLSL)
float4x4    mWorldViewProj; // World View Projection Matrix
float4x4    mWorld; // World matrix

float YRotation;//used for light calculations


//################################ Vertex Shader #########################################

void main_v(
        float4      Position   : POSITION,
        float3      Normal      : NORMAL,
        float2 UV0		: TEXCOORD0,

        out   float4      oPosition    : POSITION,
        out   float2      oUVBase      : TEXCOORD0,
        out   float3      oNormal      : TEXCOORD1,
        out   float3      oNormal2     : TEXCOORD2,
        out   float3      oUV          : TEXCOORD3,
		out   float3 	  oObjPosition : TEXCOORD4
		)
{
    float3 UV = mul(mWorld, Position).xyz;
    oUVBase=UV0;
	
		oObjPosition=Position;
		oPosition = mul(Position,mWorldViewProj);//other way around for HLSL !!!

    oUV = UV*0.0001; // frequency of texture sampling
    oNormal2=Normal;

	//sunlight:
	float3 ln;

	ln.x=(cos(YRotation)*Normal.x)-(sin(YRotation)*Normal.z);
	ln.z=(sin(YRotation)*Normal.x)+(cos(YRotation)*Normal.z);
	ln.y=Normal.y;

	oNormal=ln;



}







//################################## Functions ###########################################

float4 GetMixedColor(uniform	sampler2D   Color00, float tile, float3 UVW, float3 blend_weights)
{
 float4 color1 = tex2D(Color00, UVW.xy * tile);
 float4 color2 = tex2D(Color00, UVW.yz * tile);
 float4 color3 = tex2D(Color00, UVW.xz * tile);
 
 float4      oColor;
 oColor.xyz = color1.xyz*blend_weights.z + color2.xyz*blend_weights.x + color3.xyz*blend_weights.y;
 oColor.w = 1;
 return(oColor);
}





//########################################################################################
//                       Fragment Shader parameters
//########################################################################################

sampler2D   Selector0   : register(s0);
sampler2D   Selector1   : register(s1);

sampler2D   Color00     : register(s2);
sampler2D   Color01     : register(s3);
sampler2D   Color02     : register(s4);
sampler2D   Color03     : register(s5);
sampler2D   Color04     : register(s6);
sampler2D   Color05     : register(s7);
// sampler2D   Color06     : register(s8);

float3 eyePosition;

float fogstartdistance;
float fogenddistance;
float3 dirlight;//parellell light (the sun)
float3 ambientcolor;
float3 sunlightcolor;//sun color
float3 fogcolor;

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF Fragment Shader FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

void main_f(
    float2      UVBase       : TEXCOORD0,
    float3 LightNormal : TEXCOORD1,
    float3 Normal : TEXCOORD2,
    float3 UV : TEXCOORD3,
    float4      Position     : TEXCOORD4,
    out float4      oColor       : COLOR
		)
{

 //FOG
 float len=length(float4(eyePosition,1)-Position)-fogstartdistance;

 float fog=saturate(len/fogenddistance);


 //if fog == 100% then lets skip all the complicated calculations and bail out with Output=FogColor
 if(fog==1.0)
 {
	 oColor.rgb=fogcolor;
	 oColor.a=1;
	 return;
 }



 //Alphamap selectors:
 float4 nSelector0 = tex2D (Selector0, UVBase).xyzw;
 float4 nSelector1 = tex2D (Selector1, UVBase).xyzw;

 //we need a multiple of 1 to make seamless terrains:

 float3 nColor00 = tex2D (Color00, UVBase * 70.0f ).xyz;
 float3 nColor01 = tex2D (Color01, UVBase * 70.0f ).xyz;
 float3 nColor02 = tex2D (Color02, UVBase * 70.0f ).xyz;
 float3 nColor03 = tex2D (Color03, UVBase * 70.0f ).xyz;
 float3 nColor04 = tex2D (Color04, UVBase * 70.0f ).xyz;
 float3 nColor05 = tex2D (Color05, UVBase * 70.0f ).xyz;
 //float3 nColor06 = tex2D (Color06, UVBase * 70.0f ).xyz;


 //## mix up colors:
 float3 tmpcolor;
 
 
 
  // NVIDIA Cascades paper
 float3 blend_weights = abs(Normal) - 0.2;
 blend_weights *= 7.0;
 blend_weights = pow(blend_weights, float3(23,23,23));//we might actually REM this line!
 blend_weights = max(float3(0,0,0), blend_weights);
 blend_weights /= dot(blend_weights, float3(1,1,1));


 //here we choose which ones we want to use with triplanar mapping:
 tmpcolor = GetMixedColor(Color00, 2.3, UV, blend_weights).xyz;
 //float3 ColorE1 = nSelector0.x * GetMixedColor(Color01, 2.3, UV, blend_weights) + R0 * ColorE0;
 //float3 ColorE2 = nSelector0.y * GetMixedColor(Color02, 2.3, UV, blend_weights) + G0 * ColorE1;
 //float3 ColorE3 = nSelector0.z * GetMixedColor(Color03, 2.3, UV, blend_weights) + B0 * ColorE2;


 //tmpcolor = nColor00;//base color
 tmpcolor = lerp(tmpcolor,nColor01,nSelector0.x);
 tmpcolor = lerp(tmpcolor,nColor02,nSelector0.y);
 tmpcolor = lerp(tmpcolor,nColor03,nSelector0.z);
 tmpcolor = lerp(tmpcolor,nColor04,nSelector0.w);
 tmpcolor = lerp(tmpcolor,nColor05,nSelector1.x);


 //#Lightmap:
 //float3 lightmapcolor=tmpcolor*(nColor05.xyz);//final color after lightmap


 //cheap fix for terrain: (too much light compared to objects)
 float ambientcolorreduction=0.6;


 float3 sunlight=1.0;//saturate(dot(normalize(LightNormal),dirlight)*sunlightcolor);


 float3 finalcolor=tmpcolor*(sunlight+(ambientcolor*ambientcolorreduction));

 oColor.xyz=lerp(finalcolor,fogcolor,fog);

 
 //oColor.xyz=Normal.y;
//oColor=nSelector1;

 oColor.a=1;
}






























