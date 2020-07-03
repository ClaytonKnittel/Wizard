#pragma once

#include <gl/shader.h>

#include <screen.h>


class Entity {
protected:

    float x, y;
    float scale;

public:

    Entity(float x, float y, float scale);


    void upload_pos(program * prog);

    /*
     * draws entity to the screen, must call upload_pos to set the position of
     * the entity (shader must have a model uniform mat3 variable)
     */
    virtual void render(const Screen & cam) = 0;
};

