	#Input
	ATTRIB inTexCoord = fragment.texcoord[0];   
	ATTRIB light1Vector = fragment.texcoord[1]; 
	ATTRIB light2Vector = fragment.texcoord[2];    
	ATTRIB light1Color = fragment.color.primary;   
	ATTRIB light2Color = fragment.color.secondary; 
	
	#Output
	OUTPUT outColor = result.color;
	TEMP temp;
	TEMP temp2;
	TEMP colorMapColor;
	TEMP normalMapColor;
	
	# fetch color and normal map; 
	TXP colorMapColor, inTexCoord, texture[0], 2D; 
	TXP normalMapColor, inTexCoord, texture[1], 2D; 
	
	# calculate color of light1; 
	MAD normalMapColor, normalMapColor, {2,2,2,2}, {-1,-1,-1,-1}; 
	MAD temp, light1Vector, {2,2,2,2}, {-1,-1,-1,-1}; 
	DP3_SAT temp, normalMapColor, temp; 
	MUL temp, light1Color, temp; 
	
	# calculate color of light2; 
	MAD temp2, light2Vector, {2,2,2,2}, {-1,-1,-1,-1}; 
	DP3_SAT temp2, normalMapColor, temp2; 
	MAD temp, light2Color, temp2, temp; 
	
	# luminance * base color; 
	MUL outColor, temp, colorMapColor; 
	MOV outColor.a, light1Color.a; #write interpolated vertex alpha value
	
	END