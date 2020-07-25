
#include <tile.h>


void Tile::gen_vertices() {
    
    float x = (float) this->x;
    float y = (float) this->y;

    const texture_t * tex = texset->get_tex();

    float llx, lly, urx, ury;
    texset->get_bounds(tex_idx, llx, lly, urx, ury);

    faces[0] = {
        .vertices = {
            {
                .x = x,
                .y = y,
                .tx = llx,
                .ty = lly
            },
            {
                .x = x + 1,
                .y = y,
                .tx = urx,
                .ty = lly
            },
            {
                .x = x + 1,
                .y = y + 1,
                .tx = urx,
                .ty = ury
            }
        },
        .tex = tex
    };
    faces[1] = {
        .vertices = {
            {
                .x = x,
                .y = y,
                .tx = llx,
                .ty = lly
            },
            {
                .x = x + 1,
                .y = y + 1,
                .tx = urx,
                .ty = ury
            },
            {
                .x = x,
                .y = y + 1,
                .tx = llx,
                .ty = ury
            }
        },
        .tex = tex
    };

    changed = false;
}


// default constructor only used when fields will be manually set
Tile::Tile() : texset(nullptr), tex_idx(0), x(0), y(0), changed(false) {}

    
Tile::Tile(const TextureSet & tex, int tex_idx, int x, int y) : texset(&tex),
        tex_idx(tex_idx), x(x), y(y), changed(true) {
}

Tile::~Tile() {}


int Tile::get_x() const {
    return x;
}

int Tile::get_y() const {
    return y;
}


void Tile::set_x(int x) {
    this->x = x;
    this->changed = true;
}

void Tile::set_y(int y) {
    this->y = y;
    this->changed = true;
}


void Tile::set_tex_idx(int tex_idx) {
    this->tex_idx = tex_idx;
    this->changed = true;
}


void Tile::mark_changed() {
    this->changed = true;
}


void Tile::insert_all(Renderer & r) {
    if (changed) {
        gen_vertices();
    }
    for (uint32_t i = 0; i < TILE_N_PRIMITIVES; i++) {
        r << faces[i];
    }
}

