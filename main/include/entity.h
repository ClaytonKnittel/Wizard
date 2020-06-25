#pragma once

#include <gl/shader.h>


class Entity {
private:

    float x, y;
    float scale;

    // set if need to apply isometric transform to this guy
    bool iso;

public:

    Entity(float x, float y, float scale);


    void upload_pos(program * prog);

    /*
     * to be called on entities which need to have isometric transformation
     * applied
     */
    void make_iso();

    /*
     * draws entity to the screen, must call upload_pos to set the position of
     * the entity (shader must have a model uniform mat3 variable)
     */
    virtual void render() = 0;
};

