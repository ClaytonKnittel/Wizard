
#include <tile.h>


void Tile::gen_vertices() {
    
    float x = (float) this->x;
    float y = (float) this->y;
    float z = (float) this->z;
    
    faces[0] = {
        .vertices = {
            {
                .x = x,
                .y = y,
                .z = z,
                .tx = 0.f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y,
                .z = z,
                .tx = .2f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .z = z,
                .tx = .2f,
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
                .z = z,
                .tx = 0.f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .z = z,
                .tx = .2f,
                .ty = 1.f
            },
            {
                .x = x,
                .y = y + 1,
                .z = z,
                .tx = 0.f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[2] = {
        .vertices = {
            {
                .x = x,
                .y = y,
                .z = z - 1.f,
                .tx = .2f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y,
                .z = z - 1.f,
                .tx = .4f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y,
                .z = z,
                .tx = .4f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[3] = {
        .vertices = {
            {
                .x = x,
                .y = y,
                .z = z - 1.f,
                .tx = .2f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y,
                .z = z,
                .tx = .4f,
                .ty = 1.f
            },
            {
                .x = x,
                .y = y,
                .z = z,
                .tx = .2f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[4] = {
        .vertices = {
            {
                .x = x + 1,
                .y = y,
                .z = z - 1.f,
                .tx = .4f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .z = z - 1.f,
                .tx = .6f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .z = z,
                .tx = .6f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[5] = {
        .vertices = {
            {
                .x = x + 1,
                .y = y,
                .z = z - 1.f,
                .tx = .4f,
                .ty = 0.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .z = z,
                .tx = .6f,
                .ty = 1.f
            },
            {
                .x = x + 1,
                .y = y,
                .z = z,
                .tx = .4f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[6] = {
        .vertices = {
            {
                .x = x + 1,
                .y = y + 1,
                .z = z - 1.f,
                .tx = .6f,
                .ty = 0.f
            },
            {
                .x = x,
                .y = y + 1,
                .z = z - 1.f,
                .tx = .8f,
                .ty = 0.f
            },
            {
                .x = x,
                .y = y + 1,
                .z = z,
                .tx = .8f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[7] = {
        .vertices = {
            {
                .x = x + 1,
                .y = y + 1,
                .z = z - 1.f,
                .tx = .6f,
                .ty = 0.f
            },
            {
                .x = x,
                .y = y + 1,
                .z = z,
                .tx = .8f,
                .ty = 1.f
            },
            {
                .x = x + 1,
                .y = y + 1,
                .z = z,
                .tx = .6f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[8] = {
        .vertices = {
            {
                .x = x,
                .y = y + 1,
                .z = z - 1.f,
                .tx = .8f,
                .ty = 0.f
            },
            {
                .x = x,
                .y = y,
                .z = z - 1.f,
                .tx = 1.f,
                .ty = 0.f
            },
            {
                .x = x,
                .y = y,
                .z = z,
                .tx = 1.f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
    faces[9] = {
        .vertices = {
            {
                .x = x,
                .y = y + 1,
                .z = z - 1.f,
                .tx = .8f,
                .ty = 0.f
            },
            {
                .x = x,
                .y = y,
                .z = z,
                .tx = 1.f,
                .ty = 1.f
            },
            {
                .x = x,
                .y = y + 1,
                .z = z,
                .tx = .8f,
                .ty = 1.f
            }
        },
        .tex = tex
    };
}


// default constructor only used when fields will be manually set
Tile::Tile() {}

    
Tile::Tile(texture * tex, int x, int y, int z) : tex(tex), x(x), y(y), z(z) {

    gen_vertices();
}

Tile::~Tile() {}



void Tile::insert_all(Renderer & r) {
    for (uint32_t i = 0; i < TILE_N_PRIMITIVES; i++) {
        r << faces[i];
    }
}

