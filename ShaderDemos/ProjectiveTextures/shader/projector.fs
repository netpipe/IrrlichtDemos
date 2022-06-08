!!ARBfp1.0
OPTION ARB_precision_hint_nicest;
# cgc version 2.1.0016, build date Nov 20 2008
# command line args: -oglsl -bestprecision -O3 -profile arbfp1
# source file: projector.frag
#vendor NVIDIA Corporation
#version 2.1.0.16
#profile arbfp1
#program main
#semantic projMap
#var float4 gl_TexCoord[0] : $vin.TEX0 : TEX0 : -1 : 1
#var float4 gl_TexCoord[1] :  :  : -1 : 0
#var float4 gl_TexCoord[2] :  :  : -1 : 0
#var float4 gl_TexCoord[3] :  :  : -1 : 0
#var float4 gl_TexCoord[4] :  :  : -1 : 0
#var float4 gl_TexCoord[5] :  :  : -1 : 0
#var float4 gl_TexCoord[6] :  :  : -1 : 0
#var float4 gl_TexCoord[7] :  :  : -1 : 0
#var float4 gl_FragColor : $vout.COLOR : COL : -1 : 1
#var sampler2D projMap :  : texunit 0 : -1 : 1
#const c[0] = 0
PARAM c[1] = { { 0 } };
TEMP R0;
TXP R0, fragment.texcoord[0], texture[0], 2D;
CMP result.color, -fragment.texcoord[0].w, R0, c[0].x;
END
# 2 instructions, 1 R-regs
