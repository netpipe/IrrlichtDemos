#ifndef GUI_H_
#define GUI_H_

#include <irrlicht.h>
#include <vector>

enum 
{
	GUI_ID_CONNECT_SERVER = 101,
	GUI_ID_JOIN_CHANNEL_BUTTON,
	GUI_ID_JOIN_CHANNEL_DLG,
	GUI_ID_CONNECT_SERVER_DLG,
	GUI_ID_JOIN_CHANNEL_OK,
	GUI_ID_SERVER_CONNECT_OK,
	GUI_ID_JOIN_CHANNEL_NAME_EDIT,
	GUI_ID_SERVER_ADDRESS_EDIT,
	GUI_ID_SERVER_PORT_EDIT
};

#define GUI_ID_TREE_NODE	200

using namespace irr;
using namespace core;
using namespace gui;

class Gui
{
public:
    Gui(IGUIEnvironment* env);

	void addLine(const core::stringw& who, const core::stringw& message);
	
	bool addChannelTab(core::stringw channel);

	void removeChannelTab(core::stringw channel);

	void addServerChild(core::stringw server);

	core::stringc getActiveTabName();
	

	void ShowJoinChannel();

	void ShowConnectServer();

	IGUIEditBox* getChatBox();
	
	IGUIListBox* getChatMembers();

	IGUIListBox* getChatHistory();
	
	IGUIEditBox* getChannelNameEdit() { return this->editChannelName;  }

	IGUIWindow* getJoinChannelDlg() { return this->dlgJoinChannel;  }

	IGUIWindow * getConnectServerDlg() { return this->dlgConnectServer;  }
	
	IGUIEditBox* getServerAddressEdit() { return this->editServerAddress;  }

	IGUIEditBox* getServerPortEdit() { return this->editServerPort;  }

	IGUITabControl* getCurTabServer();

	IGUITreeViewNode* getSelectedNode() { return this->treeServer->getSelected(); }

	u32 getSelectedNodeIdx();
	
	u32 getServerIdx(core::stringw serverName);

	void SetVisibleTabeServer(u32 idx);

public:
    IGUIListBox* list;
    IGUIButton* btnConnectServer;
	IGUIButton* btnJoinChannel;

	IGUITreeView* treeServer;
	IGUITreeViewNode* treeRootNode;
	std::vector<core::stringw> listServerAddress;

	IGUIWindow* dlgJoinChannel;

	IGUIWindow* dlgConnectServer;

	IGUIButton* btnJoinChannelOk;

	IGUIButton* btnServerConnectOk;
	
private:
	IGUIEnvironment* guienv;

	std::vector<IGUITabControl*> tabServerList;

	IGUIListBox* mox;
	
	IGUIEditBox* box;

	//join channel dlg
	IGUIEditBox* editChannelName;
	
	//connect to server dlg
	IGUIEditBox* editServerAddress;
	IGUIEditBox* editServerPort;


	std::vector<IGUIListBox*> listChatHistory;
	std::vector<IGUIEditBox*> listChatBox;
	std::vector<IGUIListBox*> listChatMemebers;

};

#endif /* GUI_H_ */
