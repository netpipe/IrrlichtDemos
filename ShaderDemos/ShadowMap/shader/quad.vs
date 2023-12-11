!!ARBvp1.0

PARAM c[1] = { { 0, 1, 0.5 } };

TEMP R0;
TEMP R1;

SLT R0.zw, vertex.position.xyxy, c[0].x;
SLT R0.xy, c[0].x, vertex.position;
ADD R0.xy, R0, -R0.zwzw;
MOV R0.zw, c[0].xyxy;
MAD R1.xy, -R0, c[0].z, c[0].z;
MOV result.position, R0;
ADD result.texcoord[0].xy, -R1, c[0].y;

END
