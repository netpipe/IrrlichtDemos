// *************************************************************************************************
// Horde3D Shader Utility Library
// --------------------------------------
//		- Lighting functions -
//
// Copyright (C) 2006-2008 Nicolas Schulz
//
// You may use the following code in projects based on the Horde3D graphics engine.
//
// *************************************************************************************************

uniform 	vec3 viewer;
uniform 	vec4 lightPos;
uniform 	vec3 lightDir;
uniform 	vec3 lightColor;
uniform 	float lightCosCutoff;
uniform 	sampler2DShadow shadowMap;
uniform 	vec4 shadowSplitDists;
uniform 	mat4 shadowMats[4];
uniform 	float shadowMapSize;


float PCF( const vec4 projShadow )
{
	// 5-tap PCF with a 30° rotated grid
	
	float offset = 1.0 / shadowMapSize;
	
	vec4 shadow = shadow2D( shadowMap, projShadow.stp );
	shadow += shadow2D( shadowMap, projShadow.stp + vec3( -0.866 * offset,  0.5 * offset, 0.0 ) );
	shadow += shadow2D( shadowMap, projShadow.stp + vec3( -0.866 * offset, -0.5 * offset, 0.0 ) );
	shadow += shadow2D( shadowMap, projShadow.stp + vec3(  0.866 * offset, -0.5 * offset, 0.0 ) );
	shadow += shadow2D( shadowMap, projShadow.stp + vec3(  0.866 * offset,  0.5 * offset, 0.0 ) );
	
	return shadow.r / 5.0;
}


vec3 calcPhongSpotLight( const vec3 pos, const vec3 normal, const vec3 albedo, const float specMask,
						 const float specExp, const float viewDist, const float ambientIntensity )
{
	vec3 light = lightPos.xyz - pos;
	
	// Distance attenuation
	float lightDist = length( light ) / lightPos.w;
	float att = max( 1.0 - lightDist * lightDist, 0.0 );
	light = normalize( light );
	
	// Spotlight falloff
	float angle = dot( lightDir, -light );
	att *= clamp( (angle - lightCosCutoff) / 0.2, 0.0, 1.0 );
		
	// Lambert diffuse contribution
	float ndotl = dot( normal, light );
	float diffuse = max( ndotl, 0.0 );
	
	// Diffuse color
	vec3 col = albedo * lightColor * diffuse;
	
	// Shadow
	if( ndotl > 0.0 && att > 0.0 )
	{	
		vec4 projShadow = shadowMats[3] * vec4( pos, 1.0 );
		if( viewDist < shadowSplitDists.x ) projShadow = shadowMats[0] * vec4( pos, 1.0 );
		else if( viewDist < shadowSplitDists.y ) projShadow = shadowMats[1] * vec4( pos, 1.0 );
		else if( viewDist < shadowSplitDists.z ) projShadow = shadowMats[2] * vec4( pos, 1.0 );
		
		projShadow.z = lightDist;
		projShadow.xy /= projShadow.w;
		
		float shadowFac = PCF( projShadow );
		
		// Specular contribution
		vec3 eye = normalize( viewer - pos );
		vec3 refl = reflect( -light, normal );
		float spec = pow( clamp( dot( refl, eye ), 0.0, 1.0 ), specExp ) * specMask;
		col += lightColor * spec * shadowFac;
		
		col *= max( shadowFac, ambientIntensity );
	}
	
	return col * att;
}