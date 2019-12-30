#include "encUserTools.h"

#define UserToolNs Encryption::UserTools

UserToolNs::bCipher *UserToolNs::bCipher::instance = 0;

UserToolNs::bCipher::bCipher(const char *key, const unsigned int &keySize)
{
	instance = this;
	bfenc = new Blowfish();
	bfenc->initialize(key, keySize);
}

UserToolNs::bCipher::~bCipher()
{
	delete bfenc;
}

void UserToolNs::bCipher::encipher(void *data, const unsigned int &bytes, void *backbuffer, unsigned int *padding = 0)
{
	unsigned int totalSize = bytes;
	unsigned int pad = 0;

	// Make sure that the total amount of data is divisible by 8, if not, pad it.
	while ((totalSize / 8) != (float(totalSize) / 8.0f))
	{
		++totalSize;
		++pad;
	}

	// generate the buffers
	char *buffer = new char[totalSize];
	backbuffer = new char[totalSize];

	// Calculate the amount of longs available to us
	unsigned int longs = totalSize / 4;

	// Calculate the amount of pairs we have
	unsigned int pairs = longs / 2;

	// Generate the buffers for the longs
	unsigned long *ubuffer = new unsigned long[longs];

	// define the unions
	union
	{
		unsigned long longx;
		char charx[4];
	} temp, temp2;

	// populate the longs
	char *p = (char*)data;
	for (unsigned int i = 0; i < longs; ++i)
	{
		for (unsigned short j = 0; j < 4; ++j)
			temp.charx[j] = *p++;

		ubuffer[i] = temp.longx;
	}

	// do the encryption
	p = buffer;
    for (unsigned int i = 0; i < pairs; ++i)
    {
        temp.longx = ubuffer[(i*2)];
        temp2.longx = ubuffer[(i*2)+1];
        bfenc->encipher(&temp.longx, &temp2.longx);
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp.charx[z];
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp2.charx[z];
    }

	// Copy the encrypted contents into the return buffer
	p = (char*)backbuffer;
	for (unsigned int i = 0; i < bytes; ++i)
		*p++ = buffer[i];

	if (padding != 0)
		*padding = pad;

	delete [] ubuffer;
	delete [] buffer;
}

void UserToolNs::bCipher::decipher(void *data, const unsigned int &bytes, void *backbuffer, const unsigned int &padding = 0)
{
	// Generate the buffers
	char *buffer = new char[bytes+1];

	// Calculate the amount of longs available to us
	unsigned int longs = bytes / 4;

	// Calculate the amount of pairs we have
	unsigned int pairs = longs / 2;

	// Generate the buffers for the longs
	unsigned long *ubuffer = new unsigned long[longs];

	// define the unions
	union
	{
		unsigned long longx;
		char charx[4];
	} temp, temp2;

	// populate the longs
	char *p = (char*)data;
	for (unsigned int i = 0; i < longs; ++i)
	{
		for (unsigned short j = 0; j < 4; ++j)
			temp.charx[j] = *p++;

		ubuffer[i] = temp.longx;
	}

	// do the decryption
	p = buffer;
    for (unsigned int i = 0; i < pairs; ++i)
    {
        temp.longx = ubuffer[(i*2)];
        temp2.longx = ubuffer[(i*2)+1];
        bfenc->decipher(&temp.longx, &temp2.longx);
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp.charx[z];
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp2.charx[z];
    }

	// Copy the decrypted contents into the return buffer
	p = (char*)backbuffer;
	for (unsigned int i = 0; i < (bytes - padding); ++i)
		*p++ = buffer[i];

	delete [] ubuffer;
	delete [] buffer;
}
