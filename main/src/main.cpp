
#include <img/bmp_loader.h>

#include <gl/gl.h>

#include <camera.h>
#include <board.h>


#define WIDTH 1024
#define HEIGHT 780



static Screen *g_screen;
static Board *g_b;


void mouse_click(gl_context * c, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT &&
            action == GLFW_PRESS) {

        double x, y;
        gl_mouse_pos(c, &x, &y);
        g_screen->pix_to_int(x, y);

        int tx, ty;
        g_b->get_coords(x, y, tx, ty);
        fprintf(stderr, "%3d, %3d\r", tx, ty);
        g_b->add_tile(tx, ty, 0);

    }
}


int main(int argc, char *argv[]) {
    gl_context c;

    gl_init(&c, WIDTH, HEIGHT);

    Screen screen(WIDTH, HEIGHT);

    gl_set_bg_color(gen_color(255, 255, 255, 255));

    Board b("test_board.txt");

    g_screen = &screen;
    g_b = &b;

    //b.save("test_board.txt");
    //b.load("test_board.txt");

    gl_register_mouse_callback(&c, &mouse_click);

    while (!gl_should_exit(&c)) {
        gl_clear(&c);

        b.render(screen);
        //screen.get_cam().move(.02f, -.01f);

        gl_render(&c);
        glfwPollEvents();
    }

    gl_exit(&c);

    return 0;
}

