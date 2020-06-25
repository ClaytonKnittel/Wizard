

#include <img/bmp_loader.h>

#include <gl/texture.h>
#include <util.h>


/*
 * generate texture from bmp file
 */
int texture_init(texture_t *t, const char * img_file) {

    bmp_img img;
    if (bmp_read(&img, img_file) != 0) {
        return -1;
    }

    GLuint tex;
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0,
            img.img_fmt, GL_UNSIGNED_BYTE, img.buf);

    bmp_close(&img);

    t->tex = tex;

    return 0;
}


void texture_destroy(texture_t *t) {
    glDeleteTextures(1, &t->tex);
}


void texture_use(texture_t *t) {
    texture_use_idx(t, 0);
}

void texture_use_idx(texture_t *t, int idx) {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, t->tex);
}

