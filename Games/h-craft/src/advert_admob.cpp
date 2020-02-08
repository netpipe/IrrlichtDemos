// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.
// This cod is based on code from the "Chernobyl Game Engine" which was put under the zlib license by Patryk Nadrowski.
//
// TODO(mz): I have no idea how often the AttachCurrentThread for the JNIEnv is necessary. Maybe only once and this can be rewritten. Needs testing.

#include "compile_config.h"
#include "advert_admob.h"
#include "tinyxml/tinyxml.h"

#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#include <android/log.h>// for the occasional debugging, style: __android_log_print(ANDROID_LOG_VERBOSE, "Irrlicht", "%s\n", "We do log");

#ifndef HOVER_RELEASE
	#define CHECK_JNI(obj) if (!obj) {__android_log_print(ANDROID_LOG_VERBOSE, "Irrlicht", "%s:%d\n", __FILE__, __LINE__); }
#else	// release
	#define CHECK_JNI(obj)
#endif

AdvertAdmob::AdvertAdmob(android_app& androidApp)
: IAdvert()
, mAndroidApp(androidApp)
{
}

AdvertAdmob::~AdvertAdmob()
{
}

void AdvertAdmob::init(const TiXmlElement * provider)
{
	const char * bottom = 0;
	const char * top = 0;
	const char * fullscreen = 0;
	if (provider)
	{
		const TiXmlNode * nodeAdUnit = provider->FirstChild("ad_unit");
		if (nodeAdUnit)
		{
			const TiXmlElement* elementAdUnit = nodeAdUnit->ToElement();
			if (elementAdUnit)
			{
				bottom = elementAdUnit->Attribute("bottom");
				top = elementAdUnit->Attribute("top");
				fullscreen = elementAdUnit->Attribute("fullscreen");
			}
		}

		// Add all test-devices. You have to do that for each device which you use for testing ads.
		// Be careful - clicking real ads in testing can get you kicked out of Admob.
		const TiXmlNode* nodeTestDevice = provider->FirstChild("test_device");
		while ( nodeTestDevice )
		{
			const TiXmlElement* elementTestDevice = nodeTestDevice->ToElement();
			if (elementTestDevice)
			{
				const char * id = elementTestDevice->Attribute("id");
				if (id)
					addTestDevice(id);
			}
			nodeTestDevice = nodeTestDevice->NextSibling();
		}
	}

	// For some reason the test-id only works for banner-ads, but not for full-screen (interstitial).
	// Could not find a test-id for interstitial ads so far.
	// So to have test-ads there you have to add all devices you test on as test-device.
	// I still wonder if I'm missing something here as that's more than just a little confusing...
	const char test_id[] = "ca-app-pub-3940256099942544/6300978111";		// Test-id from Google

	setAdUnitID(bottom ? bottom : test_id, EAT_BOTTOM);
	setAdUnitID(top ? top : test_id, EAT_TOP);
	setAdUnitID(fullscreen ? fullscreen : "", EAT_FULLSCREEN);
}

void AdvertAdmob::request(E_ADVERT_TYPE pType)
{
	if (pType != EAT_NONE )
	{
		JNIEnv* jniEnv = getJNIEnv();
		if (jniEnv)
		{
			jobject tObjectActivity = mAndroidApp.activity->clazz;
			CHECK_JNI(tObjectActivity)
			jclass tClassActivity = jniEnv->GetObjectClass(tObjectActivity);
			jmethodID tMethodGetAdHelper = jniEnv->GetMethodID(tClassActivity, "getAdHelper", "()Lcom/irrgheist/hcraft_championship/AdHelper;");
			jobject tObjectAdHelper = jniEnv->CallObjectMethod(tObjectActivity, tMethodGetAdHelper);

			jclass tClassAdHelper = jniEnv->GetObjectClass(tObjectAdHelper);
			jmethodID tMethodRequest = jniEnv->GetMethodID(tClassAdHelper, "request", "(I)V");
			CHECK_JNI(tMethodRequest)

			jint tValue = int(pType);
			jniEnv->CallVoidMethod(tObjectAdHelper, tMethodRequest, tValue);

			jniEnv->DeleteLocalRef(tClassAdHelper);
			jniEnv->DeleteLocalRef(tObjectAdHelper);
			jniEnv->DeleteLocalRef(tClassActivity);

			dropJNIEnv();
		}
	}
}

void AdvertAdmob::remove(E_ADVERT_TYPE pType)
{
	if ( pType != EAT_NONE )
	{
		JNIEnv* jniEnv = getJNIEnv();
		if (jniEnv)
		{
			jobject tObjectActivity = mAndroidApp.activity->clazz;
			CHECK_JNI(tObjectActivity)
			jclass tClassActivity = jniEnv->GetObjectClass(tObjectActivity);
			jmethodID tMethodGetAdHelper = jniEnv->GetMethodID(tClassActivity, "getAdHelper", "()Lcom/irrgheist/hcraft_championship/AdHelper;");

			jobject tObjectAdHelper = jniEnv->CallObjectMethod(tObjectActivity, tMethodGetAdHelper);
			jclass tClassAdHelper = jniEnv->GetObjectClass(tObjectAdHelper);
			jmethodID tMethodRemove = jniEnv->GetMethodID(tClassAdHelper, "remove", "(I)V");
			CHECK_JNI(tMethodRemove)

			jint tValue = int(pType);
			jniEnv->CallVoidMethod(tObjectAdHelper, tMethodRemove, tValue);

			jniEnv->DeleteLocalRef(tClassAdHelper);
			jniEnv->DeleteLocalRef(tObjectAdHelper);
			jniEnv->DeleteLocalRef(tClassActivity);

			dropJNIEnv();
		}
	}
}

