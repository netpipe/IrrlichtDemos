
#ifndef GCN_IRRLICHTINPUT_HPP
#define GCN_IRRLICHTINPUT_HPP

#include <queue>

#include "irrlicht.h"

#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
    class Key;
    
    /**
    * Irrlicht implementation of Input.
    */
    class GCN_EXTENSION_DECLSPEC IrrlichtInput : public Input
    {
    public:

        /**
        * Constructor.
        */
        IrrlichtInput(irr::IrrlichtDevice* device);

        /**
        * Pushes an Irrlicht event. It should be called at least once per frame to
        * update input with user input.
        *
        * @param event an event from Irrlicht.
        */
        virtual void pushInput(const irr::SEvent& event);

        /**
        * Polls all input. It exists for input driver compatibility. If you
        * only use Irrlicht and plan sticking with Irrlicht you can safely ignore this
        * function as it in the Irrlicht case does nothing.
        */
        virtual void _pollInput() { }


        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

    protected:
        /**
        * Converts an Irrlicht event to a Guichan key value.
        *
        * @param keysym The Irrlicht event to convert.
        * @return A Guichan key value. -1 if no conversion took place.
        * @see Key
        */
        int convertIrrlichtEventToGuichanKeyValue(const irr::SEvent& event);

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        irr::IrrlichtDevice* mIrrlichtDevice;
    };
}

#endif // end GCN_IRRLICHTINPUT_HPP
