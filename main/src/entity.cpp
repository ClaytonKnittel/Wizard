
#include <math.h>

#include <entity.h>

Entity::Entity(float x, float y, float scale) : x(x), y(y), scale(scale) {}


void Entity::upload_pos(program * prog) {
    GLuint model = gl_uniform_location(prog, "model");

    float mat[] = {
        scale, 0.f,   0.f,
        0.f,   scale, 0.f,
        x,     y,     1.f
    };

    glUniformMatrix3fv(model, 1, 0, mat);
}


