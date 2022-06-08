!!ARBfp1.0

OPTION ARB_precision_hint_nicest;

PARAM c[1] = { { 0.5, 0.00019999999, 1, 0 } };

TEMP R0;

RCP R0.x, fragment.texcoord[0].w;
MUL R0.zw, fragment.texcoord[0].xyxy, R0.x;
RCP R0.x, fragment.texcoord[1].y;
MAD R0.y, fragment.texcoord[1].x, R0.x, -c[0];
MAD R0.zw, R0, c[0].x, c[0].x;
TEX R0.x, R0.zwzw, texture[0], 2D;
ADD R0.x, R0, -R0.y;
CMP R0.x, -R0, c[0].w, c[0].z;
CMP result.color, -fragment.texcoord[0].w, R0.x, c[0].w;

END
