// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef BILLING_HPP
#define BILLING_HPP

#include <string>

class TiXmlElement;

enum E_PURCHASE_STATE
{
	EPS_PURCHASED = 0,
	EPS_CANCELED = 1,
	EPS_REFUNDED = 2
};

class IBilling
{
public:
	virtual ~IBilling() {}

	virtual void init(const TiXmlElement * provider) {}

	virtual void requestServerConnnection()  {}

	virtual void requestPurchase(const std::string& articleName)  {}

	// 0 is SYNCHRONIZED = 0, 1 unconnected, 2 connecting, 3 request running and < 0 are errors.
	// See STATE_ variables in BillingHelper.java for more details.
	virtual int getSynchronizeState()  { return 1; }

	// Number of items which can be bought
	virtual int getNumAvailableItems()  { return 1; }

	// Get the name of an item which can be bought - this is the name to use in requestPurchase
	virtual std::string getItemName(int index)  { return std::string("test"); }

	// Get a string with the item price (I think in local language automatically)
	virtual std::string getItemPrice(int index)  { return std::string("0 €"); }

	// Get amount of items which are already bought
	virtual int getNumPurchasedItems()  { return 0; }

	// Get name of bought item
	virtual std::string getPurchasedItem(int index)  { return std::string(""); }
};

#endif	// BILLING_HPP
