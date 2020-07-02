#pragma once

#include <gl/shader.h>


class Camera {
private:

    float x, y;

public:

    Camera(float x, float y);
    ~Camera();


    void move(float dx, float dy);

    void apply(program * p) const;

};


