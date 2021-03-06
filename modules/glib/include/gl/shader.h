#ifndef _SHADER_H
#define _SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#ifdef __cplusplus

#define restrict

extern "C" {
#endif


typedef struct program {
    GLuint self;
} program;


int gl_load_program(program * p, const char * restrict vert_path,
        const char * restrict frag_path);

void gl_use_program(program * p);

GLuint gl_uniform_location(program * p, const char * name);

void gl_unload_program(program * p);


#ifdef __cplusplus
}
#endif

#endif /* _SHADER_H */
