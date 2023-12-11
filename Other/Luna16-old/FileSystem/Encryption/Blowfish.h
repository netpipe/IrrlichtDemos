#ifndef BLOWFISH_H
#define BLOWFISH_H

namespace Encryption
{
    class Blowfish
    {
        public:
            void encipher(unsigned long *xl, unsigned long *xr);
            void decipher(unsigned long *xl, unsigned long *xr);
            void initialize(const char *key, short keybytes);
        private:
            unsigned long F(unsigned long x);
    };
}
#endif
