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
 
#ifndef u8CODEC_S3TC_H
#define AVCODEC_S3TC_H

#include <stdint.h>

#define FF_S3TC_DXT1    0x31545844
#define FF_S3TC_DXT3    0x33545844

#ifndef AV_RL16
#define AV_RL16(x)  ((((const uint8_t*)(x))[1] << 8) | \
                    ((const uint8_t*)(x))[0])
#endif

#ifndef AV_RL32
#define AV_RL32(x) ((((const uint8_t*)(x))[3] << 24) | \
                    (((const uint8_t*)(x))[2] << 16) | \
                    (((const uint8_t*)(x))[1] <<  8) | \
                     ((const uint8_t*)(x))[0])
#endif

#ifndef AV_RL64
#define AV_RL64(x)  (((uint64_t)((const uint8_t*)(x))[7] << 56) | \
                     ((uint64_t)((const uint8_t*)(x))[6] << 48) | \
                     ((uint64_t)((const uint8_t*)(x))[5] << 40) | \
                     ((uint64_t)((const uint8_t*)(x))[4] << 32) | \
                     ((uint64_t)((const uint8_t*)(x))[3] << 24) | \
                     ((uint64_t)((const uint8_t*)(x))[2] << 16) | \
                     ((uint64_t)((const uint8_t*)(x))[1] <<  8) | \
                      (uint64_t)((const uint8_t*)(x))[0])
#endif

/**
 * Decode DXT1 encoded data to RGB32
 * @param *src source buffer, has to be aligned on a 4-byte boundary
 * @param *dst destination buffer
 * @param w width of output image
 * @param h height of output image
 * @param stride line size of output image
 */
void ff_decode_dxt1(const uint8_t *src, uint8_t *dst,
                    const unsigned int w, const unsigned int h,
                    const unsigned int stride);
/**
 * Decode DXT3 encoded data to RGB32
 * @param *src source buffer, has to be aligned on a 4-byte boundary
 * @param *dst destination buffer
 * @param w width of output image
 * @param h height of output image
 * @param stride line size of output image
 */
void ff_decode_dxt3(const uint8_t *src, uint8_t *dst,
                    const unsigned int w, const unsigned int h,
                    const unsigned int stride);

#endif /* AVCODEC_S3TC_H */
