!!ARBfp1.0

OPTION ARB_precision_hint_nicest;

PARAM c[7] = { state.light[0].diffuse,
		state.light[0].specular,
		state.material.front.diffuse,
		state.material.front.specular,
		state.material.front.shininess,
		program.local[5],
		{ 2, 1, 0 } };

TEMP R0;
TEMP R1;
TEMP R2;

DP3 R0.w, fragment.texcoord[1], fragment.texcoord[1];
TEX R0.xyz, fragment.texcoord[0], texture[1], 2D;
MAD R0.xyz, R0, c[6].x, -c[6].y;
DP3 R1.x, R0, R0;
RSQ R1.x, R1.x;
DP3 R1.w, fragment.texcoord[2], fragment.texcoord[2];
RSQ R1.w, R1.w;
MUL R2.xyz, R1.w, fragment.texcoord[2];
RSQ R2.w, R0.w;
MUL R0.xyz, R1.x, R0;
MUL R1.xyz, R2.w, fragment.texcoord[1];
DP3 R0.w, R1, R0;
MUL R0.xyz, R0, c[6].x;
MAD R0.xyz, -R0.w, -R0, -R1;
DP3_SAT R0.x, R0, R2;
MOV R1, c[3];
RCP R2.y, R2.w;
MAX R2.x, R0.w, c[6].z;
POW R0.x, R0.x, c[4].x;
MUL R1, R1, c[1];
MUL R1, R1, R0.x;
MOV R0, c[2];
MUL R0, R0, c[0];
MAD R1, R0, R2.x, R1;
MUL R2.y, -R2, c[5].x;
ADD_SAT R2.x, R2.y, c[6].y;
TEX R0, fragment.texcoord[0], texture[0], 2D;
MUL R1, R1, R2.x;
MUL result.color, R1, R0;

END
