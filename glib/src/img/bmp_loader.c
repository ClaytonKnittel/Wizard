
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <img/bmp_loader.h>


// RGBA only supported
#define BYTES_PER_PIXEL 4


// compression methods
#define BL_RGB            0
#define BL_RLE8           1
#define BL_RLE4           2
#define BL_BITFIELDS      3
#define BL_JPEG           4
#define BL_PNG            5
#define BL_ALPHABITFIELDS 6
#define BL_CMYK           11
#define BL_CMYKRLE8       12
#define BL_CMYRKLE4       13


struct __attribute__((packed)) bmp_header {
    // one of BM, BA, CI, CP, IC, or PT
    char id[2];

    uint32_t size;

    // two reserved 2-byte fields
    uint16_t res[2];

    uint32_t buf_offset;


    uint32_t header_size;

    uint32_t width;
    uint32_t height;

    uint16_t n_color_planes;
    uint16_t bits_per_pixel;

    uint32_t compression_method;
};


int bmp_read(bmp_img * bmp, const char * img_file) {

    int fd = open(img_file, O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "Could not open image file %s\n", img_file);
        return -1;
    }

    struct bmp_header h;

    if (read(fd, &h, sizeof(h)) == -1) {
        fprintf(stderr, "Unable to read %lu bytes from %s, reason "
                "%s", sizeof(h), img_file, strerror(errno));
        close(fd);
        return -1;
    }

    //printf("%.2s\n", h.id);
    //printf("%u\n%u\n%u header size\n%u x %u\n%u bpp\n", h.size, h.buf_offset, h.header_size,
    //        h.width, h.height, h.bits_per_pixel);

    if (h.compression_method != BL_RGB &&
            h.compression_method != BL_BITFIELDS) {
        fprintf(stderr, "Unsupported compression method %u for file %s\n",
                h.compression_method, img_file);
        close(fd);
        return -1;
    }

    uint64_t img_data_size = h.size;

    if (img_data_size != h.buf_offset +
            (h.bits_per_pixel / 8) * h.width * h.height) {
        fprintf(stderr, "Expected image data size %llu = %u * %u (w * h) for "
                "image file %s\n", img_data_size, h.width, h.height, img_file);
        close(fd);
        return -1;
    }

    void * img_mem = mmap(NULL, img_data_size, PROT_READ, MAP_PRIVATE, fd,
            0);

    if (img_mem == MAP_FAILED) {
        fprintf(stderr, "failed to mmap for image file %s, reason %s\n",
                img_file, strerror(errno));
        close(fd);
        return -1;
    }

    close(fd);

    bmp->mmap_reg = img_mem;
    bmp->mmap_len = img_data_size;

    bmp->buf = ((char*) img_mem) + h.buf_offset;
    bmp->width = h.width;
    bmp->height = h.height;

    if (h.compression_method == BL_RGB) {
        bmp->img_fmt = GL_RGB;
    }
    else /* == BL_BITFIELDS */ {
        bmp->img_fmt = GL_BGRA;
    }

    return 0;
}

void bmp_close(bmp_img * bmp) {
    munmap(bmp->mmap_reg, bmp->mmap_len);
}

