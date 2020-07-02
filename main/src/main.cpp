
#include <img/bmp_loader.h>

#include <gl/gl.h>

#include <camera.h>
#include <map.h>


#define WIDTH 1024
#define HEIGHT 780


int main(int argc, char *argv[]) {
    gl_context c;

    gl_init(&c, WIDTH, HEIGHT);

    Screen screen(WIDTH, HEIGHT);

    gl_set_bg_color(gen_color(255, 255, 255, 255));

    Board b("test_board.txt");

    //b.save("test_board.txt");
    //b.load("test_board.txt");

    while (!gl_should_exit(&c)) {
        gl_clear(&c);

        b.render(screen);
        //cam.move(.02f, -.01f);

        gl_render(&c);
        glfwPollEvents();
    }

    gl_exit(&c);

    return 0;
}

