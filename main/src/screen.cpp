

#include <screen.h>


Screen::Screen(unsigned int width, unsigned int height) : cam(0, 0),
        width(width), height(height) {
}

Screen::~Screen() {

}

Camera & Screen::get_cam() {
    return cam;
}

float Screen::aspect_ratio() const {
    return ((float) height) / width;
}

float Screen::inv_aspect_ratio() const {
    return ((float) width) / height;
}


void Screen::__int_apply(program * p) const {

    GLuint view = gl_uniform_location(p, "view");

    float s  = inv_aspect_ratio();

    float mat[] = {
        1.f, 0.f, 0.f,
        0.f, s,   0.f,
        0.f, 0.f, 1.f
    };

    glUniformMatrix3fv(view, 1, 0, mat);
}

void Screen::apply(program * p) const {
    cam.apply(p);
    __int_apply(p);
}

