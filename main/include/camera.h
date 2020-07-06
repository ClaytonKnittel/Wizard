#pragma once

#include <gl/shader.h>


class Camera {
private:

    float x, y, scale;

public:

    Camera(float x, float y);
    ~Camera();

    void screen_to_int(double &x, double &y) const;

    void move(float dx, float dy);

    void zoom(float factor);

    void apply(program * p) const;

};


