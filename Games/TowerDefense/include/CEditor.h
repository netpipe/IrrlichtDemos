#ifndef CEDITOR_H
#define CEDITOR_H

#include <CState.h>
#include <CMap.h>
#include <CInput_catcher.h>

class CEditor : public CState, public IMouseReceiver, public IKeyboardReceiver
{
    public:
        /** Default constructor */
        CEditor(CStateManager* manager);
        /** Default destructor */
        virtual ~CEditor();

        void update(irr::f32 timediff);

        void key_event(const irr::SEvent::SKeyInput& key);
        void mouse_event(const irr::SEvent::SMouseInput& mouse);
    protected:
        void becomeMainState(void);
        void becomeSlaveState(void);

        //sprites
        CMap Map;
        E_TILE_TYPE Type;
        bool Set;
        irr::core::stringc FileName;
        irr::gui::IGUIStaticText* Info;

    private:
};

#endif // CEDITOR_H
