!!ARBfp1.0

OPTION ARB_precision_hint_nicest;

PARAM c[1] = { { 0 } };

TEMP R0;

RCP R0.x, fragment.texcoord[0].x;
MUL R0.x, fragment.texcoord[0].z, R0;
MOV R0.zw, c[0].x;
MUL R0.y, R0.x, R0.x;
MOV result.color, R0;

END
