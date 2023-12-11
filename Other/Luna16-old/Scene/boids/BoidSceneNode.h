#ifndef BOIDSCENENODE_H_
#define BOIDSCENENODE_H_

#include <IMeshSceneNode.h>

namespace irrklang
{
	class ISoundEngine;
}

namespace irr
{
namespace scene
{

/**
 * This class implemnts a single boid in a flock
 *
 * See http://www.vergenet.net/~conrad/boids/pseudocode.html
 *
 * Rules of a boid in a flock:
 *  Separation: steer to avoid crowding local flockmates
 *  Alignment: steer towards the average heading of local flockmates
 *  Cohesion: steer to move toward the average position of local flockmates
 */
class BoidSceneNode : public irr::scene::IMeshSceneNode
{

public:

	BoidSceneNode(irr::scene::IMesh* const boidMesh, const core::vector3df& target, const irr::f32 borders[4], ISceneManager* const mgr, const s32 id = -1);

	~BoidSceneNode();

	//To compare boids
	bool operator==(const BoidSceneNode& other) const { return (this->ID == other.ID); }
	bool operator!=(const BoidSceneNode& other) const { return !(*this == other); }

	void OnAnimate(u32 timeMs);

	void render();

	const core::aabbox3d<float>& getBoundingBox() const;

	void OnRegisterSceneNode();

	video::SMaterial& getMaterial(u32 i);

	u32 getMaterialCount() const;

	void setMesh(IMesh*);

	//! Returns the current mesh
	IMesh* getMesh() { return Mesh; }

	void setReadOnlyMaterials(bool);

	bool isReadOnlyMaterials() const;

	void applyRules(
		irr::scene::ITriangleSelector* selector,
		const irr::core::array<BoidSceneNode*>& boids,
		const irr::f32 distanceToOtherBoids,
		const irr::f32 seekCenterOfMass,
		const irr::f32 matchVelocity,
		const irr::core::vector3df& target,
		const irr::f32 tendencyTowardsPlace,
		const irr::f32 mimimumAboveGround,
		const irr::f32 tendencyAvoidPlace,
		const bool scatterFlock,
		const irr::f32 scatterFlockModifier,
		const irr::f32 deltaTime,
		const irr::f32 speedLimit
#ifdef _SOUND
		, irrklang::ISoundEngine* const soundEngine = 0, const bool soundEnabled = false
#endif
	);

private:

	void startPerching(const core::vector3df& outCollisionPoint);

	void stopPerching();

	///The mesh of the boid.
	IMesh* const Mesh;

	///The boundingbox.
	core::aabbox3d<f32> Box;

	///Material to draw the mesh.
	video::SMaterial material;

	//Material for debug drawing.
	video::SMaterial debugMat;



	///only Y for now
	f32 radius;


	///The current speed of a boid. Is set by applying the rules to the boid.
	f32 velocity[3];



	///Boid is currently on ground if true
	bool perching;

	///Time to stay on ground in ms
	f32 perchTimer;

	///Time the boid is not allowed to perch again after perching.
	f32 dontPerchTimer;



	///Boids try to fly towards the centre of mass of neighbouring boids.
	f32 rule_1[3];

	///Boids try to keep a small distance away from other objects (including other boids).
	f32 rule_2[3];

	///Boids try to match velocity with near boids.
	f32 rule_3[3];


	///Tendency towards a particular place
	f32 seek[3];

	///Tendency away from a particular place
	f32 avoid[3];


	///Xmin, Xmax, Zmin, Zmax
	f32 borders[4];

	///Velocity to avoid borders.
	f32 avoidBorders[2];


	///The ray to test for terrain collision.
	 irr::core::line3d<irr::f32> groundRay;
};

}
}
#endif /* BOIDSCENENODE_H_ */
