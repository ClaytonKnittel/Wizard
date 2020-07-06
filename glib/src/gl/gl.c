
#include <stdio.h>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <gl/gl.h>


static gl_context * get_window_context(GLFWwindow *w) {
    return (gl_context*) glfwGetWindowUserPointer(w);
}


static void window_resize_cb(GLFWwindow *w, int width, int height) {
    gl_context * c = get_window_context(w);
    glfwGetFramebufferSize(w, &width, &height);

    width_height wh = {
        .w = width,
        .h = height
    };
    c->wh = wh;
}


int gl_init(gl_context *context, GLint width, GLint height) {
    GLFWwindow * window;

    if (!glfwInit()) {
        fprintf(stderr, "GLFW window could not be initialized\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        return -1;
    }

    glfwGetFramebufferSize(window, &width, &height);

    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetWindowSizeCallback(window, &window_resize_cb);

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    context->window = window;

    width_height wh = {
        .w = width,
        .h = height
    };
    context->wh = wh;

    glfwSetWindowUserPointer(window, context);

    return 0;
}

void gl_exit(gl_context *context) {
    glfwDestroyWindow(context->window);
    glfwTerminate();
}


void gl_set_bg_color(color_t color) {
    glClearColor(color_r(color), color_g(color),
                 color_b(color), color_a(color));
}




void _gl_key_callback_proxy(GLFWwindow *w, int key, int scancode, int action,
        int mods) {

    gl_context * c = get_window_context(w);
    c->key_callback(c, key, scancode, action, mods);
}


void gl_register_key_callback(gl_context *c,
        void (*callback)(gl_context*, int key, int scancode, int action,
            int mods)) {

    c->key_callback = callback;
    glfwSetKeyCallback(c->window, &_gl_key_callback_proxy);
}

static void _gl_mouse_callback_proxy(GLFWwindow *w, int button, int action,
        int mods) {

    gl_context * c = get_window_context(w);
    c->mouse_callback(c, button, action, mods);
}

void gl_register_mouse_callback(gl_context *c,
        void (*callback)(gl_context*, int button, int action, int mods)) {

    c->mouse_callback = callback;
    glfwSetMouseButtonCallback(c->window, &_gl_mouse_callback_proxy);
}


void gl_mouse_pos(gl_context *c, double * x, double * y) {
    glfwGetCursorPos(c->window, x, y);
}


void gl_clear(gl_context *c) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    width_height wh = c->wh;
    glViewport(0, 0, wh.w, wh.h);
}


void gl_render(gl_context *c) {
    // Swap buffers
    glfwSwapBuffers(c->window);
}


int gl_should_exit(gl_context *c) {
    return glfwGetKey(c->window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
        glfwWindowShouldClose(c->window);
}

int gl_num_texture_units() {
    int texture_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
    return texture_units;
}


