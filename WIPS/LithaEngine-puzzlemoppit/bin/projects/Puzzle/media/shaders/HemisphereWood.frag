!!ARBfp1.0

PARAM BRIGHTER = 1.2;

TEMP finalCol;

# Read from texture 0.
TEX finalCol, fragment.texcoord, texture[0], 2D;

# Get fragment colour and apply texture.
MUL finalCol, finalCol, fragment.color;

# Make brighter and store result
MUL result.color, finalCol, BRIGHTER;

END
