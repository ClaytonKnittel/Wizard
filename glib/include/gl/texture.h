#ifndef _TEXTURE_H
#define _TEXTURE_H


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct texture {
    GLuint tex;
} texture_t;


/*
 * generate texture from bmp file
 */
int texture_init(texture_t *t, const char * img_file);

void texture_destroy(texture_t *t);

/*
 * load a texture for rendering
 */
void texture_use(texture_t *t);

void texture_use_idx(texture_t *t, int idx);


#ifdef __cplusplus
}
#endif


#endif /* _TEXTURE_H */
