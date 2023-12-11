#include <irrlicht.h>

#pragma comment(lib, "Irrlicht.lib")

#include "CGUITextBox.h"

int main(void)
{
	irr::IrrlichtDevice * device = irr::createDevice(irr::video::EDT_SOFTWARE);
	if(!device)
	{
		return 1;
	}
	irr::video::IVideoDriver * pDrv = device->getVideoDriver();
	irr::scene::ISceneManager * pSmgr = device->getSceneManager();
	irr::gui::IGUIEnvironment * pEnv = device->getGUIEnvironment();

	irr::gui::IGUIWindow * pWnd = pEnv->addWindow(
		irr::core::rect<irr::s32>(20, 40, 340,280), false, L"Text");

	// create a big Text, the GUITextBox is created just after it.
	wchar_t text[] =
	L"(21)Lorem ipsum dolor sit amet, consectetuer"\
	L"\n(20)adipiscing elit. Praesent vestibulum"\
	L"\n(19)molestie lacus. Aenean nonummy"\
	L"\n(18)hendrerit mauris. Phasellus porta. Fusce"\
	L"\n(17)suscipit varius mi. Cum sociis natoque"\
	L"\n(16)penatibus et magnis dis parturient"\
	L"\n(15)montes, nascetur ridiculusmustumridiculusmustum."\
	L"\n(14)Lorem ispum dolor sit amet. Consects adicing eli. Il lumini amet sit."\
	L"\n(13)Praesent vestibulum molestie lacus:"\
	L"\n(12)Patriot - lorem ispum dolor sit amet. (93 works)"\
	L"\n(11)Max - lorem ispum dolor sit amet. (76 works)"\
	L"\n(10)Lector - lorem ispum dolor sit amet. (21 works)"\
	L"\n(9)Stropter - lorem ispum dolor sit amet. (85 works)"\
	L"\n(8)Dolor sit amet, consectetuer elit."\
	L"\n(7)Praesent vestibulum molestie lacus."\
	L"\n(6)dolor sit amet, consectetuer. Consect"\
	L"\n(5)eli. As pasum univer. Il lumini."\
	L"\n(4)Sit amet, consectetuer adipiscing elit."\
	L"\n(3)molestie lacus.Lorem ipsum dolor sit"\
	L"\n(2)Consect adipiscing eli. \n"\
	L"\n(1)GNU GENERAL PUBLIC LICENSEVersion 2, June 1991\n\nCopyright (C) 1989, 1991"\
	L" Free Software Foundation, Inc.  51 Franklin Street, Fifth Floor, Boston, MA  021"\
	L" 10-1301, USA\n\nEveryone is permitted to copy and distribute verbatim copiesof this li"\
	L"cense document, but changing it is not allowed.\n\nPreamble\nThe licenses for most softw"\
	L"are are designed to take away your freedom to share and change it. By contrast, the"\
	L" GNU General Public License is intended to guarantee your freedom to share and chang"\
	L"e free software--to make sure the software is free for all its users.\n This General P"\
	L"ublic License applies to most of the Free Software Foundation's software and to any o"\
	L"ther program whose authors commit to using it.\n(Some other Free Software Foundation "\
	L"software is covered by the GNU Lesser General Public License instead.)\nYou can apply "\
	L"\nit to your programs, too.\nWhen we speak of free software, we are referring to freedom,"\
	L" not price. Our General Public Licenses are designed to make sure that you have the f"\
	L"reedom to distribute copies of free software (and charge for this service if you wish"\
	L"), that you receive source code or can get it if you want it, that you can change the"\
	L" software or use pieces of it in new free programs; and that you know you can do "\
	L" things.\n\nTo protect your rights, we need to make restrictions that forbid anyone to de"\
	L"ny you these rights or to ask you to surrender the rights. These restrictions "\
	L" to certain responsibilities for you if you distribute copies of the software, or if yo"\
	L"u modify it.\n\nFor example, if you distribute copies of such a program, whether gratis or "\
	L"for a fee, you must give the recipients all the rights that you have. You must make sur"\
	L"e that they, too, receive or can get the source code. And you must show them these term"\
	L"s so they know their rights.\n\nWe protect your rights with two steps:\n (1) copyright the so"\
	L"\nftware, and\n (2) offer you this license which gives you legal permission to copy, distri"\
	L"bute and/or modify the software.\n\nAlso, for each author's protection and ours, we want to"\
	L" make certain that everyone understands that there is no warranty for this free "\
	L". If the software is modified by someone else and passed on, we want its recipients to "\
	L"know that what they have is not the original, so that any problems introduced by others"\
	L" will not reflect on the original authors' reputations.\n\nFinally, any free program is thr"\
	L"eatened constantly by software patents. We wish to avoid the danger that "\
	L" of a free program will individually obtain patent licenses, in effect making the progr"\
	L"am proprietary. To prevent this, we have made it clear that any patent must be licensed"\
	L" for everyone's free use or not licensed at all.The precise terms and conditions for co"\
	L"pying, distribution and modification follow.\n\nTERMS AND CONDITIONS FOR COPYING, DISTRIBUT"\
	L"\nION AND MODIFICATION.\nThis License applies to any program or other work which contains"\
	L" a notice placed by the copyright holder saying it may be distributed under the terms o"\
	L"f this General Public License. The \"Program\", below, refers to any such program or wo"\
	L"rk, and a \"work based on the Program\" means either the Program or any derivative "\
	L" under copyright law: that is to say, a work containing the Program or a portion of [..]";

	irr::gui::CGUITextBox * textBox = 0;

	// Create the Text Box
	textBox = new irr::gui::CGUITextBox(pEnv->getBuiltInFont(),L"Test Text", pEnv,
		irr::core::rect<irr::s32>(5, 20, 150,235),
		pWnd, -1);

	// Set the Scrollbar to the left of the Text.
	textBox->setScrollbarRight(false);
	// Scroll the text per pixel, not per line.
	textBox->setScrollModeLines(false);


	// Resizing of the Text Box is also possible.
	textBox->setRelativePosition(irr::core::rect<irr::s32>(5, 20, 315,150));

	// Set the text to something different.
	textBox->setText(text);

	while(device->run())
	{
		pDrv->beginScene(true, true, irr::video::SColor(0xffffff));

		pSmgr->drawAll();
		pEnv->drawAll();

		pDrv->endScene();
	}
}
