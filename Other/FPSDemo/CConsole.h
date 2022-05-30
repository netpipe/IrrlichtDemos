#ifndef _CCONSOLE_H_
#define _CCONSOLE_H_

#include <irrlicht.h>

#include "CScriptManager.h"

namespace irr
{
    const short KEY_TILDE = 0xc0;
}

using namespace irr;

namespace agEngine
{
    namespace console
    {
        class CConsole : public virtual IReferenceCounted
        {
        public:
            CConsole(script::CScriptManager* smgr, video::IVideoDriver* vdrv, gui::IGUIEnvironment* genv);
            ~CConsole();

            virtual void draw();
            virtual bool parseEvent(const SEvent& event);
            virtual void updateScreenDimensions(const core::dimension2d<s32>& screen);
            virtual void setTexture(video::ITexture* tex);
            virtual void addOutputToConsole(const core::stringc& output);

            virtual void flush();
        private:
            virtual void execute(const core::stringc& command);

            script::CScriptManager* smgr;
            video::IVideoDriver* vdrv;
            gui::IGUIEnvironment* genv;

            video::ITexture* texture;
            core::dimension2d<s32> screenDimensions;
            core::dimension2d<s32> textureDimensions;
            gui::IGUIStaticText* consoleEntries;
            gui::IGUIEditBox* consoleInput;

            core::stringc consoleBuffer;

            bool visible;
        };
    }
}

#endif // _CCONSOLE_H

