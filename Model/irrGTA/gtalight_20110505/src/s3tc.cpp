/* s3tc.c -- S3TC Decoding Functions

 Copyright (C) 2007 by Ivo van Poorten

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the author be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgement in the product documentation would be
    appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */
 
#include "s3tc.h"

static inline void dxt1_decode_pixels(const uint8_t *s, uint32_t *d,
                                      unsigned int qstride, unsigned int flag,
                                      uint64_t alpha) {
    unsigned int x, y, c0, c1, a = (!flag * 255) << 24;
    unsigned int rb0, rb1, rb2, rb3, g0, g1, g2, g3;
    uint32_t colors[4], pixels;

    c0 = AV_RL16(s);
    c1 = AV_RL16(s+2);

    rb0  = (c0<<3 | c0<<8) & 0xf800f8;
    rb1  = (c1<<3 | c1<<8) & 0xf800f8;
    rb0 +=        (rb0>>5) & 0x070007;
    rb1 +=        (rb1>>5) & 0x070007;
    g0   =        (c0 <<5) & 0x00fc00;
    g1   =        (c1 <<5) & 0x00fc00;
    g0  +=        (g0 >>6) & 0x000300;
    g1  +=        (g1 >>6) & 0x000300;

    colors[0] = rb0 + g0 + a;
    colors[1] = rb1 + g1 + a;

    if (c0 > c1 || flag) {
        rb2 = (((2*rb0+rb1) * 21) >> 6) & 0xff00ff;
        rb3 = (((2*rb1+rb0) * 21) >> 6) & 0xff00ff;
        g2  = (((2*g0 +g1 ) * 21) >> 6) & 0x00ff00;
        g3  = (((2*g1 +g0 ) * 21) >> 6) & 0x00ff00;
        colors[3] = rb3 + g3 + a;
    } else {
        rb2 = ((rb0+rb1) >> 1) & 0xff00ff;
        g2  = ((g0 +g1 ) >> 1) & 0x00ff00;
        colors[3] = 0;
    }

    colors[2] = rb2 + g2 + a;

    pixels = AV_RL32(s+4);
    for (y=0; y<4; y++) {
        for (x=0; x<4; x++) {
            a        = (alpha & 0x0f) << 28;
            a       += a >> 4;
            d[x]     = a + colors[pixels&3];
            pixels >>= 2;
            alpha  >>= 4;
        }
        d += qstride;
    }
}

void ff_decode_dxt1(const uint8_t *s, uint8_t *dst,
                    const unsigned int w, const unsigned int h,
                    const unsigned int stride) {
    unsigned int bx, by, qstride = stride/4;
    uint32_t *d = (uint32_t *) dst;

    for (by=0; by < h/4; by++, d += stride-w)
        for (bx=0; bx < w/4; bx++, s+=8, d+=4)
            dxt1_decode_pixels(s, d, qstride, 0, 0LL);
}

void ff_decode_dxt3(const uint8_t *s, uint8_t *dst,
                    const unsigned int w, const unsigned int h,
                    const unsigned int stride) {
    unsigned int bx, by, qstride = stride/4;
    uint32_t *d = (uint32_t *) dst;

    for (by=0; by < h/4; by++, d += stride-w)
        for (bx=0; bx < w/4; bx++, s+=16, d+=4)
            dxt1_decode_pixels(s+8, d, qstride, 1, AV_RL64(s));
}
