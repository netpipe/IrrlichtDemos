// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef ADVERT_H_
#define ADVERT_H_

class TiXmlElement;

//! Type of ads
enum E_ADVERT_TYPE
{
	//! No ads
	EAT_NONE = -1,

	//! Ads at the bottom of the screen
	EAT_BOTTOM,

	//! Ads at the top of the screen
	EAT_TOP,

	//! Fullscreen ads
	EAT_FULLSCREEN,

	E_ADVERT_TYPE_COUNT
};

class IAdvert
{
public:
	virtual ~IAdvert()
	{
		for (int i = 0; i < int(E_ADVERT_TYPE_COUNT); ++i)
			remove((E_ADVERT_TYPE)i);
	}

	virtual void init(const TiXmlElement * provider) {}

	//! Request to download a new advert
	virtual void request(E_ADVERT_TYPE pType) {}

	//! Remove an advert
	virtual void remove(E_ADVERT_TYPE pType) {}

	//! Show the ad
	// returns false when it had no advert downloaded. Following calls might succeed when a request finishes downloading.
	virtual bool show(E_ADVERT_TYPE pType) { return false; }
};

#endif

