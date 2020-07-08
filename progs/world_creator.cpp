
#include <img/bmp_loader.h>

#include <gl/gl.h>

#include <camera.h>
#include <board.h>


#define WIDTH 1024
#define HEIGHT 780



static Screen *g_screen;
static Board *g_b;

static bool clicked = false;

static bool keys[6] = { false, false, false, false, false, false };

#define SCROLL_SPEED .03f
#define ZOOM_SPEED 1.1f

void mouse_click(gl_context * c, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT &&
            action == GLFW_PRESS) {

        clicked = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT &&
            action == GLFW_RELEASE) {

        clicked = false;
    }
}


void key_press(gl_context * c, int key, int scancode, int action, int mods) {

    if (mods & GLFW_MOD_CONTROL) {
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            g_b->save("test_board.txt");
            printf("Saved to %s\n", "test_board.txt");
        }
    }
    else {
        if (key == GLFW_KEY_W) {
            keys[0] = (action != GLFW_RELEASE);
        }
        else if (key == GLFW_KEY_A) {
            keys[1] = (action != GLFW_RELEASE);
        }
        else if (key == GLFW_KEY_S) {
            keys[2] = (action != GLFW_RELEASE);
        }
        else if (key == GLFW_KEY_D) {
            keys[3] = (action != GLFW_RELEASE);
        }
        else if (key == GLFW_KEY_Z) {
            keys[4] = (action != GLFW_RELEASE);
        }
        else if (key == GLFW_KEY_X) {
            keys[5] = (action != GLFW_RELEASE);
        }
    }
}



void update(gl_context * c) {
    double mx, my;
    gl_mouse_pos(c, &mx, &my);
    g_screen->pix_to_int(mx, my);

    int tx, ty;
    g_b->get_coords(mx, my, tx, ty);

    if (clicked) {
        g_b->add_tile(tx, ty, 0);
    }

    g_b->set_preview(tx, ty, 0);

    if (keys[0]) {
        g_screen->get_cam().move(0, SCROLL_SPEED);
    }
    if (keys[1]) {
        g_screen->get_cam().move(-SCROLL_SPEED, 0);
    }
    if (keys[2]) {
        g_screen->get_cam().move(0, -SCROLL_SPEED);
    }
    if (keys[3]) {
        g_screen->get_cam().move(SCROLL_SPEED, 0);
    }
    if (keys[4]) {
        g_screen->get_cam().zoom(1 / ZOOM_SPEED);
    }
    if (keys[5]) {
        g_screen->get_cam().zoom(ZOOM_SPEED);
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

    gl_register_key_callback(&c, &key_press);
    gl_register_mouse_callback(&c, &mouse_click);

    screen.get_cam().set_scale_bounds(.3f, 20.f);

    while (!gl_should_exit(&c)) {
        update(&c);
        gl_clear(&c);

        b.render(screen);

        gl_render(&c);
        glfwPollEvents();
    }

    gl_exit(&c);

    return 0;
}
