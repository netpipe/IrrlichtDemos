!!ARBfp1.0

PARAM BRIGHTER = 1.5;

TEMP finalCol;

# Let alpha have an effect
MUL finalCol, fragment.color, fragment.color.a;

# Make brighter and store result
MUL result.color, finalCol, BRIGHTER;

END
