#include "CConsole.h"

namespace agEngine
{
    namespace console
    {
        CConsole::CConsole(script::CScriptManager* scriptManager, video::IVideoDriver* videoDriver, gui::IGUIEnvironment* guiEnvironment)
        : smgr(scriptManager), vdrv(videoDriver), genv(guiEnvironment), consoleEntries(0), visible(false)
        {
            if (scriptManager)
                scriptManager->grab();

            if (videoDriver)
                videoDriver->grab();

            if (guiEnvironment)
                guiEnvironment->grab();
        }

        CConsole::~CConsole()
        {
            consoleEntries->drop();

            genv->drop();
            vdrv->drop();
            smgr->drop();
        }

        void CConsole::draw()
        {
            if (visible)
            {
                vdrv->draw2DImage
                        (
                            texture, core::rect<s32>
                            (0, 0, screenDimensions.Width, screenDimensions.Height/2),
                            core::rect<s32>
                            (0, 0, textureDimensions.Width, textureDimensions.Height)
                        );
            }
        }

        bool CConsole::parseEvent(const SEvent& event)
        {
            if (event.KeyInput.Key == KEY_UP)
            {
                visible = !visible;

                consoleEntries->setVisible(visible);
                consoleInput->setVisible(visible);

                consoleEntries->setEnabled(visible);
                consoleInput->setEnabled(visible);
            }
            else if (event.KeyInput.Key == KEY_RETURN)
            {
                core::stringc temp;
                smgr->execute(core::stringc(consoleInput->getText()), temp);
                addOutputToConsole(temp);
                consoleInput->setText(L"");
            }

            return false;
        }

        void CConsole::execute(const core::stringc& command)
        {
            core::stringc result;
            smgr->execute(command, result);
            consoleBuffer += command + "\n";
            consoleBuffer += result + "\n";

            consoleEntries->setText(core::stringw(consoleBuffer).c_str());
        }

        void CConsole::updateScreenDimensions(const core::dimension2d<s32>& screen)
        {
            screenDimensions = screen;
            if (!consoleEntries)
            {
                consoleEntries = genv->addStaticText(L"", core::rect<s32>(10, 10, screenDimensions.Width-10, (screenDimensions.Height / 2) - 20));
                consoleInput = genv->addEditBox(L"", core::rect<s32>(10, screenDimensions.Height / 2 - 18, screenDimensions.Width - 10, (screenDimensions.Height / 2) - 5));
            }
            else
            {
                consoleEntries->setMinSize(core::dimension2di(screenDimensions.Width - 20, (screenDimensions.Height / 2) - 20));
                consoleInput->setMinSize(core::dimension2di(screenDimensions.Width - 20, 15));
            }

            consoleEntries->setVisible(visible);
            consoleInput->setVisible(visible);

            consoleEntries->setEnabled(visible);
            consoleInput->setEnabled(visible);
        }

        void CConsole::setTexture(video::ITexture* tex)
        {
            texture = tex;
            textureDimensions = tex->getSize();
        }

        void CConsole::addOutputToConsole(const core::stringc& output)
        {
            consoleBuffer += output + "\n";
            consoleEntries->setText(core::stringw(consoleBuffer).c_str());
        }

        void CConsole::flush()
        {
            consoleBuffer = "";
            consoleEntries->setText(L"");
        }
    }
}
