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

    unsigned int get_width() const;
    unsigned int get_height() const;

    float aspect_ratio() const;

    float inv_aspect_ratio() const;

    /*
     * transforms pixel coordinates to internal game coordinates
     */
    void pix_to_int(double &x, double &y) const;

    /*
     * transforms normalized screen coordinates to internal game coordinates
     */
    void norm_to_int(double &x, double &y) const;

    void apply(program * p) const;

};

