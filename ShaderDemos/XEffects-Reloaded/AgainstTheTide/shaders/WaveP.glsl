uniform sampler2D PMap;

varying vec2 TCoords;
varying float FadeFactor;

void main()
{
	vec4 PMap = texture2D(PMap, TCoords);

	vec4 finalColor = PMap;
	finalColor *= (1.0 - FadeFactor);
	
	gl_FragColor = finalColor;
}