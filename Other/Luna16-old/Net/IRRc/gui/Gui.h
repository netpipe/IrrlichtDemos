#ifndef GUI_H_
#define GUI_H_

class Gui
{

public:
irr::gui::IGUIListBox* list;

	Gui(irr::gui::IGUIEnvironment* const guienv)
	{
		irr::gui::IGUIFont* const font = guienv->getFont("./media/fontlucida.png");
		if (font)
			guienv->getSkin()->setFont(font);


		const irr::core::dimension2du& screenSize = guienv->getVideoDriver()->getScreenSize();
		const irr::core::vector2d<irr::s32> tabcontrolSize(700, 500);
		const irr::core::vector2d<irr::s32> pos((screenSize.Width-tabcontrolSize.X)/2, (screenSize.Height-tabcontrolSize.Y)/2);

		irr::gui::IGUITabControl* const tabcontrol = guienv->addTabControl(irr::core::rect<int>(pos.X, pos.Y, pos.X + tabcontrolSize.X, pos.Y + tabcontrolSize.Y), 0, true, true, -1);


		irr::gui::IGUITab* tab = tabcontrol->addTab(L"#Channel", -1);
		irr::gui::IGUITab* tab2 = tabcontrol->addTab(L"#PM", -1);

/*
		mox = guienv->addEditBox(L"", irr::core::recti(10, 10, tabcontrolSize.X-160, tabcontrolSize.Y-100), true, tab, -1);
		mox->setTextAlignment(irr::gui::EGUIA_UPPERLEFT, irr::gui::EGUIA_UPPERLEFT);
		mox->setAutoScroll(true);
*/

		mox = guienv->addListBox(irr::core::rect<int>(10, 10, tabcontrolSize.X-160, tabcontrolSize.Y-100), tab, -1, true);
		mox->setAutoScrollEnabled(true);

		box = guienv->addEditBox(L"", irr::core::rect<int>(10, tabcontrolSize.Y-90, tabcontrolSize.X-160, tabcontrolSize.Y-50), true, tab, -1);


        list = guienv->addListBox(irr::core::rect<int>(tabcontrolSize.X-150, 10, tabcontrolSize.X-10, tabcontrolSize.Y-50), tab, -1, true);
	//	list->addItem(L"VarmintTheRat");
	//	list->addItem(L"randomMesh");
	//	list->addItem(L"rooly");
       // list->addItem(L"Tecan");

	}

	void addLine(const irr::core::stringw& who, const irr::core::stringw& message) const
	{
		irr::core::stringw bla(who);
		bla += L" ";
		bla += message;
		mox->addItem(bla.c_str());
		mox->setSelected(mox->getItemCount()-1);
	}

	inline irr::gui::IGUIListBox* const getMox() const { return this->mox; }

	inline irr::gui::IGUIEditBox* const getBox() const { return this->box; }

private:

	irr::gui::IGUIListBox* mox;

	irr::gui::IGUIEditBox* box;

};

#endif /* GUI_H_ */
