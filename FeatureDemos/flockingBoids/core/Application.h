#ifndef APPLICATION_H_
#define APPLICATION_H_

namespace irr
{
	class IrrlichtDevice;
}

#ifdef _SOUND
namespace irrklang
{
	class ISoundEngine;
}
#endif

class Configuration;

class Application
{

public:

	Application();

	~Application();

	bool init();

	void run() const;

private:

	Configuration* const configuration;

	irr::IrrlichtDevice* device;
#ifdef _SOUND
	///A pointer to the irrklang engine.
	irrklang::ISoundEngine* soundEngine;
#endif
};

#endif /* APPLICATION_H_ */
