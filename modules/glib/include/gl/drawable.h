#ifndef _DRAWABLE_H
#define _DRAWABLE_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gl/shader.h>
#include <gl/color.h>

#include <util.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct drawable {
    GLuint vao, vbo;
    uint64_t size;
} drawable;


/*
 * initializes a static drawable (fixed shape, variable size and position).
 *
 * Expected data layout is as follows:
 *  +------+------+-------+
 *  |  vx  |  vy  | index |
 *  +------+------+-------+
 *
 * where index (uint32_t) determines which element of a color buffer to apply
 * to this vertex
 *
 */
int gl_load_static_indexed_drawable(drawable *d, uint32_t *data,
        size_t n_vertices);


/*
 * initializes a static drawable (fixed shape, variable size and position).
 *
 * Expected data layout is as follows:
 *  +------+------+-------+
 *  |  vx  |  vy  | color |
 *  +------+------+-------+
 *
 * where color each element is of width 4 bytes
 */
int gl_load_static_monochrome_drawable(drawable *d, uint32_t *data,
        size_t n_vertices);


/*
 * initializes a dynamic drawable (can vary texture coordinates each frame)
 *
 * Expected data layout is as follows:
 *  +------+------+------+
 *  |  vx  |  vy  |  f   |
 *  +------+------+------+
 */
int gl_load_dynamic_pos_float(drawable *d, const float *data,
        size_t n_vertices);


/*
 * initializes a dynamic drawable (can vary texture coordinates each frame)
 *
 * Expected data layout is as follows:
 *  +------+------+------+------+
 *  |  vx  |  vy  |  tx  |  ty  |
 *  +------+------+------+------+
 *
 * where (tx, ty) are the texture coordinates of the vertex
 */
int gl_load_dynamic_textured(drawable *d, const float *data,
        size_t n_vertices);


int gl_update_dynamic_pos_float(drawable *d, const float *data,
        size_t n_vertices);


int gl_update_dynamic_textured(drawable *d, const float *data,
        size_t n_vertices);


static void gl_unload_static_monochrome_drawable(drawable *d) {
    glDeleteVertexArrays(1, &d->vao);
    glDeleteBuffers(1, &d->vbo);
}


static void gl_draw(drawable *d) {
    glBindVertexArray(d->vao);
    glDrawArrays(GL_TRIANGLES, 0, d->size);
    glBindVertexArray(0);
}

static void gl_draw_sub(drawable *d, size_t size) {
    glBindVertexArray(d->vao);
    glDrawArrays(GL_TRIANGLES, 0, size);
    glBindVertexArray(0);
}

static void gl_draw_instanced(drawable *d, GLsizei primcount) {
    glBindVertexArray(d->vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, d->size, primcount);
    glBindVertexArray(0);
}

#ifdef __cplusplus
}
#endif


#endif /* _DRAWABLE_H */
