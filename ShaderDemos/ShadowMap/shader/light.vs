!!ARBvp1.0

PARAM c[13] = { program.local[0],
		state.matrix.modelview.inverse.row[0..2],
		state.matrix.modelview.transpose,
		state.light[0].position,
		state.matrix.mvp };

TEMP R0;
TEMP R1;

MUL R0.xyz, vertex.normal.y, c[2];
MAD R0.xyz, vertex.normal.x, c[1], R0;
MAD R0.xyz, vertex.normal.z, c[3], R0;
DP3 R0.w, R0, R0;
MUL R1.xyz, vertex.position.y, c[5];
MAD R1.xyz, vertex.position.x, c[4], R1;
RSQ R0.w, R0.w;
MAD R1.xyz, vertex.position.z, c[6], R1;
MUL result.texcoord[1].xyz, R0.w, R0;
MAD R0.xyz, vertex.position.w, c[7], R1;
MOV result.texcoord[0], vertex.texcoord[0];
ADD result.texcoord[2].xyz, -R0, c[8];
MOV result.texcoord[3].xyz, -R0;
DP4 result.position.w, vertex.position, c[12];
DP4 result.position.z, vertex.position, c[11];
DP4 result.position.y, vertex.position, c[10];
DP4 result.position.x, vertex.position, c[9];

END
