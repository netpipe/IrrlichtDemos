sampler2D UserMapSampler : register(s3);

#define PI 3.14159265359
#define MAX_RAY 10000.0

#define SPHERE_CENTER_1 float3(-25.0, 0.0, 0.0)
#define SPHERE_RADIUS_1 25.0

#define SPHERE_CENTER_2 float3(25.0, 0.0, 0.0)
#define SPHERE_RADIUS_2 15.0

#define PLANE_NORMAL float3(0.0, 1.0, 0.0)
#define PLANE_OFFSET 50.0

#define LIGHT_DIRECTION float3(1.0, 0.0, 0.0)
#define BACKGROUND_COLOR float4(0.3, 0.6, 1.0, 1.0)

float planeHit(float3 rayOrigin, float3 rayDirection, float3 planeNormal, float planeOffset)
{
	float t2 = dot(planeNormal, rayDirection);
	float t = -(dot(planeNormal, rayOrigin) + planeOffset) / t2;
	
	return (t2 == 0.0 || t < 0.0) ? (MAX_RAY) : (t);
}

float sphereHit(float3 rayOrigin, float3 rayDirection, float3 sphereCenter, float sphereRadius)
{
	float3 q = sphereCenter - rayOrigin;
	float c = length(q);
	float v = dot(q, rayDirection);
	float d = sphereRadius * sphereRadius - (c*c - v*v);
	float t = v - sqrt(d);

	return (d > 0.0 && t > 0.0) ? (t) : (MAX_RAY);
}

float4 sphereBrickColor(float3 hitPoint, float3 sphereCenter, float sphereRadius)
{
	float3 vp = (hitPoint - sphereCenter) / sphereRadius;
	float phi = acos(-dot( vp, float3(0.0, 1.0, 0.0)));
	float theta = acos(dot(float3(1.0, 0.0, 0.0), vp) / sin(phi)) * (0.5 / PI);
	float2 coords = float2(phi / PI,  theta);
		
	return tex2D(UserMapSampler, coords) * clamp(dot(vp, LIGHT_DIRECTION), 0.3, 1.0);
}

float4 planeReflectionColor2(float3 hitPoint, float3 lineDir, float3 planeNormal)
{
	float3 outRay = reflect(lineDir, planeNormal);
	
	// Background color.
	float4 finalColor = lerp(BACKGROUND_COLOR, float4(0.0, 0.0, 0.0, 0.0), clamp(outRay.y, 0.0, 1.0));

	return lerp(finalColor, tex2D(UserMapSampler, hitPoint.xz / 100.0), 0.3);
}

float4 sphereReflectionColor(float3 hitPoint, float3 lineDir, float3 sphereCenter, float sphereRadius)
{
	float3 vp = (hitPoint - sphereCenter) / sphereRadius;
	float3 outRay = reflect(lineDir, vp);
	
	// Background color.
	float4 finalColor = lerp(BACKGROUND_COLOR, float4(0.0, 0.0, 0.0, 0.0), clamp(outRay.y, 0.0, 1.0));
	
	float t1 = sphereHit(hitPoint, outRay, SPHERE_CENTER_1, SPHERE_RADIUS_1);
	if(t1 < MAX_RAY)
		finalColor = sphereBrickColor(hitPoint + outRay * t1, SPHERE_CENTER_1, SPHERE_RADIUS_1);
		
	float t3 = planeHit(hitPoint, outRay, PLANE_NORMAL, PLANE_OFFSET);
	if(t3 < t1)
		finalColor = planeReflectionColor2(hitPoint + outRay * t3, outRay, PLANE_NORMAL);
		
	return lerp(finalColor, float4(0.5, 0.5, 0.5, 1.0), 0.2);
}

float4 planeReflectionColor(float3 hitPoint, float3 lineDir, float3 planeNormal)
{
	float3 outRay = reflect(lineDir, planeNormal);
	
	// Background color.
	float4 finalColor = lerp(BACKGROUND_COLOR, float4(0.0, 0.0, 0.0, 0.0), clamp(outRay.y, 0.0, 1.0));
	
	float t1 = sphereHit(hitPoint, outRay, SPHERE_CENTER_1, SPHERE_RADIUS_1);
	if(t1 < MAX_RAY)
		finalColor = sphereBrickColor(hitPoint + outRay * t1, SPHERE_CENTER_1, SPHERE_RADIUS_1);
		
	float t2 = sphereHit(hitPoint, outRay, SPHERE_CENTER_2, SPHERE_RADIUS_2);
	if(t2 < t1)
		finalColor = sphereReflectionColor(hitPoint + outRay * t2, outRay, SPHERE_CENTER_2, SPHERE_RADIUS_2);
		
	return lerp(finalColor, tex2D(UserMapSampler, hitPoint.xz / 100.0), 0.3);
}

float4 pixelMain
(
	float2 TexCoords : TEXCOORD0,
	float3 LStart : TEXCOORD1,
	float3 LEnd : TEXCOORD2
) : COLOR0
{
	float3 lVec = LEnd - LStart;
	float3 lDir = normalize(lVec);
	float lLength = length(lVec);

	// Background color.
	float4 finalColor = lerp(BACKGROUND_COLOR, float4(0.0, 0.0, 0.0, 0.0), clamp(lDir.y, 0.0, 1.0));
	
	float t1 = sphereHit(LStart, lDir, SPHERE_CENTER_1, SPHERE_RADIUS_1);
	if(t1 < MAX_RAY)
		finalColor = sphereBrickColor(LStart + lDir * t1, SPHERE_CENTER_1, SPHERE_RADIUS_1);

	float t2 = sphereHit(LStart, lDir, SPHERE_CENTER_2, SPHERE_RADIUS_2);
	if(t2 < t1)
		finalColor = sphereReflectionColor(LStart + lDir * t2, lDir, SPHERE_CENTER_2, SPHERE_RADIUS_2);
		
	float t3 = planeHit(LStart, lDir, PLANE_NORMAL, PLANE_OFFSET);
	if(t3 < t2 && t3 < t1)
		finalColor = planeReflectionColor(LStart + lDir * t3, lDir, PLANE_NORMAL);
		
	return finalColor;
}