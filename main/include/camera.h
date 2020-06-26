#pragma once

#include <gl/shader.h>


class Camera {
private:

    float x, y;
    float angle;

public:

    Camera(float x, float y);
    ~Camera();


    void move(float dx, float dy);

    void rotate(float dt);

    void apply(program * p) const;

};


