
#include <math.h>

#include <camera.h>


Camera::Camera(float x, float y) : x(x), y(y), scale(1.f) {

}

Camera::~Camera() {

}


void Camera::screen_to_int(double &x, double &y) const {
    x /= scale;
    y /= scale;

    x += this->x;
    y += this->y;
}


void Camera::move(float dx, float dy) {
    x += dx;
    y += dy;
}


void Camera::zoom(float factor) {
    scale *= factor;
}


void Camera::apply(program * p) const {
    GLuint cam = gl_uniform_location(p, "cam");

    float mat[] = {
        scale,      0.f,        0.f,
        0.f,        scale,      0.f,
        -x * scale, -y * scale, 1.f
    };

    glUniformMatrix3fv(cam, 1, 0, mat);
}

