
#include <math.h>

#include <camera.h>


Camera::Camera(float x, float y) : x(x), y(y) {

}

Camera::~Camera() {

}


void Camera::move(float dx, float dy) {
    x += dx;
    y += dy;
}


void Camera::apply(program * p) const {
    GLuint cam = gl_uniform_location(p, "cam");

    float mat[] = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        x,   y,   1.f
    };

    glUniformMatrix3fv(cam, 1, 0, mat);
}

