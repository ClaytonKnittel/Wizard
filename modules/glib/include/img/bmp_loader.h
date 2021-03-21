#ifndef _BMP_LOADER_H
#define _BMP_LOADER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmp_img {
    void * mmap_reg;
    uint64_t mmap_len;

    // pointer to beginning of image data
    void * buf;

    // either GL_RGB or GL_RGBA
    uint32_t img_fmt;

    uint32_t width;
    uint32_t height;
} bmp_img;


int bmp_read(bmp_img * bmp, const char * img_file);

void bmp_close(bmp_img * bmp);

#ifdef __cplusplus
}
#endif


#endif /* _BMP_LOADER_H */
