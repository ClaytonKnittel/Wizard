
#include <img/bmp_loader.h>

#include <gl/gl.h>
#include <gl/drawable.h>
#include <gl/shader.h>
#include <gl/texture.h>


#define WIDTH 1024
#define HEIGHT 780

int main(int argc, char *argv[]) {
    gl_context c;
    drawable d;
    program prog;
    texture_t t;


    gl_init(&c, WIDTH, HEIGHT);

    gl_load_program(&prog, "main/res/tile.vs", "main/res/tile.fs");

    float data[] = {
        0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f
    };

    gl_load_dynamic_textured(&d, data, 3);

    texture_init(&t, "main/img/wab.bmp");

    GLuint trans = gl_uniform_location(&prog, "trans");

    while (!gl_should_exit(&c)) {
        gl_clear(&c);

        gl_use_program(&prog);

        float mat[] = {
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 0.f, 1.f
        };
        glUniformMatrix3fv(trans, 1, 0, mat);

        texture_use(&t);
        gl_draw(&d);

        gl_render(&c);
        glfwPollEvents();
    }

    texture_destroy(&t);
    gl_unload_static_monochrome_drawable(&d);
    gl_unload_program(&prog);

    return 0;
}

