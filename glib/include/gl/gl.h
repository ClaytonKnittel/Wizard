#ifndef _MGL_H
#define _MGL_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gl/color.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef union wh {
    // width and height of the window
    struct {
        GLint w, h;
    };
    // width and height variable in one 64-bit integer to make
    // modifying their values atomic
    uint64_t wh;
} width_height;

typedef struct gl_context {
    GLFWwindow * window;
    width_height wh;

    // key callback function, forwarded in OpenGL key listener callback
    void (*key_callback)(struct gl_context*, int, int, int, int);

    // for user to specify, will never be modified or read by these methods
    void * user_data;
} gl_context;


int gl_init(gl_context *context, GLint width, GLint height);

void gl_exit(gl_context *context);


void gl_set_bg_color(color_t color);


void _gl_key_callback_proxy(GLFWwindow *w, int key, int action, int scancode,
        int mods);

void gl_register_key_callback(gl_context *c,
        void (*callback)(gl_context*, int key, int action, int scancode,
            int mods));


void gl_clear(gl_context *c);


void gl_render(gl_context *c);


int gl_should_exit(gl_context *c);

/*
 * gives number of texture units available to fragment shader on this hardware
 */
int gl_num_texture_units();

#ifdef __cplusplus
}
#endif

#endif /* _MGL_H */
