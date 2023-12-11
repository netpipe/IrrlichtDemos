!!ARBfp1.0

OPTION ARB_precision_hint_nicest;

PARAM c[1] = { program.local[0] };

TEMP R0;
TEMP R1;

TEX R0, fragment.texcoord[0], texture[0], 2D;
TEX R1, fragment.texcoord[0], texture[1], 2D;
MAD result.color, R1, c[0].x, R0;

END
