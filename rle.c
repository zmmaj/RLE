/*
 * Copyright (c) 2026 <tvoje ime>
 * All rights reserved.
 *
 * ... (ista BSD licenca kao gore)
 */

/** @addtogroup rle
 * @{
 */
/** @file
 */

 #include <errno.h>
 #include <stdint.h>
 #include <stddef.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 
 #include "rle.h"
 
 errno_t rle_encode(const uint32_t *pixels, uint32_t width, uint32_t height,
    size_t pitch, rle_image_t *out)
{
    if (!pixels || !out || width == 0 || height == 0)
        return EINVAL;

    size_t pitch_words = pitch / 4;   /* pitch u 32-bitnim rečima */
    size_t total = (size_t)width * height;

    uint32_t *buf = malloc(total * 2 * sizeof(uint32_t));
    if (!buf) return ENOMEM;

    size_t idx = 0;
    uint32_t current = 0;
    uint32_t run = 0;
    bool first = true;

    for (uint32_t y = 0; y < height; y++) {
        const uint32_t *row = pixels + y * pitch_words;
        for (uint32_t x = 0; x < width; x++) {
            uint32_t px = row[x];
            if (first) {
                current = px;
                run = 1;
                first = false;
            } else if (px == current) {
                run++;
            } else {
                buf[idx++] = run;
                buf[idx++] = current;
                current = px;
                run = 1;
            }
        }
    }
    buf[idx++] = run;
    buf[idx++] = current;

    /* ... realloc i postavi out ... */
    out->data = buf;
    out->count = idx / 2;
    out->width = width;
    out->height = height;
    return EOK;
}
 
errno_t rle_decode(const rle_image_t *rle, uint32_t *pixels, size_t pitch)
{
    if (!rle || !rle->data || !pixels)
        return EINVAL;

    size_t pitch_words = pitch / 4;
    size_t total = (size_t)rle->width * rle->height;

    size_t pos = 0;
    for (size_t i = 0; i < rle->count; i++) {
        uint32_t run = rle->data[i * 2];
        uint32_t color = rle->data[i * 2 + 1];

        for (uint32_t j = 0; j < run; j++) {
            uint32_t y = pos / rle->width;
            uint32_t x = pos % rle->width;
            pixels[y * pitch_words + x] = color;
            pos++;
        }
    }

    return (pos == total) ? EOK : EINVAL;
}
 
 void rle_free(rle_image_t *rle)
 {
     if (!rle)
         return;
     if (rle->data) {
         free(rle->data);
         rle->data = NULL;
     }
     rle->count = 0;
     rle->width = 0;
     rle->height = 0;
 }
 
 size_t rle_size_bytes(const rle_image_t *rle)
 {
     if (!rle)
         return 0;
     return rle->count * 2 * sizeof(uint32_t);
 }
 
 errno_t rle_clone(const rle_image_t *src, rle_image_t *dst)
 {
     if (!src || !dst || !src->data)
         return EINVAL;
 
     size_t bytes = src->count * 2 * sizeof(uint32_t);
     uint32_t *copy = malloc(bytes);
     if (!copy)
         return ENOMEM;
 
     memcpy(copy, src->data, bytes);
 
     dst->data = copy;
     dst->count = src->count;
     dst->width = src->width;
     dst->height = src->height;
     return EOK;
 }
 
 /** @}
  */
