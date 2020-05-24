#ifndef _TEXTURE_H
#define _TEXTURE_H


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>



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



#endif /* _TEXTURE_H */
