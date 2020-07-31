#pragma once

#include <gl/shader.h>


#define CAM_SCALE_UNBOUNDED -1.f


class Camera {
private:

    float x, y, scale;

    /*
     * bounds on the value of scale, negative values mean no bound
     */
    float min_scale, max_scale;

public:

    Camera(float x, float y);
    ~Camera();

    float get_x() const;
    float get_y() const;
    float get_scale() const;

    void screen_to_int(double &x, double &y) const;

    void set_scale_bounds(float min_scale, float max_scale);

    void move(float dx, float dy);

    void zoom(float factor);

    void apply(program * p) const;

};


