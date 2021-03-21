
#include <unordered_map>

#include <img/bmp_loader.h>

#include <gl/gl.h>

#include <camera.h>
#include <board.h>
#include <select_tool.h>

#include "ctrl.h"


#define WIDTH 1024
#define HEIGHT 780


static Screen *g_screen;
static Board *g_b;
static Controller *g_ctrl;
static SelectTool *g_st;
static Sprite cur_item;

static bool clicked = false, held = false, released = false;

static bool keys[6] = { false, false, false, false, false, false };

#define SCROLL_SPEED .03f
#define ZOOM_SPEED 1.1f

static float speed_factor = 1.f;


void mouse_click(gl_context * c, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT &&
			action == GLFW_PRESS) {

		clicked = true;
		held = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT &&
			action == GLFW_RELEASE) {

		released = true;
		held = false;
	}
}


void key_press(gl_context * c, int key, int scancode, int action, int mods) {

	if (mods & GLFW_MOD_CONTROL) {
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			if (g_b->save("test_board.txt") == 0) {
				printf("Saved to %s\n", "test_board.txt");
			}
		}
	}
	else {

		if (action == GLFW_PRESS) {
			g_ctrl->press(key);
		}
		else if (action == GLFW_RELEASE) {
			g_ctrl->release(key);
		}
	}
}


void init_ctrl(Controller & c) {
	Node & root = c.get_root();

	const TextureCollection &texs = g_b->get_texture_collection();

	const TextureSet * test = texs["test"];
	const TextureSet * gras = texs["badgrass"];

	// default
	cur_item = { test, 0 };

	// test tiles
	{
		Node & gnode = root.add_child(GLFW_KEY_G);

		gnode.add_child(GLFW_KEY_1).make_terminal([=](void) -> void {
				cur_item = { test, 0 };
				g_st->disable();
				});

		gnode.add_child(GLFW_KEY_2).make_terminal([=](void) -> void {
				cur_item = { test, 1 };
				g_st->disable();
				});

		gnode.add_child(GLFW_KEY_3).make_terminal([=](void) -> void {
				cur_item = { test, 2 };
				g_st->disable();
				});

		gnode.add_child(GLFW_KEY_4).make_terminal([=](void) -> void {
				cur_item = { test, 3 };
				g_st->disable();
				});

		root.add_child(GLFW_KEY_G).add_child(GLFW_KEY_5).make_terminal([=](void) -> void {
				cur_item = { gras, 0 };
				g_st->disable();
				});
	}

	// eraser
	root.add_child(GLFW_KEY_E).make_terminal([=](void) -> void {
			cur_item = { nullptr, 0 };
			g_st->disable();
			});

	// select tool
	{
		Node & st_node = root.add_child(GLFW_KEY_R);

		st_node.make_terminal([=](void) -> void {
				cur_item = { nullptr, 0 };
				g_st->enable();
				});

		root.add_child(GLFW_KEY_BACKSPACE).make_terminal([=](void) -> void {
				g_st->delete_tiles();
				g_st->disappear();
				});

		root.add_child(GLFW_KEY_Y).make_terminal([=](void) -> void {
				g_st->copy();
				});

		root.add_child(GLFW_KEY_P).make_terminal([=](void) -> void {
				g_st->paste();
				});
	}

	// camera control keys
	{

		Node & up = root.add_child(GLFW_KEY_W);
		up.make_terminal([=](void) -> void {
				keys[0] = true;
				});
		up.add_release_callback([=](void) -> void {
				keys[0] = false;
				});

		Node & left = root.add_child(GLFW_KEY_A);
		left.make_terminal([=](void) -> void {
				keys[1] = true;
				});
		left.add_release_callback([=](void) -> void {
				keys[1] = false;
				});

		Node & down = root.add_child(GLFW_KEY_S);
		down.make_terminal([=](void) -> void {
				keys[2] = true;
				});
		down.add_release_callback([=](void) -> void {
				keys[2] = false;
				});

		Node & right = root.add_child(GLFW_KEY_D);
		right.make_terminal([=](void) -> void {
				keys[3] = true;
				});
		right.add_release_callback([=](void) -> void {
				keys[3] = false;
				});

		Node & zoom_in = root.add_child(GLFW_KEY_Z);
		zoom_in.make_terminal([=](void) -> void {
				keys[4] = true;
				});
		zoom_in.add_release_callback([=](void) -> void {
				keys[4] = false;
				});

		Node & zoom_out = root.add_child(GLFW_KEY_X);
		zoom_out.make_terminal([=](void) -> void {
				keys[5] = true;
				});
		zoom_out.add_release_callback([=](void) -> void {
				keys[5] = false;
				});

		Node & speed_up = root.add_child(GLFW_KEY_LEFT_SHIFT);
		speed_up.make_terminal([=](void) -> void {
				speed_factor = 5.f;
				});
		speed_up.add_release_callback([=](void) -> void {
				speed_factor = 1.f;
				});
	}
}



void update(gl_context * c) {
	double mx, my;
	gl_mouse_pos(c, &mx, &my);
	g_screen->pix_to_int(mx, my);

	int tx, ty;
	g_b->get_coords(mx, my, tx, ty);

	if (held) {
		if (g_st->is_enabled()) {
			if (clicked) {
				g_st->click(mx, my);
			}
		}
		else {
			g_b->add_tile(tx, ty, cur_item.texset, cur_item.tex_idx);
		}

		clicked = false;
	}
	if (released) {
		if (g_st->is_enabled()) {
			g_st->release();
		}

		released = false;
	}

	g_st->mouse_at(mx, my);

	g_b->set_preview(tx, ty, cur_item.texset, cur_item.tex_idx);

	if (keys[0]) {
		g_screen->get_cam().move(0, speed_factor * SCROLL_SPEED);
	}
	if (keys[1]) {
		g_screen->get_cam().move(-speed_factor * SCROLL_SPEED, 0);
	}
	if (keys[2]) {
		g_screen->get_cam().move(0, -speed_factor * SCROLL_SPEED);
	}
	if (keys[3]) {
		g_screen->get_cam().move(speed_factor * SCROLL_SPEED, 0);
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
	Controller ctrl;

	gl_set_bg_color(gen_color(255, 255, 255, 255));

	Board b("boards/clay_board.txt");
	SelectTool st(b);

	g_screen = &screen;
	g_b = &b;
	g_ctrl = &ctrl;
	g_st = &st;

	init_ctrl(ctrl);

	gl_register_key_callback(&c, &key_press);
	gl_register_mouse_callback(&c, &mouse_click);

	screen.get_cam().set_scale_bounds(.3f, 20.f);

	while (!gl_should_exit(&c)) {
		update(&c);
		gl_clear(&c);

		b.render(screen);

		double mx, my;
		gl_mouse_pos(&c, &mx, &my);
		g_screen->pix_to_int(mx, my);

		st.render(screen);

		gl_render(&c);
		glfwPollEvents();
	}

	gl_exit(&c);

	return 0;
}

