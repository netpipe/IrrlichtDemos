//Triplanar coordinate maps with normal maps support DirectX 9c version
//by Santiago A. Navascues

//Use at your own risk. I make no warranties. 
//If you want to use it, give the proper credits

float4x4 matWorldViewProjection;
float4x4 matWorld;
float tileSize;
float3 viewPos;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0; 
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float  Fog		:       FOG0;
   float2 TexcoordX :       TEXCOORD0;
   float2 TexcoordY :       TEXCOORD1;
   float2 TexcoordZ :       TEXCOORD2;
   float3 viewDirection   : TEXCOORD3;   
   float3x3 tangentSpace :  TEXCOORD4;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position         = mul( Input.Position, matWorldViewProjection );
   float3 realPos          = mul( Input.Position,matWorld).xyz;//So we obtain the world position
   
   Output.viewDirection = realPos-viewPos;
   
   Output.TexcoordX         = realPos.zy/tileSize;//here are our texture coordinates...
   Output.TexcoordY         = realPos.xz/tileSize;
   Output.TexcoordZ         = realPos.xy/tileSize;
   
   float3 worldNormal = normalize(Input.Normal);
   float3 n = worldNormal;
   n*=n;
   
   //binormal and tangent become normal dependant
   
   float3 xtan,ytan,ztan;
   float3 xbin,ybin,zbin;
   
   xtan = float3(0,0,1);//tangent space for the X aligned plane
   xbin = float3(0,1,0);
   
   ytan = float3(1,0,0);//tangent space for the Y aligned plane
   ybin = float3(0,0,1);
   
   ztan = float3(1,0,0);//tangent space for the Z aligned plane
   zbin = float3(0,1,0);
   
   float3 worldBinormal = normalize(xbin*n.x+ybin*n.y+zbin*n.z);//Average Binormal
   float3 worldTangent = normalize(xtan*n.x+ytan*n.y+ztan*n.z);//Average Tangent
   
   //This is done so the object can be later rotated
   
   worldNormal = mul(matWorld,worldNormal);
   worldBinormal = mul(matWorld,worldBinormal);
   worldTangent = mul(matWorld,worldTangent);
   
   Output.tangentSpace[0]   = worldTangent;
   Output.tangentSpace[1]   = worldBinormal; 
   Output.tangentSpace[2]   = worldNormal;

   Output.Fog = Output.Position.z/Output.Position.w;
      
   return( Output );
   
}


