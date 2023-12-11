float fFurLength;
float fMaxLength;
float fUVScale;
float fLayer; // 0 to 1 for the level
float3 vGravity;
float4 vLightDir;
float4 vAmbient;
//------------------------------------
sampler2D tFurTexture;
sampler2D tColourTexture;
//------------------------------------ 
// transformations
float4x4 worldViewProj : WORLDVIEWPROJ;
float4x4 matWorld : WORLD;

float4x4 mWorldViewProj; // World * View * Projection transformation
float4x4 mWorld;      // Inverted world matrix
//------------------------------------
struct vertexInput {
    float3 position                 : POSITION;
    float3 normal                   : NORMAL;
    float2 texCoordDiffuse          : TEXCOORD0;
};

struct vertexOutput {
    float4 HPOS     : POSITION;   
    float2 T0       : TEXCOORD0; // fur alpha
	float2 T1       : TEXCOORD1; // fur alpha
    float3 vNormal  : TEXCOORD2;
};

vertexOutput vertexMain(vertexInput IN)
{
	vertexOutput OUT = (vertexOutput)0;
	//** MAIN LINE ** MAIN LINE ** MAIN LINE ** MAIN LINE ** MAIN LINE **//
	//This single line is responsible for creating the layers!  This is it! Nothing
	//more nothing less!
	//float3 P = IN.position.xyz + (IN.normal * fFurLength);
	float3 P = IN.position.xyz + (IN.normal.xyz * ((fFurLength)*10.0f));

	//Modify our normal so it faces the correct direction for lighting if we
	//want any lighting
	//float3 normal = normalize(mul(float4(IN.normal,1.0), matWorld));
	float3 normal = normalize(mul(float4(IN.normal,1.0), mWorld));

	// Couple of lines to give a swaying effect!
	// Additional Gravit/Force Code
	float3 ovGravity = mul(float4(vGravity, 1.0), matWorld);
	//float3 ovGravity = mul(float4(vGravity, 1.0), mWorld);
	float k =  pow(fLayer, 3.0);  // We use the pow function, so that only the tips of the hairs bend
							 // As layer goes from 0 to 1, so by using pow(..) function is still
							 // goes form 0 to 1, but it increases faster! exponentially
	P = P + ovGravity*k;
	// End Gravity Force Addit Code
	OUT.T0 = IN.texCoordDiffuse * fUVScale; // Pass long texture data
	OUT.T1 = IN.texCoordDiffuse;
	// UVScale??  Well we scale the fur texture alpha coords so this effects the fur thickness
	// thinness, sort of stretches or shrinks the fur over the object!
	//OUT.HPOS = mul(float4(P, 1.0f), worldViewProj); // Output Vertice Position Data
	OUT.HPOS = mul(float4(P, 1.0f), mWorldViewProj); // Output Vertice Position Data
	OUT.vNormal = normal; // Output Normal
	return OUT;
}

//----------------------------------- ( ps 1.3 )
float4 pixelMain( vertexOutput IN): COLOR
{
	float4 diffuseTexture = tex2D( tFurTexture,  IN.T0 ); // Fur Texture - alpha is VERY IMPORTANT!
	float4 ColourTexture = tex2D( tColourTexture,  IN.T0 ); // Colour Texture for patterns
	float4 FinalColour = ColourTexture;
	//FinalColour = diffuseTexture;
	//--------------------------
	//Basic Directional Lighting
	float4 ambient = vAmbient * FinalColour;
	float4 diffuse = FinalColour;
	FinalColour = ambient + diffuse * dot(vLightDir, IN.vNormal);
	//FinalColour = ambient + diffuse;
	//End Basic Lighting Code
	//--------------------------
	if(fFurLength==0.0f)
	{
	    FinalColour.a = 1;
	}
	else
		{
	    //FinalColour.a  = diffuseTexture.a * ( (fMaxLength-fFurLength));
		FinalColour.a = 1-diffuseTexture.a; //fix 1-
		//FinalColour.a  = 1- diffuseTexture.a * ( (1.0f-fFurLength)); 
	}
	return FinalColour;      // fur colour only!
}