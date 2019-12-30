#ifndef ENCUSERTOOLS_H
#define ENCUSERTOOLS_H

#include "Blowfish.h"

namespace Encryption
{
	namespace UserTools
	{
		class bCipher
		{
		public:
			bCipher(const char* key, const unsigned int &keySize);
			~bCipher();

			void encipher(void *data, const unsigned int &bytes, void *backbuffer, unsigned int *padding);
			void decipher(void *data, const unsigned int &bytes, void *backbuffer, const unsigned int &padding);

			static bCipher *instance;
		private:
			Blowfish *bfenc;
		};
	}
}

#endif
