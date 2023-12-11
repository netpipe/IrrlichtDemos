!!ARBfp1.0

TEMP col;
TEMP finalCol;

# Read from texture 0.
TEX col, fragment.texcoord, texture[0], 2D;

# Now give brighter colours a greater influence...
# col*col*col*col*col
MUL finalCol, col, col;
MUL finalCol, finalCol, col;
#MUL finalCol, finalCol, col;
MUL result.color, finalCol, col;

END
