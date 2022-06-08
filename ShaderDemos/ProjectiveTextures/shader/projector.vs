!!ARBvp1.0
# cgc version 2.1.0016, build date Nov 20 2008
# command line args: -oglsl -bestprecision -O3 -profile arbvp1
# source file: projector.vert
#vendor NVIDIA Corporation
#version 2.1.0.16
#profile arbvp1
#program main
#semantic gl_ModelViewProjectionMatrixTranspose : state.matrix.mvp
#semantic TexGenMat
#var float4 gl_TexCoord[0] : $vout.TEX0 : TEX0 : -1 : 1
#var float4 gl_TexCoord[1] :  :  : -1 : 0
#var float4 gl_TexCoord[2] :  :  : -1 : 0
#var float4 gl_TexCoord[3] :  :  : -1 : 0
#var float4 gl_TexCoord[4] :  :  : -1 : 0
#var float4 gl_TexCoord[5] :  :  : -1 : 0
#var float4 gl_TexCoord[6] :  :  : -1 : 0
#var float4 gl_TexCoord[7] :  :  : -1 : 0
#var float4 gl_Vertex : $vin.POSITION : POSITION : -1 : 1
#var float4 gl_Position : $vout.POSITION : HPOS : -1 : 1
#var float4x4 gl_ModelViewProjectionMatrixTranspose : state.matrix.mvp : c[1], 4 : -1 : 1
#var float4x4 TexGenMat :  : c[5], 4 : -1 : 1
PARAM c[9] = { program.local[0],
		state.matrix.mvp,
		program.local[5..8] };
TEMP R0;
MUL R0, vertex.position.y, c[6];
MAD R0, vertex.position.x, c[5], R0;
MAD R0, vertex.position.z, c[7], R0;
MAD result.texcoord[0], vertex.position.w, c[8], R0;
DP4 result.position.w, vertex.position, c[4];
DP4 result.position.z, vertex.position, c[3];
DP4 result.position.y, vertex.position, c[2];
DP4 result.position.x, vertex.position, c[1];
END
# 8 instructions, 1 R-regs
