!!ARBfp1.0

PARAM BRIGHTER = 1.2;

# Make brighter and store result
MUL result.color, fragment.color, BRIGHTER;

END
