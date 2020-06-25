
#include <entity.h>

extern const float aspect_ratio;
extern const float inv_aspect_ratio;

Entity::Entity(float x, float y, float scale) : x(x), y(y), scale(scale) {}


void Entity::upload_pos(program * prog) {
    GLuint model = gl_uniform_location(prog, "model");

    float yscale = scale * inv_aspect_ratio;

    float mat[] = {
        scale, -yscale / 2.f, -yscale / 2.f, 0.f,
        scale,  yscale / 2.f,  yscale / 2.f, 0.f,
        0.f,    yscale,        0.f,          0.f,
        x,      y,             0.f,          1.f
    };
    glUniformMatrix4fv(model, 1, 0, mat);
}


