#ifndef AZSCRIPTENGINE_H
#define AZSCRIPTENGINE_H

#include <string>

#include "Headers/Azadi.h"
#include "Headers/ModelFactory.h"
#include "Headers/SoundFactory.h"
#include "Headers/TextureFactory.h"
#include "Headers/Drawing.h"
#include "Headers/Physics.h"
#include "Headers/Input.h"

class azScriptEngine
{
    public:
        virtual ~azScriptEngine() {}

        virtual void setWorkingDirectory(const std::string &workingDirectory) = 0;

        virtual bool registerAzadiCore(azCore*) = 0;
        virtual bool registerModelFactory(azModelFactory*) = 0;
        virtual bool registerSoundFactory(azSoundFactory*) = 0;
        virtual bool registerDrawingEngine(azDrawing*) = 0;
        virtual bool registerTextureFactory(azTextureFactory*) = 0;
        virtual bool registerPhysicsEngine(azPhysics*) = 0;
        virtual bool registerInputEngine(azInput*) = 0;

        virtual bool initialize() = 0;
        virtual bool isActive() = 0;
        virtual bool includeFile(const std::string &fileName) = 0;
        virtual bool execute(const std::string &fileName) = 0;

        virtual void findSelectedEntity(const float &mousex, const float &mousey, const float &mousez, const float cam[3]) = 0;

        virtual short returnedShort() = 0;
        virtual int returnedInt() = 0;
        virtual long returnedLong() = 0;
        virtual float returnedFloat() = 0;
        virtual double returnedDouble() = 0;
        virtual bool returnedBool() = 0;
        virtual std::string returnedString() = 0;

        virtual std::string getScriptEngineIdentifier() = 0;
};

#endif
