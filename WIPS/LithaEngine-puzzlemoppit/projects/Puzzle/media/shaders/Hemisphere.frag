!!ARBfp1.0

PARAM BRIGHTER = 1.2;

TEMP finalCol;

# Read from texture 0.
TEX finalCol, fragment.texcoord, texture[0], 2D;

# Get fragment colour and apply texture.
MUL finalCol, finalCol, fragment.color;

# Cloud shadow
TEMP cloudCol;
TEX cloudCol, fragment.texcoord[1], texture[1], 2D;
MAD cloudCol, cloudCol, 0.35, -0.2;
MUL cloudCol, cloudCol, fragment.texcoord[1].z;

# Only apply cloud to upwards facing surfaces
# (fragment.texcoord[1].z contains the normal Y component)
TEMP cloudEnabled;
# if normal Y > 0.0 then is enabled
SLT cloudEnabled, 0.0, fragment.texcoord[1].z;
MAD finalCol.rgb, cloudCol, cloudEnabled, finalCol;

# Let alpha have an effect
MUL finalCol, finalCol, finalCol.a;

# Make brighter and store result
MUL result.color, finalCol, BRIGHTER;

END
