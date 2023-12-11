!!ARBvp1.0

PARAM c[6] = { program.local[0],
		state.matrix.mvp,
		program.local[5] };

TEMP R0;

DP4 R0.x, vertex.position, c[4];
DP4 R0.y, vertex.position, c[3];
DP4 R0.z, vertex.position, c[2];
MOV result.texcoord[1].xy, vertex.texcoord[0];
MOV result.position.w, R0.x;
MOV result.position.z, R0.y;
MOV result.position.y, R0.z;
DP4 result.position.x, vertex.position, c[1];
MOV result.texcoord[0].x, c[5];
MOV result.texcoord[0].y, R0.z;
MOV result.texcoord[0].z, R0.y;
MOV result.texcoord[0].w, R0.x;

END
