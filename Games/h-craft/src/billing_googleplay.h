// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.
//
// Code was based on work from Patryk Nadrowski's "Chernobyl Game Engine"
// But I reduced it to the minimum needced for my project and then did some more changes.
// So not much left of the original code. That code was also under the zlib license.

#ifndef BILLING_GOOGLE_PLAY_HPP
#define BILLING_GOOGLE_PLAY_HPP

#include "billing.h"

struct android_app;
struct _JNIEnv;
typedef struct _JNIEnv JNIEnv;

class BillingGooglePlay : public IBilling
{
public:
	BillingGooglePlay (android_app& androidApp);
	virtual ~BillingGooglePlay ();

	virtual void init(const TiXmlElement * provider);

	virtual void requestServerConnnection();

	virtual void requestPurchase(const std::string& articleName);

	// 0 is SYNCHRONIZED = 0, 1 unconnected, 2 connecting, 3 request running and < 0 are errors.
	// See STATE_ variables in BillingHelper.java for more details.
	virtual int getSynchronizeState();

	// Number of items which can be bought
	virtual int getNumAvailableItems();

	// Get the name of an item which can be bought - this is the name to use in requestPurchase
	virtual std::string getItemName(int index);

	// Get a string with the item price (I think in local language automatically)
	virtual std::string getItemPrice(int index);

	// Get amount of items which are already bought
	virtual int getNumPurchasedItems();

	// Get name of bought item
	virtual std::string getPurchasedItem(int index);

protected:
	// The article has to be created in the google developer console
	void registerExpectedProduct(const char* articleName);

private:

	//! no copy constructor
	BillingGooglePlay(const BillingGooglePlay& pInAppManager);

	JNIEnv* getJNIEnv();
	void dropJNIEnv();
	int getIntMethod(const char *methodName);
	std::string getIndexedStringMethod(const char *methodName, int index);

	android_app& mAndroidApp;
};

#endif	// BILLING_HPP
