// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef ADVERT_ADMOB_H_
#define ADVERT_ADMOB_H_

#include "advert.h"

struct android_app;
struct _JNIEnv;
typedef struct _JNIEnv JNIEnv;

class AdvertAdmob : public IAdvert
{
public:
	AdvertAdmob(android_app& androidApp);
	~AdvertAdmob();

	virtual void init(const TiXmlElement * provider);

	//! Request to download an app
	virtual void request(E_ADVERT_TYPE pType);

	//! Remove an add
	virtual void remove(E_ADVERT_TYPE pType);

	//! Show the ad
	virtual bool show(E_ADVERT_TYPE pType);

protected:
	void setAdUnitID(const char* id, E_ADVERT_TYPE pType);
	void addTestDevice(const char* device);

private:
	AdvertAdmob(const AdvertAdmob&);	// no implementation

	JNIEnv* getJNIEnv();
	void dropJNIEnv();

	android_app& mAndroidApp;
};

#endif

