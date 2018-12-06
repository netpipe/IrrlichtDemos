#ifndef IRRROCKETINTERFACE_H
#define IRRROCKETINTERFACE_H

#include <Rocket/Core/SystemInterface.h>
namespace irr
{
    class ITimer;
}

class IrrRocketInterface : public Rocket::Core::SystemInterface
{
    public:
        /** Default constructor */
        IrrRocketInterface(irr::ITimer* timer);
        /** Default destructor */
        virtual ~IrrRocketInterface();

        float GetElapsedTime();
    protected:
        irr::ITimer* IrrTimer;
    private:
};

#endif // IRRROCKETINTERFACE_H
