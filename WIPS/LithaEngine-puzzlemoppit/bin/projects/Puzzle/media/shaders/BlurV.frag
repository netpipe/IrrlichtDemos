!!ARBfp1.0

# Vertical blur
PARAM BLUR_STEP = {0.0, -0.003, 0.0, 0.003};

# Initial texture read at centre texture coordinate.
TEMP col;
TEX col, fragment.texcoord, texture[0], 2D;

# Now iterate, 4 times
# Each iteration, we read the texture either side of the centre, with increasing distance.
# The constants used in the MAD instructions in each iteration is calculated as:
# c = 2.0 / i
# (where i is the current iteration, starting at 1)

TEMP texCoord;
TEMP sampleA;
TEMP sampleB;

MOV texCoord.xy, fragment.texcoord;
MOV texCoord.zw, fragment.texcoord.zwxy;


# ITERATION 1

ADD texCoord, texCoord, BLUR_STEP;

TEX sampleA, texCoord.xyzw, texture[0], 2D;
TEX sampleB, texCoord.zwxy, texture[0], 2D;

MAD col, sampleA, 2.0, col;
MAD col, sampleB, 2.0, col;

# ITERATION 2

ADD texCoord, texCoord, BLUR_STEP;

TEX sampleA, texCoord.xyzw, texture[0], 2D;
TEX sampleB, texCoord.zwxy, texture[0], 2D;

MAD col, sampleA, 1.0, col;
MAD col, sampleB, 1.0, col;

# ITERATION 3

ADD texCoord, texCoord, BLUR_STEP;

TEX sampleA, texCoord.xyzw, texture[0], 2D;
TEX sampleB, texCoord.zwxy, texture[0], 2D;

MAD col, sampleA, 0.66666, col;
MAD col, sampleB, 0.66666, col;

# ITERATION 4

ADD texCoord, texCoord, BLUR_STEP;

TEX sampleA, texCoord.xyzw, texture[0], 2D;
TEX sampleB, texCoord.zwxy, texture[0], 2D;

MAD col, sampleA, 0.5, col;
MAD col, sampleB, 0.5, col;


# Now weight the resulting colour.
# result = col * ( 1.0 / (1.0 + 2.0 * float(BLUR_SAMPLES)) );
# where BLUR_SAMPLES is the number of iterations, 4.0.

MUL result.color, col, 0.11111;

END
