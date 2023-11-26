#include "gui.h"

Gui::Gui(IGUIEnvironment* env)
{
	guienv = env;

    IGUIFont* const font = guienv->getFont("fontlucida.png");
    if (font)
        guienv->getSkin()->setFont(font);

    const core::dimension2d<u32>& screenSize = guienv->getVideoDriver()->getScreenSize();

	core::dimension2d<u32> szButton(150, 30);
	core::vector2d<u32> posServerBtn(30, 10);
	core::vector2d<u32> posChannelBtn(200, 10);

	btnConnectServer = guienv->addButton(core::rect<int>(posServerBtn.X, posServerBtn.Y, posServerBtn.X + szButton.Width, posServerBtn.Y + szButton.Height), 0, GUI_ID_CONNECT_SERVER, L"Connect to Server");
	
	btnJoinChannel = guienv->addButton(core::rect<int>(posChannelBtn.X, posChannelBtn.Y, posChannelBtn.X + szButton.Width, posChannelBtn.Y + szButton.Height), 0, GUI_ID_JOIN_CHANNEL_BUTTON, L"Join a Channel");
	btnJoinChannel->setEnabled(false);

	treeServer = guienv->addTreeView(core::rect<int>(10, 50, 210, screenSize.Height - 10));
	treeRootNode = treeServer->getRoot()->addChildBack(L"Server List");
	treeRootNode->setExpanded(true);
}

void Gui::addLine(const core::stringw& who, const core::stringw& message)
{
    core::stringw bla(who);
    bla += L" ";
    bla += message;

	s32 idx = getCurTabServer()->getActiveTab();
	if (idx < 0 || idx >= listChatHistory.size())
		return;

	IGUIListBox* chathistory = listChatHistory[idx];

	chathistory->addItem(bla.c_str());
	chathistory->setSelected(chathistory->getItemCount()-1);

}

bool Gui::addChannelTab(core::stringw channel)
{
	bool bFindTab = false;
	if (getCurTabServer() == NULL)
		return false;

	s32 i;
	for (i = 0; i < getCurTabServer()->getTabCount(); i++)
	{
		IGUITab* tab = getCurTabServer()->getTab(i);
		core::stringw name(tab->getText());
		if (channel.equals_ignore_case(name)) {
			bFindTab = true;
			break;
		}
	}
	if (bFindTab)
		return false;

	IGUITab* tab = getCurTabServer()->addTab(channel.c_str(), -1);
	
	const core::vector2d<u32> tabcontrolSize(570, 390);

	IGUIListBox* chatlist = guienv->addListBox(core::rect<int>(10, 10, tabcontrolSize.X - 160, tabcontrolSize.Y - 100), tab, -1, true);
	chatlist->setAutoScrollEnabled(true);
	listChatHistory.push_back(chatlist);
	
	IGUIEditBox* chatbox = guienv->addEditBox(L"", core::rect<int>(10, tabcontrolSize.Y - 90, tabcontrolSize.X - 160, tabcontrolSize.Y - 50), true, tab, -1);
	listChatBox.push_back(chatbox);

	IGUIListBox* chatmembers = guienv->addListBox(core::rect<int>(tabcontrolSize.X - 150, 10, tabcontrolSize.X - 10, tabcontrolSize.Y - 50), tab, -1, true);
	listChatMemebers.push_back(chatmembers);

	getCurTabServer()->setActiveTab(tab);

	return true;
}

void Gui::removeChannelTab(core::stringw channel)
{
	IGUITabControl* curTabServer = getCurTabServer();
	bool bFindTab = false;

	s32 i;
	for ( i = 0; i < curTabServer->getTabCount(); i++)
	{
		IGUITab* tab = curTabServer->getTab(i);
		core::stringw name(tab->getText());
		if (channel.equals_ignore_case(name)) {
			bFindTab = true;
			break;
		}
	}

	if (bFindTab) {
		if (i != 0) {
			curTabServer->setActiveTab(0);
		}
		
		IGUITab* tab = curTabServer->getTab(i);
		
		tab->removeChild(listChatHistory[i]);
		listChatHistory.erase(listChatHistory.begin() + i);


		tab->removeChild(listChatBox[i]);
		listChatBox.erase(listChatBox.begin() + i);

		tab->removeChild(listChatMemebers[i]);
		listChatMemebers.erase(listChatMemebers.begin() + i);
		
		curTabServer->removeTab(i);

	}
	
		

}

void Gui::addServerChild(core::stringw server)
{
	const core::dimension2d<u32>& screenSize = guienv->getVideoDriver()->getScreenSize();

	IGUITreeViewNode* node = treeRootNode->addChildBack(server.c_str());
	node->setSelected(true);
	listServerAddress.push_back(server);
	
	IGUITabControl* tab = guienv->addTabControl(core::rect<int>(220, 50, screenSize.Width - 10, screenSize.Height - 10), 0, true, true, -1);
	tabServerList.push_back(tab);
	SetVisibleTabeServer(tabServerList.size() - 1);

	
}

