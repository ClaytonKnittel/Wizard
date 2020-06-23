
#include <img/bmp_loader.h>

#include <gl/gl.h>

#include <map.h>


#define WIDTH 1024
#define HEIGHT 780

int main(int argc, char *argv[]) {
    gl_context c;

    gl_init(&c, WIDTH, HEIGHT);

    gl_set_bg_color(gen_color(255, 255, 255, 255));

    Board b;

    while (!gl_should_exit(&c)) {
        gl_clear(&c);

        b.render();

        gl_render(&c);
        glfwPollEvents();
    }

    gl_exit(&c);

    return 0;
}

