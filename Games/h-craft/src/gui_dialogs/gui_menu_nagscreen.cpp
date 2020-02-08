// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_nagscreen.h"
#include "../gui.h"
#include "../main.h"
#include "../config.h"
#include "../billing.h"
#include "../string_table.h"
#include "../start_browser.h"
#include "../app_restrictions.h"
#include "../helper_str.h"
#include "../tinyxml/tinyxml.h"

using namespace irr;
using namespace gui;


GuiMenuNagscreen::GuiMenuNagscreen(const Config& config)
    : GuiDialog(config)
    , mButtonBuy(0)
    , mStaticNagPrice(0)
    , mStaticNagAdvertise(0)
    , mBuyingRequested(false)
{
    SetSuppressSceneRendering(true);
    SetCallOnUpdate(true);
}

GuiMenuNagscreen::~GuiMenuNagscreen()
{
    RemoveFunctors();
}

bool GuiMenuNagscreen::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_quit")), new EventFunctor<GuiMenuNagscreen>(this, &GuiMenuNagscreen::OnButtonQuit) );
        AddGuiEventFunctor( GetIdForName(std::string("id_buy")), new EventFunctor<GuiMenuNagscreen>(this, &GuiMenuNagscreen::OnButtonBuy) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;
        std::string errorMsg("GuiMenuNagscreen::Load");
        mButtonBuy = static_cast<IGUIButton*>(GetElementByName(root, "id_buy", errorMsg));
    }
    return ok;
}

void GuiMenuNagscreen::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_quit")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_buy")) );
}

void GuiMenuNagscreen::Show()
{
    GuiDialog::Show();

	mItemName.clear();
    if ( mButtonBuy )
    {
		if (IS_DEMO_VERSION)
		{
			mButtonBuy->setVisible( GetUrlBuy() ? true : false );
		}
		else if (DO_DISPLAY_ADS)
		{
			IBilling*  billing = APP.GetBilling();
			if ( billing && billing->getNumAvailableItems() == 1 )
			{
				mButtonBuy->setVisible( true );
				mItemName = billing->getItemName(0);

				std::string errorMsg("GuiMenuNagscreen::Show");
				IGUIElement * root = GetDialogParent();
				mStaticNagPrice = static_cast<IGUIStaticText*>(GetElementByName(root, "nag_price", errorMsg));
				mStaticNagAdvertise = static_cast<IGUIStaticText*>(GetElementByName(root, "nag_advertise", errorMsg));

				if ( mStaticNagPrice )
				{
					core::stringw productAndPrice;
					productAndPrice = APP.GetStringTable()->Get(core::stringw(L"id_nag_product"));
					productAndPrice += ExtStr::FromUtf8( mItemName ).c_str();
					productAndPrice += L"\n";
					productAndPrice += APP.GetStringTable()->Get(core::stringw(L"id_nag_price"));
					productAndPrice +=  ExtStr::FromUtf8( billing->getItemPrice(0) ).c_str();
					mStaticNagPrice->setText(productAndPrice.c_str());
				}
			}
			else
			{
				mButtonBuy->setVisible( false );
			}
		}
    }
}

void GuiMenuNagscreen::OnUpdate(irr::u32 timeTick)
{
	if (mBuyingRequested)
	{
		IBilling*  billing = APP.GetBilling();
		if ( billing )
		{
			int syncroState = billing->getSynchronizeState();
			if ( syncroState < 0 )
			{
				// let's just ignore errors. People can still click quit.
				return;
			}
			else if ( syncroState > 0 )
				return;

			mBuyingRequested = false;
			if ( DO_DISPLAY_ADS )
			{
				mButtonBuy->setEnabled(true);
			}
			else
			{
				mButtonBuy->setVisible(false);
				if (mStaticNagPrice)
					mStaticNagPrice->setVisible(false);
				if (mStaticNagAdvertise)
					mStaticNagAdvertise->setText(APP.GetStringTable()->Get("id_nag_thanks").c_str());
			}
		}
	}
}

bool GuiMenuNagscreen::OnButtonQuit(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.StopApp();
    }

    return false;
}

bool GuiMenuNagscreen::OnButtonBuy(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
		if (IS_DEMO_VERSION)
		{
			const char *url = GetUrlBuy();
			if ( url )
			{
				OpenUrl(url);
			}
			APP.StopApp();
		}
		else if (DO_DISPLAY_ADS)
		{
			IBilling*  billing = APP.GetBilling();
			if ( billing )
			{
				// TEST-ID's:
				// billing->requestPurchase("android.test.purchased");
				// billing->requestPurchase("android.test.canceled");
				// billing->requestPurchase("android.test.refunded");
				// billing->requestPurchase("android.test.item_unavailable");

				billing->requestPurchase(mItemName);

				mBuyingRequested = true;
				mButtonBuy->setEnabled(false);
			}
		}
		else if (!mItemName.empty())
		{
			// Maybe bought in the mean-time (shouldn't happen, but not really important)
			mButtonBuy->setEnabled(false);
		}
    }

    return false;
}

const char* GuiMenuNagscreen::GetUrlBuy()
{
    TiXmlElement * xmlEtc = APP.GetConfig()->GetEtcSettings();
    if ( !xmlEtc )
        return NULL;

    const char *url = xmlEtc->Attribute("url_buy");
    if ( url && strlen(url) )
        return url;

    return NULL;
}