core::stringc Gui::getActiveTabName()
{
	u32 idx = getCurTabServer()->getActiveTab();
	IGUITab* tab = getCurTabServer()->getTab(idx);
	
	return core::stringc(tab->getText());
	
}

void Gui::ShowJoinChannel()
{
	const core::dimension2d<u32>& screenSize = guienv->getVideoDriver()->getScreenSize();
	dimension2d<u32> sz(200, 150);
	core::vector2d<u32> pos((screenSize.Width - sz.Width) / 2, (screenSize.Height - sz.Height) / 2);

	dlgJoinChannel = guienv->addWindow(rect<int>(pos.X, pos.Y, pos.X + sz.Width, pos.Y + sz.Height), true, L"Join a channel", 0, GUI_ID_JOIN_CHANNEL_DLG);
	
	guienv->addStaticText(L"Channel name:", rect<int>(30, 30, 150, 50), false, false, dlgJoinChannel);
	editChannelName = guienv->addEditBox(L"", rect<int>(20, 50, 170, 75), true, dlgJoinChannel, GUI_ID_JOIN_CHANNEL_NAME_EDIT);

	btnJoinChannelOk = guienv->addButton(rect<int>(50, 100, 150, 130), dlgJoinChannel, GUI_ID_JOIN_CHANNEL_OK, L"Join");

	btnJoinChannel->setEnabled(true);
}

void Gui::ShowConnectServer()
{
	const core::dimension2d<u32>& screenSize = guienv->getVideoDriver()->getScreenSize();
	dimension2d<u32> sz(250, 200);
	core::vector2d<u32> pos((screenSize.Width - sz.Width) / 2, (screenSize.Height - sz.Height) / 2);

	dlgConnectServer = guienv->addWindow(rect<int>(pos.X, pos.Y, pos.X + sz.Width, pos.Y + sz.Height), true, L"Connect to Server", 0, GUI_ID_CONNECT_SERVER_DLG);

	guienv->addStaticText(L"Server Address:", rect<int>(30, 30, 230, 50), false, false, dlgConnectServer);
	editServerAddress = guienv->addEditBox(L"", rect<int>(20, 50, 230, 75), true, dlgConnectServer, GUI_ID_SERVER_ADDRESS_EDIT);

	guienv->addStaticText(L"Server Port:", rect<int>(30, 90, 230, 110), false, false, dlgConnectServer);
	editServerPort = guienv->addEditBox(L"", rect<int>(20, 110, 230, 135), true, dlgConnectServer, GUI_ID_SERVER_ADDRESS_EDIT);

	btnServerConnectOk = guienv->addButton(rect<int>(50, 160, 200, 190), dlgConnectServer, GUI_ID_SERVER_CONNECT_OK, L"Connect");
	
	editServerAddress->setText(L"127.0.0.1");
	editServerPort->setText(L"6697");
}

IGUIEditBox * Gui::getChatBox()
{
	if (getCurTabServer() == NULL)
		return NULL;

	u32 idx = getCurTabServer()->getActiveTab();
	if (idx < 0 || idx >= listChatBox.size())
		return NULL;
	
	return listChatBox[idx];
}

IGUIListBox * Gui::getChatMembers()
{
	if (getCurTabServer() == NULL)
		return NULL;

	u32 idx = getCurTabServer()->getActiveTab();
	if (idx < 0 || idx >= listChatMemebers.size())
		return NULL;

	return listChatMemebers[idx];
	
}

IGUIListBox * Gui::getChatHistory()
{
	if (getCurTabServer() == NULL)
		return NULL;

	u32 idx = getCurTabServer()->getActiveTab();
	return listChatHistory[idx];
}

IGUITabControl * Gui::getCurTabServer()
{
	u32 curNode = getSelectedNodeIdx();
	if (curNode < 0 || curNode >= tabServerList.size())
		return NULL;
	else
		return tabServerList[curNode];
}

u32 Gui::getSelectedNodeIdx()
{
	IGUITreeViewNode* node = treeServer->getSelected();
	core::stringw nodename(node->getText());

	return getServerIdx(nodename);
}

u32 Gui::getServerIdx(core::stringw serverName)
{
	u32 idx = -1;
	for (u32 i = 0; i < listServerAddress.size(); i++) {
		if (serverName == listServerAddress[i]) {
			idx = i;
			break;
		}
	}
	return idx;
}

void Gui::SetVisibleTabeServer(u32 idx)
{
	if (idx > tabServerList.size())
		return;
	for (int i = 0; i < tabServerList.size(); i++) {
		if (idx == i)
			tabServerList[i]->setVisible(true);
		else
			tabServerList[i]->setVisible(false);
	}
}

