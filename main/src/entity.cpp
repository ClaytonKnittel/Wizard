
#include <entity.h>

extern const float aspect_ratio;
extern const float inv_aspect_ratio;

Entity::Entity(float x, float y, float scale) : x(x), y(y), scale(scale),
        iso(false) {}


void Entity::upload_pos(program * prog) {
    GLuint model = gl_uniform_location(prog, "model");

    float yscale = scale * inv_aspect_ratio;

    if (iso) {
        float mat[] = {
            scale, -yscale / 2.f, 0.f,
            scale,  yscale / 2.f, 0.f,
            x,      y,            1.f
        };
        glUniformMatrix3fv(model, 1, 0, mat);
    }
    else {
        float mat[] = {
            scale, 0.f,    0.f,
            0.f,   yscale, 0.f,
            x,     y,      1.f
        };
        glUniformMatrix3fv(model, 1, 0, mat);
    }
}


void Entity::make_iso() {
    iso = true;
}

