
#include <math.h>

#include <entity.h>

Entity::Entity(float x, float y, float scale) : x(x), y(y), scale(scale) {}


void Entity::upload_pos(program * prog) {
    GLuint model = gl_uniform_location(prog, "model");

    float mat[] = {
        scale, 0.f,   0.f,   0.f,
        0.f,   scale, 0.f,   0.f,
        0.f,   0.f,   scale, 0.f,
        x,     y,     0.f,   1.f
    };

    /*
    float mat[] = {
        scale, -yscale / 2.f, -yscale / 2.f, 0.f,
        scale,  yscale / 2.f,  yscale / 2.f, 0.f,
        0.f,    yscale,        0.f,          0.f,
        x,      y,             0.f,          1.f
    };*/
    glUniformMatrix4fv(model, 1, 0, mat);
}


