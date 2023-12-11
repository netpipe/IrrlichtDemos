//Note this is the 6 textures version, hence, you will need to recompile Irrlicht if you want
//to use it.

//And can be reduced further more if you use a single normal map, and sample it from every side
//everytime, leaving space for a shadowmap, the shader does the rest.

//More good news: Only PS2 required!

float4 ambient;
float4 specular;
float4 diffuse;
float specularPower;
float3 lightDirection;

sampler2D baseX;
sampler2D baseY;
sampler2D baseZ;
sampler2D normalX;
sampler2D normalY;
sampler2D normalZ;

struct PS_INPUT 
{
   float4 Position :        POSITION0;
   float2 TexcoordX :       TEXCOORD0;
   float2 TexcoordY :       TEXCOORD1;
   float2 TexcoordZ :       TEXCOORD2;
   float3 viewDirection   : TEXCOORD3;   
   float3x3 tangentSpace :  TEXCOORD4;
};

float4 ps_main( PS_INPUT Input ) : COLOR0
{      
   float3 fvLightDirection = normalize( lightDirection );
   float3 fvNormal         = normalize( Input.tangentSpace[2]);
   float3 fvViewDirection  = normalize( Input.viewDirection );
    
   float3 n                = fvNormal;
   n*=n;
   
   float4 fvBaseColor      = tex2D( baseX, Input.TexcoordX )*n.x+
                             tex2D( baseY, Input.TexcoordY )*n.y+
                             tex2D( baseZ, Input.TexcoordZ )*n.z;

   float4 fvBaseNormal     = tex2D( normalX, Input.TexcoordX )*n.x+
                             tex2D( normalY, Input.TexcoordY )*n.y+
                             tex2D( normalZ, Input.TexcoordZ )*n.z;
                             
   float3 normal           = normalize(mul(2.0*fvBaseNormal.xyz-1.0,Input.tangentSpace));    
    
   float  fNDotL           = max(0.0,dot( normal, fvLightDirection )); 
   float3 fvReflection     = reflect(fvLightDirection,normal);
   float  fRDotV           = max(0.0, dot( fvReflection, fvViewDirection ) );
                             
   float4 fvTotalAmbient   = ambient * fvBaseColor; 
   float4 fvTotalDiffuse   = diffuse * fNDotL * fvBaseColor; 
   float4 fvTotalSpecular  = specular * pow( fRDotV, specularPower )*fvBaseColor;
   
   return( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular );
      
}


