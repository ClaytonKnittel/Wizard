
#include <entity.h>


Entity::Entity(float x, float y) : x(x), y(y) {}


void Entity::upload_pos(program * prog) {
    GLuint model = gl_uniform_location(prog, "model");

    float mat[] = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        x,   y,   1.f
    };
    glUniformMatrix3fv(model, 1, 0, mat);
}

