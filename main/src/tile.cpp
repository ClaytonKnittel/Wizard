
#include <tile.h>


void Tile::gen_vertices() {
    
    float x = (float) this->x;
    float y = (float) this->y;

    faces[0] = {
        .vertices = {
            {
                .x = x,
                .y = y,
                .tx = 0.f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y,
                .tx = 1.f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .tx = 1.f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[1] = {
        .vertices = {
            {
                .x = x,
                .y = y,
                .tx = 0.f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .tx = 1.f,
                .ty = 1.f
            },
            {
                .x = x,
                .y = y + 1,
                .tx = 0.f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
}


// default constructor only used when fields will be manually set
Tile::Tile() {}

    
Tile::Tile(texture * tex, int x, int y) : tex(tex), x(x), y(y) {

    gen_vertices();
}

Tile::~Tile() {}



void Tile::insert_all(Renderer & r) {
    for (uint32_t i = 0; i < TILE_N_PRIMITIVES; i++) {
        r << faces[i];
    }
}

