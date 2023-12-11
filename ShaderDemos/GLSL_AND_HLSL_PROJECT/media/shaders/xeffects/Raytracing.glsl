uniform sampler2D UserMapSampler;

#define PI 3.14159265359
#define MAX_RAY 10000.0

#define SPHERE_CENTER_1 vec3(-25.0, 0.0, 0.0)
#define SPHERE_RADIUS_1 25.0

#define SPHERE_CENTER_2 vec3(25.0, 0.0, 0.0)
#define SPHERE_RADIUS_2 15.0

#define PLANE_NORMAL vec3(0.0, 1.0, 0.0)
#define PLANE_OFFSET 50.0

#define LIGHT_DIRECTION vec3(1.0, 0.0, 0.0)
#define BACKGROUND_COLOR vec4(0.3, 0.6, 1.0, 1.0)

float planeHit(vec3 rayOrigin, vec3 rayDirection, vec3 planeNormal, float planeOffset)
{
	float t2 = dot(planeNormal, rayDirection);
	float t = -(dot(planeNormal, rayOrigin) + planeOffset) / t2;
	
	return (t2 == 0.0 || t < 0.0) ? (MAX_RAY) : (t);
}

float sphereHit(vec3 rayOrigin, vec3 rayDirection, vec3 sphereCenter, float sphereRadius)
{
	vec3 q = sphereCenter - rayOrigin;
	float c = length(q);
	float v = dot(q, rayDirection);
	float d = sphereRadius * sphereRadius - (c*c - v*v);
	float t = v - sqrt(d);

	return (d > 0.0 && t > 0.0) ? (t) : (MAX_RAY);
}

vec4 sphereBrickColor(vec3 hitPoint, vec3 sphereCenter, float sphereRadius)
{
	vec3 vp = (hitPoint - sphereCenter) / sphereRadius;
	float phi = acos(-dot( vp, vec3(0.0, 1.0, 0.0)));
	float theta = acos(dot(vec3(1.0, 0.0, 0.0), vp) / sin(phi)) * (0.5 / PI);
	vec2 coords = vec2(phi / PI,  theta);
		
	return texture2D(UserMapSampler, coords) * clamp(dot(vp, LIGHT_DIRECTION), 0.3, 1.0);
}

vec4 planeReflectionColor2(vec3 hitPoint, vec3 lineDir, vec3 planeNormal)
{
	vec3 outRay = reflect(lineDir, planeNormal);
	
	// Background color.
	vec4 finalColor = mix(BACKGROUND_COLOR, vec4(0.0, 0.0, 0.0, 0.0), clamp(outRay.y, 0.0, 1.0));

	return mix(finalColor, texture2D(UserMapSampler, hitPoint.xz / 100.0), 0.3);
}

vec4 sphereReflectionColor(vec3 hitPoint, vec3 lineDir, vec3 sphereCenter, float sphereRadius)
{
	vec3 vp = (hitPoint - sphereCenter) / sphereRadius;
	vec3 outRay = reflect(lineDir, vp);
	
	// Background color.
	vec4 finalColor = mix(BACKGROUND_COLOR, vec4(0.0, 0.0, 0.0, 0.0), clamp(outRay.y, 0.0, 1.0));
	
	float t1 = sphereHit(hitPoint, outRay, SPHERE_CENTER_1, SPHERE_RADIUS_1);
	if(t1 < MAX_RAY)
		finalColor = sphereBrickColor(hitPoint + outRay * t1, SPHERE_CENTER_1, SPHERE_RADIUS_1);
		
	float t3 = planeHit(hitPoint, outRay, PLANE_NORMAL, PLANE_OFFSET);
	if(t3 < t1)
		finalColor = planeReflectionColor2(hitPoint + outRay * t3, outRay, PLANE_NORMAL);
		
	return mix(finalColor, vec4(0.5, 0.5, 0.5, 1.0), 0.2);
}

vec4 planeReflectionColor(vec3 hitPoint, vec3 lineDir, vec3 planeNormal)
{
	vec3 outRay = reflect(lineDir, planeNormal);
	
	// Background color.
	vec4 finalColor = mix(BACKGROUND_COLOR, vec4(0.0, 0.0, 0.0, 0.0), clamp(outRay.y, 0.0, 1.0));
	
	float t1 = sphereHit(hitPoint, outRay, SPHERE_CENTER_1, SPHERE_RADIUS_1);
	if(t1 < MAX_RAY)
		finalColor = sphereBrickColor(hitPoint + outRay * t1, SPHERE_CENTER_1, SPHERE_RADIUS_1);
		
	float t2 = sphereHit(hitPoint, outRay, SPHERE_CENTER_2, SPHERE_RADIUS_2);
	if(t2 < t1)
		finalColor = sphereReflectionColor(hitPoint + outRay * t2, outRay, SPHERE_CENTER_2, SPHERE_RADIUS_2);
		
	return mix(finalColor, texture2D(UserMapSampler, hitPoint.xz / 100.0), 0.3);
}

void main()
{
	vec3 LStart = gl_TexCoord[1].xyz;
	vec3 LEnd = gl_TexCoord[2].xyz;

	vec3 lVec = LEnd - LStart;
	vec3 lDir = normalize(lVec);
	float lLength = length(lVec);

	// Background color.
	vec4 finalColor = mix(BACKGROUND_COLOR, vec4(0.0, 0.0, 0.0, 0.0), clamp(lDir.y, 0.0, 1.0));
	
	float t1 = sphereHit(LStart, lDir, SPHERE_CENTER_1, SPHERE_RADIUS_1);
	if(t1 < MAX_RAY)
		finalColor = sphereBrickColor(LStart + lDir * t1, SPHERE_CENTER_1, SPHERE_RADIUS_1);

	float t2 = sphereHit(LStart, lDir, SPHERE_CENTER_2, SPHERE_RADIUS_2);
	if(t2 < t1)
		finalColor = sphereReflectionColor(LStart + lDir * t2, lDir, SPHERE_CENTER_2, SPHERE_RADIUS_2);
		
	float t3 = planeHit(LStart, lDir, PLANE_NORMAL, PLANE_OFFSET);
	if(t3 < t2 && t3 < t1)
		finalColor = planeReflectionColor(LStart + lDir * t3, lDir, PLANE_NORMAL);
		
	gl_FragColor = finalColor;
}