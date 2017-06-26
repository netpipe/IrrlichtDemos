
class World;

// things for the Vehicles to collect
// the purpose of their existance
class ItemManager
{
	World *world;
	
	int itemCount;
	std::vector<scene::ISceneNode *> items;
	
	void createItem(int i)
	{
		items[i] = engine.getSceneManager()->addSphereSceneNode(0.05, 4);
		// at random position
		items[i]->setPosition( world->randPos()+core::vector3df(0,0.05,0) );
		items[i]->updateAbsolutePosition();
	}
	
	void removeItem(int i)
	{
		items[i]->remove();
		
		// create a new one to replace it
		createItem(i);
	}
	
public:
	ItemManager(World *world, int itemCount)
	{
		this->world = world;
		this->itemCount = itemCount;
		
		items.assign(itemCount, (scene::ISceneNode *)NULL);
		
		for (int i = 0; i < itemCount; i ++)
			createItem(i);
	}
	
	~ItemManager()
	{
		for (int i = 0; i < items.size(); i ++)
			items[i]->remove();
	}
	
	bool collectItem(core::vector3df pos)
	{
		for (int i = 0; i < items.size(); i ++)
		{
			if (pos.getDistanceFrom(items[i]->getPosition()) < 0.2)
			{
				removeItem(i);
				return true;
			}
		}
		return false;
	}
	
	bool getCollisionPoint(const core::line3df &ray, core::vector3df &point)
	{
		std::vector<core::vector3df> collisions;
		
		// through all items, find which collide with ray, add collision point to list.
		
		for (int i = 0; i < items.size(); i ++)
		{
			core::vector3df closest = ray.getClosestPoint(items[i]->getPosition());
			
			if (closest.getDistanceFrom(items[i]->getPosition()) < 0.1)
				collisions.push_back(items[i]->getPosition());
		}
		
		// return the collision point that is closest to ray.start
		if (collisions.size() > 0)
		{
			point = collisions[0];
			
			float lastDist = 100000.f;
			
			for (int i = 0; i < collisions.size(); i ++)
			{
				float dist = ray.start.getDistanceFromSQ(collisions[i]);
				
				if (dist < lastDist)
				{
					lastDist = dist;
					point = collisions[i];
				}
			}
			return true;
		}
		
		return false;
	}
};

