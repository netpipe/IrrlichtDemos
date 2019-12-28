!!ARBfp1.0

OPTION ARB_precision_hint_nicest;

PARAM c[4] = { program.local[0],
	     { 0.090909094, -5, -4, -3 },
	     { -2, 2, 3, 4 },
	     { 5 } };

TEMP R0;
TEMP R1;
TEMP R2;
TEMP R3;
TEMP R4;

MOV R2.xyz, c[1].yzww;
MAD R0.x, R2, c[0], fragment.texcoord[0];
MOV R0.y, fragment.texcoord[0];
TEX R1, R0, texture[0], 2D;
TEX R0, fragment.texcoord[0], texture[0], 2D;
ADD R3, R0, R1;
MAD R1.x, R2.y, c[0], fragment.texcoord[0];
MOV R1.y, fragment.texcoord[0];
TEX R1, R1, texture[0], 2D;
ADD R1, R3, R1;
MOV R4, c[2];
MOV R2.y, fragment.texcoord[0];
MAD R2.x, R2.z, c[0], fragment.texcoord[0];
TEX R2, R2, texture[0], 2D;
ADD R3, R1, R2;
MOV R1.y, fragment.texcoord[0];
MAD R1.x, R4, c[0], fragment.texcoord[0];
TEX R1, R1, texture[0], 2D;
ADD R1, R3, R1;
MOV R2.y, fragment.texcoord[0];
ADD R2.x, fragment.texcoord[0], -c[0];
TEX R2, R2, texture[0], 2D;
ADD R2, R1, R2;
ADD R0, R2, R0;
MOV R1.y, fragment.texcoord[0];
ADD R1.x, fragment.texcoord[0], c[0];
TEX R1, R1, texture[0], 2D;
ADD R0, R0, R1;
MOV R1.y, fragment.texcoord[0];
MAD R1.x, R4.y, c[0], fragment.texcoord[0];
TEX R1, R1, texture[0], 2D;
ADD R0, R0, R1;
MOV R2.y, fragment.texcoord[0];
MAD R2.x, R4.z, c[0], fragment.texcoord[0];
TEX R2, R2, texture[0], 2D;
ADD R0, R0, R2;
MOV R2.x, c[3];
MOV R1.y, fragment.texcoord[0];
MAD R1.x, R4.w, c[0], fragment.texcoord[0];
TEX R1, R1, texture[0], 2D;
MOV R2.y, fragment.texcoord[0];
MAD R2.x, R2, c[0], fragment.texcoord[0];
TEX R2, R2, texture[0], 2D;
ADD R0, R0, R1;
ADD R0, R0, R2;
MUL result.color, R0, c[1].x;

END
