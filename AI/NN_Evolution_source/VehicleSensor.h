
#define LINE_COLOUR video::SColor(255, 255, 0, 0)

class Vehicle;

// needs to know the vehicle's transformation
class VehicleSensor
{
	scene::ISceneNode *parent;
	core::line3df ray;
	core::line3df rayTransformed;

public:
	VehicleSensor(scene::ISceneNode *parent, core::vector3df pos, core::vector3df dir, float length)
	{
		this->parent = parent;
		ray.setLine(pos, pos+(dir.normalize())*length);
	}

	// returns value 0-1 indicating distance of a wall
	// 0 == wall far away (or not at all)
	// 1 == wall very close
	float getWallProximity(scene::ITriangleSelector *collisionTriangles)
	{
		scene::ISceneCollisionManager *scm = engine.getSceneManager()->getSceneCollisionManager();

		core::vector3df point;
		core::triangle3df triangle;
		 scene::ISceneNode* outNode;
		if (scm->getCollisionPoint(rayTransformed, collisionTriangles, point, triangle,outNode))
			return point.getDistanceFrom(rayTransformed.end) / ray.getLength();

		return 0.f;
	}

	float getItemProximity(ItemManager *itemManager)
	{
		// collide rayTransformed with each item
		core::vector3df point;

		if (itemManager->getCollisionPoint(rayTransformed, point))
			return point.getDistanceFrom(rayTransformed.end) / ray.getLength();

		return 0.f;
	}

	/*
	// Was used so that vehicles could detect other vehicles with their sensors.
	// But didn't seem to improve their abilities at all.
	float getVehicleProximity(Vehicle *fromVehicle)
	{
		const std::vector<Vehicle *> &vehicles = engine.getWorld()->getVehicles();

		float highestProximity = 0.f;

		for (int i = 0; i < vehicles.size(); i ++)
		{
			if (vehicles[i] == fromVehicle)
				continue; // exclude the vehicle that is doing the sensing

			core::vector3df closestRayPos = rayTransformed.getClosestPoint(vehicles[i]->getPosition());

			// if intersects with ray
			if (closestRayPos.getDistanceFrom(vehicles[i]->getPosition()) < 0.1)
			{
				float proximity = closestRayPos.getDistanceFrom(rayTransformed.end) / ray.getLength();

				// closest intersection so far?
				if (proximity > highestProximity)
					highestProximity = proximity;
			}
		}

		return highestProximity;
	}
	*/

	void update()
	{
		rayTransformed = ray;

		const core::matrix4 &trans = parent->getAbsoluteTransformation();
		trans.transformVect(rayTransformed.start);
		trans.transformVect(rayTransformed.end);
	}

	void draw()
	{
		engine.getVideoDriver()->draw3DLine(rayTransformed.start, rayTransformed.end, LINE_COLOUR);
	}
};
