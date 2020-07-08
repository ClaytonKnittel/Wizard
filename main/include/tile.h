#pragma once

#include <object.h>
#include <texture_set.h>

// 2 triangles per face, 5 sides (bottom never visible)
#define TILE_N_PRIMITIVES 2


/*
 * for tiles on the board, are always a fixed size (1x1x1) and must lie on
 * integer-aligned coordinates
 */
class Tile : public Object {
    friend class Board;
private:

    // which texture will be mapped to the tile
    const TextureSet * texset;
    int tex_idx;
    int x, y;

    bool changed;

    Triangle faces[TILE_N_PRIMITIVES];

    void gen_vertices();

public:

    Tile();
    Tile(const TextureSet &, int tex_idx, int x, int y);
    virtual ~Tile();

    void set_tex_idx(int tex_idx);

    void mark_changed();

    virtual void insert_all(Renderer &);
};




