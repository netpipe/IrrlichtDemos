#pragma once

#include <irrlicht.h>

#define MENU_ITEM_WIDTH 100
#define MENU_ITEM_HEIGHT 30
#define MENU_ITEM_Y_OFFSET 10
#define MENU_ITEM_X_OFFSET 10

enum ECmdItem
{
	CMD_ID_START_BUTTON=0,
	CMD_ID_EXIT_BUTTON,
	CMD_ID_CONTINUE_BUTTON,
	CMD_ID_EXIT_MAIN_MENU_BUTTON,
	CMD_ID_CHOOSE_CAR_BUTTON,
	CMD_ID_CHOOSE_RACE_BUTTON,
	CMD_ID_PLAY_BUTTON
};

class CGUI {
public:
	//! create all gui elements
	CGUI(irr::IrrlichtDevice* device);

	//! show FPS in static text
	void showFPS(irr::u32 fps) {
		irr::core::stringw str = L"FPS: ";
		str += fps;
		m_fps_label->setText(str.c_str());
	}

	void showMainMenu();
	void showPauseMenu();

	//! show car file browser
	void showCarBrowser();

	//! show race file browser
	void showRaceBrowser();

	void hideAll()
	{
		hideMenu();
		setBrowserVisible(false);
		m_gui_label->setVisible(false);
	}

	void drawAll()
	{
		m_guienv->drawAll();
	}

	//! get selected path
	irr::io::path	getSelectedFile() const
	{
		return m_file_list->getPath() + m_list_box->getListItem(m_list_box->getSelected());
	}

private:
    irr::IrrlichtDevice* device2;
	irr::gui::IGUIEnvironment	*m_guienv;
	irr::io::IFileSystem		*m_fs;

	irr::gui::IGUIStaticText	*m_gui_label;			//! label of current UI
	irr::gui::IGUIStaticText	*m_fps_label;			//! label with FPS

	irr::gui::IGUIButton		*m_menu_buttons[4];		//! main&pause menu items

	irr::gui::IGUIListBox		*m_list_box;			//! list box for choosing file

	irr::gui::IGUIButton		*m_prev_button,			//! bottom left button
								*m_next_button;			//! bottom right button

	irr::io::IFileList			*m_file_list;			//! current list of files during car&race choosing

	void showMenu(ECmdItem *items, int count);
	void fillFileList(const irr::io::path &folder);

	inline void hideMenu()
	{
		showMenu(NULL, 0);
	}

	void showLabel(const wchar_t * label)
	{
		m_gui_label->setText(label);
		m_gui_label->setVisible(true);
	}

	void setBrowserVisible(bool v)
	{
		m_list_box->setVisible(v);
		m_prev_button->setVisible(v);
		m_next_button->setVisible(v);
	}
};
