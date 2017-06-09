#ifndef CMAIN_H
#define CMAIN_H

#include <IEventReceiver.h>
namespace irr
{
    class IrrlichtDevice;
    namespace scene
    {
        class IMetaTriangleSelector;
        class IMeshBuffer;
        class IAnimatedMeshSceneNode;
    }
}
class CDecal;
class CGUIEventDispatcher;
class CMain : public irr::IEventReceiver
{
    public:
        /** Default constructor */
        CMain();
        /** Default destructor */
        virtual ~CMain();

        void run(void);

        bool OnEvent(const irr::SEvent& event);
    protected:
        irr::IrrlichtDevice* Device;
        irr::scene::IMetaTriangleSelector* MetaSelect;
        irr::scene::IMeshBuffer* DecalBuffer;
        CDecal* Decal;
        CGUIEventDispatcher* GUIEvents;
        //irr::scene::IAnimatedMeshSceneNode* node;
    private:
};

#endif // CMAIN_H
