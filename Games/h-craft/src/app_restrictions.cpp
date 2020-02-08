// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "app_restrictions.h"
#include "billing.h"
#include "config.h"
#include "main.h"
#include "logging.h"
#include "tinyxml/tinyxml.h"

bool AppRestrictions::DoDisplayAds()
{
	PollBillingServer();

    if ( ! APP.GetConfig() )
        return false;

	TiXmlElement * billingElement = APP.GetConfig()->GetBillingSettings();
	if (!billingElement)
		return false;

	int adfree=0;
	if ( billingElement->Attribute("adfree", &adfree ) )
	{
		if (adfree < 0)
			return true;
	}

	return false;
}

int AppRestrictions::PollBillingServer()
{
	IBilling* billing = APP.GetBilling();
	Config* config = APP.GetConfig();

    if (!billing || !config)
        return -1;

	int syncroState = billing->getSynchronizeState();
	if ( syncroState < 0 )
		return -1;
	if ( syncroState > 0 )
		return 1;

	TiXmlElement * billingElement = APP.GetConfig()->GetBillingSettings();
	if (!billingElement)
		return -1;

	int numPurchasedItems = billing->getNumPurchasedItems();

	// we only have one element which can be bought. Could compare names etc... but who cares - easy to cheat this anyway if someone wants to play free that urgently.
	int old_adfree=0;
	billingElement->Attribute("adfree", &old_adfree );
	int new_adfree = numPurchasedItems > 0 ? 1 : -1;
	if (old_adfree != new_adfree)
	{
		if ( new_adfree == 1 )
		{
			std::string purchasedItem(billing->getPurchasedItem(0));
			LOG.LogLn(LP_INFO, "*** bought item: ", purchasedItem.c_str());
		}
		else
		{
			LOG.Info(L"*** sold previously bought item\n");
		}
		billingElement->SetAttribute("adfree", new_adfree);
		config->Save();
	}

	return 0;
}

#if !defined(HOVER_NO_LOCKS)

// Crypto++ Includes
// When using this library disable.
// -fno-rtti
// -fno-exceptions
#include <cstring>
#include "cryptlib.h"
#include "base32.h"     // Base32
#include "hex.h"        // Hex
#include "rijndael.h"   // AES
#include "modes.h"      // CBC_Mode< >
#include "filters.h"    // StringSource and
                        // StreamTransformation

#include <iostream>

using namespace CryptoPP;

// NOTE:
// It's an easy thing to hack for people who know how to work their
// way through binaries. But it's a good enough key-protection for
// people with no clue. It's the kind of stuff game programmers do
// in the hope that it reduces copying in the first 2-3 weeks after
// a release a little bit (after that cracked versions are around
// anyway if it's good enough to be copied by anyone). For
// proprietary games those 2-3 weeks after a release matter often more
// than all the rest of the time. Which is why such solutions make sense
// even if they make crypto guys cringe :-)
bool AppRestrictions::IsKeyUnlocked()
{
    // The features available
    const unsigned int FEATURE_UNLOCK_FULL_VERSION  = 0x01;  //  	0000 0001
    const unsigned int FEATURE_RESERVED1 = 0x02;  	//              0000 0010
    const unsigned int FEATURE_RESERVED2 = 0x04;  	//              0000 0100
    const unsigned int FEATURE_RESERVED3 = 0x08;  	//              0000 1000
    const unsigned int FEATURE_RESERVED4 = 0x0100;  // 0000 0001 0000 0000
    const unsigned int FEATURE_RESERVED5 = 0x0200;  // 0000 0010 0000 0000
    const unsigned int FEATURE_RESERVED6 = 0x0400;  // 0000 0100 0000 0000
    const unsigned int FEATURE_RESERVED7 = 0x0800;  // 0000 1000 0000 0000
    const unsigned int FEATURE_MAGIC = 0x23232323;

    if ( ! APP.GetConfig() )
        return false;

    std::string pubKey;
    if ( !APP.GetConfig()->GetKey(pubKey, APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem()) )
    {
        return false;
    }

    // Key and IV setup
    byte key[Rijndael::DEFAULT_KEYLENGTH] =
        { 0x7A, 0xC0, 0x09, 0x33, 0x21, 0x22, 0x9A, 0xFE,
          0x00, 0x31, 0x64, 0x32, 0x8E, 0xD0, 0x90, 0xAA };

	// initializing vector.
    byte  iv[Rijndael::BLOCKSIZE] =
        { 0x31, 0x44, 0x40, 0x12, 0xB0, 0x66, 0x16, 0x97,
          0xCC, 0xCE, 0xAB, 0xFA, 0x78, 0x63, 0xD3, 0x9F };

	// Prevents at least that the key is in cleartext in the binary
	// Not much harder to hack as it's still in memory.
	byte keyObfuscate[ Rijndael::DEFAULT_KEYLENGTH ] =
        { 0xE4, 0x7A, 0x43, 0x3E, 0xBE, 0xB3, 0x7E, 0x42,
          0x77, 0x12, 0x54, 0xEA, 0xBA, 0xA8, 0x42, 0xE5 };
	for ( int i=0; i < Rijndael::DEFAULT_KEYLENGTH; ++i )
	{
		key[i] ^= keyObfuscate[Rijndael::DEFAULT_KEYLENGTH-(i+1)];
	}

	try
	{
        std::string recoveredText = "";         // Recovered Text Sink
        // Remove Appendage for Pretty Printing
        pubKey = pubKey.substr( 0, pubKey.length() - 2 );

        CBC_Mode<Rijndael>::Decryption decryptor( key, sizeof(key), iv );

        // it will delete the memory itself
        StringSource( pubKey, true,
            new Base32Decoder(
                new StreamTransformationFilter( decryptor,
                    new StringSink( recoveredText )
                ) // StreamTransformationFilter
            ) // Base32Decoder
        ); // StringSource

        // Salt
        unsigned int recoveredSalt = *( (unsigned int*)(recoveredText.substr( 0, 4 ).data() ) );
        // Step Over Salt
        if ( recoveredText.length() < 4 )
            return false;
        recoveredText = recoveredText.substr( 4 );

        // Magic
        unsigned int recoveredMagic =
            *( (unsigned int*)(recoveredText.substr( 0, 4 ).data() ) );
        // Step Over Magic
        if ( recoveredText.length() < 4 )
            return false;
        recoveredText = recoveredText.substr( 4 );

        //            Key Tampering?            //
        if ( FEATURE_MAGIC != recoveredMagic )
            return false;

        // Features
        unsigned int recoveredFeatures =
            *( (unsigned int*)(recoveredText.substr( 0, 4 ).data() ) );
        // Step over Features
        if ( recoveredText.length() < 4 )
            return false;

        // is unlocked?
        if( FEATURE_UNLOCK_FULL_VERSION == ( recoveredFeatures & FEATURE_UNLOCK_FULL_VERSION ) )
            return true;
	}
	catch ( ... )
	{
	    return false;
	}

    return false;
}

#endif // !HOVER_NO_LOCKS
