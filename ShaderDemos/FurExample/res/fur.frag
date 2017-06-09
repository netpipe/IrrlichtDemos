uniform float fFurLength;
uniform float fMaxLength;
uniform float fUVScale;
uniform float fForce;
uniform vec3  vGravity;
uniform vec4  vAmbient;
uniform float fLayer; // 0 to 1 for the level

uniform vec3 vLightDir; 

//textures
uniform sampler2D FurTexture;
uniform sampler2D ColourTexture;

varying vec3 normal;

void main()
{
	vec4 diffuseTexture = texture2D( FurTexture,  vec2(gl_TexCoord[0]) ); // Fur Texture - alpha is VERY IMPORTANT!
	vec4 ColourTexture  = texture2D( ColourTexture, vec2(gl_TexCoord[1]) ); // Colour Texture for patterns
  
    //return diffuseTexture; // Return just the fur colour (red in this demo)
    //return (ColourTexture * (1,1,1,diffuseTexture.a));//Return texture colour looking like fur!
    
	vec4 FinalColour = ColourTexture;
	FinalColour.a = diffuseTexture.r;
	
	//--------------------------
	//Basic Directional Lighting
	vec4 ambient = vAmbient * FinalColour;
	vec4 diffuse = FinalColour;
	//FinalColour = ambient + diffuse * (dot(vLightDir, normal) * 0.5);
      FinalColour = ambient + diffuse;
	FinalColour = FinalColour - (fFurLength*0.35);
	//End Basic Lighting Code
	//--------------------------
	
	if(fFurLength==0.0)
	{
	    FinalColour.a = 1.0;
	}
	else
		{
	    FinalColour.a  = diffuseTexture.r * ( (fMaxLength-fFurLength));
            
	}
	
	
	//return diffuseTexture;  // fur colour only!
	gl_FragColor = FinalColour; // Use texture colour

}
