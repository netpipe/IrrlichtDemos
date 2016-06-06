#ifndef BOIDSCENENODE_H_
#define BOIDSCENENODE_H_

#include <IMeshSceneNode.h>

#ifdef _SOUND
#include <irrKlang.h>
#endif

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

	BoidSceneNode(
		f32 radius, u32 polyCountX, u32 polyCountY,
		const core::vector3df& target, ITriangleSelector* selector, irr::f32 borders[4],
		ISceneNode* const parent, ISceneManager* const mgr, const s32 id,
		const core::vector3df& position = core::vector3df(0.0f, 0.0f, 0.0f),
		const core::vector3df& rotation = core::vector3df(0.0f, 0.0f, 0.0f),
		const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

	~BoidSceneNode();

	//To compare boids
	bool operator==(const BoidSceneNode& other) const { return (this->ID == other.ID); }
	bool operator!=(const BoidSceneNode& other) const { return !(*this == other); }

	virtual void OnAnimate(u32 timeMs);

	virtual void render();

	virtual const core::aabbox3d<float>& getBoundingBox() const;

	void OnRegisterSceneNode();

	virtual video::SMaterial& getMaterial(u32 i);

	virtual u32 getMaterialCount() const;

	virtual void setMesh(IMesh*);

	//! Returns the current mesh
	virtual IMesh* getMesh() { return Mesh; }

	virtual void setReadOnlyMaterials(bool);

	virtual bool isReadOnlyMaterials() const;

	void applyRules(
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
		, irrklang::ISoundEngine* const soundEngine = 0, bool soundEnabled = false
#endif
	);

private:

	IMesh* Mesh;

	core::aabbox3d<f32> Box;

	f32 radius;


	ITriangleSelector* selector;

	///The current speed of a boid. Is set by applying the rules to the boid.
	irr::f32 velocity[3];

	///Time to stay on ground in ms
	irr::f32 perchTimer;

	///Boid is currently on ground if true
	bool perching;

	///Time to don't perch
	irr::f32 dontPerchTimer;

	///True if boid should not perch
	bool dontperch;


	///Boids try to fly towards the centre of mass of neighbouring boids.
	irr::f32 rule_1[3];

	///Boids try to keep a small distance away from other objects (including other boids).
	irr::f32 rule_2[3];

	///Boids try to match velocity with near boids.
	irr::f32 rule_3[3];

	///Tendency towards a particular place
	irr::f32 seek[3];

	///Tendency away from a particular place
	irr::f32 avoid[3];

	///Xmin, Xmax, Zmin, Zmax
	irr::f32 borders[4];


	void startPerching(const irr::core::vector3df& outCollisionPoint);

	void stopPerching();

	/* A sphere with proper normals and texture coords */
	IMesh* createSphereMesh(f32 radius, u32 polyCountX, u32 polyCountY);
};

}
}
#endif /* BOIDSCENENODE_H_ */
