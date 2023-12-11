!!ARBfp1.0

OPTION ARB_precision_hint_nicest;

PARAM c[6] = { state.light[0].diffuse,
		state.light[0].specular,
		state.material.front.diffuse,
		state.material.front.specular,
		state.material.front.shininess,
		{ 0, 2 } };

TEMP R0;
TEMP R1;
TEMP R2;

DP3 R0.y, fragment.texcoord[3], fragment.texcoord[3];
RSQ R0.y, R0.y;
DP3 R0.x, fragment.texcoord[2], fragment.texcoord[2];
RSQ R0.x, R0.x;
MUL R1.xyz, R0.x, fragment.texcoord[2];
MUL R2.xyz, R0.y, fragment.texcoord[3];
DP3 R2.w, R1, fragment.texcoord[1];
MUL R0.xyz, fragment.texcoord[1], c[5].y;
MAD R0.xyz, -R2.w, -R0, -R1;
DP3 R0.x, R0, R2;
MAX R0.x, R0, c[5];
POW R2.x, R0.x, c[4].x;
MOV R1, c[3];
MOV R0, c[2];
MUL R1, R1, c[1];
MUL R1, R1, R2.x;
MUL R0, R0, c[0];
MAX R2.x, R2.w, c[5];
MAD R1, R0, R2.x, R1;
TEX R0, fragment.texcoord[0], texture[0], 2D;
MUL result.color, R0, R1;

END
