// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MODELLIST_H
#define GUI_MODELLIST_H

#include "../compile_config.h"

#ifdef HC1_ENABLE_EDITOR

#include "../irrlicht_manager.h"

class GuiEditor;

class GuiModelList
{
public:
    GuiModelList(irr::gui::IGUIEnvironment* env_, GuiEditor * editor_);
    ~GuiModelList();

    void Show();
    void Close();
    bool OnEvent(const irr::SEvent &event);

private:
    irr::gui::IGUIEnvironment*   mEnvironment;
    irr::gui::IGUIListBox * mListModels;
    GuiEditor * mEditor;
};

#endif // HC1_ENABLE_EDITOR

#endif // GUI_MODELLIST_H
