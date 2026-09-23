# librle

Run-Length Encoding (RLE) biblioteka za  **SrBin OS**.

Namijenjena za kompresiju slika u memoriji — idealna za **undo/redo** i **čuvanje crteža** u aplikacijama kao što je [Bojanka](https://github.com/zmmaj/Bojanka).

---

## Zašto RLE?

RLE (Run-Length Encoding) je jednostavna **lossless** kompresija koja grupiše uzastopne piksele iste boje u parove `[count, color]`.

**Za slike sa velikim površinama iste boje** (kao Paint crteži), RLE daje **odlične rezultate**:

| Slika | Raw | RLE | Kompresija |
|---|---|---|---|
| Prazno platno (500×400) | 800 KB | **8 B** | 100.000× |
| Par linija | 800 KB | ~1 KB | 800× |
| Ispunjen krug | 800 KB | ~400 B | 2000× |
| Kompleksna slika | 800 KB | ~40 KB | 20× |

**Za razliku od PNG** (koji koristi zlib/DEFLATE):
- **Brže** — encode/decode su O(n) sa malim konstantama
- **Manje memorije** — nema buffering-a, nema zlib state
- **Bez zavisnosti** — ne treba `zlib`

---

## API

### Struktura


typedef struct {
    uint32_t *data;      /* [count, color, count, color, ...] */
    size_t    count;     /* broj parova */
    uint32_t  width;
    uint32_t  height;
} rle_image_t;


## Funkcije
Funkcija	Opis
rle_encode(pixels, w, h, pitch, &out)	Encode niz piksela u RLE
rle_decode(&rle, pixels, pitch)	Decode RLE u niz piksela
rle_free(&rle)	Oslobodi memoriju
rle_size_bytes(&rle)	Veličina u bajtovima
rle_clone(&src, &dst)	Duboka kopija
Sve funkcije vraćaju errno_t (EOK, ENOMEM, EINVAL).

## Primer upotrebe
===================================================================================
#include <rle/rle.h>

/* Uzmi piksele iz bitmape */
gfx_bitmap_alloc_t alloc;
gfx_bitmap_get_alloc(bitmap, &alloc);
uint32_t *pixels = (uint32_t *)((uint8_t *)alloc.pixels + alloc.off0);

/* Encode */
rle_image_t rle;
errno_t rc = rle_encode(pixels, width, height, alloc.pitch, &rle);
if (rc != EOK) {
    printf("Greska: %d\n", rc);
    return;
}

printf("RLE: %zu parova, %zu bajtova\n",
    rle.count, rle_size_bytes(&rle));

/* ... kasnije ... */

/* Decode */
rc = rle_decode(&rle, pixels, alloc.pitch);
if (rc != EOK) {
    printf("Greska pri decode: %d\n", rc);
}

/* Oslobodi */
rle_free(&rle);

==============================================================

## RLE poštuje pitch:
-------------------------------------------------
size_t pitch_words = pitch / sizeof(uint32_t);
for (uint32_t y = 0; y < height; y++) {
    const uint32_t *row = pixels + y * pitch_words;
    /* ... */
}
=========================================================

Zašto je ovo važno: bez pitch-a, RLE bi čitao padding kao piksele — što daje pogrešne rezultate i iskrivljenu sliku.

## Ograničenja
Max count po paru: 4.294.967.295 (2³²−1) piksela
Max dimenzije: 2³²−1 × 2³²−1
Memorija: najgori slučaj = 2 × width × height × 4 bajtova (privremeno, pre realloc)

## Autor
ZmajSoft © 2026

## Zahvalnost
Helen OS tim — za gfx_bitmap_t API

Jiri Svoboda — za inspiraciju iz uidemo

Sean Barrett — za stb_image (inspiracija za jednostavan API)

## Doprinosi
Pull requests su dobrodošli.

## Ideje za buduće verzije
□ RLE sa 16-bit bojama (RGB565)
□ RLE sa delta encoding (samo razlike)
□ RLE sa 2D optimizacijom (red po red)
□ Thread-safe verzija
□ Streaming API (za velike slike)
