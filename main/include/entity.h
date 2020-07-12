#pragma once

#include <gl/shader.h>

#include <screen.h>


class Entity {
protected:

    float x, y;
    float scale;

public:

    Entity(float x, float y, float scale);


    float get_x() const;
    float get_y() const;
    float get_scale() const;

    void upload_pos(program * prog);

    /*
     * draws entity to the screen, must call upload_pos to set the position of
     * the entity (shader must have a model uniform mat3 variable)
     */
    virtual void render(const Screen & screen) = 0;
};

