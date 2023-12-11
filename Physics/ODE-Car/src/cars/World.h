#pragma once
#include <irrlicht.h>
#include <ode/ode.h>

class CCar;

/**
*	World Singleton.
*/
class CWorld : public irr::IReferenceCounted
{
private:
	static CWorld * m_instance;

	irr::scene::ISceneManager	*m_scene_manager;
	irr::io::IFileSystem		*m_file_system;
	irr::scene::ISceneNode		*m_world_root,
								*m_cars_root;

	dWorldID					m_world;
	dSpaceID					m_space;
	dJointGroupID				m_contactgroup;
	dGeomID						m_ground;
	
	irr::core::vector3df		m_default_camera_pos,		//! place to move camera during main menu is active
								m_default_camera_target;	//! place to look camera during main menu is active
	
	irr::core::vector3df		m_start_car_pos,			//! place to put player game at start
								m_start_car_rot;			//! player car start orientation

	bool						m_running;

	//! collusion callback
	static void collusionCallback(void *data, dGeomID o1, dGeomID o2);
	
	//! load graphic and physic part of scene node
	void	loadSceneNode(const irr::io::IXMLReader * reader, irr::scene::IMeshCache * cache);

	//! create ODE box based on AABB
	dGeomID	createPhysicBox(const irr::scene::IMesh * mesh, const irr::core::vector3df &pos);

	//! create ODE TriMesh based on IMesh
	dGeomID	createPhysicMesh(const irr::scene::IMesh * mesh, const irr::core::vector3df &pos);

	//! Constructor
	CWorld(irr::scene::ISceneManager * smgr, irr::io::IFileSystem * fs);
public:
	//! create single instance of world
	static CWorld*	create(irr::scene::ISceneManager * smgr, irr::io::IFileSystem * fs);
	//! destroy world
	virtual ~CWorld(void);

	void								startSimulation() { m_running = true; }
	void								stopSimulation() { m_running = false; }

	//! clear scene without cameras, lights
	void								clearScene();

	//! load scene from XML
	bool								loadScene(const irr::io::path &path);

	void								animate(float time);

	inline irr::scene::ISceneManager*	getSceneManager() { return m_scene_manager; }

	inline irr::scene::ISceneNode*		getWorldRoot() { return m_world_root; }
	inline irr::scene::ISceneNode*		getCarsRoot() { return m_cars_root; }

	inline dWorldID						getPhysicWorld() { return m_world; }
	inline dSpaceID						getPhysicSpace() { return m_space; }

	const irr::core::vector3df&			getDefaultCameraPos() const { return m_default_camera_pos; }
	const irr::core::vector3df&			getDefaultCameraTarget() const { return m_default_camera_target; }


	//! load car from file
	CCar*	createCar(const irr::io::path &path, 
					const irr::core::vector3df &position = irr::core::vector3df(0.0f, 0.0f, 0.0f), 
					const irr::core::vector3df &rotation = irr::core::vector3df(0.0f, 0.0f, 0.0f));

	//! remove car from list
	void	removeCar(CCar * car);
};

