#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <irrString.h>
#include <SIrrCreationParameters.h>

/**
 * An object of this class holds all the variables the user may edit plus
 * some basic stuff like the name and version of the game.
 */
class Configuration
{

public:

	///The constructor sets up sane default values.
	Configuration();

	///Reads the configuration from an xml file.
	void readFromFile(const irr::c8* const fileName);

	///Writes the configuration to an xml file.
	void writeToFile(const irr::c8* const fileName) const;

	// getter

	inline const irr::core::stringw& getProjectName() const
	{ return this->projectName; }

	inline const irr::SIrrlichtCreationParameters& getIrrlichtParams() const
	{ return this->params; }

	inline const irr::u32 getGuiTransparency() const
	{ return this->guiTransparency; }
#ifdef _SOUND
	inline const bool isSoundEnabled() const
	{ return this->soundEnabled; }
#endif

	inline const irr::f32 getCameraSpeed() const
	{ return this->cameraSpeed; }

	inline const irr::f32 getScatterFlockModifier() const
	{ return this->scatterFlockModifier; }

	inline const irr::f32 getSeekCenterOfMass() const
	{ return this->seekCenterOfMass; }

	inline const irr::f32 getDistanceToOtherBoids() const
	{ return this->distanceToOtherBoids; }

	inline const irr::f32 getMatchVelocity() const
	{ return this->matchVelocity; }

	inline const irr::f32 getTendencyTowardsPlace() const
	{ return this->tendencyTowardsPlace; }

	inline const irr::f32 getTendencyAvoidPlace() const
	{ return this->tendencyAvoidPlace; }

	inline const irr::f32 getMimimumAboveGround() const
	{ return this->minimumAboveGround; }

	inline const irr::f32 getSpeedLimit() const
	{ return this->speedLimit; }

	//setter

#ifdef _SOUND
	inline void setSoundEnabled(const bool enable)
	{ this->soundEnabled = enable; }
#endif

private:

	///Holds the name of the game.
	irr::core::stringw projectName;

	//stuff user can change

	///For Irrlicht device creation.
	irr::SIrrlichtCreationParameters params;
#ifdef _SOUND
	///true if player wants to hear sounds
	bool soundEnabled;
#endif
	///Transparency of gui.
	irr::u32 guiTransparency;

	///Speed in units per millisecond with which the camera is moved.
	irr::f32 cameraSpeed;


	//flock

	irr::f32 scatterFlockModifier;

	irr::f32 seekCenterOfMass;

	irr::f32 distanceToOtherBoids;

	irr::f32 matchVelocity;

	irr::f32 tendencyTowardsPlace;

	irr::f32 tendencyAvoidPlace;

	irr::f32 minimumAboveGround;

	irr::f32 speedLimit;
};

#endif /*CONFIGURATION_H_*/
