#pragma once
#include <assert.h>
#include <irrlicht.h>
#include <ode/ode.h>

#define CHASSIS_MASS 1.0f		// chassis mass
#define WHEEL_MASS 0.2f		// wheel mass

class CWorld;

/**
*	Vehicle.
*	Represents car.\ Consist of  Contain main mesh and texture for clean and damage state.\
*/
class CCar : public irr::IReferenceCounted
{
	irr::core::stringc			m_name;
	CWorld						*m_world;

	irr::scene::ISceneNode		*m_root;
	irr::scene::IMeshSceneNode	*m_chassis,
								*m_glass,
								*m_wheel[4];

	struct graphic {
		irr::scene::IMesh			*clean_model,			
									*damaged_model,			//! if NULL uses clean_model 
									*clean_glass,				
									*damaged_glass,			//! if NULL uses clean_glass 
									*wheel;

		irr::video::ITexture		*clean_chassis_texture,
									*damaged_chassis_texture,	//! if NULL uses clean_chassis_texture 
									*clean_glass_texture,		//! if NULL uses clean_chassis_texture 
									*damaged_glass_texture,		//! if NULL uses clean_glass_texture 
									*wheel_texture;				//! if NULL uses clean_chassis_texture 

		graphic() 
			: clean_model(0), damaged_model(0), clean_glass(0), damaged_glass(0), wheel(0),
			clean_chassis_texture(0), damaged_chassis_texture(0), clean_glass_texture(0), damaged_glass_texture(0), wheel_texture(0) {}   
	
		inline	irr::video::ITexture * getChassisTexture() {
			return clean_chassis_texture;
		}
		inline	irr::video::ITexture * getDamagedChassisTexture() {
			return (damaged_chassis_texture ? damaged_chassis_texture : getChassisTexture());
		}
		inline	irr::video::ITexture * getGlassTexture() {
			return (clean_glass_texture ? clean_glass_texture : getChassisTexture());
		}
		inline	irr::video::ITexture * getDamagedGlassTexture() {
			return (damaged_glass_texture ? damaged_glass_texture : getDamagedChassisTexture());
		}
		inline	irr::video::ITexture * getWheelTexture() {
			return (wheel_texture ? wheel_texture : getChassisTexture());
		} 
	} g;	//! graphic resources

	struct {
		dSpaceID					car_space;
		dBodyID						chassis_body,
									wheel_body[4];
		dGeomID						chassis_geom,
									wheel_geom[4];
		dJointID					joint[4];

		float						chassis_mass,
									wheel_mass;
	} p;	//! physic resources

	bool						m_damaged;

	float						m_speed,
								m_steer,
								m_acceleration,
								m_steer_acceleration,
								m_max_speed;
public:
	enum EWheel {
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		BACK_LEFT,
		BACK_RIGHT
	};

	//! create car and add it to the world
	CCar(CWorld * world);

	//! clean all resources
	virtual ~CCar(void);

	inline void				setName(irr::core::stringc name)	{ m_name = name; }
	inline const irr::c8*	getName() const						{ return m_name.c_str(); }

	//! set clean chassis mesh and physic model
	void	setChassis(irr::scene::IMesh * mesh, irr::video::ITexture * map, const irr::core::vector3df &position, float mass = CHASSIS_MASS);

	//! set clean glass mesh
	void	setGlass(irr::scene::IMesh * mesh, irr::video::ITexture * map = NULL);

	//! add wheel
	void	addWheel(EWheel id, const irr::core::vector3df &position, float mass = WHEEL_MASS);

	//! set wheel's  mesh and physic model
	void	setWheel(irr::scene::IMesh * mesh, irr::video::ITexture * map) {
		g.wheel = mesh;
		g.wheel_texture = map;
	}

	//! set damaged chassis mesh and physic model
	void	setDamagedChassis(irr::scene::IMesh * mesh, irr::video::ITexture * map = NULL) {
		g.damaged_model = mesh;
		g.damaged_chassis_texture = map;	
	}

	//! set damaged glass mesh
	void	setDamagedGlass(irr::scene::IMesh * mesh, irr::video::ITexture * map = NULL) {
		g.damaged_glass = mesh;
		g.damaged_glass_texture = map;
	}

	//! get chassis position
	inline const irr::core::vector3df&	getPosition() const	{
		assert(m_chassis != NULL);
		return m_chassis->getPosition(); 
	}
	//! set car position
	void								setPosition (const irr::core::vector3df &newpos);

	//! get chassis rotation
	inline const irr::core::vector3df&	getRotation() const	{ 
		assert(m_chassis != NULL);
		return m_chassis->getRotation(); 
	}
	//! set chassis rotation
	void								setRotation (const irr::core::vector3df &rotation);

	//! get chassis scene node
	const irr::scene::IMeshSceneNode *	getChassis() const { return m_chassis; }

	//! animate car moving
	void animate();

	//! set car looking as damaged
	void damage();

	//! set car looking as new
	void reset();

	void turnRight(irr::s32 angle = 0);
	void turnLeft(irr::s32 angle = 0);

	void accelerate(irr::s32 force = 0);
	void slowdown(irr::s32 force = 0);
};

