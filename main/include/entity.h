#pragma once

#include <gl/shader.h>


class Entity {
private:

    float x, y;

public:

    Entity(float x, float y);


    void upload_pos(program * prog);

    /*
     * draws entity to the screen, must call upload_pos to set the position of
     * the entity (shader must have a model uniform mat3 variable)
     */
    virtual void render() = 0;
};

