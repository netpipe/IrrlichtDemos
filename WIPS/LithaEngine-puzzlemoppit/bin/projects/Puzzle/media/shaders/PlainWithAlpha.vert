!!ARBvp1.0

PARAM mvp[4] = { state.matrix.mvp };

# Transform vertex
DP4 result.position.x, mvp[0], vertex.position;
DP4 result.position.y, mvp[1], vertex.position;
DP4 result.position.z, mvp[2], vertex.position;
DP4 result.position.w, mvp[3], vertex.position;

# Colour
MOV result.color, vertex.color;

END