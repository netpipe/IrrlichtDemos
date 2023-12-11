#include "Blowfish.h"
#include "encKeys.h"

const short Rounds = 16;

unsigned long Encryption::Blowfish::F(unsigned long x)
{
    unsigned short a, b, c, d;
    unsigned long y;

    d = x & 0x00FF;
    x >>= 8;
    c = x & 0x00FF;
    x >>= 8;
    b = x & 0x00FF;
    x >>= 8;
    a = x & 0x00FF;

    y = sbox[0][a] + sbox[1][b];
    y = y ^ sbox[2][c];
    y = y + sbox[3][d];

    return y;
}

void Encryption::Blowfish::encipher(unsigned long *xl, unsigned long *xr)
{
    unsigned long Xl;
    unsigned long Xr;
    unsigned long temp;
    short i;

    Xl = *xl;
    Xr = *xr;

    for (i = 0; i < 16; ++i)
    {
        Xl = Xl ^ parray[i];
        Xr = F(Xl) ^ Xr;

        temp = Xl;
        Xl = Xr;
        Xr = temp;
    }

    temp = Xl;
    Xl = Xr;
    Xr = temp;

    Xr = Xr ^ parray[Rounds];
    Xl = Xl ^ parray[Rounds + 1];

    *xl = Xl;
    *xr = Xr;
}

void Encryption::Blowfish::decipher(unsigned long *xl, unsigned long *xr)
{
    unsigned long Xl;
    unsigned long Xr;
    unsigned long temp;
    short i;

    Xl = *xl;
    Xr = *xr;

    for (i = Rounds + 1; i > 1; --i)
    {
        Xl = Xl ^ parray[i];
        Xr = F(Xl) ^ Xr;

        temp = Xl;
        Xl = Xr;
        Xr = temp;
    }

    temp = Xl;
    Xl = Xr;
    Xr = temp;

    Xr = Xr ^ parray[1];
    Xl = Xl ^ parray[0];

    *xl = Xl;
    *xr = Xr;
}

void Encryption::Blowfish::initialize(const char *key, short keybytes)
{
    short i;
    short j;
    short k;
    unsigned long data;
    unsigned long datal;
    unsigned long datar;

    j = 0;
    for (i = 0; i < Rounds + 2; ++i)
    {
        data = 0x00000000;
        for (k = 0; k < 4; ++k)
        {
            data <<= 8;
            data |= (unsigned long)key[j] & 0xFF;
            ++j;
            if (j >= keybytes)
            {
                j = 0;
            }
        }
        parray[i] = parray[i] ^ data;
    }

    datal = 0x00000000;
    datar = 0x00000000;

    for (i = 0; i < Rounds + 2; i+=2)
    {
        encipher(&datal, &datar);
        parray[i] = datal;
        parray[i+1] = datar;
    }

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 256; j+=2)
        {
            encipher(&datal, &datar);

            sbox[i][j] = datal;
            sbox[i][j + 1] = datar;
        }
    }
}

