
#include "Litha.h"

// Note: This doesn't handle concave meshes (holes are treated as filled).
// To handle concave meshes would need a ray cast system that returns all
// collision points, not just the closest one.
class CameraCollider : public ICameraCollider
{
	IPhysics *physics;
	u32 collisionLayer;
	Set<ICollisionGeometry *> excludedGeometries;
	
	f32 minCameraBackDist;
	f32 minCameraFrontDist;
	
	// it is assumed that the point is on the line of the segment
	bool insideSegment(const core::vector3df &point, const core::line3df &segment)
	{
		// Distances from point to both ends of segment are both smaller than segment length.
		f32 length = segment.getLengthSQ();
		
		return point.getDistanceFromSQ(segment.start) <= length
				&& point.getDistanceFromSQ(segment.end) <= length;
	}
	
public:
	CameraCollider(IPhysics *physics)
	{
		this->physics = physics;
		collisionLayer = 0;
		
		minCameraBackDist = 0.5;
		minCameraFrontDist = 0.5;
	}
	
	void SetCollisionLayer(u32 layer)
	{
		collisionLayer = layer;
	}
	
	void ExcludeGeometry(const Set<ICollisionGeometry *> &excluding)
	{
		excludedGeometries.Union(excluding);
	}
	
	void ClearExcludedGeometry()
	{
		excludedGeometries.clear();
	}
	
	// minimum space behind the camera
	void SetMinCameraDistBack(f32 dist)
	{
		minCameraBackDist = dist;
	}
	
	// minimum space in front of the camera
	void SetMinCameraDistFront(f32 dist)
	{
		minCameraFrontDist = dist;
	}
	
	bool ProcessCollision(const core::line3df &cameraRay, core::vector3df &resultPos)
	{
		// Get all geoms...
		std::vector<RayCollision> collisions = physics->RayCastExcluding(cameraRay, excludedGeometries, collisionLayer);
		
		if (collisions.size() == 0)
			return false;
		
		// Make aabb that contains all...
		core::aabbox3df box = collisions[0].geometry->GetAABB();
		
		for (u32 i = 1; i < collisions.size(); i ++)
			box.addInternalBox(collisions[i].geometry->GetAABB());
		
		// this is a rough approximation and probably much larger than necessary
		f32 diameter = box.getExtent().getLength() + minCameraBackDist + minCameraFrontDist + 1.0;
		
		// Now from this we can create a seemingly infinite ray
		// (i.e. it will fully encompass all objects along the cameraRay)
		
		core::line3df infiniteRay = cameraRay;
		core::vector3df rayVec = cameraRay.getVector();
		rayVec.normalize();
		// this makes it much bigger than necessary, but that's not a huge problem...
		// (really only needs to reach edges of box bound sphere)
		infiniteRay.start -= rayVec * diameter;
		infiniteRay.end += rayVec * diameter;
		
		// Now we have a ray that passes completely through all geoms that touched the original cameraRay.
		// So now we can send that ray in both directions and we will get two contact points for each geom.
		// Can then use that information to find where gaps and solids are along the ray.
		
		std::vector<RayCollision> forwardsCollisions
				= physics->RayCastExcluding(infiniteRay, excludedGeometries, collisionLayer);
		
		std::vector<RayCollision> backwardsCollisions
				= physics->RayCastExcluding(core::line3df(infiniteRay.end,infiniteRay.start),
						excludedGeometries, collisionLayer);
		
		// Find collision pairs
		// These should indicate segments of solid geometry.
		// We can ignore non-pairs as they should be geometries that are either not touching
		// the original camera ray, or are only just being intersected.
		
		std::vector<core::line3df> segments;
		
		for (u32 i = 0; i < forwardsCollisions.size(); i ++)
		{
			for (u32 j = 0; j < backwardsCollisions.size(); j ++)
			{
				if (forwardsCollisions[i].geometry == backwardsCollisions[j].geometry)
				{
					// Found pair!
					segments.push_back(
							core::line3df(forwardsCollisions[i].collision.pos, backwardsCollisions[j].collision.pos)
							);
				}
			}
		}
		
		// Extend segments towards the camera by a minimum distance
		// and also towards the target point by a minimum distance
		for (u32 i = 0; i < segments.size(); i ++)
		{
			segments[i].start -= rayVec * minCameraFrontDist;
			segments[i].end += rayVec * minCameraBackDist;
		}
		
		// Some points...
		// A is the camera original point
		// B is the camera target point
		// Afar is the "infinite" ray point beyond the camera original point.
		// Bfar is the "infinite" ray point beyond the camera target point.
		// Amoving is the new position for the camera. (starts at A and then moves towards B).
		core::vector3df A = cameraRay.end;
		core::vector3df B = cameraRay.start;
		core::vector3df Afar = infiniteRay.end;
		core::vector3df Bfar = infiniteRay.start;
		core::vector3df Amoving = A;
		
		// Now, the actual algorithm!
		
		bool cameraMoved = false;
		
		while (true)
		{
			bool wasCollision = false;
			
			// Test all segments against Amoving.
			// If Amoving is inside a segment, then move Amoving to the start of the segment
			// (in the direction of the camera target point)
			// Then remove that segment from list...
			for (u32 i = 0; i < segments.size(); i ++)
			{
				if (insideSegment(Amoving, segments[i]))
				{
					Amoving = segments[i].start;
					segments.erase(segments.begin()+i);
					wasCollision = true;
					cameraMoved = true;
					break;
				}
			}
			
			// If none of the segments collided, finished.
			// Otherwise loop continues and we re-test all.
			if (!wasCollision)
				break;
			
			// Or if Amoving has passed B, we have finished...
			// (distance from Amoving to Bfar is smaller than distance from B to Bfar)
			if (Amoving.getDistanceFromSQ(Bfar) <= B.getDistanceFromSQ(Bfar))
			{
				Amoving = B;
				cameraMoved = true;
				break;
			}
		}
		
		if (cameraMoved)
			resultPos = Amoving;
		
		return cameraMoved;
	}
};


