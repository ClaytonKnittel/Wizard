

#include <screen.h>


Screen::Screen(unsigned int width, unsigned int height) : cam(0, 0),
        width(width), height(height) {
}

Screen::~Screen() {

}

Camera & Screen::get_cam() {
    return cam;
}

unsigned int Screen::get_width() const {
    return width;
}

unsigned int Screen::get_height() const {
    return height;
}

float Screen::aspect_ratio() const {
    return ((float) height) / width;
}

float Screen::inv_aspect_ratio() const {
    return ((float) width) / height;
}


void Screen::get_screen_bounds(float &llx, float &lly, float &urx, float &ury) const {
    llx = cam.get_x() - cam.get_scale();
    lly = cam.get_y() - aspect_ratio() * cam.get_scale();
    urx = cam.get_x() + cam.get_scale();
    ury = cam.get_y() + aspect_ratio() * cam.get_scale();
}


void Screen::pix_to_int(double &x, double &y) const {
    x = x * 2 / width - 1;
    y = (-y * 2 / height + 1) * aspect_ratio();
    cam.screen_to_int(x, y);
}

void Screen::norm_to_int(double &x, double &y) const {
    y *= aspect_ratio();
    cam.screen_to_int(x, y);
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

