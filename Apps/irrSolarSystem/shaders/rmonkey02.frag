uniform vec4 fvAmbient;
uniform vec4 fvSpecular;
uniform vec4 fvDiffuse;
uniform float fSpecularPower;

uniform sampler2D colormap;
uniform sampler2D bumpMap;

varying vec2 Texcoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
varying vec3 Normal;


varying vec4 vTexCoords00;
varying vec4 vTexCoords01;
varying vec4 vTexCoords02;
varying vec4 vTexCoords10;
varying vec4 vTexCoords12;
varying vec4 vTexCoords20;
varying vec4 vTexCoords21;
varying vec4 vTexCoords22;

void main (void)
{
   //
   //  Calculate normalmap values here
   //

   vec4 lightness;
   lightness = vec4(0.3, 0.59, 0.11, 0.0);
   
   vec4 s00;
   vec4 s01;
   vec4 s02;

   vec4 s10;
   vec4 s12;
   
   vec4 s20;
   vec4 s21;
   vec4 s22;
 
   s00 = texture2DProj(colormap, vTexCoords00);
   s01 = texture2DProj(colormap, vTexCoords01);
   s02 = texture2DProj(colormap, vTexCoords02);

   s10 = texture2DProj(colormap, vTexCoords10);
   s12 = texture2DProj(colormap, vTexCoords12);
   
   s20 = texture2DProj(colormap, vTexCoords20);
   s21 = texture2DProj(colormap, vTexCoords21);
   s22 = texture2DProj(colormap, vTexCoords22);

   // Slope in X direction
   vec4 sobelX = s00 + 2.0 * s10 + s20 - s02 - 2.0 * s12 - s22;
   // Slope in Y direction
   vec4 sobelY = s00 + 2.0 * s01 + s02 - s20 - 2.0 * s21 - s22;
   
   // Weight the slope in all channels, we use grayscale as height
   float sx = dot(sobelX.xyz, lightness.xyz);
   float sy = dot(sobelY.xyz, lightness.xyz);
   
   // Compose the normal
   vec3 normal = normalize(vec3(sx, sy, 1.0));
   
   // Pack [-1, 1] into [0, 1]
   vec4 normalMapPixel;
   normalMapPixel = vec4(normal * 0.5 + 0.5, 1.0);
   
   //
   //  Etc
   //   
   

   vec3  fvLightDirection = normalize( LightDirection );
   vec3  fvNormal         = normalize( ( normalMapPixel.xyz * 4.0 ) - 1.0 );
   
   float fNDotL           = dot( fvNormal, fvLightDirection );
   
   vec3  fvReflection     = normalize( ( ( 2.0 * fvNormal ) * fNDotL ) - fvLightDirection );
   vec3  fvViewDirection  = normalize( ViewDirection );
   float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );
   
   vec4  fvBaseColor      = texture2D( colormap, Texcoord );
   
   vec4  fvTotalAmbient   = fvAmbient * fvBaseColor;
   vec4  fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor;
   vec4  fvTotalSpecular  = fvSpecular * ( pow( fRDotV, fSpecularPower ) );
 
   gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular );   
   //gl_FragColor =  fvTotalDiffuse + fvTotalSpecular; 
   //gl_FragColor = texture2D( colormap, Texcoord );

} 