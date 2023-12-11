!!ARBvp1.0

PARAM GROUND_COL = {0.53, 0.53, 0.56, 1.0};
PARAM SKY_COL = {0.93, 0.93, 0.53, 1.0};
PARAM SUN_POS = {0.0, 100.0, 0.0, 1.0};

PARAM mvp[4] = { state.matrix.mvp };
PARAM transWorldMatrix[4] = { program.local[0..3] };
PARAM timeSeconds = program.local[4];


### VERTEX POSITION ###

# Transform vertex
DP4 result.position.x, mvp[0], vertex.position;
DP4 result.position.y, mvp[1], vertex.position;
DP4 result.position.z, mvp[2], vertex.position;
DP4 result.position.w, mvp[3], vertex.position;


### HEMISPHERE LIGHTING ###

# Get world position

TEMP worldPos;
DP4 worldPos.x, vertex.position, transWorldMatrix[0];
DP4 worldPos.y, vertex.position, transWorldMatrix[1];
DP4 worldPos.z, vertex.position, transWorldMatrix[2];
DP4 worldPos.w, vertex.position, transWorldMatrix[3];

# Vector to the sun.
TEMP sunVec;
SUB sunVec, SUN_POS, worldPos;

# Normalise sun vector

DP3 sunVec.w, sunVec, sunVec;
RSQ sunVec.w, sunVec.w;
MUL sunVec, sunVec, sunVec.w;

# Fraction between ground and sky light colour that this surface receives.

TEMP mixAlpha;
DP3 mixAlpha, vertex.normal, sunVec;
MAD mixAlpha, mixAlpha, 0.5, 0.5;

# GLSL mix
# mix(GROUND_COL, SKY_COL, mixAlpha)
# just a linear interpolation...
# mix(x,y,a): x*(1.0-a) + y*a

TEMP mixResult;
MUL mixResult, SKY_COL, mixAlpha;
SUB mixAlpha, 1.0, mixAlpha;
MAD mixResult, GROUND_COL, mixAlpha, mixResult;
MOV mixResult.w, 1.0;

# FINAL COLOUR!!

MUL result.color, mixResult, vertex.color;

END
