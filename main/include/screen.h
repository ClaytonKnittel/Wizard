#pragma once


#include <gl/shader.h>

#include <camera.h>


class Screen {
private:

    Camera cam;

    unsigned int width, height;


    void __int_apply(program * p) const;

public:

    Screen(unsigned int width, unsigned int height);
    ~Screen();

    Camera & get_cam();

    float aspect_ratio() const;

    float inv_aspect_ratio() const;

    void apply(program * p) const;

};