bool AdvertAdmob::show(E_ADVERT_TYPE pType)
{
	if (pType != EAT_NONE)
	{
		JNIEnv* jniEnv = getJNIEnv();

		if (jniEnv)
		{
			jobject tObjectActivity = mAndroidApp.activity->clazz;
			CHECK_JNI(tObjectActivity)
			jclass tClassActivity = jniEnv->GetObjectClass(tObjectActivity);
			jmethodID tMethodGetAdHelper = jniEnv->GetMethodID(tClassActivity, "getAdHelper", "()Lcom/irrgheist/hcraft_championship/AdHelper;");
			jobject tObjectAdHelper = jniEnv->CallObjectMethod(tObjectActivity, tMethodGetAdHelper);
			jclass tClassAdHelper = jniEnv->GetObjectClass(tObjectAdHelper);
			jmethodID tMethodShow = jniEnv->GetMethodID(tClassAdHelper, "show", "(I)Z");
			CHECK_JNI(tMethodShow)
			jint tValue = int(pType);
			jboolean result = jniEnv->CallBooleanMethod(tObjectAdHelper, tMethodShow, tValue);
			jniEnv->DeleteLocalRef(tClassAdHelper);
			jniEnv->DeleteLocalRef(tObjectAdHelper);
			jniEnv->DeleteLocalRef(tClassActivity);

			dropJNIEnv();

			if (result)
				return true;
		}
	}

	return false;
}

void AdvertAdmob::setAdUnitID(const char* id, E_ADVERT_TYPE pType)
{
	if ( pType != EAT_NONE )
	{
		JNIEnv* jniEnv = getJNIEnv();
		if (jniEnv)
		{
			jobject tObjectActivity = mAndroidApp.activity->clazz;
			CHECK_JNI(tObjectActivity)
			jclass tClassActivity = jniEnv->GetObjectClass(tObjectActivity);
			jmethodID tMethodGetAdHelper = jniEnv->GetMethodID(tClassActivity, "getAdHelper", "()Lcom/irrgheist/hcraft_championship/AdHelper;");

			jobject tObjectAdHelper = jniEnv->CallObjectMethod(tObjectActivity, tMethodGetAdHelper);
			jclass tClassAdHelper = jniEnv->GetObjectClass(tObjectAdHelper);
			jmethodID tMethodSetAdUnitID = jniEnv->GetMethodID(tClassAdHelper, "setAdUnitID", "(Ljava/lang/String;I)V");
			CHECK_JNI(tMethodSetAdUnitID)

			jstring tId = jniEnv->NewStringUTF(id);
			CHECK_JNI(tId)
			jint tValue = int(pType);
			jniEnv->CallVoidMethod(tObjectAdHelper, tMethodSetAdUnitID, tId, tValue);

			jniEnv->DeleteLocalRef(tId);
			jniEnv->DeleteLocalRef(tClassAdHelper);
			jniEnv->DeleteLocalRef(tObjectAdHelper);
			jniEnv->DeleteLocalRef(tClassActivity);

			dropJNIEnv();
		}
	}
}

void AdvertAdmob::addTestDevice(const char* device)
{
	JNIEnv* jniEnv = getJNIEnv();
	if (jniEnv)
	{
		jobject tObjectActivity = mAndroidApp.activity->clazz;
		CHECK_JNI(tObjectActivity)
		jclass tClassActivity = jniEnv->GetObjectClass(tObjectActivity);
		jmethodID tMethodGetAdHelper = jniEnv->GetMethodID(tClassActivity, "getAdHelper", "()Lcom/irrgheist/hcraft_championship/AdHelper;");

		jobject tObjectAdHelper = jniEnv->CallObjectMethod(tObjectActivity, tMethodGetAdHelper);
		jclass tClassAdHelper = jniEnv->GetObjectClass(tObjectAdHelper);
		jmethodID tMethodAddTestDevice = jniEnv->GetMethodID(tClassAdHelper, "addTestDevice", "(Ljava/lang/String;)V");
		CHECK_JNI(tMethodAddTestDevice)

		jstring tDevice = jniEnv->NewStringUTF(device);
		CHECK_JNI(tDevice)
		jniEnv->CallVoidMethod(tObjectAdHelper, tMethodAddTestDevice, tDevice);

		jniEnv->DeleteLocalRef(tDevice);
		jniEnv->DeleteLocalRef(tClassAdHelper);
		jniEnv->DeleteLocalRef(tObjectAdHelper);
		jniEnv->DeleteLocalRef(tClassActivity);

		dropJNIEnv();
	}
}

JNIEnv* AdvertAdmob::getJNIEnv()
{
	JNIEnv* jni = 0;
	mAndroidApp.activity->vm->AttachCurrentThread(&jni, NULL);
	return jni;
}

void AdvertAdmob::dropJNIEnv()
{
	mAndroidApp.activity->vm->DetachCurrentThread();
}

#endif
