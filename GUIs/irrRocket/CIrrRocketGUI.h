#ifndef CIRRROCKETGUI_H
#define CIRRROCKETGUI_H

namespace irr
{
    class IrrlichtDevice;
    class IEventReceiver;
    namespace scene
    {
        class ICameraSceneNode;
    }
}
namespace Rocket
{
    namespace Core
    {
        class Context;
    }
}
class CIrrRocketEventReceiver;
class CIrrRocketGUI
{
    public:
        /** Default constructor */
        CIrrRocketGUI(irr::IrrlichtDevice* device);
        /** Default destructor */
        virtual ~CIrrRocketGUI();

        irr::IEventReceiver* getGUIEventReceiver(void);

        void run(void);
    protected:
        irr::IrrlichtDevice* Device;
        Rocket::Core::Context* RocketContext;

        irr::scene::ICameraSceneNode* GUI_Camera;
        CIrrRocketEventReceiver* InputEvents;

        static unsigned int RocketContextCount;
    private:
};

#endif // CIRRROCKETGUI_H
