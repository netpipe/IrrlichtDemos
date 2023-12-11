!!ARBfp1.0

OPTION ARB_precision_hint_nicest;

PARAM c[2] = { program.local[0],
	     { 0.29899999, 0.58700001, 0.114, 0 } };

TEMP R0;
TEMP R1;

TEX R0, fragment.texcoord[0], texture[0], 2D;
DP3 R1.x, R0, c[1];
ADD R1.x, R1, -c[0];
CMP result.color.xyz, R1.x, c[1].w, R0;
MOV result.color.w, R0;

END
