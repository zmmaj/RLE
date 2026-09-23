/*
 * Copyright (c) 2026 zmaj
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @addtogroup rle
 * @{
 */
/** @file
 */

 #ifndef LIBC_RLE_H_
 #define LIBC_RLE_H_
 
 #include <stdint.h>
 #include <stddef.h>
 #include <errno.h>
 
 /** RLE kompresovana slika.
  *
  * Format: niz 32-bitnih parova [count, color, count, color, ...].
  * - count: koliko uzastopnih piksela ima istu boju
  * - color: 32-bitni ARGB piksel
  */
 typedef struct {
     uint32_t *data;
     size_t    count;    /* broj parova */
     uint32_t  width;
     uint32_t  height;
 } rle_image_t;
 
 /** Encode: iz niza piksela u RLE.
  *
  * @param pixels Ulazni niz piksela (ARGB8888), veličine width*height
  * @param width  Širina slike
  * @param height Visina slike
  * @param out    Izlazna RLE struktura (alocira se)
  * @return EOK na uspeh, ENOMEM ako nema memorije, EINVAL za loše argumente
  */
 extern errno_t rle_encode(const uint32_t *pixels, uint32_t width,
    uint32_t height, size_t pitch, rle_image_t *out);
 
 /** Decode: iz RLE u niz piksela.
  *
  * @param rle Ulazna RLE struktura
  * @param out Izlazni bafer, veličine rle->width * rle->height
  * @return EOK na uspeh, EINVAL za loše argumente
  */
 extern errno_t rle_decode(const rle_image_t *rle, uint32_t *pixels,
    size_t pitch);
 
 /** Oslobodi memoriju RLE strukture.
  *
  * @param rle RLE struktura
  */
 extern void rle_free(rle_image_t *rle);
 
 /** Veličina RLE strukture u bajtovima.
  *
  * @param rle RLE struktura
  * @return Veličina u bajtovima
  */
 extern size_t rle_size_bytes(const rle_image_t *rle);
 
 /** Duboka kopija RLE strukture.
  *
  * @param src Izvorna RLE struktura
  * @param dst Odredišna RLE struktura (alocira se)
  * @return EOK na uspeh, ENOMEM ako nema memorije
  */
 extern errno_t rle_clone(const rle_image_t *src, rle_image_t *dst);
 
 #endif
 
 /** @}
  */
