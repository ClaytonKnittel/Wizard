
#include <math.h>

#include <camera.h>


Camera::Camera(float x, float y) : x(x), y(y), angle(0) {

}

Camera::~Camera() {

}


void Camera::move(float dx, float dy) {
    x += dx;
    y += dy;
}

void Camera::rotate(float dt) {
    angle += dt;
}


void Camera::apply(program * p) const {
    GLuint cam = gl_uniform_location(p, "cam");

    float c = cos(angle);
    float s = sin(angle);

    float mat[] = {
        c,             -s,             0.f, 0.f,
        s,              c,             0.f, 0.f,
        0.f,            0.f,           1.f, 0.f,
        x * c - y * s,  x * s + y * c, 0.f, 1.f
    };

    glUniformMatrix4fv(cam, 1, 0, mat);
}

