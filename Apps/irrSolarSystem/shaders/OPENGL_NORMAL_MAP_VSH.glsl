	#input
	# 0-3: transposed world matrix;
	#;12: Light01 position 
	#;13: x,y,z: Light01 color; .w: 1/LightRadius^2 
	#;14: Light02 position 
	#;15: x,y,z: Light02 color; .w: 1/LightRadius^2 
	
	ATTRIB InPos = vertex.position;
	ATTRIB InColor = vertex.color;
	ATTRIB InNormal = vertex.normal;
	ATTRIB InTexCoord = vertex.texcoord[0];
	ATTRIB InTangent = vertex.texcoord[1];
	ATTRIB InBinormal = vertex.texcoord[2];
	
	#output
	OUTPUT OutPos = result.position;
	OUTPUT OutLightColor1 = result.color.primary;
	OUTPUT OutLightColor2 = result.color.secondary;
	OUTPUT OutTexCoord = result.texcoord[0];
	OUTPUT OutLightVector1 = result.texcoord[1];
	OUTPUT OutLightVector2 = result.texcoord[2];
	
	PARAM MVP[4] = { state.matrix.mvp }; # modelViewProjection matrix.
	TEMP Temp;
	TEMP TempColor;
	TEMP TempNormal;
	TEMP TempTangent;
	TEMP TempBinormal;
	TEMP TempLightVector1;
	TEMP TempLightVector2;
	TEMP TempTransLightV1;
	TEMP TempTransLightV2;
	
	# transform position to clip space 
	DP4 OutPos.x, MVP[0], InPos;
	DP4 OutPos.y, MVP[1], InPos;
	DP4 OutPos.z, MVP[2], InPos;
	DP4 OutPos.w, MVP[3], InPos;
	
	# transform normal 
	DP3 TempNormal.x, InNormal.x, program.local[0];
	DP3 TempNormal.y, InNormal.y, program.local[1]; 
	DP3 TempNormal.z, InNormal.z, program.local[2];
	
	# transform tangent 
	DP3 TempTangent.x, InTangent.x, program.local[0];
	DP3 TempTangent.y, InTangent.y, program.local[1]; 
	DP3 TempTangent.z, InTangent.z, program.local[2];
	
	# transform binormal 
	DP3 TempBinormal.x, InBinormal.x, program.local[0];
	DP3 TempBinormal.y, InBinormal.y, program.local[1]; 
	DP3 TempBinormal.z, InBinormal.z, program.local[2];
	
	# vertex into world position 
	DP4 Temp.x, InPos, program.local[0];
	DP4 Temp.y, InPos, program.local[1];
	DP4 Temp.z, InPos, program.local[2];
	DP4 Temp.w, InPos, program.local[3];
	
	# vertex - lightpositions 
	SUB TempLightVector1, program.local[12], Temp; 
	SUB TempLightVector2, program.local[14], Temp; 
	
	# transform the light vector 1 with U, V, W 
	DP3 TempTransLightV1.x, TempTangent, TempLightVector1; 
	DP3 TempTransLightV1.y, TempBinormal, TempLightVector1; 
	DP3 TempTransLightV1.z, TempNormal, TempLightVector1; 
	
	# transform the light vector 2 with U, V, W 
	DP3 TempTransLightV2.x, TempTangent, TempLightVector2; 
	DP3 TempTransLightV2.y, TempBinormal, TempLightVector2; 
	DP3 TempTransLightV2.z, TempNormal, TempLightVector2; 
	
	# normalize light vector 1 
	DP3 TempTransLightV1.w, TempTransLightV1, TempTransLightV1; 
	RSQ TempTransLightV1.w, TempTransLightV1.w; 
	MUL TempTransLightV1, TempTransLightV1, TempTransLightV1.w;
	
	# normalize light vector 2 
	DP3 TempTransLightV2.w, TempTransLightV2, TempTransLightV2; 
	RSQ TempTransLightV2.w, TempTransLightV2.w; 
	MUL TempTransLightV2, TempTransLightV2, TempTransLightV2.w;
	
	
	# move light vectors out
	MAD OutLightVector1, TempTransLightV1, {0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}; 
	MAD OutLightVector2, TempTransLightV2, {0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}; 
	
	# calculate attenuation of light 1
	MOV TempLightVector1.w, {0,0,0,0}; 
	DP3 TempLightVector1.x, TempLightVector1, TempLightVector1; 
	MUL TempLightVector1.x, TempLightVector1.x, program.local[13].w;  
	RSQ TempLightVector1, TempLightVector1.x; 
	MUL OutLightColor1, TempLightVector1, program.local[13]; # resulting light color = lightcolor * attenuation 
	
	# calculate attenuation of light 2
	MOV TempLightVector2.w, {0,0,0,0}; 
	DP3 TempLightVector2.x, TempLightVector2, TempLightVector2; 
	MUL TempLightVector2.x, TempLightVector2.x, program.local[15].w;  
	RSQ TempLightVector2, TempLightVector2.x; 
	MUL OutLightColor2, TempLightVector2, program.local[15]; # resulting light color = lightcolor * attenuation 
	
	# move out texture coordinates and original alpha value
	MOV OutTexCoord, InTexCoord; 
	MOV OutLightColor1.w, InColor.w; 
	
	END