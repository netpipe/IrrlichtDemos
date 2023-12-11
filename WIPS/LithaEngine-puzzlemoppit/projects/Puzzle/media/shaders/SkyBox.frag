!!ARBfp1.0

# params.x - scroll amount (a scaled time value)
# params.y - brightness (increased when camera looks into the sun)
PARAM params = program.local[0];

# Regular sky box colour
TEMP col;
TEX col, fragment.texcoord, texture[0], 2D;


### Ripple effect (clouds moving above and below) ###

# Get and centre world position (sky box coordinates go 0.0 - 1.0, we want -0.5 to +0.5)
TEMP worldPos;
SUB worldPos, fragment.color.secondary, 0.5;

# Normalise world position (convert box coordinates to spherical)
DP3 worldPos.w, worldPos, worldPos;
RSQ worldPos.w, worldPos.w;
MUL worldPos, worldPos, worldPos.w;

# Find UV coord to read ripple texture...
# (Intersection of spherical world position vector with ground or sky plane at y = +/-0.5)
TEMP rippleSampleCoord;

# Calculate y gradient, this can be used to find intersection point
# Store it in z since we don't use that.
# gradient = 0.5 / worldPos.y
RCP rippleSampleCoord.z, worldPos.y;
MUL rippleSampleCoord.z, rippleSampleCoord, 0.5; # 0.5 is distance from sky box centre to top or bottom.

# Get intersection point using the gradient
MUL rippleSampleCoord.xy, worldPos.xzyw, rippleSampleCoord.z;

# Scroll UV coord
# Invert if we are at the top of the sky box, so both below and above clouds move in same direction
# params.x is the scroll amount
TEMP scrollDir;
CMP scrollDir, worldPos.y, 1.0, -1.0;
MAD rippleSampleCoord, params.x, -scrollDir, rippleSampleCoord;

# Scale UV coord?
#MUL rippleSampleCoord, rippleSampleCoord, 2.5;

# Read from ripple texture
TEMP rippleCol;
TEX rippleCol, rippleSampleCoord, texture[1], 2D;

# Amount of influence the ripples have. 0 - 1.0
TEMP rippleInfluence;

# Lower ripple influence
# 1.0 influence from y -0.5  to -0.3
# plus a fade out to zero influence from -0.3 to -0.1
MAX rippleInfluence, worldPos.y, -0.3; # clamp minimum
MIN rippleInfluence, rippleInfluence, -0.1; # clamp maximum
#ADD rippleInfluence, rippleInfluence, 0.3; # range 0.0 - 0.1
#MUL rippleInfluence, rippleInfluence, 5.0; # range 0.0 - 1.0
# previous ADD and MUL replaced with MAD:
MAD rippleInfluence, rippleInfluence, 5.0, 1.5; # 1.5 = 0.3*5.0
SUB rippleInfluence, 1.0, rippleInfluence; # invert

# Upper ripple influence
TEMP rippleInfluenceTop;
MAX rippleInfluenceTop, worldPos.y, 0.1; # clamp minimum
MIN rippleInfluenceTop, rippleInfluenceTop, 0.3; # clamp maximum
#ADD rippleInfluenceTop, rippleInfluenceTop, -0.1; # range 0.0 - 0.1
#MUL rippleInfluenceTop, rippleInfluenceTop, 5.0; # range 0.0 - 1.0
# previous ADD and MUL replaced with MAD:
MAD rippleInfluenceTop, rippleInfluenceTop, 5.0, -0.5; # -0.5 = -0.1 * 5.0

# Add ripple influences
# Should be a gap between the two at the horizon, so will only ever have either
# the top or bottom influences having an effect.
ADD rippleInfluence, rippleInfluence, rippleInfluenceTop;

# Scale the influence (currently it goes from 0 to 1, we don't want that much)
MUL rippleInfluence, rippleInfluence, 0.2;

# Apply the influence to the ripple colour
MUL rippleCol, rippleCol, rippleInfluence;

# Apply the inverted influence to the actual sky box colour
# And add the resulting weighted colours together.
# (so is a linear interpolation between the actual colour and the cloud/ripple colour)
SUB rippleInfluence, 1.0, rippleInfluence; # invert
MAD col, col, rippleInfluence, rippleCol;


### And finish... ###

# Dynamic brightness and apply
MUL result.color, col, params.y;

END
