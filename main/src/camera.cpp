
#include <math.h>

#include <camera.h>


Camera::Camera(float x, float y) : x(x), y(y), scale(1.f),
                                   min_scale(CAM_SCALE_UNBOUNDED),
                                   max_scale(CAM_SCALE_UNBOUNDED) {
}

Camera::~Camera() {

}


float Camera::get_x() const {
    return this->x;
}

float Camera::get_y() const {
    return this->y;
}

float Camera::get_scale() const {
    return this->scale;
}


void Camera::screen_to_int(double &x, double &y) const {
    x *= scale;
    y *= scale;

    x += this->x;
    y += this->y;
}


void Camera::set_scale_bounds(float min_scale, float max_scale) {
    this->min_scale = min_scale;
    this->max_scale = max_scale;
}


void Camera::move(float dx, float dy) {
    x += dx * scale;
    y += dy * scale;
}


void Camera::zoom(float factor) {
    scale *= factor;
    if (min_scale > 0 && scale < min_scale) {
        scale = min_scale;
    }
    if (max_scale > 0 && scale > max_scale) {
        scale = max_scale;
    }
}


void Camera::apply(program * p) const {
    GLuint cam = gl_uniform_location(p, "cam");

    // take reciprocal, as zooming in (smaller scale) means more
    // spread in coordinates, and vice versa
    float scale = 1.f / this->scale;

    float mat[] = {
        scale,      0.f,        0.f,
        0.f,        scale,      0.f,
        -x * scale, -y * scale, 1.f
    };

    glUniformMatrix3fv(cam, 1, 0, mat);
}

