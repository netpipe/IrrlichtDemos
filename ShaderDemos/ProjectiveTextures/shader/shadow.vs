!!ARBvp1.0

PARAM c[10] = { program.local[0],
		state.matrix.mvp,
		program.local[5..9] };

TEMP R0;

MUL R0, vertex.position.y, c[6];
MAD R0, vertex.position.x, c[5], R0;
MAD R0, vertex.position.z, c[7], R0;
MAD R0, vertex.position.w, c[8], R0;
MOV result.texcoord[0], R0;
MOV result.texcoord[1].x, R0.z;
DP4 result.position.w, vertex.position, c[4];
DP4 result.position.z, vertex.position, c[3];
DP4 result.position.y, vertex.position, c[2];
DP4 result.position.x, vertex.position, c[1];
MOV result.texcoord[1].y, c[9].x;

END
